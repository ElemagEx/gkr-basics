#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <utility>
#include <cstring>
#include <cstddef>
#include <algorithm>
#include <type_traits>

#ifndef GKR_LOCKFREE_QUEUE_SINGLE_HEADER

#ifndef __cpp_lib_exchange_function
#include "cpp/lib_exchange_function.h"
#endif

#include "diag/diagnostics.h"

#else

#ifndef __cpp_lib_exchange_function
#error  You must use C++14 or preinclude implementation of std::exchange
#endif

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Assert_NotNullPtr
#define Assert_NotNullPtr(ptr)
#endif
#ifndef Assert_Check
#define Assert_Check(cond)
#endif
#ifndef Check_ValidState
#define Check_ValidState(cond, ...)  if(!(cond)) return __VA_ARGS__
#endif
#ifndef Check_Arg_IsValid
#define Check_Arg_IsValid(cond, ...) if(!(cond)) return __VA_ARGS__
#endif

#endif

namespace gkr
{
using std::size_t;

template<typename Queue, typename Element>
class queue_producer_element
{
    queue_producer_element           (const queue_producer_element&) noexcept = delete;
    queue_producer_element& operator=(const queue_producer_element&) noexcept = delete;

public:
    queue_producer_element(queue_producer_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr))
    {
    }
    queue_producer_element& operator=(queue_producer_element&& other) noexcept
    {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

public:
    queue_producer_element(Queue& queue, Element* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_producer_element() noexcept(DIAG_NOEXCEPT)
    {
        finish_push();
    }

public:
    bool push_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }

public:
    void finish_push() noexcept(DIAG_NOEXCEPT)
    {
        if(push_in_progress())
        {
            m_queue->template release_producer_element_ownership<true>(std::exchange(m_element, nullptr));
        }
    }
    void cancel_push() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(push_in_progress())
        {
            m_queue->template release_producer_element_ownership<false>(std::exchange(m_element, nullptr));
        }
    }

public:
    const Element* data() const noexcept
    {
        return m_element;
    }
    const Element& value() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    const Element& operator*() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    const Element* operator->() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return m_element;
    }

public:
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
    queue_producer_element(queue_producer_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr))
    {
    }
    queue_producer_element& operator=(queue_producer_element&& other) noexcept
    {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

public:
    queue_producer_element(Queue& queue, void* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_producer_element() noexcept(DIAG_NOEXCEPT)
    {
        finish_push();
    }

public:
    bool push_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }

public:
    void finish_push() noexcept(DIAG_NOEXCEPT)
    {
        if(push_in_progress())
        {
            m_queue->template release_producer_element_ownership<true>(std::exchange(m_element, nullptr));
        }
    }
    void cancel_push() noexcept(DIAG_NOEXCEPT)
    {
        if(push_in_progress())
        {
            m_queue->template release_producer_element_ownership<false>(std::exchange(m_element, nullptr));
        }
    }

public:
    const void* data() const noexcept
    {
        return m_element;
    }
    template<typename T>
    const T* data() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        return static_cast<const T*>(m_element);
    }
    template<typename T>
    const T& value() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        Assert_NotNullPtr(m_element);
        return *static_cast<const T*>(m_element);
    }

public:
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
    bool resize(size_t size) noexcept(false)
    {
        if(push_in_progress())
        {
            return m_queue->change_element_size(size, &m_element);
        }
        else
        {
            return false;
        }
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
    queue_consumer_element(queue_consumer_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr))
    {
    }
    queue_consumer_element& operator=(queue_consumer_element&& other) noexcept
    {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

public:
    queue_consumer_element(Queue& queue, Element* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_consumer_element() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        finish_pop();
    }

public:
    bool pop_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }

public:
    void finish_pop() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value)
    {
        if(pop_in_progress())
        {
            m_queue->template release_consumer_element_ownership<true>(std::exchange(m_element, nullptr));
        }
    }
    void cancel_pop() noexcept(DIAG_NOEXCEPT)
    {
        if(pop_in_progress())
        {
            m_queue->template release_consumer_element_ownership<false>(std::exchange(m_element, nullptr));
        }
    }

public:
    const Element* data() const noexcept
    {
        return m_element;
    }
    const Element& value() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    const Element& operator*() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    const Element* operator->() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_element);
        return m_element;
    }

public:
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
    queue_consumer_element(queue_consumer_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr))
    {
    }
    queue_consumer_element& operator=(queue_consumer_element&& other) noexcept
    {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

public:
    queue_consumer_element(Queue& queue, void* element) noexcept : m_queue(&queue), m_element(element)
    {
    }
    ~queue_consumer_element() noexcept(DIAG_NOEXCEPT)
    {
        finish_pop();
    }

public:
    bool pop_in_progress() const noexcept
    {
        return (m_element != nullptr);
    }

public:
    void finish_pop() noexcept(DIAG_NOEXCEPT)
    {
        if(pop_in_progress())
        {
            m_queue->template release_consumer_element_ownership<true>(std::exchange(m_element, nullptr));
        }
    }
    void cancel_pop() noexcept(DIAG_NOEXCEPT)
    {
        if(pop_in_progress())
        {
            m_queue->template release_consumer_element_ownership<false>(std::exchange(m_element, nullptr));
        }
    }

public:
    const void* data() const noexcept
    {
        return m_element;
    }
    template<typename T>
    const T* data() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        return static_cast<T*>(m_element);
    }
    template<typename T>
    const T& value() const noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(T) <= m_queue->element_alignment());
        Assert_Check( sizeof(T) <= m_queue->element_size     ());

        Assert_NotNullPtr(m_element);
        return *static_cast<T*>(m_element);
    }

public:
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
    bool resize(size_t size) noexcept(false)
    {
        if(pop_in_progress())
        {
            return m_queue->resize_ex(Queue::npos, size, &m_element);
        }
        else
        {
            return false;
        }
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

class queue_threading
{
    queue_threading           (const queue_threading&) noexcept = delete;
    queue_threading& operator=(const queue_threading&) noexcept = delete;

public:
    queue_threading() noexcept = default;
   ~queue_threading() noexcept = default;

    using thread_id_t = std::thread::id;

    queue_threading(queue_threading&& other) noexcept
        : m_producer_tid(std::exchange(other.m_producer_tid, thread_id_t()))
        , m_consumer_tid(std::exchange(other.m_consumer_tid, thread_id_t()))
    {
    }
    queue_threading& operator=(queue_threading&& other) noexcept
    {
        m_producer_tid = std::exchange(other.m_producer_tid, thread_id_t());
        m_consumer_tid = std::exchange(other.m_consumer_tid, thread_id_t());
        return *this;
    }
    void swap(queue_threading& other) noexcept
    {
        std::swap(m_producer_tid, other.m_producer_tid);
        std::swap(m_consumer_tid, other.m_consumer_tid);
    }

public:
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
        return (m_consumer_tid == thread_id_t()) || (m_consumer_tid == std::this_thread::get_id());
    }

private:
    thread_id_t m_producer_tid;
    thread_id_t m_consumer_tid;
};

class queue_no_wait_support
{
    queue_no_wait_support           (const queue_no_wait_support&) noexcept = delete;
    queue_no_wait_support& operator=(const queue_no_wait_support&) noexcept = delete;

protected:
    queue_no_wait_support() noexcept = default;
   ~queue_no_wait_support() noexcept = default;

    queue_no_wait_support           (queue_no_wait_support&&) noexcept = default;
    queue_no_wait_support& operator=(queue_no_wait_support&&) noexcept = default;

    void swap(queue_no_wait_support&) noexcept
    {
    }

protected:
    void reset(size_t) noexcept
    {
    }
    void resize(size_t) noexcept
    {
    }

protected:
    void notify_producer_ownership_fail() noexcept
    {
    }
    void notify_producer_ownership_start() noexcept
    {
    }
    void notify_producer_ownership_finish() noexcept
    {
    }
    void notify_producer_ownership_cancel() noexcept
    {
    }

protected:
    void notify_consumer_ownership_fail() noexcept
    {
    }
    void notify_consumer_ownership_start() noexcept
    {
    }
    void notify_consumer_ownership_finish() noexcept
    {
    }
    void notify_consumer_ownership_cancel() noexcept
    {
    }

#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
private:
    template<typename Rep, typename Period>
    static constexpr bool has_wait() noexcept
    {
        return false;
    }

protected:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>&) noexcept
    {
        static_assert(has_wait<Rep, Period>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
        return false;
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>&) noexcept
    {
        static_assert(has_wait<Rep, Period>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
        return false;
    }
#endif
};

template<typename WaitSupport>
class queue_pausing : public WaitSupport
{
    queue_pausing           (const queue_pausing&) noexcept = delete;
    queue_pausing& operator=(const queue_pausing&) noexcept = delete;

    using base_t = WaitSupport;

protected:
    using tid_owner_t = long long;

    static constexpr long long initial_ns_to_wait = 1000000U; // 1 millisec

private:
    std::atomic<tid_owner_t> m_tid_paused {0};
    long long                m_ns_to_wait = initial_ns_to_wait;

protected:
    queue_pausing() noexcept = default;
   ~queue_pausing() noexcept = default;

    queue_pausing(queue_pausing&& other) noexcept
        : base_t(other)
        , m_tid_paused(other.m_tid_paused.exchange(0))
        , m_ns_to_wait(std::exchange(other.m_ns_to_wait, initial_ns_to_wait))
    {
    }
    queue_pausing& operator=(queue_pausing&& other) noexcept
    {
        base_t::operator=(std::move(other));

        m_tid_paused = other.m_tid_paused.exchange(0);

        m_ns_to_wait = std::exchange(other.m_ns_to_wait, initial_ns_to_wait);
        return *this;
    }

    void swap(queue_pausing& other) noexcept
    {
        base_t::swap(other);

        m_tid_paused = other.m_tid_paused.exchange(m_tid_paused);

        std::swap(m_ns_to_wait, other.m_ns_to_wait);
    }

protected:
    static tid_owner_t get_current_tid() noexcept
    {
        union { tid_owner_t tid_owner; std::thread::id id; } values {};
        values.id = std::this_thread::get_id();
        return values.tid_owner;
    }

protected:
    bool pause() noexcept
    {
        tid_owner_t expected = 0;

        if(!m_tid_paused.compare_exchange_strong(expected, get_current_tid())) return false;

        return true;
    }
    void resume() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_tid_paused == get_current_tid(), );

        m_tid_paused = 0;
    }

protected:
    void wait_a_while() noexcept
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_ns_to_wait));
    }
    void ensure_not_paused() noexcept
    {
        while(m_tid_paused != 0)
        {
            wait_a_while();
        }
    }

public:
    template<typename Rep, typename Period>
    void set_wait_a_while_duration(std::chrono::duration<Rep, Period> timeout) noexcept
    {
        m_ns_to_wait = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
    }
};

template<bool MultipleConsumersMultipleProducersSupport, typename WaitSupport, typename BaseAllocator>
class basic_lockfree_queue;

template<typename WaitSupport, typename BaseAllocator>
class basic_lockfree_queue<false, WaitSupport, BaseAllocator> : public queue_pausing<WaitSupport>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    static constexpr size_t npos = size_t(-1);

    queue_threading threading;

private:
    using base_t = queue_pausing<WaitSupport>;

    using tid_owner_t = typename base_t::tid_owner_t;

    std::atomic<size_t> m_count {0};

    size_t m_capacity = 0;
    size_t m_tail_pos = 0;
    size_t m_head_pos = 0;

    tid_owner_t m_producer_tid_owner = 0;
    tid_owner_t m_consumer_tid_owner = 0;

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator&) noexcept
    {
    }
    ~basic_lockfree_queue() noexcept
    {
    }

    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept
        : base_t    (std::move(other))
        , threading (std::move(other.threading))
        , m_count   (other.m_count.exchange(0))
        , m_capacity(std::exchange(other.m_capacity, 0))
        , m_tail_pos(std::exchange(other.m_tail_pos, 0))
        , m_head_pos(std::exchange(other.m_head_pos, 0))
        , m_producer_tid_owner(std::exchange(other.m_producer_tid_owner, 0))
        , m_consumer_tid_owner(std::exchange(other.m_consumer_tid_owner, 0))
    {
    }
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept
    {
        base_t::operator=(std::move(other));

        threading = std::move(other.threading);

        m_count = other.m_count.exchange(0);

        m_capacity = std::exchange(other.m_capacity, 0);
        m_tail_pos = std::exchange(other.m_tail_pos, 0);
        m_head_pos = std::exchange(other.m_head_pos, 0);

        m_producer_tid_owner = std::exchange(other.m_producer_tid_owner, 0);
        m_consumer_tid_owner = std::exchange(other.m_consumer_tid_owner, 0);
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
    size_t count() const noexcept
    {
        return m_count;
    }
    size_t capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(size_t index, size_t& pos) const noexcept
    {
        if(index >= m_capacity) return false;

        const size_t tail_index = (m_tail_pos % m_capacity);
        const size_t head_index = (m_head_pos % m_capacity);

        if(head_index <= tail_index)
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
    void reset(size_t capacity) noexcept
    {
        base_t::reset(capacity);

        m_count    = 0;
        m_capacity = capacity;
        m_head_pos = 0;
        m_tail_pos = 0;

        m_producer_tid_owner = 0;
        m_consumer_tid_owner = 0;
    }
    void resize(size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(capacity > m_capacity);

        base_t::resize(capacity);

        m_capacity = capacity;

        m_head_pos = 0;
        m_tail_pos = m_count;
    }

protected:
    size_t try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), npos);

        Check_ValidState(m_producer_tid_owner == 0, npos);

        base_t::ensure_not_paused();

        if(m_count == m_capacity)
        {
            base_t::notify_producer_ownership_fail();
            return npos;
        }
        base_t::notify_producer_ownership_start();

        m_producer_tid_owner = base_t::get_current_tid();

        const size_t index = (m_tail_pos % m_capacity);

        return index;
    }
    template<bool push>
    bool drop_producer_element_ownership(size_t index) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_ValidState(m_producer_tid_owner == base_t::get_current_tid(), false);

        Check_Arg_IsValid(index == (m_tail_pos % m_capacity), false);

        m_producer_tid_owner = 0;

#ifdef __cpp_if_constexpr
        if constexpr(push)
#else
        if          (push)
#endif
        {
            ++m_tail_pos;
            ++m_count;

            base_t::notify_producer_ownership_finish();
        }
        else
        {
            base_t::notify_producer_ownership_cancel();
        }
        return true;
    }

protected:
    size_t try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        Check_ValidState(m_consumer_tid_owner == 0, npos);

        base_t::ensure_not_paused();

        if(m_count == 0)
        {
            base_t::notify_consumer_ownership_fail();
            return npos;
        }
        base_t::notify_consumer_ownership_start();

        m_consumer_tid_owner = base_t::get_current_tid();

        const size_t index = (m_head_pos % m_capacity);

        return index;
    }
    template<bool pop>
    bool drop_consumer_element_ownership(size_t index) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_ValidState(m_consumer_tid_owner == base_t::get_current_tid(), false);

        Check_ValidState(index == (m_head_pos % m_capacity), false);

        m_consumer_tid_owner = 0;

#ifdef __cpp_if_constexpr
        if constexpr(pop)
#else
        if          (pop)
#endif
        {
            ++m_head_pos;
            --m_count;

            base_t::notify_consumer_ownership_finish();
        }
        else
        {
            base_t::notify_consumer_ownership_cancel();
        }
        return true;
    }

protected:
    bool this_thread_owns_elements(size_t count) const noexcept
    {
        const tid_owner_t current_tid = base_t::get_current_tid();

        if(m_producer_tid_owner == current_tid) --count;
        if(m_consumer_tid_owner == current_tid) --count;

        return (count == 0);
    }
    bool some_thread_owns_element() const noexcept
    {
        return (m_producer_tid_owner != 0) || (m_consumer_tid_owner != 0);
    }
    bool some_thread_owns_elements(size_t count) const noexcept
    {
        switch(count)
        {
            default:
            case 0 : return (m_producer_tid_owner != 0) || (m_consumer_tid_owner != 0);//???
            case 1 : return (m_producer_tid_owner == 0) == (m_consumer_tid_owner == 0);//???
            case 2 : return (m_producer_tid_owner == 0) && (m_consumer_tid_owner == 0);//???
        }
    }
#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
protected:
    bool can_take_producer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_producer() && (m_producer_tid_owner == 0));
    }
    bool can_take_consumer_element_ownership() const noexcept
    {
        return ((m_capacity > 0) && threading.this_thread_is_valid_consumer() && (m_consumer_tid_owner == 0));
    }

protected:
    template<typename Rep, typename Period>
    size_t take_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
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
    size_t take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
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
template<typename WaitSupport, typename BaseAllocator>
class basic_lockfree_queue<true, WaitSupport, BaseAllocator> : public queue_pausing<WaitSupport>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    static constexpr size_t npos = size_t(-1);

    queue_threading threading;

private:
    using base_t = queue_pausing<WaitSupport>;

    using tid_owner_t = typename base_t::tid_owner_t;

    struct dequeues_entry
    {
        size_t      free_index;
        size_t      busy_index;
        tid_owner_t  tid_owner;
    };
    static_assert(std::is_trivial<dequeues_entry>::value, "Must be trivial");

    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<dequeues_entry>;

    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    TypeAllocator   m_allocator;
    size_t          m_capacity = 0;
    dequeues_entry* m_entries  = nullptr;

    std::atomic<size_t> m_count     {0};
    std::atomic<size_t> m_occupied  {0};
    std::atomic<size_t> m_available {0};

    std::atomic<size_t> m_free_tail {0};
    std::atomic<size_t> m_free_head {0};

    std::atomic<size_t> m_busy_tail {0};
    std::atomic<size_t> m_busy_head {0};

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator& allocator) noexcept(
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : m_allocator(allocator)
    {
    }
    ~basic_lockfree_queue() noexcept
    {
        clear();
    }

protected:
    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<TypeAllocator>::value
        )
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
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept(move_is_noexcept)
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

private:
    void clear() noexcept
    {
        if(m_entries != nullptr)
        {
            m_allocator.deallocate(m_entries, m_capacity);
        }
    }

private:
    void relocate_entries(dequeues_entry*& entries, TypeAllocator& allocator) noexcept(false)
    {
        if(m_capacity == 0)
        {
            entries = nullptr;
        }
        else
        {
            entries = allocator.allocate(m_capacity);

            std::memcpy(entries, m_entries, m_capacity * sizeof(dequeues_entry));

            m_allocator.deallocate(m_entries, m_capacity);
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
    void swap_entries(basic_lockfree_queue& other) noexcept(swap_is_noexcept)
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

public:
    bool empty() const noexcept
    {
        return (m_count == 0);
    }
    bool full() const noexcept
    {
        return (m_count == m_capacity);
    }
    size_t count() const noexcept
    {
        return m_count;
    }
    size_t capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(size_t index, size_t& pos) const noexcept
    {
        if(index >= m_capacity) return false;

        for(size_t busy_pos = m_busy_head; busy_pos < m_busy_tail; ++busy_pos)
        {
            if(index == m_entries[busy_pos % m_capacity].busy_index)
            {
                pos = (busy_pos - m_busy_head);
                return true;
            }
        }
        return false;
    }
    void reset(size_t capacity) noexcept(false)
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
            }
        }
        for(size_t index = 0; index < m_capacity; ++index)
        {
            m_entries[index] = dequeues_entry{index, 0, 0};
        }
        m_count     = 0;
        m_occupied  = 0;
        m_available = 0;

        m_busy_head = 0;
        m_busy_tail = 0;

        m_free_head = 0;
        m_free_tail = m_capacity;
    }
    void resize(size_t capacity) noexcept(false)
    {
        Assert_Check(capacity > m_capacity);

        base_t::resize(capacity);

        clear();

        dequeues_entry* entries = m_allocator.allocate(capacity);

        const size_t count = m_count;

        for(size_t index = 0; index < capacity; ++index)
        {
            entries[index] = dequeues_entry{index + count, index, 0};
        }
        m_busy_head = 0;
        m_busy_tail = count;

        m_free_head = 0;
        m_free_tail = capacity - count;

        m_capacity  = capacity;
    }

protected:
    size_t try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), npos);

        base_t::ensure_not_paused();

        if(++m_occupied > m_capacity)
        {
            --m_occupied;
            base_t::notify_producer_ownership_fail();
            return npos;
        }
        base_t::notify_producer_ownership_start();

        const size_t index = m_entries[m_free_head++ % m_capacity].free_index;

        m_entries[index].tid_owner = base_t::get_current_tid();

        return index;
    }
    template<bool push>
    bool drop_producer_element_ownership(size_t index) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == base_t::get_current_tid(), false);

        m_entries[index].tid_owner = 0;

#ifdef __cpp_if_constexpr
        if constexpr(push)
#else
        if          (push)
#endif
        {
            m_entries[m_busy_tail++ % m_capacity].busy_index = index;

            ++m_available;
            ++m_count;

            base_t::notify_producer_ownership_finish();
        }
        else
        {
            m_entries[--m_free_head % m_capacity].free_index = index;

            --m_occupied;

            base_t::notify_producer_ownership_cancel();
        }
        return true;
    }

protected:
    size_t try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), npos);

        base_t::ensure_not_paused();

        if(std::ptrdiff_t(--m_available) < 0)
        {
            ++m_available;
            base_t::notify_consumer_ownership_fail();
            return npos;
        }
        base_t::notify_consumer_ownership_start();

        const size_t index = m_entries[m_busy_head++ % m_capacity].busy_index;

        m_entries[index].tid_owner = base_t::get_current_tid();

        return index;
    }
    template<bool pop>
    bool drop_consumer_element_ownership(size_t index) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == base_t::get_current_tid(), false);

        m_entries[index].tid_owner = 0;

#ifdef __cpp_if_constexpr
        if constexpr(pop)
#else
        if          (pop)
#endif
        {
            m_entries[m_free_tail++ % m_capacity].free_index = index;

            --m_occupied;
            --m_count;

            base_t::notify_consumer_ownership_finish();
        }
        else
        {
            m_entries[--m_busy_head % m_capacity].busy_index = index;

            ++m_available;

            base_t::notify_consumer_ownership_cancel();
        }
        return true;
    }

protected:
    bool this_thread_owns_elements(size_t count) const noexcept
    {
        const tid_owner_t current_tid = base_t::get_current_tid();

        for(size_t index = 0; index < m_capacity; ++index)
        {
            if(m_entries[index].tid_owner == current_tid)
            {
                if(std::ptrdiff_t(--count) < 0)
                {
                    return false;
                }
            }
        }
        return (count == 0);
    }
    bool some_thread_owns_element() const noexcept
    {
        const size_t busy_count = (m_busy_tail - m_busy_head);
        const size_t free_count = (m_free_tail - m_free_head);

        const size_t idle_count = busy_count + free_count;

        return ((m_capacity - idle_count) == 0);
    }
    bool some_thread_owns_elements(size_t count) const noexcept
    {
        const size_t busy_count = (m_busy_tail - m_busy_head);
        const size_t free_count = (m_free_tail - m_free_head);

        const size_t idle_count = busy_count + free_count;

        return ((m_capacity - idle_count) == count);
    }
#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
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
    template<typename Rep, typename Period>
    size_t take_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
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
    size_t take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
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

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    typename WaitSupport  =impl::queue_no_wait_support,
    typename BaseAllocator=std::allocator<char>
    >
class lockfree_queue
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<   MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;

public:
    using base_t::npos;

    using element_t = T;

    using queue_producer_element_t = queue_producer_element<self_t, element_t>;
    using queue_consumer_element_t = queue_consumer_element<self_t, element_t>;

private:
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<T>;

    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    TypeAllocator m_allocator;
    element_t*    m_elements = nullptr;

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(size_t capacity, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity);
    }
    ~lockfree_queue() noexcept(std::is_nothrow_destructible<element_t>::value)
    {
        clear(true);
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<TypeAllocator>::value
        )
        : base_t     (std::move(other))
        , m_allocator(std::move(other.m_allocator))
        , m_elements (std::exchange(other.m_elements, nullptr))
    {
    }
    lockfree_queue& operator=(lockfree_queue&& other) noexcept(
        move_is_noexcept &&
        std::is_nothrow_destructible<element_t>::value
        )
    {
        if(this != &other)
        {
            clear(true);
            move_elements(std::move(other));

            base_t::operator=(std::move(other));
        }
        return *this;
    }
    void swap(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if(this != &other)
        {
            swap_elements(other);

            base_t::swap(other);
        }
    }

private:
    void destroy_element(size_t index) noexcept(std::is_nothrow_destructible<element_t>::value)
    {
        m_elements[index].~element_t();
    }
    void clear(bool deallocate) noexcept(std::is_nothrow_destructible<element_t>::value)
    {
        if(m_elements != nullptr)
        {
            for(size_t pos = npos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    destroy_element(index);
                }
            }
            if(deallocate)
            {
                m_allocator.deallocate(m_elements, base_t::capacity());
            }
        }
    }

private:
    void relocate_elements(element_t*& elements, TypeAllocator& allocator) noexcept(false)
    {
        if(base_t::capacity() == 0)
        {
            elements = nullptr;
        }
        else
        {
            elements = allocator.allocate(base_t::capacity());

            for(size_t pos = npos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    new (elements + index) element_t(std::move(m_elements[index]));

                    destroy_element(index);
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
    void swap_elements(lockfree_queue& other) noexcept(swap_is_noexcept)
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
    size_t element_alignment() const noexcept
    {
        return alignof(element_t);
    }
    size_t element_size() const noexcept
    {
        return sizeof(element_t);
    }

public:
    void reset(size_t capacity = npos) noexcept(false)
    {
        if(capacity == npos)
        {
            capacity = base_t::capacity();
        }
        const bool reallocate = (capacity != base_t::capacity());

        clear(reallocate);
        base_t::reset(capacity);

        if(reallocate)
        {
            if(base_t::capacity() == 0)
            {
                m_elements = nullptr;
            }
            else
            {
                m_elements = m_allocator.allocate(base_t::capacity());
            }
        }
    }

public:
    bool resize(size_t capacity) noexcept(false)
    {
        if(capacity <= base_t::capacity()) return true;

        Check_ValidState(base_t::this_thread_owns_elements(0), false);

        if(!base_t::pause())
        {
            return false;
        }
        while(base_t::some_thread_owns_element())
        {
            base_t::wait_a_while();
        }
        element_t* elements = m_allocator.allocate(capacity);

        for(size_t pos = npos, index = 0; index < base_t::capacity(); ++index)
        {
            if(base_t::element_has_value(index, pos))
            {
                new (elements + pos) element_t(std::move(m_elements[index]));

                destroy_element(index);
            }
        }
        m_allocator.deallocate(m_elements, base_t::capacity());

        m_elements = elements;

        base_t::resize(capacity);
        base_t::resume();
        return true;
    }

public:
    template<typename... Args>
    element_t* try_acquire_producer_element_ownership_ex(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        const size_t index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    element_t* try_acquire_producer_element_ownership() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        const size_t index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    element_t* try_acquire_producer_element_ownership(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        const size_t index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    element_t* try_acquire_producer_element_ownership(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        const size_t index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    element_t* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::try_take_consumer_element_ownership();

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }

private:
    size_t index_of_element(element_t* element) const noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return npos;

        const size_t index = size_t(element - m_elements);

        Check_ValidState(index < base_t::capacity(), npos);

        return index;
    }

public:
    template<bool push>
    bool release_producer_element_ownership(element_t* element) noexcept(
        DIAG_NOEXCEPT &&
        (push || std::is_nothrow_destructible<element_t>::value)
        )
    {
        const size_t index = index_of_element(element);

#ifdef __cpp_if_constexpr
        if constexpr(!push)
#else
        if          (!push)
#endif
        {
            if(index != npos)
            {
                destroy_element(index);
            }
        }
        return base_t::template drop_producer_element_ownership<push>(index);
    }
    template<bool pop>
    bool release_consumer_element_ownership(element_t* element) noexcept(
        DIAG_NOEXCEPT &&
        (!pop || std::is_nothrow_destructible<element_t>::value)
        )
    {
        const size_t index = index_of_element(element);

#ifdef __cpp_if_constexpr
        if constexpr(pop)
#else
        if          (pop)
#endif
        {
            if(index != npos)
            {
                destroy_element(index);
            }
        }
        return base_t::template drop_consumer_element_ownership<pop>(index);
    }

public:
    template<typename... Args>
    queue_producer_element_t try_start_emplace(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        T* element = try_acquire_producer_element_ownership_ex(std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }

    queue_producer_element_t try_start_push() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        T* element = try_acquire_producer_element_ownership();

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t try_start_push(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        T* element = try_acquire_producer_element_ownership(std::move(value));

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t try_start_push(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        T* element = try_acquire_producer_element_ownership(value);

        return queue_producer_element_t(*this, element);
    }

    queue_consumer_element_t try_start_pop() noexcept(
        DIAG_NOEXCEPT
        )
    {
        T* element = try_acquire_consumer_element_ownership();

        return queue_consumer_element_t(*this, element);
    }

public:
    template<typename... Args>
    bool try_emplace(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        return try_start_emplace(std::forward<Args>(args)...).push_in_progress();
    }

    bool try_push() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        return try_start_push().push_in_progress();
    }
    bool try_push(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return try_start_push(std::move(value)).push_in_progress();
    }
    bool try_push(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
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
    element_t* acquire_producer_element_ownership_ex(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    template<typename... Args>
    element_t* acquire_producer_element_ownership_ex(Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership_ex(std::chrono::nanoseconds::max(), std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    element_t* acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership(std::chrono::nanoseconds::max());
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    element_t* acquire_producer_element_ownership(element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership(std::chrono::nanoseconds::max(), std::move(value));
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    element_t* acquire_producer_element_ownership(const element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership(std::chrono::nanoseconds::max(), value);
    }

    template<typename Rep, typename Period>
    element_t* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const size_t index = base_t::take_consumer_element_ownership(timeout);

        if(index == npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }
    element_t* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_consumer_element_ownership(std::chrono::nanoseconds::max());
    }

public:
    template<typename Rep, typename Period, typename... Args>
    element_t* start_emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_producer_element_ownership_ex(timeout, std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }
    template<typename... Args>
    element_t* start_emplace(Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        return start_emplace(std::chrono::nanoseconds::max(), std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_producer_element_ownership(timeout);

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push() noexcept(DIAG_NOEXCEPT)
    {
        return start_push(std::chrono::nanoseconds::max());
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_producer_element_ownership(timeout, std::move(value));

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push(element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        return start_push(std::chrono::nanoseconds::max(), std::move(value));
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_producer_element_ownership(timeout, value);

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push(const element_t& value)
    {
        return start_push(std::chrono::nanoseconds::max(), value);
    }

    template<typename Rep, typename Period>
    queue_consumer_element_t start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_consumer_element_ownership(timeout);

        return queue_consumer_element_t(*this, element);
    }
    queue_consumer_element_t start_pop()
    {
        return start_pop(std::chrono::nanoseconds::max());
    }

public:
    template<typename Rep, typename Period, typename... Args>
    bool emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        return start_emplace(timeout, std::forward<Args>(args)...).push_in_progress();
    }
    template<typename... Args>
    bool emplace(Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        return emplace(std::chrono::nanoseconds::max(), std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        return start_push(timeout).push_in_progress();
    }
    bool push() noexcept(DIAG_NOEXCEPT)
    {
        return push(std::chrono::nanoseconds::max());
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        return start_push(timeout, std::move(value)).push_in_progress();
    }
    bool push(element_t&& value) noexcept(DIAG_NOEXCEPT)
    {
        return push(std::chrono::nanoseconds::max(), std::move(value));
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        return start_push(timeout, value).push_in_progress();
    }
    bool push(const element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        return push(std::chrono::nanoseconds::max(), value);
    }

public:
    template<typename Rep, typename Period>
    bool pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        return start_pop(timeout).pop_in_progress();
    }
    bool pop() noexcept(DIAG_NOEXCEPT)
    {
        return pop(std::chrono::nanoseconds::max());
    }

    template<typename Rep, typename Period>
    bool copy_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = consumer_element.value();

        return true;
    }
    bool copy_and_pop(element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        return copy_and_pop(std::chrono::nanoseconds::max(), value);
    }

    template<typename Rep, typename Period>
    bool move_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = std::move(consumer_element.value());

        return true;
    }
    bool move_and_pop(element_t& value) noexcept(DIAG_NOEXCEPT)
    {
        return move_and_pop(std::chrono::nanoseconds::max(), value);
    }
#endif
};
template<
    bool MultipleConsumersMultipleProducersSupport,
    typename WaitSupport,
    typename BaseAllocator
    >
class lockfree_queue<void, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<void, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;
    using base_t = impl::basic_lockfree_queue<      MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;

public:
    using base_t::npos;

    using queue_producer_element_t = queue_producer_element<self_t, void>;
    using queue_consumer_element_t = queue_consumer_element<self_t, void>;

private:
    using alloc_value_t = std::max_align_t;
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<alloc_value_t>;

    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_move_assignment::value;
    static constexpr bool swap_is_noexcept = AllocatorTraits::is_always_equal::value || AllocatorTraits::propagate_on_container_swap::value;

    TypeAllocator m_allocator;
    char*         m_elements  = nullptr;
    size_t        m_offset    = 0;
    size_t        m_size      = 0;
    size_t        m_alignment = 0;
    size_t        m_padding   = 0;
    size_t        m_stride    = 0;

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(size_t capacity, size_t size, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, 0);
    }
    lockfree_queue(size_t capacity, size_t size, size_t alignment, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, alignment);
    }

    ~lockfree_queue() noexcept(DIAG_NOEXCEPT)
    {
        clear();
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<TypeAllocator>::value
        )
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
        if(this != &other)
        {
            clear();
            move_elements(std::move(other));

            m_size      = std::exchange(other.m_size     , 0);
            m_alignment = std::exchange(other.m_alignment, 0);
            m_padding   = std::exchange(other.m_padding  , 0);
            m_stride    = std::exchange(other.m_stride   , 0);

            base_t::operator=(std::move(other));
        }
        return *this;
    }
    void swap(lockfree_queue&& other) noexcept(swap_is_noexcept)
    {
        if(this != &other)
        {
            swap_elements(other);

            std::swap(m_size     , other.m_size     );
            std::swap(m_alignment, other.m_alignment);
            std::swap(m_padding  , other.m_padding  );
            std::swap(m_stride   , other.m_stride   );

            base_t::swap(other);
        }
    }

private:
    static bool is_power_of_2(size_t value) noexcept
    {
        return (value > 0) && ((value & (value - 1)) == 0);
    }
    static size_t calc_alignment(size_t size) noexcept
    {
        size_t alignment;

        for(alignment = 1; (alignment < size) && (alignment < alignof(alloc_value_t)); alignment <<= 1);

        return alignment;
    }
    static size_t calc_offset(char* elements, size_t alignment) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_power_of_2(alignment));

        const size_t displace = reinterpret_cast<size_t>(elements) & (alignment - 1);

        const size_t offset = (displace == 0) ? 0 : (alignment - displace);

        return offset;
    }
    static void calc_stride_and_padding(size_t size, size_t alignment, size_t& stride, size_t& padding) noexcept(DIAG_NOEXCEPT)
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
    static size_t calc_count(size_t capacity, size_t stride, size_t padding) noexcept(DIAG_NOEXCEPT)
    {
        const size_t cb = (capacity * stride + padding);

        Assert_Check((cb % sizeof(alloc_value_t)) == 0);

        const size_t count = (cb / sizeof(alloc_value_t));

        return count;
    }

private:
    void clear() noexcept(DIAG_NOEXCEPT)
    {
        if(m_elements != nullptr)
        {
            const size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

            m_elements -= m_offset;
            m_allocator.deallocate(reinterpret_cast<alloc_value_t*>(m_elements), count);
        }
    }

private:
    void relocate_elements(size_t& offset, char*& elements, TypeAllocator& allocator) noexcept(false)
    {
        if(base_t::capacity() == 0)
        {
            elements = nullptr;
        }
        else
        {
            const size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

            elements = reinterpret_cast<char*>(allocator.allocate(count));
            offset   = calc_offset(elements, m_alignment);

            elements += offset;

            std::memcpy(elements, m_elements, base_t::capacity() * m_stride);

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
    void swap_elements(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
#ifdef __cpp_if_constexpr
        if constexpr(!swap_is_noexcept)
#else
        if          (!swap_is_noexcept)
#endif
        {
            if(m_allocator != other.m_allocator)
            {
                size_t offset   = 0;
                char*  elements = nullptr;

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
    size_t element_alignment() const noexcept
    {
        return m_alignment;
    }
    size_t element_size() const noexcept
    {
        return m_size;
    }

public:
    void reset(size_t capacity = npos, size_t size = npos, size_t alignment = npos) noexcept(false)
    {
        Check_Arg_IsValid((alignment == npos) || (alignment == 0) || is_power_of_2(alignment), );

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
        size_t stride;
        size_t padding;
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
            const size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

            m_elements = reinterpret_cast<char*>(m_allocator.allocate(count));
            m_offset   = calc_offset(m_elements, m_alignment);

            m_elements += m_offset;
        }
    }

public:
    bool resize(size_t capacity) noexcept(false)
    {
        if(capacity <= base_t::capacity()) return true;

        Check_ValidState(base_t::this_thread_owns_elements(0), false);

        if(!base_t::pause())
        {
            return false;
        }
        while(base_t::some_thread_owns_element())
        {
            base_t::wait_a_while();
        }
        const size_t count = calc_count(capacity, m_stride, m_padding);

        char*  elements = reinterpret_cast<char*>(m_allocator.allocate(count));
        size_t offset   = calc_offset(elements, m_alignment);

        elements += offset;

        for(size_t pos = npos, index = 0; index < base_t::capacity(); ++index)
        {
            if(base_t::element_has_value(index, pos))
            {
                std::memcpy(elements + pos * m_stride, m_elements + index * m_stride, m_stride);
            }
        }
        clear();

        m_elements = elements;
        m_offset   = offset;

        base_t::resize(capacity);
        base_t::resume();
        return true;
    }
    bool change_element_size(size_t size, void** owned_elements = nullptr, size_t count_elements = 1) noexcept(false)
    {
        if(size <= m_size) return true;

        if(owned_elements == nullptr)
        {
            count_elements = 0;
        }
        Check_ValidState(base_t::this_thread_owns_elements(count_elements), false);

        size_t pos = npos;
        Check_Arg_Array(index, count_elements, base_t::element_has_value(index_of_element(owned_elements[index]), pos), false);

        if(!base_t::pause())
        {
            return false;
        }
        while(base_t::some_thread_owns_elements(count_elements))
        {
            base_t::wait_a_while();
        }

        size_t stride;
        size_t padding;
        calc_stride_and_padding(size, m_alignment, stride, padding);

        Assert_Check(padding == m_padding);

        if(stride != m_stride)
        {
            const size_t count = calc_count(base_t::capacity(), stride, padding);

            char*  elements = reinterpret_cast<char*>(m_allocator.allocate(count));
            size_t offset   = calc_offset(elements, m_alignment);

            elements += offset;

            Assert_Check(stride >= m_stride);

            for(size_t index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    std::memcpy(elements + index * stride, m_elements + index * m_stride, stride);
                }
            }
            for( ; count_elements-- > 0; ++owned_elements)
            {
                const size_t index = index_of_element(*owned_elements);

                *owned_elements = elements + index * stride;
            }
            clear();

            m_elements = elements;
            m_offset   = offset;
            m_stride   = stride;
        }
        m_size = size;
        base_t::resume();
        return true;
    }
    bool resize_ex(size_t capacity = npos, size_t size = npos, void** owned_elements = nullptr, size_t count_elements = 1) noexcept(false)
    {
        //
        // BUG:Both MPMC version of base class resize(...) does not preserve tid_owner_t value of owned elements
        //
        Check_Arg_IsValid((capacity == npos) || (capacity > base_t::capacity()), false);
        Check_Arg_IsValid((size     == npos) || (size     > m_size)            , false);

        if(owned_elements == nullptr)
        {
            count_elements = 0;
        }
        Check_ValidState(base_t::this_thread_owns_elements(count_elements), false);

        //TODO:begin - move this begin/end section after next while statement
        size_t pos = npos;

        Check_Arg_Array(index, count_elements, base_t::element_has_value(index_of_element(owned_elements[index]), pos), false);

        if(capacity == npos) capacity = base_t::capacity();
        if(size     == npos) size     = m_size;

        const bool capacity_changed = (capacity != base_t::capacity());

        size_t stride;
        size_t padding;
        calc_stride_and_padding(size, m_alignment, stride, padding);

        if(!capacity_changed && (stride == m_stride) && (padding == m_padding))
        {
            m_size = size;
            return true;
        }
        //TODO:end - move this begin/end section after next while statement
        if(!base_t::pause())
        {
            return false;
        }
        while(base_t::some_thread_owns_elements(count_elements))
        {
            base_t::wait_a_while();
        }
        const size_t count = calc_count(capacity, stride, padding);

        char*  elements = reinterpret_cast<char*>(m_allocator.allocate(count));
        size_t offset   = calc_offset(elements, m_alignment);

        elements += offset;

        Assert_Check(stride >= m_stride);

        for(size_t index = 0; index < base_t::capacity(); ++index)
        {
            if(base_t::element_has_value(index, pos))
            {
                if(!capacity_changed) pos = index;

                std::memcpy(elements + pos * stride, m_elements + index * m_stride, stride);
            }
        }
        if(owned_elements != nullptr)
        {
            for( ; count_elements-- == 0; ++owned_elements)
            {
                const size_t index = index_of_element(*owned_elements);

                *owned_elements = elements + index * stride;
            }
        }
        clear();

        m_elements = elements;
        m_offset   = offset;
        m_size     = size;

        if(capacity_changed)
        {
            base_t::resize(capacity);
        }
        base_t::resume();
        return true;
    }

public:
    void* try_acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_t index = base_t::try_take_producer_element_ownership();

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element>
    Element* try_acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_producer_element_ownership());
    }

public:
    void* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_t index = base_t::try_take_consumer_element_ownership();

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<class Element>
    Element* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_consumer_element_ownership());
    }

private:
    size_t index_of_element(void* element) const noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return npos;

        const size_t index = size_t(static_cast<char*>(element) - m_elements) / m_stride;

        Check_ValidState(index < base_t::capacity(), npos);

        return index;
    }

public:
    template<bool push>
    bool release_producer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return false;

        const size_t index = index_of_element(element);

        return base_t::template drop_producer_element_ownership<push>(index);
    }
    template<bool pop>
    bool release_consumer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return false;

        const size_t index = index_of_element(element);

        return base_t::template drop_consumer_element_ownership<pop>(index);
    }

public:
    queue_producer_element_t try_start_push() noexcept(DIAG_NOEXCEPT)
    {
        void* element = try_acquire_producer_element_ownership();

        return queue_producer_element_t(*this, element);
    }
    queue_consumer_element_t try_start_pop() noexcept(DIAG_NOEXCEPT)
    {
        void* element = try_acquire_consumer_element_ownership();

        return queue_consumer_element_t(*this, element);
    }

public:
    bool try_push(const void* data = nullptr, size_t size = npos) noexcept(DIAG_NOEXCEPT)
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
    bool try_pop(void* data = nullptr, size_t size = npos) noexcept(DIAG_NOEXCEPT)
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
    void* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    void* acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership(std::chrono::nanoseconds::max());
    }

    template<class Element, typename Rep, typename Period>
    Element* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_producer_element(timeout));
    }
    template<class Element>
    Element* acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_producer_element_ownership<Element>(std::chrono::nanoseconds::max());
    }

public:
    template<typename Rep, typename Period>
    void* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        if(m_stride == 0) return nullptr;

        const size_t index = base_t::take_consumer_element_ownership(timeout);

        if(index == npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    void* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_consumer_element_ownership(std::chrono::nanoseconds::max());
    }

    template<class Element, typename Rep, typename Period>
    Element* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_consumer_element(timeout));
    }
    template<class Element>
    Element* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        return acquire_consumer_element_ownership<Element>(std::chrono::nanoseconds::max());
    }

public:
    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_producer_element_ownership(timeout);

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push() noexcept(DIAG_NOEXCEPT)
    {
        return start_push(std::chrono::nanoseconds::max());
    }

    template<typename Rep, typename Period>
    queue_consumer_element_t start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_consumer_element_ownership(timeout);

        return queue_consumer_element_t(*this, element);
    }
    queue_consumer_element_t start_pop() noexcept(DIAG_NOEXCEPT)
    {
        return start_pop(std::chrono::nanoseconds::max());
    }

public:
    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const void* data = nullptr, size_t size = npos)  noexcept(DIAG_NOEXCEPT)
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
    bool push(const void* data = nullptr, size_t size = npos) noexcept(DIAG_NOEXCEPT)
    {
        return push(std::chrono::nanoseconds::max(), data, size);
    }

    template<typename Rep, typename Period>
    bool pop(std::chrono::duration<Rep, Period> timeout, void* data = nullptr, size_t size = npos) noexcept(DIAG_NOEXCEPT)
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
    bool pop(void* data = nullptr, size_t size = npos) noexcept(DIAG_NOEXCEPT)
    {
        return pop(std::chrono::nanoseconds::max(), data, size);
    }
#endif
};

}

namespace std
{
template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport,
    typename WaitSupport,
    typename BaseAllocator
    >
void swap(
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>& lhs,
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>& rhs
    )
    noexcept(
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>::swap_is_noexcept
    )
{
    lhs.swap(rhs);
}
}
