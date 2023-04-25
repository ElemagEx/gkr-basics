#pragma once

#include <gkr/lockfree_queue.h>
#include <gkr/waitable_event.h>

namespace gkr
{
namespace impl
{
template<unsigned ProducerMaxWaiters, unsigned ConsumerMaxWaiters>
class queue_wait_support
{
    queue_wait_support           (const queue_wait_support&) noexcept = delete;
    queue_wait_support& operator=(const queue_wait_support&) noexcept = delete;

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
    void reset(std::size_t capacity) noexcept
    {
        m_busy_count = 0;
        m_free_count = capacity;

        if(capacity > 0)
        {
            m_has_space_event.fire();
        }
        else
        {
            m_has_space_event.reset();
        }
        m_has_items_event.reset();
    }
    void resize(std::size_t capacity) noexcept
    {
        m_free_count = capacity - m_busy_count;
    }

protected:
    void notify_producer_ownership_fail() noexcept
    {
        m_has_space_event.reset();
    }
    void notify_producer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        const std::ptrdiff_t new_free_count = std::ptrdiff_t(--m_free_count);

        Assert_Check(new_free_count >= 0);

        if(new_free_count == 0)
        {
            m_has_space_event.reset();
        }
    }
    void notify_producer_ownership_finish() noexcept
    {
        if(++m_busy_count == 1)
        {
            m_has_items_event.fire();
        }
    }
    void notify_producer_ownership_cancel() noexcept
    {
        if(++m_free_count == 1)
        {
            m_has_space_event.fire();
        }
    }

protected:
    void notify_consumer_ownership_fail() noexcept
    {
        m_has_items_event.reset();
    }
    void notify_consumer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        const std::ptrdiff_t new_busy_count = std::ptrdiff_t(--m_busy_count);

        Assert_Check(new_busy_count >= 0);

        if(new_busy_count == 0)
        {
            m_has_items_event.reset();
        }
    }
    void notify_consumer_ownership_finish() noexcept
    {
        if(++m_free_count == 1)
        {
            m_has_space_event.fire();
        }
    }
    void notify_consumer_ownership_cancel() noexcept
    {
        if(++m_busy_count == 1)
        {
            m_has_items_event.fire();
        }
    }

private:
    template<typename Rep, typename Period>
    static bool wait(std::chrono::duration<Rep, Period>& timeout, waitable_object& object)
    {
        using duration_t = std::chrono::duration<Rep, Period>;

        if(timeout <= duration_t::zero()) return false;

        const auto start_time = std::chrono::system_clock::now();

        this_thread_objects_waiter().wait(timeout, object);

        if(timeout != duration_t::max())
        {
            const auto end_time = std::chrono::system_clock::now();

            const duration_t elapsed_time = std::chrono::duration_cast<duration_t>(end_time - start_time);

            timeout -= elapsed_time;
        }
        return true;
    }

public:
    waitable_object* queue_has_space_waitable_object() noexcept
    {
        return &m_has_space_event;
    }
    waitable_object* queue_has_items_waitable_object() noexcept
    {
        return &m_has_items_event;
    }

protected:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        return wait(timeout, *queue_has_space_waitable_object());
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        return wait(timeout, *queue_has_items_waitable_object());
    }

private:
    std::atomic<std::size_t> m_busy_count {0};
    std::atomic<std::size_t> m_free_count {0};

    waitable_event<true, ProducerMaxWaiters> m_has_space_event;
    waitable_event<true, ConsumerMaxWaiters> m_has_items_event;
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    typename WaitSupport  =impl::queue_wait_support<1,1>,
    typename BaseAllocator=std::allocator<char>
    >
using waitable_lockfree_queue = lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;

}
