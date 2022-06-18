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
        node_t() noexcept = default;

        node_t           (const node_t&) noexcept = delete;
        node_t& operator=(const node_t&) noexcept = delete;

        T      value {};
        next_t next  {nullptr};
    };

public:
    using     value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_t>;

private:
    static constexpr bool move_is_noexcept =
        std::allocator_traits<allocator_type>::is_always_equal::value ||
        std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value
        ;
    static_assert(move_is_noexcept, "Only always-equal or propagate-on-move-assignment allocators are supported");

private:
    next_t         m_first {nullptr};
    allocator_type m_allocator;

    static constexpr bool add_new_is_noexcept = std::is_nothrow_default_constructible<T>::value;
    static constexpr bool   clear_is_noexcept = std::is_nothrow_destructible         <T>::value;

public:
    lockfree_forward_list(Allocator allocator = Allocator()) noexcept(std::is_nothrow_copy_constructible<Allocator>::value)
        : m_allocator(allocator)
    {
    }
    ~lockfree_forward_list() noexcept(clear_is_noexcept)
    {
        clear();
    }
    lockfree_forward_list(lockfree_forward_list&& other) noexcept(std::is_nothrow_move_constructible<allocator_type>::value)
        : m_first    (other.exchange(nullptr))
        , m_allocator(std::move(other.m_allocator))
    {
    }
    lockfree_forward_list& operator=(lockfree_forward_list&& other) noexcept(clear_is_noexcept)
    {
        if(this != &other)
        {
            clear();
            m_first = other.m_first.exchange(nullptr);

#ifdef __cpp_if_constexpr
            if constexpr(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)
#else
            if          (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)
#endif
            {
                m_allocator = std::move(other.m_allocator);
            }
        }
        return *this;
    }

private:
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
    struct iterator
    {
        iterator() noexcept = default;
       ~iterator() noexcept = default;

        iterator(node_t* n) noexcept : node(n) {}

        iterator(      iterator&& other) noexcept : node(std::exchange(other.node, nullptr)) {}
        iterator(const iterator&  other) noexcept : node(              other.node          ) {}

        iterator& operator=(      iterator&& other) noexcept { if(this != &other) node = std::exchange(other.node, nullptr); return *this; }
        iterator& operator=(const iterator&  other) noexcept { if(this != &other) node =               other.node          ; return *this; }

        void swap(iterator& other) noexcept { std::swap(node, other.node); }

        bool operator==(const iterator& other) const noexcept { return (node == other.node); }
        bool operator!=(const iterator& other) const noexcept { return (node != other.node); }

        T& operator* () noexcept { return  node->value; }
        T* operator->() noexcept { return &node->value; }

        iterator& operator++()    noexcept {                        node = node->next; return *this; }
        iterator  operator++(int) noexcept { iterator prev = *this; node = node->next; return  prev; }

        node_t* node = nullptr;
    };

public:
    iterator first() noexcept
    {
        return iterator(reinterpret_cast<node_t*>(std::size_t(-1)));
    }
    iterator begin() noexcept
    {
        return iterator(m_first);
    }
    iterator end() noexcept
    {
        return iterator(nullptr);
    }

private:
    node_t* add_new(next_t& prev) noexcept(add_new_is_noexcept)
    {
        node_t* node = m_allocator.allocate(1);

        new (node) node_t;

        node->next = prev.exchange(node);

        return node;
    }

public:
    bool get_next_or_add_new(iterator& it) noexcept(add_new_is_noexcept)
    {
        if(it == first())
        {
            if(m_first.load() != nullptr)
            {
                it = begin();
                return false;
            }
            else
            {
                it = iterator(add_new(m_first));
                return true;
            }
        }
        else
        {
            if(it.node->next.load() != nullptr)
            {
                ++it;
                return false;
            }
            else
            {
                it = iterator(add_new(it.node->next));
                return true;
            }
        }
    }
};

}
