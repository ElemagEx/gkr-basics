#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <utility>
#include <cstring>
#include <cstddef>
#include <algorithm>
#include <type_traits>

#include "diagnostics.h"
#include "cpp_feature/lib_exchange_function.h"
#include "cpp_feature/lib_raw_memory_algorithms.h"

namespace gkr
{
namespace policy
{

class producer_consumer_threading
{
    producer_consumer_threading           (const producer_consumer_threading&) noexcept = delete;
    producer_consumer_threading& operator=(const producer_consumer_threading&) noexcept = delete;

public:
    producer_consumer_threading() noexcept = default;
   ~producer_consumer_threading() noexcept = default;

    producer_consumer_threading(producer_consumer_threading&& other) noexcept
        : m_producer_tid(std::exchange(other.m_producer_tid, thread_id_t()))
        , m_consumer_tid(std::exchange(other.m_consumer_tid, thread_id_t()))
    {
    }
    producer_consumer_threading& operator=(producer_consumer_threading&& other) noexcept
    {
        m_producer_tid = std::exchange(other.m_producer_tid, thread_id_t());
        m_consumer_tid = std::exchange(other.m_consumer_tid, thread_id_t());
        return *this;
    }
    void swap(producer_consumer_threading& other) noexcept
    {
        std::swap(m_producer_tid, other.m_producer_tid);
        std::swap(m_consumer_tid, other.m_consumer_tid);
    }

public:
    using thread_id_t = std::thread::id;

    void set_this_thread_as_exclusive_producer() noexcept
    {
        m_producer_tid = std::this_thread::get_id();
    }
    void set_thread_as_exclusive_producer(const std::thread& thread) noexcept
    {
        m_producer_tid = thread.get_id();
    }
    void any_thread_can_be_producer() noexcept
    {
        m_producer_tid = thread_id_t();
    }

    void set_this_thread_as_exclusive_consumer() noexcept
    {
        m_consumer_tid = std::this_thread::get_id();
    }
    void set_thread_as_exclusive_consumer(const std::thread& thread) noexcept
    {
        m_consumer_tid = thread.get_id();
    }
    void any_thread_can_be_consumer() noexcept
    {
        m_consumer_tid = thread_id_t();
    }

public:
    bool this_thread_is_valid_producer() const noexcept
    {
        const thread_id_t no_thread_id;

        return (m_producer_tid == no_thread_id) || (m_producer_tid == std::this_thread::get_id());
    }
    bool this_thread_is_valid_consumer() const noexcept
    {
        const thread_id_t no_thread_id;

        return (m_consumer_tid == no_thread_id) || (m_consumer_tid == std::this_thread::get_id());
    }

private:
    thread_id_t m_producer_tid;
    thread_id_t m_consumer_tid;
};

}

template<typename Queue, typename Element>
class queue_producer_element
{
    queue_producer_element           (const queue_producer_element&) noexcept = delete;
    queue_producer_element& operator=(const queue_producer_element&) noexcept = delete;

public:
    queue_producer_element           (queue_producer_element&&) noexcept = default;
    queue_producer_element& operator=(queue_producer_element&&) noexcept = default;

public:
    void cancel_push() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(push_in_progress())
        {
            m_queue->release_producer_element(std::exchange(m_element, nullptr), false);
        }
    }
    void finish_push() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(push_in_progress())
        {
            m_queue->release_producer_element(std::exchange(m_element, nullptr), true);
        }
    }

public:
    queue_producer_element(Queue& queue, Element* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_producer_element() noexcept(noexcept(finish_push()))
    {
        finish_push();
    }

public:
    bool push_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }
    Element* data() noexcept
    {
        return m_element;
    }
    Element& value() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element& operator*() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element* operator->() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return m_element;
    }

public:
    Element* detach() noexcept
    {
        return std::exchange(m_element, nullptr);
    }

private:
    Queue*   m_queue;
    Element* m_element;
};
template<typename Queue>
class queue_producer_element<Queue, void>
{
    queue_producer_element           (const queue_producer_element&) noexcept = delete;
    queue_producer_element& operator=(const queue_producer_element&) noexcept = delete;

public:
    queue_producer_element           (queue_producer_element&&) noexcept = default;
    queue_producer_element& operator=(queue_producer_element&&) noexcept = default;

public:
    void cancel_push() noexcept(DIAG_NOEXCEPT)
    {
        if(push_in_progress())
        {
            m_queue->release_producer_element(std::exchange(m_element, nullptr), false);
        }
    }
    void finish_push() noexcept(DIAG_NOEXCEPT)
    {
        if(push_in_progress())
        {
            m_queue->release_producer_element(std::exchange(m_element, nullptr), true);
        }
    }

public:
    queue_producer_element(Queue& queue, void* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_producer_element() noexcept(noexcept(finish_push()))
    {
        finish_push();
    }

public:
    bool push_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }
    void* data() noexcept
    {
        return m_element;
    }
    template<typename T>
    T* data() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        return static_cast<T*>(m_element);
    }
    template<typename T>
    T& value() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        Assert_NotNullPtr(m_element);
        return *static_cast<T*>(m_element);
    }

public:
    void* detach() noexcept
    {
        return std::exchange(m_element, nullptr);
    }

private:
    Queue* m_queue;
    void*  m_element;
};

template<typename Queue, typename Element>
class queue_consumer_element
{
    queue_consumer_element           (const queue_consumer_element&) noexcept = delete;
    queue_consumer_element& operator=(const queue_consumer_element&) noexcept = delete;

public:
    queue_consumer_element           (queue_consumer_element&&) noexcept = default;
    queue_consumer_element& operator=(queue_consumer_element&&) noexcept = default;

public:
    void cancel_pop() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(pop_in_progress())
        {
            m_queue->release_consumer_element(std::exchange(m_element, nullptr), false);
        }
    }
    void finish_pop() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(pop_in_progress())
        {
            m_queue->release_consumer_element(std::exchange(m_element, nullptr), true);
        }
    }

public:
    queue_consumer_element(Queue& queue, Element* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_consumer_element() noexcept(noexcept(finish_pop()))
    {
        finish_pop();
    }

public:
    bool pop_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }
    Element* data() noexcept
    {
        return m_element;
    }
    Element& value() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element& operator*() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element* operator->() noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return m_element;
    }

public:
    Element* detach() noexcept
    {
        return std::exchange(m_element, nullptr);
    }

private:
    Queue*   m_queue;
    Element* m_element;
};
template<typename Queue>
class queue_consumer_element<Queue, void>
{
    queue_consumer_element           (const queue_consumer_element&) noexcept = delete;
    queue_consumer_element& operator=(const queue_consumer_element&) noexcept = delete;

public:
    queue_consumer_element           (queue_consumer_element&&) noexcept = default;
    queue_consumer_element& operator=(queue_consumer_element&&) noexcept = default;

public:
    void cancel_pop() noexcept(DIAG_NOEXCEPT)
    {
        if(pop_in_progress())
        {
            m_queue->release_consumer_element(std::exchange(m_element, nullptr), false);
        }
    }
    void finish_pop() noexcept(DIAG_NOEXCEPT)
    {
        if(pop_in_progress())
        {
            m_queue->release_consumer_element(std::exchange(m_element, nullptr), true);
        }
    }

public:
    queue_consumer_element(Queue& queue, void* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_consumer_element() noexcept(noexcept(finish_pop()))
    {
        finish_pop();
    }

public:
    bool pop_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }
    void* data() noexcept
    {
        return m_element;
    }
    template<typename T>
    T* data() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        return static_cast<T*>(m_element);
    }
    template<typename T>
    T& value() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        Assert_NotNullPtr(m_element);
        return *static_cast<T*>(m_element);
    }

public:
    void* detach() noexcept
    {
        return std::exchange(m_element, nullptr);
    }

private:
    Queue* m_queue;
    void*  m_element;
};

namespace impl
{

template<bool MultipleConsumersMultipleProducersSupport, typename BaseAllocator>
class basic_lockfree_queue;

#ifdef RESIZER
template<bool ResizeSupport>
class resizer_helper;
#endif

template<typename BaseAllocator>
class basic_lockfree_queue<false, BaseAllocator>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

    static constexpr size_type npos = size_type(-1);

    policy::producer_consumer_threading threading;

private:
    std::atomic<size_type> m_count {0};

    size_type m_capacity = 0;
    size_type m_tail_pos = 0;
    size_type m_head_pos = 0;

    bool m_producer_owns_tail = false;
    bool m_consumer_owns_head = false;

protected:
    basic_lockfree_queue() noexcept = delete;
   ~basic_lockfree_queue() noexcept = default;

    basic_lockfree_queue(const BaseAllocator&) noexcept
    {
    }
    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept
        : threading(std::move(other.threading))
        , m_count  (other.m_count.exchange(0))
        , m_capacity(std::exchange(other.m_capacity, 0))
        , m_tail_pos(std::exchange(other.m_tail_pos, 0))
        , m_head_pos(std::exchange(other.m_head_pos, 0))
        , m_producer_owns_tail(std::exchange(m_producer_owns_tail, false))
        , m_consumer_owns_head(std::exchange(m_consumer_owns_head, false))
    {
    }
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept
    {
        threading = std::move(other.threading);

        m_count   = other.m_count.exchange(0);

        m_capacity = std::exchange(other.m_capacity, 0);
        m_tail_pos = std::exchange(other.m_tail_pos, 0);
        m_head_pos = std::exchange(other.m_head_pos, 0);

        m_producer_owns_tail = std::exchange(other.m_producer_owns_tail, false);
        m_consumer_owns_head = std::exchange(other.m_consumer_owns_head, false);
        return *this;
    }
    void swap(basic_lockfree_queue& other) noexcept
    {
        threading.swap(other.threading);

        m_count = other.m_count.exchange(m_count);

        std::swap(m_capacity, other.m_capacity);
        std::swap(m_tail_pos, other.m_tail_pos);
        std::swap(m_head_pos, other.m_head_pos);

        std::swap(m_producer_owns_tail, other.m_producer_owns_tail);
        std::swap(m_consumer_owns_head, other.m_consumer_owns_head);
    }

public:
    bool empty() const noexcept
    {
        return (m_count == 0);
    }
    bool full() const noexcept
    {
        return (m_count == m_capacity);
    }
    size_type count() const noexcept
    {
        return m_count;
    }
    size_type capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(size_type index, size_type& pos) const noexcept
    {
        if(index >= m_capacity) return false;

        const size_type tail_index = (m_tail_pos % m_capacity);
        const size_type head_index = (m_head_pos % m_capacity);

        if(head_index < tail_index)
        {
            pos = (index - head_index);

            return ((head_index <= index) && (index < tail_index));
        }
        else
        {
            pos = (index >= head_index)
                ? (index              - head_index)
                : (index + m_capacity - head_index);

            return ((head_index <= index) || (index < tail_index));
        }
    }
    void reset(size_type capacity) noexcept
    {
        m_count    = 0;
        m_capacity = capacity;
        m_head_pos = 0;
        m_tail_pos = 0;

        m_producer_owns_tail = false;
        m_consumer_owns_head = false;
    }

protected:
    diff_type get_free_elements() const noexcept
    {
        return diff_type(m_capacity - m_count);
    }
    bool has_owned_elements() const noexcept
    {
        return (m_producer_owns_tail || m_producer_owns_tail);
    }
    bool can_take_producer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_producer() && !m_producer_owns_tail);
    }
    bool can_take_consumer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_consumer() && !m_consumer_owns_head);
    }

protected:
    size_type try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), npos);

        Check_ValidState(!m_producer_owns_tail, npos);

        if(m_count == m_capacity)
        {
            return npos;
        }
        m_producer_owns_tail = true;

        const size_type index = (m_tail_pos % m_capacity);

        return index;
    }
//  template<typename Rep, typename Period>
//  size_type take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
//  {
//  }
    bool drop_producer_element_ownership(size_type index, bool push) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_ValidState(m_producer_owns_tail, false);

        Check_ValidArg(index == (m_tail_pos % m_capacity), false);

        m_producer_owns_tail = false;

        if(push)
        {
            ++m_tail_pos;
            ++m_count;
        }
        return true;
    }

protected:
    size_type try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        Check_ValidState(!m_consumer_owns_head, npos);

        if(m_count == 0) 
        {
            return npos;
        }

        m_consumer_owns_head = true;

        const size_type index = (m_head_pos % m_capacity);

        return index;
    }
    bool drop_consumer_element_ownership(size_type index, bool pop) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_ValidState(m_consumer_owns_head, false);

        Check_ValidState(index == (m_head_pos % m_capacity), false);

        m_consumer_owns_head = false;

        if(pop)
        {
            ++m_head_pos;
            --m_count;
        }
        return true;
    }
};
template<typename BaseAllocator>
class basic_lockfree_queue<true, BaseAllocator>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

    static constexpr size_type npos = size_type(-1);

    policy::producer_consumer_threading threading;

private:
    struct dequeues_entry
    {
        size_type free_index;
        size_type busy_index;
    };
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<dequeues_entry>;

    TypeAllocator   m_allocator;
    size_type       m_capacity = 0;
    dequeues_entry* m_entries  = nullptr;

    std::atomic<size_type> m_count     {0};
    std::atomic<size_type> m_occupied  {0};
    std::atomic<size_type> m_available {0};

    std::atomic<size_type> m_free_tail {0};
    std::atomic<size_type> m_free_head {0};

    std::atomic<size_type> m_busy_tail {0};
    std::atomic<size_type> m_busy_head {0};

private:
    void clear() noexcept(false)
    {
        if(m_entries != nullptr)
        {
            std::destroy_n(m_entries, m_capacity);

            m_allocator.deallocate(m_entries, m_capacity);
        }
    }

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator& allocator) noexcept(std::is_nothrow_copy_constructible<BaseAllocator>::value)
        : m_allocator(allocator)
    {
    }
    ~basic_lockfree_queue() noexcept(noexcept(clear()))
    {
        clear();
    }

private:
    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    void relocate_entries(dequeues_entry*& entries, TypeAllocator& allocator) noexcept(noexcept(clear()))
    {
        if(m_capacity == 0)
        {
            entries = nullptr;
        }
        else
        {
            entries = allocator.allocate(m_capacity);

            std::uninitialized_move_n(m_entries, m_capacity, entries);

            clear();
            m_entries = nullptr;
        }
    }
    void move_entries(basic_lockfree_queue&& other) noexcept(move_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!move_is_noexcept)
#else
        if          (!move_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_entries(m_entries, m_allocator);
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
#else
        if          (AllocatorTraits::propagate_on_container_move_assignment::value)
#endif
        {
            m_allocator = std::move(other.m_allocator);
        }
        m_entries = std::exchange(other.m_entries, nullptr);
    }
    void swap_entries(basic_lockfree_queue&& other) noexcept(swap_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!swap_is_noexcept)
#else
        if          (!swap_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                dequeues_entry* entries = nullptr;

                this->relocate_entries(  entries, other.m_allocator);
                other.relocate_entries(m_entries,       m_allocator);

                other.m_entries = entries;
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_swap::value)
#else
        if          (AllocatorTraits::propagate_on_container_swap::value)
#endif
        {
            std::swap(m_allocator, other.m_allocator);
        }
        std::swap(m_entries, other.m_entries);
    }

protected:
    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept(std::is_nothrow_move_constructible<TypeAllocator>::value)
        :   threading(std::move(other.  threading))
        , m_allocator(std::move(other.m_allocator))
        , m_capacity (std::exchange(other.m_capacity, 0))
        , m_entries  (std::exchange(other.m_entries , nullptr))
        , m_count    (other.m_count    .exchange(0))
        , m_occupied (other.m_occupied .exchange(0))
        , m_available(other.m_available.exchange(0))
        , m_free_tail(other.m_free_tail.exchange(0))
        , m_free_head(other.m_free_head.exchange(0))
        , m_busy_tail(other.m_busy_tail.exchange(0))
        , m_busy_head(other.m_busy_head.exchange(0))
    {
    }
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept(move_is_noexcept && noexcept(clear()))
    {
        clear();

        threading   = std::move(other.threading);

        m_count     = other.m_count    .exchange(0);
        m_occupied  = other.m_occupied .exchange(0);
        m_available = other.m_available.exchange(0);
        m_free_tail = other.m_free_tail.exchange(0);
        m_free_head = other.m_free_head.exchange(0);
        m_busy_tail = other.m_busy_tail.exchange(0);
        m_busy_head = other.m_busy_head.exchange(0);

        move_entries(std::move(other));

        m_capacity  = std::exchange(other.m_capacity, 0);
        return *this;
    }
    void swap(basic_lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        threading.swap(other.threading);

        m_count     = other.m_count    .exchange(m_count    );
        m_occupied  = other.m_occupied .exchange(m_occupied );
        m_available = other.m_available.exchange(m_available);
        m_free_tail = other.m_free_tail.exchange(m_free_tail);
        m_free_head = other.m_free_head.exchange(m_free_head);
        m_busy_tail = other.m_busy_tail.exchange(m_busy_tail);
        m_busy_head = other.m_busy_head.exchange(m_busy_head);

        swap_entries(std::move(other));

        std::swap(m_capacity, other.m_capacity);
    }

public:
    bool empty() const noexcept
    {
        return (m_count == 0);
    }
    bool full() const noexcept
    {
        return (m_count == m_capacity);
    }
    size_type count() const noexcept
    {
        return m_count;
    }
    size_type capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(size_type index, size_type& pos) const noexcept
    {
        if(index >= m_capacity) return false;

        for(size_type busy_pos = m_busy_head; busy_pos < m_busy_tail; ++busy_pos)
        {
            if(index == m_entries[busy_pos % m_capacity].busy_index)
            {
                pos = (busy_pos - m_busy_head);
                return true;
            }
        }
        return false;
    }
    void reset(size_type capacity) noexcept(false)
    {
        if(capacity != m_capacity)
        {
            clear();

            m_capacity = capacity;

            if(m_capacity == 0)
            {
                m_entries = nullptr;
            }
            else
            {
                m_entries = m_allocator.allocate(m_capacity);

                std::uninitialized_default_construct_n(m_entries, m_capacity);
            }
        }
        for(size_type index = 0; index < m_capacity; ++index)
        {
            m_entries[index].free_index = index;
        }
        m_count     = 0;
        m_occupied  = 0;
        m_available = 0;

        m_busy_head = 0;
        m_busy_tail = 0;

        m_free_head = 0;
        m_free_tail = m_capacity;
    }

protected:
    diff_type get_free_elements() const noexcept
    {
        return diff_type(m_capacity - m_occupied);
    }
    bool has_owned_elements() const noexcept
    {
        const size_type busy_count = (m_busy_tail - m_busy_head);
        const size_type free_count = (m_free_tail - m_free_head);

        const size_type idle_count = busy_count + free_count;

        return (idle_count < m_capacity);
    }
    bool can_take_producer_element_ownership() const noexcept
    {
        return (m_capacity > 0) && threading.this_thread_is_valid_producer();
    }
    bool can_take_consumer_element_ownership() const noexcept
    {
        return (m_capacity > 0) && threading.this_thread_is_valid_consumer();
    }

protected:
    size_type try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), npos);

        if(++m_occupied > m_capacity)
        {
            --m_occupied;
            return npos;
        }

        const size_type index = m_entries[m_free_head++ % m_capacity].free_index;

        return index;
    }
    bool drop_producer_element_ownership(size_type index, bool push) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_ValidArg(index < m_capacity, false);

        if(push)
        {
            m_entries[m_busy_tail++ % m_capacity].busy_index = index;

            ++m_available;
        }
        else
        {
            m_entries[--m_free_head % m_capacity].free_index = index;

            --m_occupied;
        }
        return true;
    }

protected:
    size_type try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        if(diff_type(--m_available) < 0)
        {
            ++m_available;
            return npos;
        }

        const size_type index = m_entries[m_busy_head++ % m_capacity].busy_index;

        return index;
    }
    bool drop_consumer_element_ownership(size_type index, bool pop) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_ValidArg(index < m_capacity, false);

        if(pop)
        {
            m_entries[m_free_tail++ % m_capacity].free_index = index;

            --m_occupied;
        }
        else
        {
            m_entries[--m_busy_head % m_capacity].busy_index = index;

            ++m_available;
        }
        return true;
    }
};

#ifdef RESIZER
class resizer_state
{
    resizer_helper<true>* m_resizer;
    std::size_t           m_capacity;
    std::size_t           m_index;

public:
    bool need_realloc() const
    {
        return (m_capacity > 0);
    }
    std::size_t get_index() const
    {
        return m_index;
    }
    template<bool MultipleConsumersMultipleProducersSupport, typename BaseAllocator>
    void update(basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, BaseAllocator>& basic_queue)
    {
        m_index = m_resizer.update(basic_queue, m_capacity);
    }

    ~resizer_state() noexcept
    {
        m_resizer->deactivate(m_capacity);
    }
};
template<>
class resizer_helper<false>
{
};
template<>
class resizer_helper<true>
{
    std::size_t m_increment = 0;
    std::size_t m_threshold = 0;
    std::size_t m_timeout   = 0;

    std::atomic<bool> m_inactive {false};

private:
    void wait_a_while()
    {
        if(m_timeout == 0)
        {
            std::this_thread::yield();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(m_timeout));
        }
    }
    friend class resizer_state;
    void activate()
    {
    }
    void deactivate(std::size_t capacity)
    {
    }
    template<bool MultipleConsumersMultipleProducersSupport, typename BaseAllocator>
    std::size_t update(basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, BaseAllocator>& basic_queue, std::size_t capacity)
    {
        if(capacity == 0) return npos;

        basic_queue.resize(capacity);

        return basic_queue.try_take_producer_element_ownership();
    }

public:
    template<bool MultipleConsumersMultipleProducersSupport, typename BaseAllocator>
    resizer_state take_producer_element_ownership(basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, BaseAllocator>& basic_queue)
    {
        while(m_inactive)
        {
            wait_a_while();
        }
        if(basic_queue.get_free_elements() > std::diff_type(m_threshold))
        {
            const std::size_t index = basic_queue.try_take_producer_element_ownership();

            return {this, 0, index};
        }
        if(!basic_queue.can_take_producer_element_ownership())
        {
            return {this, 0, npos};
        }

        if(deactivate())
        {
            while(basic_queue.has_owned_elements())
            {
                wait_a_while();
            }
        }
        else
        {
            while(m_inactive)
            {
                wait_a_while();
            }
        }

        const std::size_t capacity =  (m_increment > 0)
            ? (basic_queue.capacity() + m_increment)
            : (basic_queue.capacity() * 2);

        return {this, capacity, npos};
    }
};
#endif
}

template<typename T, bool MultipleConsumersMultipleProducersSupport=false, bool ResizeSupport=false, typename BaseAllocator = std::allocator<char>>
class lockfree_queue : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<T, MultipleConsumersMultipleProducersSupport, ResizeSupport, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<   MultipleConsumersMultipleProducersSupport,                BaseAllocator>;

public:
    using base_t::npos;

    using element_t = T;
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

private:
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<T>;

    TypeAllocator m_allocator;
    element_t*    m_elements = nullptr;

private:
    void clear() noexcept(false)
    {
        if(m_elements != nullptr)
        {
            for(size_type pos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    std::destroy_at(m_elements + index);
                }
            }
            m_allocator.deallocate(m_elements, base_t::capacity());
        }
    }

public:
    void reset(size_type capacity = npos) noexcept(false)
    {
        if(capacity == npos)
        {
            capacity = base_t::capacity();
        }
        if(capacity == base_t::capacity())
        {
            for(size_type pos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    std::destroy_at(m_elements + index);
                }
            }
            base_t::reset(capacity);
            return;
        }

        clear();
        base_t::reset(capacity);

        if(base_t::capacity() == 0)
        {
            m_elements = nullptr;
        }
        else
        {
            m_elements = m_allocator.allocate(base_t::capacity());
        }
    }

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(std::is_nothrow_copy_constructible<BaseAllocator>::value)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(size_type capacity, const TypeAllocator& allocator = TypeAllocator()) noexcept(noexcept(reset()))
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity);
    }
    ~lockfree_queue() noexcept(noexcept(clear()))
    {
        clear();
    }

private:
    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    void relocate_elements(element_t*& elements, TypeAllocator& allocator) noexcept(false)
    {
        if(base_t::capacity() == 0)
        {
            elements = nullptr;
        }
        else
        {
            elements = allocator.allocate(base_t::capacity());

            for(size_type pos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    ::new (elements + index) element_t(std::move(m_elements[index]));

                    std::destroy_at(m_elements + index);
                }
            }
            m_allocator.deallocate(m_elements, base_t::capacity());
            m_elements = nullptr;
        }
    }
    void move_elements(lockfree_queue&& other) noexcept(move_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!move_is_noexcept)
#else
        if          (!move_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_elements(m_elements, m_allocator);
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
#else
        if          (AllocatorTraits::propagate_on_container_move_assignment::value)
#endif
        {
            m_allocator = std::move(other.m_allocator);
        }
        m_elements = std::exchange(other.m_elements, nullptr);
    }
    void swap_elements(lockfree_queue&& other) noexcept(swap_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!swap_is_noexcept)
#else
        if          (!swap_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                element_t* elements = nullptr;

                this->relocate_elements(  elements, other.m_allocator);
                other.relocate_elements(m_elements,       m_allocator);

                other.m_elements = elements;
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
#else
        if          (AllocatorTraits::propagate_on_container_move_assignment::value)
#endif
        {
            std::swap(m_allocator, other.m_allocator);
        }
        std::swap(m_elements, other.m_elements);
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(std::is_nothrow_move_constructible<TypeAllocator>::value)
        : base_t     (std::move(other))
        , m_allocator(std::move(other.m_allocator))
        , m_elements (std::exchange(other.m_elements, nullptr))
    {
    }
    lockfree_queue& operator=(lockfree_queue&& other) noexcept(false)
    {
        if(this == std::addressof(other))
        {
            return *this;
        }
        clear();
        move_elements(std::move(other));

        base_t::operator=(std::move(other));
        return *this;
    }
    void swap(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if(this == std::addressof(other))
        {
            return;
        }
        swap_elements(other);

        base_t::swap(other);
    }

public:
    size_type element_alignment() const noexcept
    {
        return size_type(sizeof(element_t));
    }
    size_type element_size() const noexcept
    {
        return size_type(sizeof(element_t));
    }
#ifdef RESIZER
private:
    constexpr bool resize_noexcept = ResizeSupport ? false : DIAG_NOEXCEPT;

    size_type take_producer_element_ownership() noexcept(resize_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!ResizeSupport)
#else
        if          (!ResizeSupport)
#endif
        {
            return base_t::try_take_producer_element_ownership();
        }

        impl::resizer_state state = m_resizer.take_producer_element_ownership(*this);

        if(state.need_realloc())
        {
            element_t* elements = m_allocator.allocate(capacity);

            for(size_type pos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    ::new (elements + pos) element_t(std::move(m_elements[index]));

                    std::destroy_at(m_elements + index);
                }
            }
            m_allocator.deallocate(m_elements, base_t::capacity());

            m_elements = elements;

            state.update(*this);
        }

        return state.get_index();;
    }
#endif
public:
    template<typename... Args>
    element_t* try_acquire_producer_element_ex(Args&&... args) noexcept(DIAG_NOEXCEPT && std::is_nothrow_constructible<element_t, Args...>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        T* element = (m_elements + index);

        return new (element) T(std::forward<Args>(args)...);
    }
    element_t* try_acquire_producer_element() noexcept(DIAG_NOEXCEPT && std::is_nothrow_default_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        T* element = (m_elements + index);

        return new (element) T();
    }
    element_t* try_acquire_producer_element(T&& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        T* element = (m_elements + index);

        return new (element) T(std::move(value));
    }
    element_t* try_acquire_producer_element(const T& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        T* element = (m_elements + index);

        return new (element) T(value);
    }
    element_t* try_acquire_consumer_element() noexcept(DIAG_NOEXCEPT)
    {
        const size_type index = base_t::try_take_consumer_element_ownership();

        if(index == npos) return nullptr;

        T* element = (m_elements + index);

        return element;
    }

private:
    size_type index_of_element(element_t* element) const noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return npos;

        const size_type index = size_type(element - m_elements);

        Check_ValidState(index < base_t::capacity(), npos);

        return index;
    }

public:
    bool release_producer_element(element_t* element, bool push) noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<element_t>::value)
    {
        const size_type index = index_of_element(element);

        if((index != npos) && !push)
        {
            std::destroy_at(m_elements + index);
        }
        return base_t::drop_producer_element_ownership(index, push);
    }
    bool release_consumer_element(element_t* element, bool pop) noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<element_t>::value)
    {
        const size_type index = index_of_element(element);

        if((index != npos) && pop)
        {
            std::destroy_at(m_elements + index);
        }
        return base_t::drop_consumer_element_ownership(index, pop);
    }

public:
    using queue_producer_element_t = queue_producer_element<self_t, element_t>;
    using queue_consumer_element_t = queue_consumer_element<self_t, element_t>;

    template<typename... Args>
    queue_producer_element_t try_start_emplace(Args&&... args) noexcept(DIAG_NOEXCEPT && std::is_nothrow_constructible<element_t, Args...>::value)
    {
        T* element = try_acquire_producer_element_ex(std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }

    queue_producer_element_t try_start_push() noexcept(DIAG_NOEXCEPT && std::is_nothrow_default_constructible<element_t>::value)
    {
        T* element = try_acquire_producer_element();

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t try_start_push(element_t&& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<element_t>::value)
    {
        T* element = try_acquire_producer_element(std::move(value));

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t try_start_push(const element_t& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<element_t>::value)
    {
        T* element = try_acquire_producer_element(value);

        return queue_producer_element_t(*this, element);
    }

    queue_consumer_element_t try_start_pop() noexcept(DIAG_NOEXCEPT)
    {
        T* element = try_acquire_consumer_element();

        return queue_consumer_element_t(*this, element);
    }

public:
    template<typename... Args>
    bool try_emplace(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        return try_start_emplace(std::forward<Args>(args)...).push_in_progress();
    }

    bool try_push() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        return try_start_push().push_in_progress();
    }
    bool try_push(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return try_start_push(std::move(value)).push_in_progress();
    }
    bool try_push(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return try_start_push(value).push_in_progress();
    }

public:
    bool try_pop() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value
        )
    {
        return try_start_push().pop_in_progress();
    }
    bool try_pop_and_copy(element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_assignable<element_t>::value
        )
    {
        auto consumer_element = try_start_pop();

        if(!consumer_element.pop_in_progress()) return false;

        value = consumer_element.value();

        return true;
    }
    bool try_pop_and_move(element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_move_assignable<element_t>::value
        )
    {
        auto consumer_element = try_start_pop();

        if(!consumer_element.pop_in_progress()) return false;

        value = std::move(consumer_element.value());

        return true;
    }
};

template<bool MultipleConsumersMultipleProducersSupport, bool ResizeSupport, typename BaseAllocator>
class lockfree_queue<void, MultipleConsumersMultipleProducersSupport, ResizeSupport, BaseAllocator> : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<void, MultipleConsumersMultipleProducersSupport, ResizeSupport, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<      MultipleConsumersMultipleProducersSupport,                BaseAllocator>;

public:
    using base_t::npos;

    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

private:
    using alloc_value_t = std::max_align_t;
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<alloc_value_t>;

    TypeAllocator m_allocator;
    char*         m_elements  = nullptr;
    size_type     m_offset    = 0;
    size_type     m_size      = 0;
    size_type     m_alignment = 0;
    size_type     m_padding   = 0;
    size_type     m_stride    = 0;

private:
    static bool is_power_of_2(size_type value) noexcept
    {
        return (value > 0) && ((value & (value - 1)) == 0);
    }
    static size_type calc_alignment(size_type size) noexcept
    {
        size_type alignment;

        for(alignment = 1; (alignment < size) && (alignment < alignof(alloc_value_t)); alignment <<= 1);

        return alignment;
    }
    static size_type calc_offset(char* elements, size_type alignment) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_power_of_2(alignment));

        const size_type displace = reinterpret_cast<size_type>(elements) & (alignment - 1);

        const size_type offset = (displace == 0) ? 0 : (alignment - displace);

        return offset;
    }
    static void calc_stride_and_padding(size_type size, size_type alignment, size_type& stride, size_type& padding) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_power_of_2(alignment));

        if(size <= alignment)
        {
            stride = alignment;
        }
        else if((size & (alignment - 1)) == 0)
        {
            stride = size;
        }
        else
        {
            stride = (size & ~(alignment - 1)) + alignment;
        }
        if(alignment <= alignof(alloc_value_t))
        {
            padding = 0;
        }
        else
        {
            padding = alignment - alignof(alloc_value_t);
        }
    }
    size_type calc_count() const noexcept(DIAG_NOEXCEPT)
    {
        const size_type cb = (base_t::capacity() * m_stride + m_padding);

        Assert_Check((cb % sizeof(alloc_value_t)) == 0);

        const size_type count = (cb / sizeof(alloc_value_t));

        return count;
    }

private:
    void clear() noexcept(false)
    {
        if(m_elements != nullptr)
        {
            const size_type count = calc_count();

            m_elements -= m_offset;
            m_allocator.deallocate(reinterpret_cast<alloc_value_t*>(m_elements), count);
        }
    }

public:
    void reset(size_type capacity = npos, size_type size = npos, size_type alignment = npos) noexcept(false)
    {
        if(capacity == npos)
        {
            capacity = base_t::capacity();
        }
        if(size != npos)
        {
            m_size = size;
        }
        if(alignment != npos)
        {
            m_alignment = alignment;
        }
        if(m_alignment == 0)
        {
            m_alignment = calc_alignment(m_size);
        }
        size_type stride;
        size_type padding;
        calc_stride_and_padding(m_size, m_alignment, stride, padding);

        if((capacity == base_t::capacity()) && (stride == m_stride) && (padding == m_padding))
        {
            base_t::reset(capacity);
            return;
        }

        clear();
        base_t::reset(capacity);

        m_stride  = stride;
        m_padding = padding;

        if(base_t::capacity() == 0)
        {
            m_elements = nullptr;
            m_offset   = 0;
        }
        else
        {
            const size_type count = calc_count();

            m_elements = reinterpret_cast<char*>(m_allocator.allocate(count));
            m_offset   = calc_offset(m_elements, m_alignment);

            m_elements += m_offset;
        }
    }

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(std::is_nothrow_copy_constructible<BaseAllocator>::value)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(size_type capacity, size_type size, const BaseAllocator& allocator = BaseAllocator()) noexcept(noexcept(reset()))
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, 0);
    }
    lockfree_queue(size_type capacity, size_type size, size_type alignment, const BaseAllocator& allocator = BaseAllocator()) noexcept(noexcept(reset()))
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, alignment);
    }
    ~lockfree_queue() noexcept(noexcept(clear()))
    {
        clear();
    }

private:
    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    void relocate_elements(size_type& offset, char*& elements, TypeAllocator& allocator) noexcept(false)
    {
        if(base_t::capacity() == 0)
        {
            elements = nullptr;
        }
        else
        {
            const size_type count = calc_count();

            elements = reinterpret_cast<char*>(allocator.allocate(count));
            offset   = calc_offset(elements, m_alignment);

            elements += offset;

            std::memcpy(elements, m_elements, count * sizeof(alloc_value_t));

            m_elements -= m_offset;
            m_allocator.deallocate(reinterpret_cast<alloc_value_t*>(m_elements), count);
            m_elements = nullptr;
            m_offset   = 0;
        }
    }
    void move_elements(lockfree_queue&& other) noexcept(move_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!move_is_noexcept)
#else
        if          (!move_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_elements(m_offset, m_elements, m_allocator);
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
#else
        if          (AllocatorTraits::propagate_on_container_move_assignment::value)
#endif
        {
            m_allocator = std::move(other.m_allocator);
        }
        m_elements = std::exchange(other.m_elements, nullptr);
        m_offset   = std::exchange(other.m_offset  , 0);
    }
    void swap_elements(lockfree_queue&& other) noexcept(swap_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!swap_is_noexcept)
#else
        if          (!swap_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                size_type offset   = 0;
                char*     elements = nullptr;

                this->relocate_elements(  offset,   elements, other.m_allocator);
                other.relocate_elements(m_offset, m_elements,       m_allocator);

                other.m_elements = elements;
                other.m_offset   = offset;
                return;
            }
        }
#ifdef __cpp_if_constexpr
        if constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
#else
        if          (AllocatorTraits::propagate_on_container_move_assignment::value)
#endif
        {
            std::swap(m_allocator, other.m_allocator);
        }
        std::swap(m_elements, other.m_elements);
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(std::is_nothrow_move_constructible<TypeAllocator>::value)
        : base_t     (std::move(other))
        , m_allocator(std::move(other.m_allocator))
        , m_elements (std::exchange(other.m_elements , nullptr))
        , m_offset   (std::exchange(other.m_offset   , 0))
        , m_size     (std::exchange(other.m_size     , 0))
        , m_alignment(std::exchange(other.m_alignment, 0))
        , m_padding  (std::exchange(other.m_padding  , 0))
        , m_stride   (std::exchange(other.m_stride   , 0))
    {
    }
    lockfree_queue& operator=(lockfree_queue&& other) noexcept(false)
    {
        if(this == std::addressof(other))
        {
            return *this;
        }
        clear();
        move_elements(std::move(other));

        m_size      = std::exchange(other.m_size     , 0);
        m_alignment = std::exchange(other.m_alignment, 0);
        m_padding   = std::exchange(other.m_padding  , 0);
        m_stride    = std::exchange(other.m_stride   , 0);

        base_t::operator=(std::move(other));
        return *this;
    }
    void swap(lockfree_queue&& other) noexcept(swap_is_noexcept)
    {
        if(this == std::addressof(other))
        {
            return;
        }
        swap_elements(other);

        std::swap(m_size     , other.m_size     );
        std::swap(m_alignment, other.m_alignment);
        std::swap(m_padding  , other.m_padding  );
        std::swap(m_stride   , other.m_stride   );

        base_t::swap(other);
    }

public:
    size_type element_alignment() const noexcept
    {
        return m_alignment;
    }
    size_type element_size() const noexcept
    {
        return m_size;
    }

public:
    void* try_acquire_producer_element() noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element>
    Element* try_acquire_producer_element() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_producer_element());
    }

public:
    void* try_acquire_consumer_element() noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_type index = base_t::try_take_consumer_element_ownership();

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element>
    Element* try_acquire_consumer_element() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_consumer_element());
    }

private:
    size_type index_of_element(void* element) const  noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return npos;

        const size_type index = size_type(static_cast<char*>(element) - m_elements) / m_stride;

        Check_ValidState(index < base_t::capacity(), npos);

        return index;
    }

public:
    bool release_producer_element(void* element, bool push) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return false;

        const size_type index = index_of_element(element);

        return base_t::drop_producer_element_ownership(index, push);
    }
    bool release_consumer_element(void* element, bool pop) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return false;

        const size_type index = index_of_element(element);

        return base_t::drop_consumer_element_ownership(index, pop);
    }

public:
    queue_producer_element<self_t, void> try_start_push() noexcept(DIAG_NOEXCEPT)
    {
        void* element = try_acquire_producer_element();

        return queue_producer_element<self_t, void>(*this, element);
    }
    template<typename Element>
    queue_producer_element<self_t, Element> try_start_push() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        Element* element = try_acquire_producer_element<Element>();

        return queue_producer_element<self_t, Element>(*this, element);
    }

    queue_consumer_element<self_t, void> try_start_pop() noexcept(DIAG_NOEXCEPT)
    {
        void* element = try_acquire_consumer_element();

        return queue_consumer_element<self_t, void>(*this, element);
    }
    template<typename Element>
    queue_consumer_element<self_t, Element> try_start_pop() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        Element* element = try_acquire_consumer_element<Element>();

        return queue_consumer_element<self_t, Element>(*this, element);
    }

public:
    bool try_push(const void* data = nullptr, size_type size = npos) noexcept(false)
    {
        auto producer_element = try_start_push();

        if(!producer_element.push_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(producer_element.data(), data, size);
        }
        return true;
    }
    bool try_pop(void* data = nullptr, size_type size = npos) noexcept(false)
    {
        auto producer_element = try_start_push();

        if(!producer_element.push_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(data, producer_element.data(), size);
        }
        return true;
    }
};

}
