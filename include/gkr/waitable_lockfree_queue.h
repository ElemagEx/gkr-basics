#pragma once

#include "lockfree_queue.h"
#include "waitable_event.h"
#include "objects_waiter.h"

namespace gkr
{
namespace impl
{
template<unsigned ProducerMaxWaiters, unsigned ConsumerMaxWaiters>
class queue_basic_wait_support
{
    queue_basic_wait_support           (const queue_basic_wait_support&) noexcept = delete;
    queue_basic_wait_support& operator=(const queue_basic_wait_support&) noexcept = delete;

protected:
    queue_basic_wait_support() noexcept = default;
   ~queue_basic_wait_support() noexcept = default;

    queue_basic_wait_support(queue_basic_wait_support&& other) noexcept
        : m_busy_count(other.m_busy_count.exchange(0))
        , m_free_count(other.m_free_count.exchange(0))
        , m_has_space_event(std::move(other.m_has_space_event))
        , m_has_items_event(std::move(other.m_has_items_event))
    {
    }
    queue_basic_wait_support& operator=(queue_basic_wait_support&& other) noexcept
    {
        m_busy_count = other.m_busy_count.exchange(0);
        m_free_count = other.m_free_count.exchange(0);

        m_has_space_event = std::move(other.m_has_space_event);
        m_has_items_event = std::move(other.m_has_items_event);
        return *this;
    }

    void swap(queue_basic_wait_support& other) noexcept
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

protected:
    template<typename Rep, typename Period>
    static bool wait(objects_waiter& waiter, std::chrono::duration<Rep, Period>& timeout, waitable_object& object)
    {
        using duration_t = std::chrono::duration<Rep, Period>;

        if(timeout <= duration_t::zero()) return false;

        const auto start_time = std::chrono::steady_clock::now();

        waiter.wait(timeout, object);

        if(timeout != duration_t::max())
        {
            const auto end_time = std::chrono::steady_clock::now();

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

private:
    std::atomic<std::size_t> m_busy_count {0};
    std::atomic<std::size_t> m_free_count {0};

    waitable_event<true, ProducerMaxWaiters> m_has_space_event;
    waitable_event<true, ConsumerMaxWaiters> m_has_items_event;
};

template<unsigned ProducerMaxWaiters, unsigned ConsumerMaxWaiters>
class queue_simple_wait_support : public queue_basic_wait_support<ProducerMaxWaiters, ConsumerMaxWaiters>
{
    queue_simple_wait_support           (const queue_simple_wait_support&) noexcept = delete;
    queue_simple_wait_support& operator=(const queue_simple_wait_support&) noexcept = delete;

    using base_t = queue_basic_wait_support<ProducerMaxWaiters, ConsumerMaxWaiters>;

protected:
    queue_simple_wait_support() noexcept = default;
   ~queue_simple_wait_support() noexcept = default;

    queue_simple_wait_support(queue_simple_wait_support&& other) noexcept
        : base_t(std::move(other))
        , m_producer_waiter(std::exchange(other.m_producer_waiter, nullptr))
        , m_consumer_waiter(std::exchange(other.m_consumer_waiter, nullptr))
    {
    }
    queue_simple_wait_support& operator=(queue_simple_wait_support&& other) noexcept
    {
        base_t::operator=(std::move(other));
        m_producer_waiter = std::exchange(other.m_producer_waiter, nullptr);
        m_consumer_waiter = std::exchange(other.m_consumer_waiter, nullptr);
        return *this;
    }

    void swap(queue_simple_wait_support& other) noexcept
    {
        base_t::swap(other);
        std::swap(m_producer_waiter, other.m_producer_waiter);
        std::swap(m_consumer_waiter, other.m_consumer_waiter);
    }

public:
    void set_producer_waiter(objects_waiter& waiter) noexcept
    {
        m_producer_waiter = &waiter;
    }
    void set_consumer_waiter(objects_waiter& waiter) noexcept
    {
        m_consumer_waiter = &waiter;
    }

protected:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        Check_NotNullPtr(m_producer_waiter, false);

        return base_t::wait(*m_producer_waiter, timeout, *base_t::queue_has_space_waitable_object());
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        Check_NotNullPtr(m_consumer_waiter, false);

        return base_t::wait(*m_consumer_waiter, timeout, *base_t::queue_has_items_waitable_object());
    }

private:
    objects_waiter* m_producer_waiter = nullptr;
    objects_waiter* m_consumer_waiter = nullptr;
};

template<unsigned ProducerMaxWaiters, unsigned ConsumerMaxWaiters>
class queue_default_wait_support : public queue_basic_wait_support<ProducerMaxWaiters, ConsumerMaxWaiters>
{
    queue_default_wait_support           (const queue_default_wait_support&) noexcept = delete;
    queue_default_wait_support& operator=(const queue_default_wait_support&) noexcept = delete;

    using base_t = queue_basic_wait_support<ProducerMaxWaiters, ConsumerMaxWaiters>;

protected:
    queue_default_wait_support() noexcept = default;
   ~queue_default_wait_support() noexcept = default;

    queue_default_wait_support(queue_default_wait_support&& other) noexcept
        : base_t(std::move(other))
        , m_producer_waiter(std::exchange(other.m_producer_waiter, nullptr))
        , m_consumer_waiter(std::exchange(other.m_consumer_waiter, nullptr))
    {
    }
    queue_default_wait_support& operator=(queue_default_wait_support&& other) noexcept
    {
        base_t::operator=(std::move(other));
        m_producer_waiter = std::exchange(other.m_producer_waiter, nullptr);
        m_consumer_waiter = std::exchange(other.m_consumer_waiter, nullptr);
        return *this;
    }

    void swap(queue_default_wait_support& other) noexcept
    {
        base_t::swap(other);
        std::swap(m_producer_waiter, other.m_producer_waiter);
        std::swap(m_consumer_waiter, other.m_consumer_waiter);
    }

public:
    void set_producer_waiter(objects_waiter& waiter) noexcept
    {
        m_producer_waiter = &waiter;
    }
    void set_consumer_waiter(objects_waiter& waiter) noexcept
    {
        m_consumer_waiter = &waiter;
    }

private:
    objects_waiter* get_producer_waiter()
    {
        if(m_producer_waiter != nullptr)
        {
            return m_producer_waiter;
        }
        else
        {
            return get_this_thread_objects_waiter();
        }
    }
    objects_waiter* get_consumer_waiter()
    {
        if(m_producer_waiter != nullptr)
        {
            return m_consumer_waiter;
        }
        else
        {
            return get_this_thread_objects_waiter();
        }
    }

protected:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        objects_waiter* waiter = get_producer_waiter();

        Check_NotNullPtr(waiter, false);

        return base_t::wait(*waiter, timeout, *base_t::queue_has_space_waitable_object());
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>& timeout)
    {
        objects_waiter* waiter = get_consumer_waiter();

        Check_NotNullPtr(waiter, false);

        return base_t::wait(*m_consumer_waiter, timeout, *base_t::queue_has_items_waitable_object());
    }

private:
    objects_waiter* m_producer_waiter = nullptr;
    objects_waiter* m_consumer_waiter = nullptr;
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    typename WaitSupport  =impl::queue_default_wait_support<1,1>,
    typename BaseAllocator=std::allocator<char>
    >
using waitable_lockfree_queue = lockfree_queue<T, MultipleConsumersMultipleProducersSupport, WaitSupport, BaseAllocator>;

}
