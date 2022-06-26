#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <utility>
#include <cstring>
#include <cstddef>
#include <algorithm>
#include <type_traits>

#include <gkr/diag/diagnostics.h>

#ifndef __cpp_lib_exchange_function
#include "cpp/lib_exchange_function.h"
#endif
#ifndef __cpp_lib_raw_memory_algorithms
#include "cpp/lib_raw_memory_algorithms.h"
#endif

namespace gkr
{

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
        return (m_producer_tid == thread_id_t()) || (m_producer_tid == std::this_thread::get_id());
    }
    bool this_thread_is_valid_consumer() const noexcept
    {
        const thread_id_t no_thread_id;

        return (m_consumer_tid == thread_id_t()) || (m_consumer_tid == std::this_thread::get_id());
    }

private:
    thread_id_t m_producer_tid;
    thread_id_t m_consumer_tid;
};

class queue_no_synchronization
{
    queue_no_synchronization           (const queue_no_synchronization&) noexcept = delete;
    queue_no_synchronization& operator=(const queue_no_synchronization&) noexcept = delete;

protected:
    queue_no_synchronization() noexcept = default;
   ~queue_no_synchronization() noexcept = default;

    queue_no_synchronization           (queue_no_synchronization&&) noexcept = default;
    queue_no_synchronization& operator=(queue_no_synchronization&&) noexcept = default;

    void swap(queue_no_synchronization&) noexcept
    {
    }

protected:
    void reset(std::size_t)
    {
    }

protected:
    void notify_producer_overtake()
    {
    }
    void notify_producer_take()
    {
    }
    void notify_producer_commit()
    {
    }
    void notify_producer_cancel()
    {
    }

protected:
    void notify_consumer_overtake()
    {
    }
    void notify_consumer_take()
    {
    }
    void notify_consumer_commit()
    {
    }
    void notify_consumer_cancel()
    {
    }

#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
private:
    template<typename Rep, typename Period>
    static constexpr bool has_wait()
    {
        return false;
    }

protected:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>&) noexcept
    {
        static_assert(has_wait<Rep, Period>(), "You cannot wait on this queue - use other queue synchronization class to activate waiting functionality");
        return false;
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>&) noexcept
    {
        static_assert(has_wait<Rep, Period>(), "You cannot wait on this queue - use other queue synchronization class to activate waiting functionality");
        return false;
    }
#endif
};

template<bool MultipleConsumersMultipleProducersSupport, typename Synchronization, typename BaseAllocator>
class basic_lockfree_queue;

template<typename Synchronization, typename BaseAllocator>
class basic_lockfree_queue<false, Synchronization, BaseAllocator> : public Synchronization
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

    static constexpr size_type npos = size_type(-1);

    impl::producer_consumer_threading threading;

private:
    using base_t = Synchronization;

    std::atomic<size_type> m_count {0};

    size_type m_capacity = 0;
    size_type m_tail_pos = 0;
    size_type m_head_pos = 0;

    using thread_id_t = std::thread::id;

    thread_id_t m_producer_tid_owner = thread_id_t();
    thread_id_t m_consumer_tid_owner = thread_id_t();

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator&) noexcept
    {
    }
    ~basic_lockfree_queue() noexcept
    {
    }

    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept
        : base_t   (std::move(other))
        , threading(std::move(other.threading))
        , m_count  (other.m_count.exchange(0))
        , m_capacity(std::exchange(other.m_capacity, 0))
        , m_tail_pos(std::exchange(other.m_tail_pos, 0))
        , m_head_pos(std::exchange(other.m_head_pos, 0))
        , m_producer_tid_owner(std::exchange(other.m_producer_tid_owner, thread_id_t()))
        , m_consumer_tid_owner(std::exchange(other.m_consumer_tid_owner, thread_id_t()))
    {
    }
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept
    {
        base_t::operator=(std::move(other));

        threading = std::move(other.threading);

        m_count   = other.m_count.exchange(0);

        m_capacity = std::exchange(other.m_capacity, 0);
        m_tail_pos = std::exchange(other.m_tail_pos, 0);
        m_head_pos = std::exchange(other.m_head_pos, 0);

        m_producer_tid_owner = std::exchange(other.m_producer_tid_owner, thread_id_t());
        m_consumer_tid_owner = std::exchange(other.m_consumer_tid_owner, thread_id_t());
        return *this;
    }
    void swap(basic_lockfree_queue& other) noexcept
    {
        base_t::swap(other);

        threading.swap(other.threading);

        m_count = other.m_count.exchange(m_count);

        std::swap(m_capacity, other.m_capacity);
        std::swap(m_tail_pos, other.m_tail_pos);
        std::swap(m_head_pos, other.m_head_pos);

        std::swap(m_producer_tid_owner, other.m_producer_tid_owner);
        std::swap(m_consumer_tid_owner, other.m_consumer_tid_owner);
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
        base_t::reset(capacity);

        m_count    = 0;
        m_capacity = capacity;
        m_head_pos = 0;
        m_tail_pos = 0;

        m_producer_tid_owner = thread_id_t();
        m_consumer_tid_owner = thread_id_t();
    }

protected:
    bool can_take_producer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_producer() && (m_producer_tid_owner == thread_id_t()));
    }
    bool can_take_consumer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_consumer() && (m_consumer_tid_owner == thread_id_t()));
    }

protected:
    size_type try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), npos);

        Check_ValidState(m_producer_tid_owner == thread_id_t(), npos);

        if(m_count == m_capacity)
        {
            base_t::notify_producer_overtake();
            return npos;
        }
        base_t::notify_producer_take();

        m_producer_tid_owner = std::this_thread::get_id();

        const size_type index = (m_tail_pos % m_capacity);

        return index;
    }
    bool drop_producer_element_ownership(size_type index, bool push) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_ValidState(m_producer_tid_owner == std::this_thread::get_id(), false);

        Check_Arg_IsValid(index == (m_tail_pos % m_capacity), false);

        m_producer_tid_owner = thread_id_t();

        if(push)
        {
            ++m_tail_pos;
            ++m_count;

            base_t::notify_producer_commit();
        }
        else
        {
            base_t::notify_producer_cancel();
        }
        return true;
    }

protected:
    size_type try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        Check_ValidState(m_consumer_tid_owner == thread_id_t(), npos);

        if(m_count == 0) 
        {
            base_t::notify_consumer_overtake();
            return npos;
        }
        base_t::notify_consumer_take();

        m_consumer_tid_owner = std::this_thread::get_id();

        const size_type index = (m_head_pos % m_capacity);

        return index;
    }
    bool drop_consumer_element_ownership(size_type index, bool pop) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_ValidState(m_consumer_tid_owner == std::this_thread::get_id(), false);

        Check_ValidState(index == (m_head_pos % m_capacity), false);

        m_consumer_tid_owner = thread_id_t();

        if(pop)
        {
            ++m_head_pos;
            --m_count;

            base_t::notify_producer_commit();
        }
        else
        {
            base_t::notify_producer_cancel();
        }
        return true;
    }

#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
protected:
    template<typename Rep, typename Period>
    size_type take_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const size_t index = try_take_producer_element_ownership();

            if(index != npos) return index;

            if(!can_take_producer_element_ownership()) return npos;

            if(!base_t::producer_wait(timeout)) return npos;
        }
    }
    template<typename Rep, typename Period>
    size_type take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const size_t index = try_take_consumer_element_ownership();

            if(index != npos) return index;

            if(!can_take_consumer_element_ownership()) return npos;

            if(!base_t::consumer_wait(timeout)) return npos;
        }
    }
#endif
};
template<typename Synchronization, typename BaseAllocator>
class basic_lockfree_queue<true, Synchronization, BaseAllocator> : public Synchronization
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

    static constexpr size_type npos = size_type(-1);

    impl::producer_consumer_threading threading;

private:
    using base_t = Synchronization;

    using thread_id_t = std::thread::id;

    struct dequeues_entry
    {
        size_type   free_index;
        size_type   busy_index;
        thread_id_t  tid_owner;
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
        :   base_t   (std::move(other))
        ,   threading(std::move(other.  threading))
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

        base_t::operator=(std::move(other));

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
        base_t::swap(other);

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
        base_t::reset(capacity);

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
            m_entries[index] = dequeues_entry{index, 0, thread_id_t()};
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
            base_t::notify_producer_overtake();
            --m_occupied;
            return npos;
        }
        base_t::notify_producer_take();

        const size_type index = m_entries[m_free_head++ % m_capacity].free_index;

        m_entries[index].tid_owner = std::this_thread::get_id();

        return index;
    }
    bool drop_producer_element_ownership(size_type index, bool push) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == std::this_thread::get_id(), false);

        m_entries[index].tid_owner = thread_id_t();

        if(push)
        {
            m_entries[m_busy_tail++ % m_capacity].busy_index = index;

            ++m_available;
            ++m_count;

            base_t::notify_producer_commit();
        }
        else
        {
            m_entries[--m_free_head % m_capacity].free_index = index;

            --m_occupied;

            base_t::notify_producer_cancel();
        }
        return true;
    }

protected:
    size_type try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        if(diff_type(--m_available) < 0)
        {
            base_t::notify_consumer_overtake();
            ++m_available;
            return npos;
        }
        base_t::notify_consumer_take();

        const size_type index = m_entries[m_busy_head++ % m_capacity].busy_index;

        m_entries[index].tid_owner = std::this_thread::get_id();

        return index;
    }
    bool drop_consumer_element_ownership(size_type index, bool pop) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == std::this_thread::get_id(), false);

        m_entries[index].tid_owner = thread_id_t();

        if(pop)
        {
            m_entries[m_free_tail++ % m_capacity].free_index = index;

            --m_occupied;
            --m_count;

            base_t::notify_consumer_commit();
        }
        else
        {
            m_entries[--m_busy_head % m_capacity].busy_index = index;

            ++m_available;

            base_t::notify_consumer_cancel();
        }
        return true;
    }

#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
protected:
    template<typename Rep, typename Period>
    size_type take_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const size_t index = try_take_producer_element_ownership();

            if(index != npos) return index;

            if(!can_take_producer_element_ownership()) return npos;

            if(!base_t::producer_wait(timeout)) return npos;
        }
    }
    template<typename Rep, typename Period>
    size_type take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const size_type index = try_take_consumer_element_ownership();

            if(index != npos) return index;

            if(!can_take_consumer_element_ownership()) return npos;

            if(!base_t::consumer_wait(timeout)) return npos;
        }
    }
#endif
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    bool ResizeSupport=false,
    typename Synchronization=impl::queue_no_synchronization,
    typename BaseAllocator  =std::allocator<char>
    >
class lockfree_queue
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, Synchronization, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<T, MultipleConsumersMultipleProducersSupport, ResizeSupport, Synchronization, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<   MultipleConsumersMultipleProducersSupport,                Synchronization, BaseAllocator>;

public:
    using base_t::npos;

    using element_t = T;
    using size_type = std::size_t;
    using diff_type = std::ptrdiff_t;

    using queue_producer_element_t = queue_producer_element<self_t, element_t>;
    using queue_consumer_element_t = queue_consumer_element<self_t, element_t>;

private:
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<T>;

    TypeAllocator m_allocator;
    element_t*    m_elements = nullptr;

private:
    void clear() noexcept(false)
    {
        if(m_elements != nullptr)
        {
            for(size_type pos = npos, index = 0; index < base_t::capacity(); ++index)
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
            for(size_type pos = npos, index = 0; index < base_t::capacity(); ++index)
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

            for(size_type pos = npos, index = 0; index < base_t::capacity(); ++index)
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
        if(this == &other) return *this;

        clear();
        move_elements(std::move(other));

        base_t::operator=(std::move(other));
        return *this;
    }
    void swap(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if(this == &other) return;

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

public:
    bool resize(size_type capacity) noexcept(false)
    {
        static_assert(ResizeSupport, "Resize support is not active for this queue");
        return false;
    }

public:
    template<typename... Args>
    element_t* try_acquire_producer_element_ex(Args&&... args) noexcept(DIAG_NOEXCEPT && std::is_nothrow_constructible<element_t, Args...>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    element_t* try_acquire_producer_element() noexcept(DIAG_NOEXCEPT && std::is_nothrow_default_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    element_t* try_acquire_producer_element(element_t&& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    element_t* try_acquire_producer_element(const element_t& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<element_t>::value)
    {
        const size_type index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    element_t* try_acquire_consumer_element() noexcept(DIAG_NOEXCEPT)
    {
        const size_type index = base_t::try_take_consumer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

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
        return try_start_pop().pop_in_progress();
    }
    bool try_copy_and_pop(element_t& value) noexcept(
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
    bool try_move_and_pop(element_t& value) noexcept(
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
#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
public:
    template<typename Rep, typename Period, typename... Args>
    element_t* acquire_producer_element_ex(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(DIAG_NOEXCEPT && std::is_nothrow_constructible<element_t, Args...>::value)
    {
        const size_type index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    template<typename Rep, typename Period>
    element_t* acquire_producer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT && std::is_nothrow_default_constructible<element_t>::value)
    {
        const size_type index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    template<typename Rep, typename Period>
    element_t* acquire_producer_element(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<element_t>::value)
    {
        const size_type index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    template<typename Rep, typename Period>
    element_t* acquire_producer_element(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<element_t>::value)
    {
        const size_type index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    template<typename Rep, typename Period>
    element_t* acquire_consumer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const size_type index = base_t::take_consumer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }

public:
    template<typename Rep, typename Period, typename... Args>
    element_t* start_emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(DIAG_NOEXCEPT && std::is_nothrow_constructible<element_t, Args...>::value)
    {
        element_t* element = acquire_producer_element_ex(timeout, std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT && std::is_nothrow_default_constructible<element_t>::value)
    {
        element_t* element = acquire_producer_element(timeout);

        return queue_producer_element_t(*this, element);
    }
    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<element_t>::value)
    {
        element_t* element = acquire_producer_element(timeout, std::move(value));

        return queue_producer_element_t(*this, element);
    }
    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<element_t>::value)
    {
        element_t* element = acquire_producer_element(timeout, value);

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_consumer_element_t start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_consumer_element(timeout);

        return queue_consumer_element_t(*this, element);
    }
public:
    template<typename Rep, typename Period, typename... Args>
    bool emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        return start_emplace(timeout, std::forward<Args>(args)...).push_in_progress();
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        return start_push(timeout).push_in_progress();
    }
    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return start_push(timeout, std::move(value)).push_in_progress();
    }
    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return start_push(timeout, value).push_in_progress();
    }

public:
    template<typename Rep, typename Period>
    bool pop(std::chrono::duration<Rep, Period> timeout) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value
        )
    {
        return start_pop(timeout).pop_in_progress();
    }
    template<typename Rep, typename Period>
    bool copy_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_copy_assignable<element_t>::value
        )
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = consumer_element.value();

        return true;
    }
    template<typename Rep, typename Period>
    bool move_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value &&
        std::is_nothrow_move_assignable<element_t>::value
        )
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = std::move(consumer_element.value());

        return true;
    }
#endif
};

template<
    bool MultipleConsumersMultipleProducersSupport,
    bool ResizeSupport,
    typename Synchronization,
    typename BaseAllocator
    >
class lockfree_queue<void, MultipleConsumersMultipleProducersSupport, ResizeSupport, Synchronization, BaseAllocator>
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, Synchronization, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<void, MultipleConsumersMultipleProducersSupport, ResizeSupport, Synchronization, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<      MultipleConsumersMultipleProducersSupport,                Synchronization, BaseAllocator>;

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
        if(this == &other) return *this;

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
        if(this == &other) return;

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
    bool resize(size_type capacity = npos, size_type size = npos, size_type alignment = npos) noexcept(false)
    {
        static_assert(ResizeSupport, "Resize support is not active for this queue");
        return false;
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
        auto producer_element = try_start_pop();

        if(!producer_element.pop_in_progress()) return false;

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
#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
public:
    template<typename Rep, typename Period>
    void* acquire_producer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_type index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element, typename Rep, typename Period>
    Element* acquire_producer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_producer_element(timeout));
    }

public:
    template<typename Rep, typename Period>
    void* acquire_consumer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_type index = base_t::take_consumer_element_ownership(timeout);

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element, typename Rep, typename Period>
    Element* acquire_consumer_element(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_consumer_element(timeout));
    }

public:
    template<typename Rep, typename Period>
    queue_producer_element<self_t, void> start_push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_producer_element(timeout);

        return queue_producer_element<self_t, void>(*this, element);
    }
    template<class Element, typename Rep, typename Period>
    queue_producer_element<self_t, Element> start_push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        Element* element = acquire_producer_element<Element>(timeout);

        return queue_producer_element<self_t, Element>(*this, element);
    }

    template<typename Rep, typename Period>
    queue_consumer_element<self_t, void> start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_consumer_element(timeout);

        return queue_consumer_element<self_t, void>(*this, element);
    }
    template<class Element, typename Rep, typename Period>
    queue_consumer_element<self_t, Element> start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        Element* element = acquire_consumer_element<Element>(timeout);

        return queue_consumer_element<self_t, Element>(*this, element);
    }

public:
    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const void* data = nullptr, size_type size = npos) noexcept(false)
    {
        auto producer_element = start_push(timeout);

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
    template<typename Rep, typename Period>
    bool pop(std::chrono::duration<Rep, Period> timeout, void* data = nullptr, size_type size = npos) noexcept(false)
    {
        auto producer_element = start_pop(timeout);

        if(!producer_element.pop_in_progress()) return false;

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
#endif
};

}
