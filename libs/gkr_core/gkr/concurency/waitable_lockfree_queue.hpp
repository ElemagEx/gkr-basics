#pragma once

#include <gkr/container/lockfree_queue.hpp>
#include <gkr/concurency/waitable_event.hpp>

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

    waitable_event m_has_space_event = true;
    waitable_event m_has_items_event = true;

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
    void reserve(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
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
    bool producer_wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_space_event.wait(timeout_ns);
    }
    bool consumer_wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
    {
        return m_has_items_event.wait(timeout_ns);
    }

public:
    waitable_object& get_producer_waitable_object() noexcept
    {
        return m_has_space_event;
    }
    waitable_object& get_consumer_waitable_object() noexcept
    {
        return m_has_items_event;
    }
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    bool Pausable=false,
    typename Allocator=std::allocator<impl::queue_allocator_value_type<T>>
    >
using waitable_lockfree_queue = lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, Allocator, impl::queue_wait_support>;

}
