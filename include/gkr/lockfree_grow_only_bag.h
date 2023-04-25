#pragma once

#include <memory>
#include <atomic>
#include <utility>
#include <type_traits>

#ifndef GKR_LOCKFREE_GROW_ONLY_BAG_SINGLE_HEADER

#include "diag/diagnostics.h"

#ifndef __cpp_lib_exchange_function
#include "cpp/lib_exchange_function.h"
#endif

#else

#ifndef __cpp_lib_exchange_function
#error  You must use C++14 or preinclude implementation of std::exchange
#endif

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Assert_NotNullPtr
#define Assert_NotNullPtr(cond)
#endif

#endif

namespace gkr
{

template<typename T, typename Allocator=std::allocator<T>>
class lockfree_grow_only_bag
{
    lockfree_grow_only_bag           (const lockfree_grow_only_bag&) noexcept = delete;
    lockfree_grow_only_bag& operator=(const lockfree_grow_only_bag&) noexcept = delete;

private:
    struct node_t
    {
        T       value {};
        node_t* next  {nullptr};

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

    static constexpr bool      type_is_supported = std::is_default_constructible<value_type>::value;
    static constexpr bool allocator_is_supported =
        allocator_traits::is_always_equal::value || (
        allocator_traits::propagate_on_container_swap::value &&
        allocator_traits::propagate_on_container_move_assignment::value
        );
    static_assert(     type_is_supported, "Only defaul-constructible value types are supported");
    static_assert(allocator_is_supported, "Only always-equal or propagate-on move-assignment/swap allocators are supported");

private:
    std::atomic<node_t*> m_first {nullptr};

    [[no_unique_address]]
    allocator_type m_allocator;

public:
    lockfree_grow_only_bag(Allocator allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<allocator_type, Allocator>::value
        )
        : m_allocator(allocator)
    {
    }
    ~lockfree_grow_only_bag()
    {
        clear();
    }

    lockfree_grow_only_bag(lockfree_grow_only_bag&& other) noexcept(
        std::is_nothrow_move_constructible<allocator_type>::value
        )
        : m_first    (other.m_first.exchange(nullptr))
        , m_allocator(std::move(other.m_allocator))
    {
    }
    lockfree_grow_only_bag& operator=(lockfree_grow_only_bag&& other) //noexcept(false)
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
            m_first = other.m_first.exchange(nullptr);
        }
        return *this;
    }
    void swap(lockfree_grow_only_bag& other) noexcept
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
            m_first = other.m_first.exchange(m_first);
        }
    }

public:
    bool empty() const noexcept
    {
        return (m_first == nullptr);
    }
    const allocator_type& get_allocator() const noexcept
    {
        return m_allocator;
    }
    allocator_type& get_allocator() noexcept
    {
        return m_allocator;
    }

public:
    void clear() noexcept(false)
    {
        for(node_t* node = m_first.exchange(nullptr); node != nullptr; )
        {
            node_t* next = node->next;

            node->~node_t();

            m_allocator.deallocate(node, 1);

            node = next;
        }
    }

private:
    void insert(node_t* node) noexcept
    {
        node_t* first = m_first;
        do
        {
            node->next = first;
        }
        while(!m_first.compare_exchange_strong(first, node));
    }

public:
    T& add()
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t();

        insert(node);

        return node->value;
    }
    T& add(T&& value)
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t(std::move(value));

        insert(node);

        return node->value;
    }
    T& add(const T& value)
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t(value);

        insert(node);

        return node->value;
    }
    template<typename... Args>
    T& emplace(Args&&... args)
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t(std::forward<Args>(args)...);

        insert(node);

        return node->value;
    }

public:
    template<typename Node, typename Value>
    class iterator_t
    {
        Node* node;
    public:
        iterator_t() noexcept = delete;
       ~iterator_t() noexcept = default;

        iterator_t(Node* n) noexcept : node(n) {}

        iterator_t(      iterator_t&& other) noexcept : node(std::exchange(other.node, nullptr)) {}
        iterator_t(const iterator_t&  other) noexcept : node(              other.node          ) {}

        iterator_t& operator=(      iterator_t&& other) noexcept { if(this != &other) node = std::exchange(other.node, nullptr); return *this; }
        iterator_t& operator=(const iterator_t&  other) noexcept { if(this != &other) node =               other.node          ; return *this; }

        void swap(iterator_t& other) noexcept { if(this != &other) std::swap(node, other.node); }

        bool operator==(const iterator_t& other) const noexcept { return (node == other.node); }
        bool operator!=(const iterator_t& other) const noexcept { return (node != other.node); }

        Value& operator* () noexcept(DIAG_NOEXCEPT) { Assert_NotNullPtr(node); return  node->value; }
        Value* operator->() noexcept(DIAG_NOEXCEPT) { Assert_NotNullPtr(node); return &node->value; }

        void increment() noexcept(DIAG_NOEXCEPT) { Assert_NotNullPtr(node); node = node->next; }

        iterator_t& operator++()    noexcept(DIAG_NOEXCEPT) {                          increment(); return *this; }
        iterator_t  operator++(int) noexcept(DIAG_NOEXCEPT) { iterator_t prev = *this; increment(); return  prev; }
    };
    using       iterator = iterator_t<      node_t,       value_type>;
    using const_iterator = iterator_t<const node_t, const value_type>;

public:
    const_iterator begin() const noexcept
    {
        return const_iterator(m_first);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(nullptr);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(m_first);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(nullptr);
    }

public:
    iterator begin() noexcept
    {
        return iterator(m_first);
    }
    iterator end() noexcept
    {
        return iterator(nullptr);
    }
};

}

namespace std
{
template<typename T, typename Allocator>
void swap(
    gkr::lockfree_grow_only_bag<T, Allocator>& lhs,
    gkr::lockfree_grow_only_bag<T, Allocator>& rhs
    )
    noexcept
{
    lhs.swap(rhs);
}
}
