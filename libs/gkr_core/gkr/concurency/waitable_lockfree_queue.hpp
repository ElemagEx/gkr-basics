#pragma once

#include <gkr/container/lockfree_queue.hpp>
#include <gkr/concurency/events_waiting.hpp>

namespace gkr
{
namespace impl
{
class queue_wait_support
{
    queue_wait_support           (const queue_wait_support&) noexcept = delete;
    queue_wait_support& operator=(const queue_wait_support&) noexcept = delete;

private:
    std::atomic<std::size_t> m_busy_count {0};
    std::atomic<std::size_t> m_free_count {0};

    event_controller m_has_space_event;
    event_controller m_has_items_event;

protected:
    queue_wait_support() noexcept = default;
   ~queue_wait_support() noexcept = default;

    queue_wait_support(queue_wait_support&& other) noexcept
        : m_busy_count(other.m_busy_count.exchange(0))
        , m_free_count(other.m_free_count.exchange(0))
        , m_has_space_event(std::move(other.m_has_space_event))
        , m_has_items_event(std::move(other.m_has_items_event))
    {
    }
    queue_wait_support& operator=(queue_wait_support&& other) noexcept
    {
        m_busy_count = other.m_busy_count.exchange(0);
        m_free_count = other.m_free_count.exchange(0);

        m_has_space_event = std::move(other.m_has_space_event);
        m_has_items_event = std::move(other.m_has_items_event);
        return *this;
    }

    void swap(queue_wait_support& other) noexcept
    {
        m_busy_count = other.m_busy_count.exchange(m_busy_count);
        m_free_count = other.m_free_count.exchange(m_free_count);

        m_has_space_event.swap(other.m_has_space_event);
        m_has_items_event.swap(other.m_has_items_event);
    }

protected:
    void reset(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        m_busy_count = 0;
        m_free_count = capacity;

        if(m_free_count > 0)
        {
            m_has_space_event.fire();
        }
        else
        {
            m_has_space_event.reset();
        }
        m_has_items_event.reset();
    }
    void resize(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        m_free_count = capacity - m_busy_count;

        if(m_free_count > 0)
        {
            m_has_space_event.fire();
        }
        else
        {
            m_has_space_event.reset();
        }
    }

protected:
    void notify_producer_ownership_fail() noexcept(DIAG_NOEXCEPT)
    {
        m_has_space_event.reset();
        m_has_items_event.fire();
    }
    void notify_producer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count-- == 1)
        {
            m_has_space_event.reset();
        }
    }
    void notify_producer_ownership_finish() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count++ == 0)
        {
            m_has_items_event.fire();
        }
    }
    void notify_producer_ownership_cancel() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count++ == 0)
        {
            m_has_space_event.fire();
        }
    }

protected:
    void notify_consumer_ownership_fail() noexcept(DIAG_NOEXCEPT)
    {
        m_has_space_event.fire();
        m_has_items_event.reset();
    }
    void notify_consumer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count-- == 1)
        {
            m_has_items_event.reset();
        }
    }
    void notify_consumer_ownership_finish() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count++ == 0)
        {
            m_has_space_event.fire();
        }
    }
    void notify_consumer_ownership_cancel() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count++ == 0)
        {
            m_has_items_event.fire();
        }
    }

protected:
    template<typename Clock, typename Duration>
    bool producer_wait(const std::chrono::time_point<Clock, Duration>& wait_time) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_space_event.wait_until(wait_time);
    }
    void producer_wait() noexcept(DIAG_NOEXCEPT)
    {
        m_has_space_event.wait();
    }
    template<typename Clock, typename Duration>
    bool consumer_wait(const std::chrono::time_point<Clock, Duration>& wait_time) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_items_event.wait_until(wait_time);
    }
    void consumer_wait() noexcept(DIAG_NOEXCEPT)
    {
        m_has_items_event.wait();
    }

public:
    bool bind_with_producer_waiter(events_waiter& waiter) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_space_event.bind_with(waiter, true, false);
    }
    bool bind_with_consumer_waiter(events_waiter& waiter) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_items_event.bind_with(waiter, true, false);
    }

public:
    bool producer_event_is_signaled(wait_result_t wait_result) const noexcept
    {
        return m_has_space_event.is_signaled(wait_result);
    }
    bool consumer_event_is_signaled(wait_result_t wait_result) const noexcept
    {
        return m_has_items_event.is_signaled(wait_result);
    }
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    bool Pausable=false,
    typename BaseAllocator=std::allocator<std::max_align_t>,
    typename WaitSupport  =impl::queue_wait_support
    >
using waitable_lockfree_queue = lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, BaseAllocator, WaitSupport>;

}
