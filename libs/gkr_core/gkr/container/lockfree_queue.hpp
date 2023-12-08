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

#include <gkr/diagnostics.h>

#ifndef __cpp_lib_is_swappable
#include <gkr/cpp/lib_is_swappable.hpp>
#endif
#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.hpp>
#endif

#else

#ifndef __cpp_lib_is_swappable
namespace std {
template<typename T>
struct is_nothrow_swappable {
    static constexpr bool value = std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value;
};
}
};
#endif
#ifndef __cpp_lib_exchange_function
namespace std {
template<class T, class U=T>
T exchange(T& obj, U&& new_value) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_assignable<T&, U>::value) {
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
#define Assert_NotNullPtr(ptr) assert(ptr)
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

constexpr std::size_t queue_npos = std::size_t(-1);

namespace impl
{
template<typename Queue, typename Element>
class queue_element
{
    queue_element           (const queue_element&) noexcept = delete;
    queue_element& operator=(const queue_element&) noexcept = delete;

protected:
    Queue*   m_queue;
    Element* m_element;

protected:
    queue_element(queue_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr)) {
    }
    queue_element& operator=(queue_element&& other) noexcept {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

public:
    queue_element(Queue& queue, Element* element) noexcept : m_queue(&queue), m_element(element) {
    }
    ~queue_element() noexcept {
    }

public:
    const Element* data() const noexcept {
        return m_element;
    }
    Element* data() noexcept {
        return m_element;
    }

public:
    const Element& value() const noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element& value() noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }

    const Element& operator*() const noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }
    Element& operator*() noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return *m_element;
    }

    const Element* operator->() const noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return m_element;
    }
    Element* operator->() noexcept(DIAG_NOEXCEPT) {
        Assert_NotNullPtr(m_element);
        return m_element;
    }

public:
    Element* detach() noexcept(DIAG_NOEXCEPT) {
        Check_NotNullPtr(m_element, nullptr);
        return std::exchange(m_element, nullptr);
    }
};
template<typename Queue>
class queue_element<Queue, void>
{
    queue_element           (const queue_element&) noexcept = delete;
    queue_element& operator=(const queue_element&) noexcept = delete;

protected:
    Queue* m_queue;
    void*  m_element;

protected:
    queue_element(queue_element&& other) noexcept
        : m_queue  (std::exchange(other.m_queue  , nullptr))
        , m_element(std::exchange(other.m_element, nullptr)) {
    }
    queue_element& operator=(queue_element&& other) noexcept {
        m_queue   = std::exchange(other.m_queue  , nullptr);
        m_element = std::exchange(other.m_element, nullptr);
        return *this;
    }

protected:
    queue_element(Queue& queue, void* element) noexcept : m_queue(&queue), m_element(element) {
        Check_ValidState(((std::size_t(element) & (element_alignment() - 1)) == 0), );
    }
    ~queue_element() noexcept {
    }

public:
    const void* data() const noexcept {
        return m_element;
    }
    void* data() noexcept {
        return m_element;
    }

public:
    template<typename T>
    const T* data() const noexcept(DIAG_NOEXCEPT) {
        static_assert(alignof(T) <= element_alignment(), "The alignment of the type T must be less or equal of the alignment of the queue");
        Assert_Check(  sizeof(T) <=  get_element_size());
        return static_cast<const T*>(m_element);
    }
    template<typename T>
    T* data() noexcept(DIAG_NOEXCEPT) {
        static_assert(alignof(T) <= element_alignment(), "The alignment of the type T must be less or equal of the alignment of the queue");
        Assert_Check(  sizeof(T) <=  get_element_size());
        return static_cast<T*>(m_element);
    }

    template<typename T>
    const T& as() const noexcept(DIAG_NOEXCEPT) {
        static_assert(alignof(T) <= element_alignment(), "The alignment of the type T must be less or equal of the alignment of the queue");
        Assert_Check(  sizeof(T) <=  get_element_size());
        Assert_NotNullPtr(m_element);
        return *static_cast<const T*>(m_element);
    }
    template<typename T>
    T& as() noexcept(DIAG_NOEXCEPT) {
        static_assert(alignof(T) <= element_alignment(), "The alignment of the type T must be less or equal of the alignment of the queue");
        Assert_Check(  sizeof(T) <=  get_element_size());
        Assert_NotNullPtr(m_element);
        return *static_cast<T*>(m_element);
    }

public:
    static constexpr std::size_t element_alignment() noexcept {
        return Queue::element_alignment0();
    }
    std::size_t get_element_size() const noexcept {
        return m_queue->element_size();
    }
    bool set_element_size(std::size_t size) noexcept(false) {
        if(m_element != nullptr) {
            return m_queue->change_element_size(size, &m_element);
        } else {
            return false;
        }
    }

public:
    void* detach() noexcept(DIAG_NOEXCEPT) {
        Check_NotNullPtr(m_element, nullptr);
        return std::exchange(m_element, nullptr);
    }
};
}

template<typename Queue>
class queue_producer_element : public impl::queue_element<Queue, typename Queue::element_t>
{
    queue_producer_element           (const queue_producer_element&) noexcept = delete;
    queue_producer_element& operator=(const queue_producer_element&) noexcept = delete;

    using base_t = impl::queue_element<Queue, typename Queue::element_t>;

    using base_t::m_queue;
    using base_t::m_element;

public:
    queue_producer_element(queue_producer_element&& other) noexcept : base_t(std::move(other)) {
    }
    queue_producer_element& operator=(queue_producer_element&& other) noexcept {
        base_t::operator=(std::move(other));
        return *this;
    }

public:
    using Element = typename Queue::element_t;

    queue_producer_element(Queue& queue, Element* element) noexcept : base_t(queue, element) {
    }
    ~queue_producer_element() noexcept(DIAG_NOEXCEPT) {
        finish_push();
    }

public:
    bool push_in_progress() const noexcept {
        return (m_element != nullptr);
    }

public:
    void finish_push() noexcept(DIAG_NOEXCEPT) {
        if(push_in_progress()) {
            m_queue->release_producer_element_ownership(std::exchange(m_element, nullptr));
        }
    }
    void cancel_push() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value) {
        if(push_in_progress()) {
            m_queue->cancel_producer_element_ownership(std::exchange(m_element, nullptr));
        }
    }
};
template<typename Queue>
class queue_consumer_element : public impl::queue_element<Queue, typename Queue::element_t>
{
    queue_consumer_element           (const queue_consumer_element&) noexcept = delete;
    queue_consumer_element& operator=(const queue_consumer_element&) noexcept = delete;

    using base_t = impl::queue_element<Queue, typename Queue::element_t>;

    using base_t::m_queue;
    using base_t::m_element;

public:
    queue_consumer_element(queue_consumer_element&& other) noexcept : base_t(std::move(other)) {
    }
    queue_consumer_element& operator=(queue_consumer_element&& other) noexcept {
        base_t::operator=(std::move(other));
        return *this;
    }

public:
    using Element = typename Queue::element_t;

    queue_consumer_element(Queue& queue, Element* element) noexcept : base_t(queue, element) {
    }
    ~queue_consumer_element() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value) {
        finish_pop();
    }

public:
    bool pop_in_progress() const noexcept {
        return (m_element != nullptr);
    }

public:
    void finish_pop() noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<Element>::value) {
        if(pop_in_progress()) {
            m_queue->release_consumer_element_ownership(std::exchange(m_element, nullptr));
        }
    }
    void cancel_pop() noexcept(DIAG_NOEXCEPT) {
        if(pop_in_progress()) {
            m_queue->cancel_consumer_element_ownership(std::exchange(m_element, nullptr));
        }
    }
};

namespace impl
{

#ifdef _WIN32
using queue_tid_t = int;
#else
using queue_tid_t = long;
#endif
static_assert(sizeof(queue_tid_t) == sizeof(std::thread::id), "The size of queue_tid_id must be equal to the size of std::thread::id");

inline queue_tid_t get_current_tid() noexcept
{
    union { queue_tid_t tid; std::thread::id id; } values {};
    values.id = std::this_thread::get_id();
    return values.tid;
}
inline queue_tid_t get_other_tid(const std::thread& thread)
{
    union { queue_tid_t tid; std::thread::id id; } values {};
    values.id = thread.get_id();
    return values.tid;
}

class queue_threading
{
    queue_threading           (const queue_threading&) noexcept = delete;
    queue_threading& operator=(const queue_threading&) noexcept = delete;

private:
    queue_tid_t m_producer_tid = 0;
    queue_tid_t m_consumer_tid = 0;

public:
    queue_threading() noexcept = default;
   ~queue_threading() noexcept = default;

    queue_threading(queue_threading&& other) noexcept
        : m_producer_tid(std::exchange(other.m_producer_tid, 0))
        , m_consumer_tid(std::exchange(other.m_consumer_tid, 0))
    {
    }
    queue_threading& operator=(queue_threading&& other) noexcept
    {
        m_producer_tid = std::exchange(other.m_producer_tid, 0);
        m_consumer_tid = std::exchange(other.m_consumer_tid, 0);
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
        m_producer_tid = get_current_tid();
    }
    void set_thread_as_exclusive_producer(const std::thread& thread) noexcept
    {
        m_producer_tid = get_other_tid(thread);
    }
    void any_thread_can_be_producer() noexcept
    {
        m_producer_tid = 0;
    }

    void set_this_thread_as_exclusive_consumer() noexcept
    {
        m_consumer_tid = get_current_tid();
    }
    void set_thread_as_exclusive_consumer(const std::thread& thread) noexcept
    {
        m_consumer_tid = get_other_tid(thread);
    }
    void any_thread_can_be_consumer() noexcept
    {
        m_consumer_tid = 0;
    }

public:
    bool this_thread_is_valid_producer() const noexcept
    {
        return (m_producer_tid == 0) || (m_producer_tid == get_current_tid());
    }
    bool this_thread_is_valid_consumer() const noexcept
    {
        return (m_consumer_tid == 0) || (m_consumer_tid == get_current_tid());
    }
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
    void reset(std::size_t) noexcept
    {
    }
    void resize(std::size_t) noexcept
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
    template<typename>
    static constexpr bool has_wait() noexcept
    {
        return false;
    }

protected:
    template<typename Clock, typename Duration>
    bool producer_wait(const std::chrono::time_point<Clock, Duration>&) noexcept
    {
        static_assert(has_wait<Clock>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
        return false;
    }
    template<typename T=void>
    void producer_wait() noexcept
    {
        static_assert(has_wait<T>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
    }
    template<typename Clock, typename Duration>
    bool consumer_wait(const std::chrono::time_point<Clock, Duration>&) noexcept
    {
        static_assert(has_wait<Clock>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
        return false;
    }
    template<typename T=void>
    void consumer_wait() noexcept
    {
        static_assert(has_wait<T>(), "You cannot wait on this queue - use other queue wait support class to activate waiting functionality");
    }
#endif
};

template<bool Pausable, typename WaitSupport>
class queue_pausing;

template<typename WaitSupport>
class queue_pausing<false, WaitSupport> : public WaitSupport
{
    queue_pausing           (const queue_pausing&) noexcept = delete;
    queue_pausing& operator=(const queue_pausing&) noexcept = delete;

    using self_t = queue_pausing<false, WaitSupport>;
    using base_t = WaitSupport;

protected:
    queue_pausing() noexcept(std::is_nothrow_default_constructible<base_t>::value) = default;
   ~queue_pausing() noexcept(std::is_nothrow_destructible         <base_t>::value) = default;

    queue_pausing(queue_pausing&& other) noexcept(std::is_nothrow_move_constructible<base_t>::value)
        : base_t(other)
    {
    }
    queue_pausing& operator=(queue_pausing&& other) noexcept(std::is_nothrow_move_assignable<base_t>::value)
    {
        base_t::operator=(std::move(other));
        return *this;
    }

    void swap(queue_pausing& other) noexcept(std::is_nothrow_swappable<base_t>::value)
    {
        base_t::swap(other);
    }

protected:
    class prevent_pause_sentry
    {
        prevent_pause_sentry           (const prevent_pause_sentry&) noexcept = delete;
        prevent_pause_sentry& operator=(const prevent_pause_sentry&) noexcept = delete;

        prevent_pause_sentry           (prevent_pause_sentry&&) noexcept = delete;
        prevent_pause_sentry& operator=(prevent_pause_sentry&&) noexcept = delete;

    public:
        [[nodiscard]] explicit
        prevent_pause_sentry(self_t& qp) {}
       ~prevent_pause_sentry() = default;
    };

public:
    template<typename Rep, typename Period>
    void set_wait_a_while_duration(std::chrono::duration<Rep, Period> timeout) noexcept
    {
    }
};
template<typename WaitSupport>
class queue_pausing<true, WaitSupport> : public WaitSupport
{
    queue_pausing           (const queue_pausing&) noexcept = delete;
    queue_pausing& operator=(const queue_pausing&) noexcept = delete;

    using base_t = WaitSupport;

private:
    static constexpr long long initial_ns_to_wait = 1000000U; // 1 millisec

    static constexpr std::size_t max_op_threads = std::size_t(-1) / 4;

private:
    long long                m_ns_to_wait {initial_ns_to_wait};
    std::atomic<std::size_t> m_op_threads {0};

protected:
    queue_pausing() noexcept(std::is_nothrow_default_constructible<base_t>::value) = default;
   ~queue_pausing() noexcept(std::is_nothrow_destructible         <base_t>::value) = default;

    queue_pausing(queue_pausing&& other) noexcept(std::is_nothrow_move_constructible<base_t>::value)
        : base_t(other)
        , m_ns_to_wait(std::exchange(other.m_ns_to_wait, initial_ns_to_wait))
        , m_op_threads(other.m_op_threads.exchange(0))
    {
    }
    queue_pausing& operator=(queue_pausing&& other) noexcept(std::is_nothrow_move_assignable<base_t>::value)
    {
        base_t::operator=(std::move(other));

        m_ns_to_wait = std::exchange(other.m_ns_to_wait, initial_ns_to_wait);

        m_op_threads = other.m_op_threads.exchange(0);
        return *this;
    }

    void swap(queue_pausing& other) noexcept(std::is_nothrow_swappable<base_t>::value)
    {
        base_t::swap(other);

        std::swap(m_ns_to_wait, other.m_ns_to_wait);

        m_op_threads = other.m_op_threads.exchange(m_op_threads);
    }

protected:
    void wait_a_while() noexcept
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(m_ns_to_wait));
    }

private:
    void enter_pause_prevention() noexcept
    {
        while(++m_op_threads > max_op_threads)
        {
            --m_op_threads;
            wait_a_while();
        }
    }
    void leave_pause_prevention() noexcept
    {
        --m_op_threads;
    }

private:
    void pause() noexcept
    {
        for( ; ; )
        {
            std::size_t expected = 0;
            if(m_op_threads.compare_exchange_strong(expected, max_op_threads)) break;
            wait_a_while();
        }
    }
    void resume() noexcept
    {
        m_op_threads -= max_op_threads;
    }

private:
    friend class prevent_pause_sentry;
    friend class pause_resume_sentry;

protected:
    class prevent_pause_sentry
    {
        prevent_pause_sentry           (const prevent_pause_sentry&) noexcept = delete;
        prevent_pause_sentry& operator=(const prevent_pause_sentry&) noexcept = delete;

        prevent_pause_sentry           (prevent_pause_sentry&&) noexcept = delete;
        prevent_pause_sentry& operator=(prevent_pause_sentry&&) noexcept = delete;

        queue_pausing& m_qp;
    public:
        [[nodiscard]] explicit
        prevent_pause_sentry(queue_pausing& qp) : m_qp(qp) { m_qp.enter_pause_prevention(); }
       ~prevent_pause_sentry()                             { m_qp.leave_pause_prevention(); }
    };
    class pause_resume_sentry
    {
        pause_resume_sentry           (const pause_resume_sentry&) noexcept = delete;
        pause_resume_sentry& operator=(const pause_resume_sentry&) noexcept = delete;

        pause_resume_sentry           (pause_resume_sentry&&) noexcept = delete;
        pause_resume_sentry& operator=(pause_resume_sentry&&) noexcept = delete;

        queue_pausing& m_qp;
    public:
        [[nodiscard]] explicit
        pause_resume_sentry(queue_pausing& qp) : m_qp(qp) { m_qp.pause (); }
       ~pause_resume_sentry()                             { m_qp.resume(); }
    };

public:
    template<typename Rep, typename Period>
    void set_wait_a_while_duration(std::chrono::duration<Rep, Period> timeout) noexcept
    {
        m_ns_to_wait = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
    }
};

template<bool MultipleConsumersMultipleProducersSupport, bool Pausable, typename BaseAllocator, typename WaitSupport>
class basic_lockfree_queue;

template<bool Pausable, typename BaseAllocator, typename WaitSupport>
class basic_lockfree_queue<false, Pausable, BaseAllocator, WaitSupport> : public queue_pausing<Pausable, WaitSupport>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    queue_threading threading;

private:
    using base_t = queue_pausing<Pausable, WaitSupport>;

    std::atomic<std::size_t> m_count {0};

    std::size_t m_capacity = 0;
    std::size_t m_tail_pos = 0;
    std::size_t m_head_pos = 0;

    queue_tid_t m_producer_tid_owner = 0;
    queue_tid_t m_consumer_tid_owner = 0;

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator&) noexcept(std::is_nothrow_default_constructible<base_t>::value) : base_t()
    {
    }
    ~basic_lockfree_queue() noexcept(std::is_nothrow_destructible<base_t>::value)
    {
    }

    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept(std::is_nothrow_move_constructible<base_t>::value)
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
    basic_lockfree_queue& operator=(basic_lockfree_queue&& other) noexcept(std::is_nothrow_move_assignable<base_t>::value)
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
    void swap(basic_lockfree_queue& other) noexcept(std::is_nothrow_swappable<base_t>::value)
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
    std::size_t count() const noexcept
    {
        return m_count;
    }
    std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(std::size_t index, std::size_t& pos) const noexcept
    {
        if(index >= m_capacity) return false;

        const std::size_t tail_index = (m_tail_pos % m_capacity);
        const std::size_t head_index = (m_head_pos % m_capacity);

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
    void reset(std::size_t capacity) noexcept
    {
        base_t::reset(capacity);

        m_count    = 0;
        m_capacity = capacity;
        m_head_pos = 0;
        m_tail_pos = 0;

        m_producer_tid_owner = 0;
        m_consumer_tid_owner = 0;
    }
    void resize(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(capacity >= m_count);

        base_t::resize(capacity);

        m_capacity = capacity;

        m_head_pos = 0;
        m_tail_pos = m_count;
    }

private:
    bool can_take_producer_element_ownership() const noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_capacity > 0, false);
        Check_ValidState(m_producer_tid_owner == 0, false);
        Check_ValidState(threading.this_thread_is_valid_producer(), false);
        return true;
    }
    bool can_take_consumer_element_ownership() const noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_capacity > 0, false);
        Check_ValidState(m_consumer_tid_owner == 0, false);
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);
        return true;
    }

protected:
    std::size_t try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        if(!can_take_producer_element_ownership()) return queue_npos;

        if(m_count == m_capacity)
        {
            base_t::notify_producer_ownership_fail();
            return queue_npos;
        }
        base_t::notify_producer_ownership_start();

        m_producer_tid_owner = get_current_tid();

        const std::size_t index = (m_tail_pos % m_capacity);

        return index;
    }
    template<bool push>
    bool drop_producer_element_ownership(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == queue_npos) return false;

        Check_ValidState(m_producer_tid_owner == get_current_tid(), false);

        Check_Arg_IsValid(index == (m_tail_pos % m_capacity), false);

        m_producer_tid_owner = 0;

        if_constexpr(push)
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
    std::size_t try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        if(!can_take_consumer_element_ownership()) return queue_npos;

        if(m_count == 0)
        {
            base_t::notify_consumer_ownership_fail();
            return queue_npos;
        }
        base_t::notify_consumer_ownership_start();

        m_consumer_tid_owner = get_current_tid();

        const std::size_t index = (m_head_pos % m_capacity);

        return index;
    }
    template<bool pop>
    bool drop_consumer_element_ownership(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == queue_npos) return false;

        Check_ValidState(m_consumer_tid_owner == get_current_tid(), false);

        Check_ValidState(index == (m_head_pos % m_capacity), false);

        m_consumer_tid_owner = 0;

        if_constexpr(pop)
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
    bool this_thread_owns_elements(std::size_t count) const noexcept
    {
        const queue_tid_t current_tid = get_current_tid();

        if(m_producer_tid_owner == current_tid) --count;
        if(m_consumer_tid_owner == current_tid) --count;

        return (count == 0);
    }
    bool some_thread_owns_element() const noexcept
    {
        return (m_producer_tid_owner != 0) || (m_consumer_tid_owner != 0);
    }
    bool some_thread_owns_elements(std::size_t count) const noexcept //TODO:implementation must be tested
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
    template<typename Rep, typename Period>
    std::size_t take_producer_element_ownership(const std::chrono::duration<Rep, Period>& timeout) noexcept(DIAG_NOEXCEPT)
    {
        const auto wait_time = std::chrono::high_resolution_clock::now() + timeout;

        for( ; ; )
        {
            const std::size_t index = try_take_producer_element_ownership();

            if(index != queue_npos) return index;

            if(!base_t::producer_wait(wait_time)) return queue_npos;
        }
    }
    std::size_t take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const std::size_t index = try_take_producer_element_ownership();

            if(index != queue_npos) return index;

            base_t::producer_wait();
        }
    }
    template<typename Rep, typename Period>
    std::size_t take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const auto wait_time = std::chrono::high_resolution_clock::now() + timeout;

        for( ; ; )
        {
            const std::size_t index = try_take_consumer_element_ownership();

            if(index != queue_npos) return index;

            if(!base_t::consumer_wait(wait_time)) return queue_npos;
        }
    }
    std::size_t take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const std::size_t index = try_take_consumer_element_ownership();

            if(index != queue_npos) return index;

            base_t::consumer_wait();
        }
    }
#endif
};
template<bool Pausable, typename BaseAllocator, typename WaitSupport>
class basic_lockfree_queue<true, Pausable, BaseAllocator, WaitSupport> : public queue_pausing<Pausable, WaitSupport>
{
    basic_lockfree_queue           (const basic_lockfree_queue&) noexcept = delete;
    basic_lockfree_queue& operator=(const basic_lockfree_queue&) noexcept = delete;

public:
    queue_threading threading;

private:
    using base_t = queue_pausing<Pausable, WaitSupport>;

    struct dequeues_entry
    {
        std::size_t free_index;
        std::size_t busy_index;
        queue_tid_t tid_owner;
    };
    static_assert(std::is_trivial<dequeues_entry>::value, "Must be trivial");

    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<dequeues_entry>;

    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = (
        std::is_nothrow_move_assignable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_move_assignment::value &&
                std::is_nothrow_move_assignable<TypeAllocator>::value
                )));
    static constexpr bool swap_is_noexcept = (
        std::is_nothrow_swappable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_swap::value &&
                std::is_nothrow_swappable<TypeAllocator>::value
                )));

    gkr_attr_no_unique_address TypeAllocator m_allocator;

    std::size_t     m_capacity = 0;
    dequeues_entry* m_entries  = nullptr;

    std::atomic<std::size_t> m_count     {0};
    std::atomic<std::size_t> m_occupied  {0};
    std::atomic<std::size_t> m_available {0};

    std::atomic<std::size_t> m_free_tail {0};
    std::atomic<std::size_t> m_free_head {0};

    std::atomic<std::size_t> m_busy_tail {0};
    std::atomic<std::size_t> m_busy_head {0};

protected:
    basic_lockfree_queue() noexcept = delete;

    basic_lockfree_queue(const BaseAllocator& allocator) noexcept(
        std::is_nothrow_default_constructible<base_t>::value &&
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : base_t()
        , m_allocator(allocator)
    {
    }
    ~basic_lockfree_queue() noexcept(
        std::is_nothrow_destructible<TypeAllocator>::value &&
        std::is_nothrow_destructible<base_t       >::value
        )
    {
        clean();
    }

protected:
    basic_lockfree_queue(basic_lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<base_t>::value &&
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
        clean();

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

        swap_entries(other);

        std::swap(m_capacity, other.m_capacity);
    }

private:
    void clean() noexcept
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
        if_constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            m_allocator = std::move(other.m_allocator);
        }
        else if_constexpr(!AllocatorTraits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_entries(m_entries, m_allocator);
                return;
            }
        }
        m_entries = std::exchange(other.m_entries, nullptr);
    }
    void swap_entries(basic_lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if_constexpr(AllocatorTraits::propagate_on_container_swap::value)
        {
            std::swap(m_allocator, other.m_allocator);
        }
        else if_constexpr(!AllocatorTraits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                Check_Recovery(
                    "According to the standard"
                    " (see AllocatorAwareContainer at https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)"
                    " this case is undefined behaviour."
                    " Recovery code follows"
                    );
                dequeues_entry* entries;

                this->relocate_entries(  entries, other.m_allocator);
                other.relocate_entries(m_entries,       m_allocator);

                other.m_entries = entries;
                return;
            }
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
    std::size_t count() const noexcept
    {
        return m_count;
    }
    std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

protected:
    bool element_has_value(std::size_t index, std::size_t& pos) const noexcept
    {
        if(index >= m_capacity) return false;
        //TODO:implementation must be tested
        for(std::size_t busy_pos = m_busy_head, busy_end = m_busy_tail; busy_pos < busy_end; ++busy_pos)
        {
            if(index == m_entries[busy_pos % m_capacity].busy_index)
            {
                pos = (busy_pos - m_busy_head);
                return true;
            }
        }
        return false;
    }
    void reset(std::size_t capacity) noexcept(false)
    {
        base_t::reset(capacity);

        if(capacity != m_capacity)
        {
            clean();

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
        for(std::size_t index = 0; index < m_capacity; ++index)
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
    void resize(std::size_t capacity) noexcept(false)
    {
        Assert_Check(capacity >= m_count);

        base_t::resize(capacity);

        clean();

        dequeues_entry* entries = m_allocator.allocate(capacity);

        const std::size_t count = m_count;

        for(std::size_t index = 0; index < capacity; ++index)
        {
            entries[index] = dequeues_entry{index + count, index, 0};
        }
        m_busy_head = 0;
        m_busy_tail = count;

        m_free_head = 0;
        m_free_tail = capacity - count;

        m_capacity  = capacity;
    }

private:
    bool can_take_producer_element_ownership() const noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_capacity > 0, false);
        Check_ValidState(threading.this_thread_is_valid_producer(), false);
        return true;
    }
    bool can_take_consumer_element_ownership() const noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_capacity > 0, false);
        Check_ValidState(threading.this_thread_is_valid_consumer(), false);
        return true;
    }

protected:
    std::size_t try_take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        if(!can_take_producer_element_ownership()) return queue_npos;

        if(++m_occupied > m_capacity)
        {
            --m_occupied;
            base_t::notify_producer_ownership_fail();
            return queue_npos;
        }
        base_t::notify_producer_ownership_start();

        const std::size_t index = m_entries[m_free_head++ % m_capacity].free_index;

        m_entries[index].tid_owner = get_current_tid();

        return index;
    }
    template<bool push>
    bool drop_producer_element_ownership(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        Check_ValidState(threading.this_thread_is_valid_producer(), false);

        if(index == queue_npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == get_current_tid(), false);

        m_entries[index].tid_owner = 0;

        if_constexpr(push)
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
    std::size_t try_take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        if(!can_take_consumer_element_ownership()) return queue_npos;

        if(std::ptrdiff_t(--m_available) < 0)
        {
            ++m_available;
            base_t::notify_consumer_ownership_fail();
            return queue_npos;
        }
        base_t::notify_consumer_ownership_start();

        const std::size_t index = m_entries[m_busy_head++ % m_capacity].busy_index;

        m_entries[index].tid_owner = get_current_tid();

        return index;
    }
    template<bool pop>
    bool drop_consumer_element_ownership(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        typename base_t::prevent_pause_sentry sentry(*this);

        Check_ValidState(threading.this_thread_is_valid_consumer(), false);

        if(index == queue_npos) return false;

        Check_Arg_IsValid(index < m_capacity, false);

        Check_Arg_IsValid(m_entries[index].tid_owner == get_current_tid(), false);

        m_entries[index].tid_owner = 0;

        if_constexpr(pop)
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
    bool this_thread_owns_elements(std::size_t count) const noexcept
    {
        const queue_tid_t current_tid = get_current_tid();

        for(std::size_t index = 0; index < m_capacity; ++index)
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
        const std::size_t busy_count = (m_busy_tail - m_busy_head);
        const std::size_t free_count = (m_free_tail - m_free_head);

        const std::size_t idle_count = busy_count + free_count;

        return ((m_capacity - idle_count) != 0);
    }
    bool some_thread_owns_elements(std::size_t count) const noexcept
    {
        const std::size_t busy_count = (m_busy_tail - m_busy_head);
        const std::size_t free_count = (m_free_tail - m_free_head);

        const std::size_t idle_count = busy_count + free_count;

        return ((m_capacity - idle_count) == count);
    }
#ifndef GKR_LOCKFREE_QUEUE_EXCLUDE_WAITING
protected:
    template<typename Rep, typename Period>
    std::size_t take_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const auto wait_time = std::chrono::high_resolution_clock::now() + timeout;

        for( ; ; )
        {
            const std::size_t index = try_take_producer_element_ownership();

            if(index != queue_npos) return index;

            if(!base_t::producer_wait(wait_time)) return queue_npos;
        }
    }
    std::size_t take_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const std::size_t index = try_take_producer_element_ownership();

            if(index != queue_npos) return index;

            base_t::producer_wait();
        }
    }
    template<typename Rep, typename Period>
    std::size_t take_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const auto wait_time = std::chrono::high_resolution_clock::now() + timeout;

        for( ; ; )
        {
            const std::size_t index = try_take_consumer_element_ownership();

            if(index != queue_npos) return index;

            if(!base_t::consumer_wait(wait_time)) return queue_npos;
        }
    }
    std::size_t take_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        for( ; ; )
        {
            const std::size_t index = try_take_consumer_element_ownership();

            if(index != queue_npos) return index;

            base_t::consumer_wait();
        }
    }
#endif
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    bool Pausable=false,
    typename BaseAllocator=std::allocator<char>,
    typename WaitSupport  =impl::queue_no_wait_support
    >
class lockfree_queue
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>;
    using base_t = impl::basic_lockfree_queue<   MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>;

public:
    using element_t = T;

    using queue_producer_element_t = queue_producer_element<self_t>;
    using queue_consumer_element_t = queue_consumer_element<self_t>;

    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<T>;

private:
    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = (
        std::is_nothrow_move_assignable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_move_assignment::value &&
                std::is_nothrow_move_assignable<TypeAllocator>::value
                )));
    static constexpr bool swap_is_noexcept = (
        std::is_nothrow_swappable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_swap::value &&
                std::is_nothrow_swappable<TypeAllocator>::value
                )));

    gkr_attr_no_unique_address TypeAllocator m_allocator;

    element_t* m_elements = nullptr;

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(
        std::is_nothrow_constructible<base_t       , BaseAllocator>::value &&
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(std::size_t capacity, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity);
    }
    ~lockfree_queue() noexcept(
        std::is_nothrow_destructible<element_t    >::value &&
        std::is_nothrow_destructible<TypeAllocator>::value &&
        std::is_nothrow_destructible<base_t       >::value
        )
    {
        clear();
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<base_t       >::value &&
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
            clear();
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
    void destroy_element(std::size_t index) noexcept(std::is_nothrow_destructible<element_t>::value)
    {
        m_elements[index].~element_t();
    }
    void clear() noexcept(std::is_nothrow_destructible<element_t>::value)
    {
        if(m_elements != nullptr)
        {
            for(std::size_t pos = queue_npos, index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    destroy_element(index);
                }
            }
            m_allocator.deallocate(m_elements, base_t::capacity());
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

            for(std::size_t pos = queue_npos, index = 0; index < base_t::capacity(); ++index)
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
        if_constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            m_allocator = std::move(other.m_allocator);
        }
        else if_constexpr(!AllocatorTraits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_elements(m_elements, m_allocator);
                return;
            }
        }
        m_elements = std::exchange(other.m_elements, nullptr);
    }
    void swap_elements(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if_constexpr(AllocatorTraits::propagate_on_container_swap::value)
        {
            std::swap(m_allocator, other.m_allocator);
        }
        else if_constexpr(!AllocatorTraits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                Check_Recovery(
                    "According to the standard"
                    " (see AllocatorAwareContainer at https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)"
                    " this case is undefined behaviour."
                    " Recovery code follows"
                    );
                element_t* elements;

                this->relocate_elements(  elements, other.m_allocator);
                other.relocate_elements(m_elements,       m_allocator);

                other.m_elements = elements;
                return;
            }
        }
        std::swap(m_elements, other.m_elements);
    }

public:
    static constexpr std::size_t element_alignment0() noexcept
    {
        return alignof(element_t);
    }

    std::size_t element_alignment() const noexcept
    {
        return alignof(element_t);
    }
    std::size_t element_size() const noexcept
    {
        return sizeof(element_t);
    }

public:
    void reset() noexcept(!MultipleConsumersMultipleProducersSupport && std::is_nothrow_destructible<element_t>::value)
    {
        for(std::size_t pos = queue_npos, index = 0; index < base_t::capacity(); ++index)
        {
            if(base_t::element_has_value(index, pos))
            {
                destroy_element(index);
            }
        }
        base_t::reset(base_t::capacity());
    }
    void reset(std::size_t capacity) noexcept(false)
    {
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
    bool resize(std::size_t capacity) noexcept(false)
    {
        static_assert(Pausable, "Cannot resize not pausable queue");

        typename base_t::pause_resume_sentry sentry(*this);

        if(capacity <= base_t::count()) return false;

        Check_ValidState(base_t::this_thread_owns_elements(0), false);

        while(base_t::some_thread_owns_element())
        {
            base_t::wait_a_while();
        }
        element_t* elements = m_allocator.allocate(capacity);

        for(std::size_t pos = queue_npos, index = 0; index < base_t::capacity(); ++index)
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
        return true;
    }

public:
    template<typename... Args>
    element_t* try_acquire_producer_element_ownership_ex(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        const std::size_t index = base_t::try_take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    element_t* try_acquire_producer_element_ownership() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::try_take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    element_t* try_acquire_producer_element_ownership(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::try_take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    element_t* try_acquire_producer_element_ownership(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::try_take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    element_t* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t index = base_t::try_take_consumer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }

private:
    std::size_t index_of_element(element_t* element) const noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return queue_npos;

        const std::size_t index = std::size_t(element - m_elements);

        Check_ValidState(index < base_t::capacity(), queue_npos);

        return index;
    }

public:
    bool release_producer_element_ownership(element_t* element) noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t index = index_of_element(element);

        return base_t::template drop_producer_element_ownership<true>(index);
    }
    bool cancel_producer_element_ownership(element_t* element) noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<element_t>::value)
    {
        const std::size_t index = index_of_element(element);

        if(index != queue_npos)
        {
            destroy_element(index);
        }
        return base_t::template drop_producer_element_ownership<false>(index);
    }

    bool release_consumer_element_ownership(element_t* element) noexcept(DIAG_NOEXCEPT && std::is_nothrow_destructible<element_t>::value)
    {
        const std::size_t index = index_of_element(element);

        if(index != queue_npos)
        {
            destroy_element(index);
        }
        return base_t::template drop_consumer_element_ownership<true>(index);
    }
    bool cancel_consumer_element_ownership(element_t* element) noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t index = index_of_element(element);

        return base_t::template drop_consumer_element_ownership<false>(index);
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
        std::is_nothrow_move_constructible<element_t>::value
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
    element_t* acquire_producer_element_ownership_ex(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }
    template<typename... Args>
    element_t* acquire_producer_element_ownership_ex(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }
    element_t* acquire_producer_element_ownership() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t();
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }
    element_t* acquire_producer_element_ownership(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(std::move(value));
    }

    template<typename Rep, typename Period>
    element_t* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }
    element_t* acquire_producer_element_ownership(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        const std::size_t index = base_t::take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return new (element) element_t(value);
    }

    template<typename Rep, typename Period>
    element_t* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t index = base_t::take_consumer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }
    element_t* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t index = base_t::take_consumer_element_ownership();

        if(index == queue_npos) return nullptr;

        element_t* element = (m_elements + index);

        return element;
    }

public:
    template<typename Rep, typename Period, typename... Args>
    element_t* start_emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        element_t* element = acquire_producer_element_ownership_ex(timeout, std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }
    template<typename... Args>
    element_t* start_emplace(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        element_t* element = acquire_producer_element_ownership_ex(std::forward<Args>(args)...);

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership(timeout);

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership();

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership(timeout, std::move(value));

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership(std::move(value));

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_producer_element_t start_push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership(timeout, value);

        return queue_producer_element_t(*this, element);
    }
    queue_producer_element_t start_push(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        element_t* element = acquire_producer_element_ownership(value);

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_consumer_element_t start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_consumer_element_ownership(timeout);

        return queue_consumer_element_t(*this, element);
    }
    queue_consumer_element_t start_pop() noexcept(DIAG_NOEXCEPT)
    {
        element_t* element = acquire_consumer_element_ownership();

        return queue_consumer_element_t(*this, element);
    }

public:
    template<typename Rep, typename Period, typename... Args>
    bool emplace(std::chrono::duration<Rep, Period> timeout, Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        return start_emplace(timeout, std::forward<Args>(args)...).push_in_progress();
    }
    template<typename... Args>
    bool emplace(Args&&... args) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_constructible<element_t, Args...>::value
        )
    {
        return start_emplace(std::forward<Args>(args)...).push_in_progress();
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        return start_push(timeout).push_in_progress();
    }
    bool push() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_default_constructible<element_t>::value
        )
    {
        return start_push().push_in_progress();
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        return start_push(timeout, std::move(value)).push_in_progress();
    }
    bool push(element_t&& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_constructible<element_t>::value
        )
    {
        return start_push(std::move(value)).push_in_progress();
    }

    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return start_push(timeout, value).push_in_progress();
    }
    bool push(const element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_constructible<element_t>::value
        )
    {
        return start_push(value).push_in_progress();
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
    bool pop() noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_destructible<element_t>::value
        )
    {
        return start_pop().pop_in_progress();
    }

    template<typename Rep, typename Period>
    bool copy_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_assignable<element_t>::value &&
        std::is_nothrow_destructible   <element_t>::value
        )
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = consumer_element.value();

        return true;
    }
    bool copy_and_pop(element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_copy_assignable<element_t>::value &&
        std::is_nothrow_destructible   <element_t>::value
        )
    {
        auto consumer_element = start_pop();

        if(!consumer_element.pop_in_progress()) return false;

        value = consumer_element.value();

        return true;
    }

    template<typename Rep, typename Period>
    bool move_and_pop(std::chrono::duration<Rep, Period> timeout, element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_assignable<element_t>::value &&
        std::is_nothrow_destructible   <element_t>::value
        )
    {
        auto consumer_element = start_pop(timeout);

        if(!consumer_element.pop_in_progress()) return false;

        value = std::move(consumer_element.value());

        return true;
    }
    bool move_and_pop(element_t& value) noexcept(
        DIAG_NOEXCEPT &&
        std::is_nothrow_move_assignable<element_t>::value &&
        std::is_nothrow_destructible   <element_t>::value
        )
    {
        auto consumer_element = start_pop();

        if(!consumer_element.pop_in_progress()) return false;

        value = std::move(consumer_element.value());

        return true;
    }
#endif
};
template<
    bool MultipleConsumersMultipleProducersSupport,
    bool Pausable,
    typename BaseAllocator,
    typename WaitSupport
    >
class lockfree_queue<void, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>
    : public impl::basic_lockfree_queue<MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>
{
    lockfree_queue           (const lockfree_queue&) noexcept = delete;
    lockfree_queue& operator=(const lockfree_queue&) noexcept = delete;

private:
    using self_t =             lockfree_queue<void, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>;
    using base_t = impl::basic_lockfree_queue<      MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>;

public:
    using element_t = void;

    using queue_producer_element_t = queue_producer_element<self_t>;
    using queue_consumer_element_t = queue_consumer_element<self_t>;

private:
    using alloc_value_t = std::max_align_t;
    using TypeAllocator = typename std::allocator_traits<BaseAllocator>::template rebind_alloc<alloc_value_t>; //replace alloc_value_t with char

    using AllocatorTraits = std::allocator_traits<TypeAllocator>;

    static constexpr bool move_is_noexcept = (
        std::is_nothrow_move_assignable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_move_assignment::value &&
                std::is_nothrow_move_assignable<TypeAllocator>::value
                )));
    static constexpr bool swap_is_noexcept = (
        std::is_nothrow_swappable<base_t>::value && (
            AllocatorTraits::is_always_equal::value || (
                AllocatorTraits::propagate_on_container_swap::value &&
                std::is_nothrow_swappable<TypeAllocator>::value
                )));

    gkr_attr_no_unique_address TypeAllocator m_allocator;

    char*       m_elements  = nullptr;
    std::size_t m_offset    = 0;
    std::size_t m_size      = 0;
    std::size_t m_alignment = 0;
    std::size_t m_padding   = 0;
    std::size_t m_stride    = 0;

public:
    lockfree_queue(const BaseAllocator& allocator = BaseAllocator()) noexcept(
        std::is_nothrow_constructible<base_t       , BaseAllocator>::value &&
        std::is_nothrow_constructible<TypeAllocator, BaseAllocator>::value
        )
        : base_t     (allocator)
        , m_allocator(allocator)
    {
    }
    lockfree_queue(std::size_t capacity, std::size_t size, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, 0);
    }
    lockfree_queue(std::size_t capacity, std::size_t size, std::size_t alignment, const BaseAllocator& allocator = BaseAllocator()) noexcept(false)
        : base_t     (allocator)
        , m_allocator(allocator)
    {
        reset(capacity, size, alignment);
    }
    ~lockfree_queue() noexcept(
        std::is_nothrow_destructible<TypeAllocator>::value &&
        std::is_nothrow_destructible<base_t       >::value &&
        DIAG_NOEXCEPT
        )
    {
        clear();
    }

public:
    lockfree_queue(lockfree_queue&& other) noexcept(
        std::is_nothrow_move_constructible<base_t       >::value &&
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
    lockfree_queue& operator=(lockfree_queue&& other) noexcept(move_is_noexcept)
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
    static bool is_power_of_2(std::size_t value) noexcept
    {
        return (value > 0) && ((value & (value - 1)) == 0);
    }
    static std::size_t calc_alignment(std::size_t size) noexcept
    {
        std::size_t alignment;

        for(alignment = 1; (alignment < size) && (alignment < alignof(alloc_value_t)); alignment <<= 1);

        return alignment;
    }
    static std::size_t calc_offset(char* elements, std::size_t alignment) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_power_of_2(alignment));

        const std::size_t displace = reinterpret_cast<std::size_t>(elements) & (alignment - 1);

        const std::size_t offset = (displace == 0) ? 0 : (alignment - displace);

        return offset;
    }
    static void calc_stride_and_padding(std::size_t size, std::size_t alignment, std::size_t& stride, std::size_t& padding) noexcept(DIAG_NOEXCEPT)
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
    static std::size_t calc_count(std::size_t capacity, std::size_t stride, std::size_t padding) noexcept(DIAG_NOEXCEPT)
    {
        const std::size_t cb = (capacity * stride + padding);

        Assert_Check((cb % sizeof(alloc_value_t)) == 0);

        const std::size_t count = (cb / sizeof(alloc_value_t));

        return count;
    }

private:
    void clear() noexcept(DIAG_NOEXCEPT)
    {
        if(m_elements != nullptr)
        {
            const std::size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

            m_elements -= m_offset;
            m_allocator.deallocate(reinterpret_cast<alloc_value_t*>(m_elements), count);
        }
    }

private:
    void relocate_elements(std::size_t& offset, char*& elements, TypeAllocator& allocator) noexcept(false)
    {
        if(base_t::capacity() == 0)
        {
            elements = nullptr;
        }
        else
        {
            const std::size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

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
        if_constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            m_allocator = std::move(other.m_allocator);
        }
        else if_constexpr(!AllocatorTraits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_elements(m_offset, m_elements, m_allocator);
                return;
            }
        }
        m_elements = std::exchange(other.m_elements, nullptr);
        m_offset   = std::exchange(other.m_offset  , 0);
    }
    void swap_elements(lockfree_queue& other) noexcept(swap_is_noexcept)
    {
        if_constexpr(AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            std::swap(m_allocator, other.m_allocator);
        }
        else if_constexpr(!swap_is_noexcept)
        {
            if(m_allocator != other.m_allocator)
            {
                Check_Recovery(
                    "According to the standard"
                    " (see AllocatorAwareContainer at https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)"
                    " this case is undefined behaviour."
                    " Recovery code follows"
                    );
                std::size_t offset;
                char*     elements;

                this->relocate_elements(  offset,   elements, other.m_allocator);
                other.relocate_elements(m_offset, m_elements,       m_allocator);

                other.m_elements = elements;
                other.m_offset   = offset;
                return;
            }
        }
        std::swap(m_elements, other.m_elements);
    }

public:
    static constexpr std::size_t element_alignment0() noexcept
    {
        return alignof(std::max_align_t);
    }

    std::size_t element_alignment() const noexcept
    {
        return m_alignment;
    }
    std::size_t element_size() const noexcept
    {
        return m_size;
    }

public:
    void reset() noexcept(!MultipleConsumersMultipleProducersSupport)
    {
        base_t::reset(base_t::capacity());
    }
    void reset(std::size_t capacity, std::size_t size = queue_npos, std::size_t alignment = queue_npos) noexcept(false)
    {
        Check_Arg_IsValid((alignment == queue_npos) || (alignment == 0) || is_power_of_2(alignment), );

        if(capacity == queue_npos)
        {
            capacity = base_t::capacity();
        }
        if(size != queue_npos)
        {
            m_size = size;
        }
        if(alignment != queue_npos)
        {
            m_alignment = alignment;
        }
        if(m_alignment == 0)
        {
            m_alignment = calc_alignment(m_size);
        }
        std::size_t stride;
        std::size_t padding;
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
            const std::size_t count = calc_count(base_t::capacity(), m_stride, m_padding);

            m_elements = reinterpret_cast<char*>(m_allocator.allocate(count));
            m_offset   = calc_offset(m_elements, m_alignment);

            m_elements += m_offset;
        }
    }

public:
    bool resize(std::size_t capacity) noexcept(false)
    {
        static_assert(Pausable, "Cannot resize not pausable queue");

        typename base_t::pause_resume_sentry sentry(*this);

        if(capacity <= base_t::count()) return false;

        Check_ValidState(base_t::this_thread_owns_elements(0), false);

        while(base_t::some_thread_owns_element())
        {
            base_t::wait_a_while();
        }
        const std::size_t count = calc_count(capacity, m_stride, m_padding);

        char*     elements = reinterpret_cast<char*>(m_allocator.allocate(count));
        std::size_t offset = calc_offset(elements, m_alignment);

        elements += offset;

        for(std::size_t pos = queue_npos, index = 0; index < base_t::capacity(); ++index)
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
        return true;
    }
    bool change_element_size(std::size_t size, void** owned_elements = nullptr, std::size_t count_elements = 1) noexcept(false)
    {
        static_assert(Pausable, "Cannot change element size of not pausable queue");

        typename base_t::pause_resume_sentry sentry(*this);

        if(size <= m_size) return false;

        if(owned_elements == nullptr)
        {
            count_elements = 0;
        }
        Check_ValidState(base_t::this_thread_owns_elements(count_elements), false);

        std::size_t pos = queue_npos;
        Check_Arg_Array(index, count_elements, base_t::element_has_value(index_of_element(owned_elements[index]), pos), false);

        while(base_t::some_thread_owns_elements(count_elements))
        {
            base_t::wait_a_while();
        }

        std::size_t stride;
        std::size_t padding;
        calc_stride_and_padding(size, m_alignment, stride, padding);

        Assert_Check(padding == m_padding);
        Assert_Check(stride  >= m_stride );

        if(stride != m_stride)
        {
            const std::size_t count = calc_count(base_t::capacity(), stride, padding);

            char*     elements = reinterpret_cast<char*>(m_allocator.allocate(count));
            std::size_t offset = calc_offset(elements, m_alignment);

            elements += offset;

            for(std::size_t index = 0; index < base_t::capacity(); ++index)
            {
                if(base_t::element_has_value(index, pos))
                {
                    std::memcpy(elements + index * stride, m_elements + index * m_stride, stride);
                }
            }
            for( ; count_elements-- > 0; ++owned_elements)
            {
                const std::size_t index = index_of_element(*owned_elements);

                *owned_elements = elements + index * stride;
            }
            clear();

            m_elements = elements;
            m_offset   = offset;
            m_stride   = stride;
        }
        m_size = size;
        return true;
    }

public:
    void* try_acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::try_take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<typename Element>
    Element* try_acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_producer_element_ownership());
    }

public:
    void* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::try_take_consumer_element_ownership();

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    template<typename Element>
    Element* try_acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(try_acquire_consumer_element_ownership());
    }

private:
    std::size_t index_of_element(void* element) const noexcept(DIAG_NOEXCEPT)
    {
        if(element == nullptr) return queue_npos;

        const std::size_t index = std::size_t(static_cast<char*>(element) - m_elements) / m_stride;

        Check_ValidState(index < base_t::capacity(), queue_npos);

        return index;
    }

public:
    bool release_producer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, false);

        const std::size_t index = index_of_element(element);

        return base_t::template drop_producer_element_ownership<true>(index);
    }
    bool cancel_producer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, false);

        const std::size_t index = index_of_element(element);

        return base_t::template drop_producer_element_ownership<false>(index);
    }

    bool release_consumer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, false);

        const std::size_t index = index_of_element(element);

        return base_t::template drop_consumer_element_ownership<true>(index);
    }
    bool cancel_consumer_element_ownership(void* element) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, false);

        const std::size_t index = index_of_element(element);

        return base_t::template drop_consumer_element_ownership<false>(index);
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
    bool try_push(const void* data = nullptr, std::size_t size = queue_npos) noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = try_start_push();

        if(!producer_element.push_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(producer_element.data(), data, size);
        }
        return true;
    }
    bool try_pop(void* data = nullptr, std::size_t size = queue_npos) noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = try_start_pop();

        if(!producer_element.pop_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
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
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::take_producer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    void* acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::take_producer_element_ownership();

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }

    template<typename Element, typename Rep, typename Period>
    Element* acquire_producer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_producer_element_ownership(timeout));
    }
    template<typename Element>
    Element* acquire_producer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_producer_element_ownership());
    }

public:
    template<typename Rep, typename Period>
    void* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::take_consumer_element_ownership(timeout);

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }
    void* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(m_stride > 0, nullptr);

        const std::size_t index = base_t::take_consumer_element_ownership();

        if(index == queue_npos) return nullptr;

        void* element = m_elements + m_stride * index;

        return element;
    }

    template<typename Element, typename Rep, typename Period>
    Element* acquire_consumer_element_ownership(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_consumer_element_ownership(timeout));
    }
    template<typename Element>
    Element* acquire_consumer_element_ownership() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(alignof(Element) <= m_alignment);
        Assert_Check( sizeof(Element) <= m_size     );

        return static_cast<Element*>(acquire_consumer_element_ownership());
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
        void* element = acquire_producer_element_ownership();

        return queue_producer_element_t(*this, element);
    }

    template<typename Rep, typename Period>
    queue_consumer_element_t start_pop(std::chrono::duration<Rep, Period> timeout) noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_consumer_element_ownership(timeout);

        return queue_consumer_element_t(*this, element);
    }
    queue_consumer_element_t start_pop() noexcept(DIAG_NOEXCEPT)
    {
        void* element = acquire_consumer_element_ownership();

        return queue_consumer_element_t(*this, element);
    }

public:
    template<typename Rep, typename Period>
    bool push(std::chrono::duration<Rep, Period> timeout, const void* data = nullptr, std::size_t size = queue_npos)  noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = start_push(timeout);

        if(!producer_element.push_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(producer_element.data(), data, size);
        }
        return true;
    }
    bool push(const void* data = nullptr, std::size_t size = queue_npos) noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = start_push();

        if(!producer_element.push_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(producer_element.data(), data, size);
        }
        return true;
    }

    template<typename Rep, typename Period>
    bool pop(std::chrono::duration<Rep, Period> timeout, void* data = nullptr, std::size_t size = queue_npos) noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = start_pop(timeout);

        if(!producer_element.pop_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
                ? m_size
                : std::min(size, m_size)
                ;
            std::memcpy(data, producer_element.data(), size);
        }
        return true;
    }
    bool pop(void* data = nullptr, std::size_t size = queue_npos) noexcept(DIAG_NOEXCEPT)
    {
        auto producer_element = start_pop();

        if(!producer_element.pop_in_progress()) return false;

        if(data != nullptr)
        {
            size = (size == queue_npos)
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

namespace std
{
template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport,
    bool Pausable,
    typename BaseAllocator,
    typename WaitSupport
    >
void swap(
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>& lhs,
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>& rhs
    )
    noexcept(
    gkr::lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>::swap_is_noexcept
    )
{
    lhs.swap(rhs);
}
}
