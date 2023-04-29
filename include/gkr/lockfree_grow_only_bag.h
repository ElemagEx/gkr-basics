#pragma once

#include <memory>
#include <atomic>
#include <utility>
#include <type_traits>

#ifndef GKR_LOCKFREE_GROW_ONLY_BAG_SINGLE_HEADER

#include <gkr/diag/diagnostics.h>

#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.h>
#endif

#else

#ifndef __cpp_lib_exchange_function
#error  You must use C++14 or preinclude implementation of std::exchange
#endif

#include <cassert>

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Assert_NotNullPtr
#define Assert_NotNullPtr(cond) assert(cond)
#endif
#ifndef Check_Recovery
#define Check_Recovery(msg)
#endif

#endif

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
#endif

namespace gkr
{

template<typename T, typename Allocator=std::allocator<T>>
class lockfree_grow_only_bag
{
public:
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;

    using difference_type = std::ptrdiff_t;
    using size_type       = std::size_t;

private:
    struct node_type
    {
        value_type value {};
        node_type* next  {nullptr};

        node_type() noexcept(std::is_nothrow_default_constructible<value_type>::value) = default;
       ~node_type() noexcept(std::is_nothrow_destructible         <value_type>::value) = default;

        node_type           (node_type&&) noexcept = delete;
        node_type& operator=(node_type&&) noexcept = delete;

        node_type           (const node_type&) noexcept = delete;
        node_type& operator=(const node_type&) noexcept = delete;

        node_type(const value_type&  v) noexcept(std::is_trivially_copy_constructible<value_type>::value) : value(          v ) {}
        node_type(      value_type&& v) noexcept(std::is_trivially_move_constructible<value_type>::value) : value(std::move(v)) {}

        template<typename... Args>
        node_type(Args&&... args) noexcept(std::is_nothrow_constructible<Args...>::value) : value(std::forward<Args>(args)...) {}
    };

private:
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;

    using allocator_traits = std::allocator_traits<allocator_type>;

private:
    std::atomic<node_type*> m_first {nullptr};

    [[no_unique_address]]
    allocator_type m_allocator;

private:
    node_type* copy_elements(const node_type* other_first) noexcept(false)
    {
        node_type* new_first = nullptr;

        for(const node_type* src_node = other_first; src_node != nullptr; src_node = src_node->next)
        {
            node_type* new_node = m_allocator.allocate(1);

            new (new_node) node_type(src_node->value);

            new_node->next = new_first;

            new_first = new_node;
        }
        return new_first;
    }
    node_type* move_elements(node_type* other_first) noexcept(false)
    {
        node_type* new_first = nullptr;

        for(const node_type* src_node = other_first; src_node != nullptr; src_node = src_node->next)
        {
            node_type* new_node = m_allocator.allocate(1);

            new (new_node) node_type(std::move(src_node->value));

            new_node->next = new_first;

            new_first = new_node;
        }
        return new_first;
    }

public:
    lockfree_grow_only_bag(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<allocator_type, const Allocator&>::value
        )
        : m_allocator(allocator)
    {
    }
    ~lockfree_grow_only_bag() noexcept(
        std::is_nothrow_destructible<allocator_type>::value &&
        std::is_nothrow_destructible<     node_type>::value
        )
    {
        clear();
    }

    lockfree_grow_only_bag(const lockfree_grow_only_bag& other) noexcept(false)
        : m_allocator(allocator_traits::select_on_container_copy_construction(other.m_allocator))
    {
        m_first = copy_elements(other.m_first);
    }
    lockfree_grow_only_bag& operator=(const lockfree_grow_only_bag& other) noexcept(false)
    {
        if(this != std::addressof(other))
        {
            clear();

            if_constexpr(allocator_traits::propagate_on_container_copy_assignment::value)
            {
                m_allocator = allocator_traits::select_on_container_copy_construction(other.m_allocator);
            }

            m_first = copy_elements(other.m_first);
        }
        return *this;
    }

    lockfree_grow_only_bag(lockfree_grow_only_bag&& other) noexcept(
        std::is_nothrow_move_constructible<allocator_type>::value
        )
        : m_first    (other.m_first.exchange(nullptr))
        , m_allocator(std::move(other.m_allocator))
    {
    }
    lockfree_grow_only_bag& operator=(lockfree_grow_only_bag&& other) noexcept(
        std::is_nothrow_destructible<node_type>::value && (
            allocator_traits::is_always_equal::value
            ||
            (allocator_traits::propagate_on_container_move_assignment::value &&
            std::is_nothrow_move_constructible<allocator_type>::value))
        )
    {
        if(this != std::addressof(other))
        {
            clear();

            if_constexpr(allocator_traits::propagate_on_container_move_assignment::value)
            {
                m_allocator = std::move(other.m_allocator);

                m_first = other.m_first.exchange(nullptr);
            }
            else if_constexpr(allocator_traits::is_always_equal::value)
            {
                m_first = other.m_first.exchange(nullptr);
            }
            else if(m_allocator == other.m_allocator)
            {
                m_first = other.m_first.exchange(nullptr);
            }
            else
            {
                m_first = move_elements(other.m_first);

                other.clear();
            }
        }
        return *this;
    }
public:
    static constexpr bool swap_is_noexcept = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_swap::value &&
        std::is_nothrow_move_constructible<allocator_type>::value &&
        std::is_nothrow_move_assignable   <allocator_type>::value )
        );

    void swap(lockfree_grow_only_bag& other) noexcept(swap_is_noexcept)
    {
        if(this != std::addressof(other))
        {
            if_constexpr(allocator_traits::propagate_on_container_swap::value)
            {
                std::swap(m_allocator, other.m_allocator);

                m_first = other.m_first.exchange(m_first);
            }
            else if_constexpr(allocator_traits::is_always_equal::value)
            {
                m_first = other.m_first.exchange(m_first);
            }
            else if(m_allocator == other.m_allocator)
            {
                m_first = other.m_first.exchange(m_first);
            }
            else
            {
                Check_Recovery(
                    "According to the standard"
                    " (see AllocatorAwareContainer at https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)"
                    " this case is undefined behaviour."
                    " Uneffective code follows"
                    );
                auto    first = this->move_elements(other.m_first);
                other.m_first = other.move_elements(this->m_first);

                clear();
                m_first = first;
            }
        }
    }

public:
    static size_type max_size() noexcept {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }
    size_type size() const noexcept
    {
        size_type size = 0;

        for(node_type* node = m_first; node != nullptr; node = node->next) ++size;

        return size;
    }
    bool empty() const noexcept
    {
        return (m_first == nullptr);
    }
    allocator_type get_allocator() const noexcept(std::is_nothrow_copy_constructible<allocator_type>::value)
    {
        return allocator_traits::select_on_container_copy_construction(m_allocator);
    }

public:
    void clear() noexcept(std::is_nothrow_destructible<node_type>::value)
    {
        for(node_type* node = m_first.exchange(nullptr); node != nullptr; )
        {
            node_type* next = node->next;

            node->~node_type();

            node = next;
        }
    }

private:
    void put(node_type* node) noexcept
    {
        node_type* first = m_first;
        do
        {
            node->next = first;
        }
        while(!m_first.compare_exchange_strong(first, node));
    }

public:
    T& add() noexcept(false)
    {
        node_type* node = m_allocator.allocate(1);

        new (node) node_type();

        put(node);

        return node->value;
    }
    T& add(T&& value) noexcept(false)
    {
        node_type* node = m_allocator.allocate(1);

        new (node) node_type(std::move(value));

        put(node);

        return node->value;
    }
    T& add(const T& value) noexcept(false)
    {
        node_type* node = m_allocator.allocate(1);

        new (node) node_type(value);

        put(node);

        return node->value;
    }
    template<typename... Args>
    T& emplace(Args&&... args) noexcept(false)
    {
        node_type* node = m_allocator.allocate(1);

        new (node) node_type(std::forward<Args>(args)...);

        put(node);

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

public:
    using       iterator = iterator_t<      node_type,       value_type>;
    using const_iterator = iterator_t<const node_type, const value_type>;
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
    noexcept(gkr::lockfree_grow_only_bag<T, Allocator>::swap_is_noexcept)
{
    lhs.swap(rhs);
}
}
