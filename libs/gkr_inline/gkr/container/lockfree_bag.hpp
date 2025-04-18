#pragma once

#include <memory>
#include <atomic>
#include <utility>
#include <type_traits>

#ifndef GKR_LOCKFREE_BAG_SINGLE_HEADER

#include <gkr/diagnostics.hpp>

#ifndef __cpp_lib_is_swappable
#include <gkr/cpp/lib_is_swappable.hpp>
#endif
#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.hpp>
#endif

#else

#ifndef __cpp_lib_is_swappable
namespace std
{
template<typename T>
struct is_nothrow_swappable
{
    static constexpr bool value =
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_move_assignable<T>::value;
};
}
#endif
#ifndef __cpp_lib_exchange_function
namespace std
{
template<class T, class U=T>
T exchange(T& obj, U&& new_value) noexcept(
    std::is_nothrow_move_constructible<T>::value &&
    std::is_nothrow_assignable<T&, U>::value
    )
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
}
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
#ifndef gkr_attr_no_unique_address
#if __has_cpp_attribute(no_unique_address)
#define gkr_attr_no_unique_address [[no_unique_address]]
#else
#define gkr_attr_no_unique_address
#endif
#endif

namespace gkr
{

//
// Conforms C++11 named requirement: AllocatorAwareContainer
//  https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer
//
template<typename T, typename Allocator=std::allocator<T>>
class lockfree_bag
{
public:
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    struct node_data
    {
        value_type value {};
        node_data* next  {nullptr};

        node_data() noexcept(std::is_nothrow_default_constructible<value_type>::value) = default;
       ~node_data() noexcept(std::is_nothrow_destructible         <value_type>::value) = default;

        node_data           (node_data&&) noexcept = delete;
        node_data& operator=(node_data&&) noexcept = delete;

        node_data           (const node_data&) noexcept = delete;
        node_data& operator=(const node_data&) noexcept = delete;

        node_data(const value_type&  v) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : value(          v ) {}
        node_data(      value_type&& v) noexcept(std::is_nothrow_move_constructible<value_type>::value) : value(std::move(v)) {}

        template<typename... Args>
        node_data(Args&&... args) noexcept(std::is_nothrow_constructible<Args...>::value) : value(std::forward<Args>(args)...) {}
    };

public:
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_data>;

    using allocator_value_type = typename allocator_type::value_type;

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

public:
    static constexpr bool move_is_noexcept = std::is_nothrow_destructible<node_data>::value && (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_move_assignment::value && std::is_nothrow_move_constructible<allocator_type>::value)
        );
    static constexpr bool swap_is_noexcept = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_swap::value && std::is_nothrow_swappable<allocator_type>::value)
        );

private:
    std::atomic<node_data*> m_first {nullptr};

    gkr_attr_no_unique_address allocator_type m_allocator;

private:
    node_data* copy_elements(const node_data* other_first) noexcept(false)
    {
        node_data* new_first = nullptr;

        for(const node_data* src_node = other_first; src_node != nullptr; src_node = src_node->next)
        {
            node_data* new_node = allocator_traits::allocate(m_allocator, 1);

            allocator_traits::construct(m_allocator, new_node, src_node->value);

            new_node->next = new_first;

            new_first = new_node;
        }
        return new_first;
    }
    node_data* move_elements(node_data* other_first) noexcept(false)
    {
        node_data* new_first = nullptr;

        for(node_data* src_node = other_first; src_node != nullptr; src_node = src_node->next)
        {
            node_data* new_node = allocator_traits::allocate(m_allocator, 1);

            allocator_traits::construct(m_allocator, new_node, std::move(src_node->value));

            new_node->next = new_first;

            new_first = new_node;
        }
        return new_first;
    }

public:
    lockfree_bag(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<allocator_type, const Allocator&>::value
        )
        : m_allocator(allocator)
    {
    }
    ~lockfree_bag() noexcept(
        std::is_nothrow_destructible<allocator_type>::value &&
        std::is_nothrow_destructible<     node_data>::value
        )
    {
        clear();
    }

    lockfree_bag(const lockfree_bag& other) noexcept(false)
        : m_allocator(allocator_traits::select_on_container_copy_construction(other.m_allocator))
    {
        m_first = copy_elements(other.m_first);
    }
    lockfree_bag(const lockfree_bag& other, const Allocator& allocator) noexcept(false)
        : m_allocator(allocator)
    {
        m_first = copy_elements(other.m_first);
    }
    lockfree_bag& operator=(const lockfree_bag& other) noexcept(false)
    {
        if(this != &other)
        {
            clear();

            if_constexpr(allocator_traits::propagate_on_container_copy_assignment::value)
            {
                m_allocator = other.m_allocator;
            }

            m_first = copy_elements(other.m_first);
        }
        return *this;
    }

    lockfree_bag(lockfree_bag&& other) noexcept(
        std::is_nothrow_move_constructible<allocator_type>::value
        )
        : m_first    (other.m_first.exchange(nullptr))
        , m_allocator(std::move(other.m_allocator))
    {
    }
    lockfree_bag(lockfree_bag&& other, const Allocator& allocator) noexcept(
        std::is_nothrow_constructible<allocator_type, const Allocator&>::value &&
        allocator_traits::is_always_equal::value
        )
        : m_allocator(allocator)
    {
        if_constexpr(allocator_traits::is_always_equal::value)
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
    lockfree_bag& operator=(lockfree_bag&& other) noexcept(move_is_noexcept)
    {
        if(this != &other)
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
    void swap(lockfree_bag& other) noexcept(swap_is_noexcept)
    {
        if(this != &other)
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
                auto    first = this->move_elements(other.m_first); other.clear();
                other.m_first = other.move_elements(this->m_first); this->clear();

                m_first = first;
            }
        }
    }

public:
    size_type max_size() const noexcept
    {
        return allocator_traits::max_size(m_allocator);
    }
    size_type size() const noexcept
    {
        size_type size = 0;

        for(node_data* node = m_first; node != nullptr; node = node->next) ++size;

        return size;
    }
    bool empty() const noexcept
    {
        return (m_first == nullptr);
    }
    Allocator get_allocator() const noexcept(
        std::is_nothrow_constructible<Allocator, const allocator_type&>::value
        )
    {
        return Allocator(m_allocator);
    }

public:
    const_reference front() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_first);
        return m_first->value;
    }
    reference front() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_first);
        return m_first->value;
    }

public:
    void clear() noexcept(std::is_nothrow_destructible<node_data>::value)
    {
        for(node_data* node = m_first.exchange(nullptr); node != nullptr; )
        {
            node_data* next = node->next;

            allocator_traits::destroy(m_allocator, node);

            allocator_traits::deallocate(m_allocator, node, 1);

            node = next;
        }
    }

public:
    template<typename Node, typename Value>
    class iterator_t
    {
        Node* node {nullptr};

        friend class lockfree_bag;
        iterator_t(Node* n) noexcept : node(n) {}

    public:
        iterator_t() noexcept = default;
       ~iterator_t() noexcept = default;

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
    using       iterator = iterator_t<      node_data,       value_type>;
    using const_iterator = iterator_t<const node_data, const value_type>;

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

public:
    bool operator==(const lockfree_bag& other) const noexcept
    {
        if(this == &other) return true;

        if(size() != other.size()) return false;

        const_iterator  this_begin = this->begin(),  this_end = this->end();
        const_iterator other_begin = other.begin(), other_end = other.end();

        for(const_iterator outer_it = this_begin; outer_it != this_end; ++outer_it)
        {
            const_iterator inner_it = this_begin;

            for( ; inner_it != outer_it; ++inner_it)
            {
                if(*outer_it == *inner_it)
                {
                    inner_it = this_end;
                    break;
                }
            }
            if(inner_it == this_end)
            {
                continue;
            }

            size_type this_count = 1, other_count = 0;

            for(++inner_it; inner_it != this_end; ++inner_it)
            {
                if(*outer_it == *inner_it)
                {
                    ++this_count;
                }
            }
            for(const_iterator other_it = other_begin; other_it != other_end; ++other_it)
            {
                if(*outer_it == *other_it)
                {
                    if(this_count == other_count++)
                    {
                        return false;
                    }
                }
            }
            if(this_count != other_count)
            {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const lockfree_bag& other) const noexcept
    {
        return !operator==(other);
    }

private:
    void add(node_data* node) noexcept
    {
        node_data* first = m_first;
        do
        {
            node->next = first;
        }
        while(!m_first.compare_exchange_strong(first, node));
    }

public:
    iterator insert() noexcept(false)
    {
        node_data* node = allocator_traits::allocate(m_allocator, 1);

        allocator_traits::construct(m_allocator, node);

        add(node);

        return iterator(node);
    }
    iterator insert(value_type&& value) noexcept(false)
    {
        node_data* node = allocator_traits::allocate(m_allocator, 1);

        allocator_traits::construct(m_allocator, node, std::move(value));

        add(node);

        return iterator(node);
    }
    iterator insert(const value_type& value) noexcept(false)
    {
        node_data* node = allocator_traits::allocate(m_allocator, 1);

        allocator_traits::construct(m_allocator, node, value);

        add(node);

        return iterator(node);
    }
    template<typename... Args>
    iterator emplace(Args&&... args) noexcept(false)
    {
        node_data* node = allocator_traits::allocate(m_allocator, 1);

        allocator_traits::construct(m_allocator, node, std::forward<Args>(args)...);

        add(node);

        return iterator(node);
    }

private:
    node_data* remove(node_data* node) noexcept(
        std::is_nothrow_destructible<node_data>::value
        )
    {
        node_data* next = node->next;

        allocator_traits::destroy(m_allocator, node);

        allocator_traits::deallocate(m_allocator, node, 1);

        return next;
    }

public:
    iterator erase(const_iterator pos) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<node_data>::value
        )
    {
        Assert_NotNullPtr(pos.node);

        node_data* prev = nullptr;
        node_data* node = m_first;

        for( ; ; )
        {
            Assert_NotNullPtr(node);

            if(node != pos.node)
            {
                prev = node;
                node = node->next;
                continue;
            }
            node = remove(node);
            break;
        }
        if(prev == nullptr)
        {
            m_first = node;
        }
        else
        {
            prev->next = node;
        }
        return iterator(node);
    }
    iterator erase(iterator pos) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<node_data>::value
        )
    {
        return erase(const_iterator(pos.node));
    }
    iterator erase(const_iterator first, const_iterator last) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<node_data>::value
        )
    {
        Assert_NotNullPtr(first.node);

        node_data* prev = nullptr;
        node_data* node = m_first;

        for( ; ; )
        {
            if(node != first.node)
            {
                Assert_NotNullPtr(node);
                prev = node;
                node = node->next;
                continue;
            }
            while(node != last.node)
            {
                Assert_NotNullPtr(node);
                node = remove(node);
            }
            break;
        }
        if(prev == nullptr)
        {
            m_first = node;
        }
        else
        {
            prev->next = node;
        }
        return iterator(node);
    }
    iterator erase(iterator first, iterator last) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<node_data>::value
        )
    {
        return erase(const_iterator(first.node), const_iterator(last.node));
    }

    size_type erase(const value_type& value) noexcept(
        std::is_nothrow_destructible<node_data>::value
        )
    {
        size_type count = 0;

        node_data* prev = nullptr;
        node_data* node = m_first;

        while(node != nullptr)
        {
            if(!(node->value == value))
            {
                prev = node;
                node = node->next;
            }
            else
            {
                ++count;
                node = remove(node);
                if(prev == nullptr)
                {
                    m_first = node;
                }
                else
                {
                    prev->next = node;
                }
            }
        }
        return count;
    }

public:
    iterator find(const value_type& value) noexcept
    {
        for(iterator it = begin(); it != end(); ++it)
        {
            if(*it == value)
            {
                return it;
            }
        }
        return end();
    }
    const_iterator find(const value_type& value) const noexcept
    {
        for(const_iterator it = begin(); it != end(); ++it)
        {
            if(*it == value)
            {
                return it;
            }
        }
        return end();
    }

    template<class Key>
    iterator find(const Key& key) noexcept
    {
        for(iterator it = begin(); it != end(); ++it)
        {
            if(*it == key)
            {
                return it;
            }
        }
        return end();
    }
    template<class Key>
    const_iterator find(const Key& key) const noexcept
    {
        for(iterator it = begin(); it != end(); ++it)
        {
            if(*it == key)
            {
                return it;
            }
        }
        return end();
    }

public:
    bool contains(const value_type& value) const noexcept
    {
        return (find(value) != end());
    }

    size_type count(const value_type& value) const noexcept
    {
        size_type count = 0;

        for(const_iterator it = begin(); it != end(); ++it)
        {
            if(*it == value)
            {
                ++count;
            }
        }
        return count;
    }
};

}

namespace std
{
template<typename T, typename Allocator>
void swap(
    gkr::lockfree_bag<T, Allocator>& lhs,
    gkr::lockfree_bag<T, Allocator>& rhs
    )
    noexcept(gkr::lockfree_bag<T, Allocator>::swap_is_noexcept)
{
    lhs.swap(rhs);
}
}
