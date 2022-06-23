#pragma once

#include <memory>
#include <atomic>
#include <utility>
#include <type_traits>

#include "diagnostics.h"
#include "cpp_feature/lib_exchange_function.h"
#include "cpp_feature/lib_raw_memory_algorithms.h"

namespace gkr
{

template<typename T, typename Allocator=std::allocator<T>>
class lockfree_forward_list
{
    lockfree_forward_list           (const lockfree_forward_list&) noexcept = delete;
    lockfree_forward_list& operator=(const lockfree_forward_list&) noexcept = delete;

private:
    struct node_t;
    using  next_t = std::atomic<node_t*>;

    struct node_t
    {
        T      value {};
        next_t next  {nullptr};

        node_t() noexcept = default;
       ~node_t() noexcept = default;

        node_t           (node_t&&) noexcept = delete;
        node_t& operator=(node_t&&) noexcept = delete;

        node_t           (const node_t&) noexcept = delete;
        node_t& operator=(const node_t&) noexcept = delete;
    };

public:
    using       value_type = T;
    using   allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_t>;

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

    static constexpr bool      clear_is_noexcept = std::is_nothrow_destructible<T>::value;
    static constexpr bool    add_new_is_noexcept = std::is_nothrow_default_constructible<T>::value;
    static constexpr bool      type_is_supported = std::is_default_constructible<T>::value;
    static constexpr bool allocator_is_supported =
        allocator_traits::is_always_equal::value || (
        allocator_traits::propagate_on_container_swap::value &&
        allocator_traits::propagate_on_container_move_assignment::value
        );
    static_assert(     type_is_supported, "Only defaul-constructible value types are supported");
    static_assert(allocator_is_supported, "Only always-equal or propagate-on move-assignment/swap allocators are supported");

private:
    next_t m_first {nullptr};

    [[no_unique_address]]
    allocator_type m_allocator;

public:
    lockfree_forward_list(Allocator allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<allocator_type, Allocator>::value
        )
        : m_allocator(allocator)
    {
    }
    ~lockfree_forward_list() noexcept(clear_is_noexcept)
    {
        clear();
    }

    lockfree_forward_list(lockfree_forward_list&& other) noexcept(
        std::is_nothrow_move_constructible<allocator_type>::value
        )
        : m_first    (other.m_first.exchange(nullptr))
        , m_allocator(std::move(other.m_allocator))
    {
    }
    lockfree_forward_list& operator=(lockfree_forward_list&& other) noexcept(clear_is_noexcept)
    {
        if(this != &other)
        {
            clear();

#ifdef __cpp_if_constexpr
            if constexpr(allocator_traits::propagate_on_container_move_assignment::value)
#else
            if          (allocator_traits::propagate_on_container_move_assignment::value)
#endif
            {
                m_allocator = std::move(other.m_allocator);
            }
            m_first.store(other.m_first.exchange(nullptr));
        }
        return *this;
    }
    void swap(lockfree_forward_list& other) noexcept
    {
        if(this != &other)
        {
#ifdef __cpp_if_constexpr
            if constexpr(allocator_traits::propagate_on_container_swap::value)
#else
            if          (allocator_traits::propagate_on_container_swap::value)
#endif
            {
                std::swap(m_allocator, other.m_allocator);
            }
            m_first.store(other.m_first.exchange(m_first.load()));
        }
    }

public:
    bool empty() const noexcept
    {
        return (m_first.load() == nullptr);
    }
    const allocator_type get_allocator() const noexcept
    {
        return m_allocator;
    }
    allocator_type get_allocator() noexcept
    {
        return m_allocator;
    }

public:
    void clear() noexcept(clear_is_noexcept)
    {
        for(node_t* node = m_first.exchange(nullptr); node != nullptr; )
        {
            node_t* next = node->next.exchange(nullptr);

            std::destroy_at(node);

            m_allocator.deallocate(node, 1);

            node = next;
        }
    }

public:
    template<typename Node, typename Value>
    struct iterator_t
    {
        static Node* before_begin_node() { return reinterpret_cast<Node*>(std::size_t(-1)); }

        iterator_t() noexcept = delete;
       ~iterator_t() noexcept = default;

        iterator_t(Node* n) noexcept : node(n) {}

        iterator_t(      iterator_t&& other) noexcept : node(std::exchange(other.node, nullptr)) {}
        iterator_t(const iterator_t&  other) noexcept : node(              other.node          ) {}

        iterator_t& operator=(      iterator_t&& other) noexcept { if(this != &other) node = std::exchange(other.node, nullptr); return *this; }
        iterator_t& operator=(const iterator_t&  other) noexcept { if(this != &other) node =               other.node          ; return *this; }

        void swap(iterator_t& other) noexcept { std::swap(node, other.node); }

        bool is_before_begin() const { return (node == before_begin_node()); }
        bool has_normal_node() const { return (node != before_begin_node()) && (node != nullptr); }

        bool operator==(const iterator_t& other) const noexcept { return (node == other.node); }
        bool operator!=(const iterator_t& other) const noexcept { return (node != other.node); }

        Value& operator* () noexcept(DIAG_NOEXCEPT) { Assert_Check(has_normal_node()); return  node->value; }
        Value* operator->() noexcept(DIAG_NOEXCEPT) { Assert_Check(has_normal_node()); return &node->value; }

        void increment() noexcept(DIAG_NOEXCEPT) { Assert_Check(has_normal_node()); node = node->next.load(); }

        iterator_t& operator++()    noexcept(DIAG_NOEXCEPT) {                          increment(); return *this; }
        iterator_t  operator++(int) noexcept(DIAG_NOEXCEPT) { iterator_t prev = *this; increment(); return  prev; }

        Node* node;
    };
    using       iterator = iterator_t<      node_t,       T>;
    using const_iterator = iterator_t<const node_t, const T>;

public:
    const_iterator begin() const noexcept
    {
        return const_iterator(m_first.load());
    }
    const_iterator end() const noexcept
    {
        return const_iterator(nullptr);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(m_first.load());
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

public:
    iterator before_begin() noexcept
    {
        return iterator(iterator::before_begin_node());
    }
    iterator begin() noexcept
    {
        return iterator(m_first.load());
    }
    iterator end() noexcept
    {
        return iterator(nullptr);
    }

private:
    node_t* add_new(next_t& next) noexcept(add_new_is_noexcept)
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t();

        node->next.store(next.exchange(node));

        return node;
    }

public:
    bool get_next_or_add_new(iterator& it) noexcept(add_new_is_noexcept)
    {
        Check_Arg_IsValid(it != end(), false);

        next_t& next = it.is_before_begin()
            ? m_first
            : it.node->next
            ;
        node_t* node = next.load();

        if(node != nullptr)
        {
            it = iterator(node);
            return false;
        }
        else
        {
            it = iterator(add_new(next));
            return true;
        }
    }
    T& push_back() noexcept(add_new_is_noexcept)
    {
        for(node_t* node = nullptr; ; )
        {
            next_t& next = (node == nullptr)
                ? m_first
                : node->next
                ;

            node = next.load();

            if(node == nullptr)
            {
                return add_new(next)->value;
            }
        }
    }
};

}
