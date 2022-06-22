#pragma once

#include "lockfree_queue.h"
#include "waitable_event.h"
#include "objects_waiter.h"

#ifdef LOCKFREE_QUEUE_EXCLUDE_WAITING
#error The classes in this file are not functional when lock-free queue waiting is excluded
#endif

namespace gkr
{
namespace detail
{
class queue_simple_synchronization
{
    queue_simple_synchronization           (const queue_simple_synchronization&) noexcept = delete;
    queue_simple_synchronization& operator=(const queue_simple_synchronization&) noexcept = delete;

public:
    queue_simple_synchronization() noexcept = default;
   ~queue_simple_synchronization() noexcept = default;

    queue_simple_synchronization(queue_simple_synchronization&& other) noexcept
        : m_objects_waiter(other.m_objects_waiter)
        , m_has_space_event(std::move(other.m_has_space_event))
        , m_non_empty_event(std::move(other.m_non_empty_event))
    {
    }
    queue_simple_synchronization& operator=(queue_simple_synchronization&& other) noexcept
    {
        m_has_space_event = std::move(other.m_has_space_event);
        m_non_empty_event = std::move(other.m_non_empty_event);
        return *this;
    }

    void swap(queue_simple_synchronization& other) noexcept
    {
        m_has_space_event.swap(other.m_has_space_event);
        m_non_empty_event.swap(other.m_non_empty_event);
    }

public:
    void set_waiter(objects_waiter& waiter)
    {
        m_objects_waiter = &waiter;
    }

public:
    template<bool IsFull>
    void notify_queue_full() noexcept
    {
#ifdef __cpp_if_constexpr
        if constexpr(IsFull)
#else
        if          (IsFull)
#endif
        {
            m_has_space_event.reset();
        }
        else
        {
            m_has_space_event.fire();
        }
    }
    template<bool IsEmpty>
    void notify_queue_empty() noexcept
    {
#ifdef __cpp_if_constexpr
        if constexpr(IsEmpty)
#else
        if          (IsEmpty)
#endif
        {
            m_non_empty_event.reset();
        }
        else
        {
            m_non_empty_event.fire();
        }
    }

private:
    template<typename Rep, typename Period>
    static bool wait(objects_waiter& waiter, std::chrono::duration<Rep, Period>& timeout, waitable_object& object)
    {
        using duration_t = std::chrono::duration<Rep, Period>;

        if(timeout <= duration_t::zero()) return false;

        const auto start_time = std::chrono::steady_clock::now();

        waiter.wait(timeout, object);

        const auto end_time = std::chrono::steady_clock::now();

        const duration_t elapsed_time = std::chrono::duration_cast<duration_t>(end_time - start_time);

        timeout -= elapsed_time;

        return true;
    }

public:
    template<typename Rep, typename Period>
    static constexpr bool has_wait()
    {
        return true;
    }

public:
    template<typename Rep, typename Period>
    bool producer_wait(std::chrono::duration<Rep, Period>& timeout) noexcept
    {
        Check_NotNullPtr(m_objects_waiter, false);

        return wait(*m_objects_waiter, timeout, m_has_space_event);
    }
    template<typename Rep, typename Period>
    bool consumer_wait(std::chrono::duration<Rep, Period>& timeout) noexcept
    {
        Check_NotNullPtr(m_objects_waiter, false);

        return wait(*m_objects_waiter, timeout, m_non_empty_event);
    }

public:
    waitable_object* queue_has_space_waitable_object() noexcept
    {
        return &m_has_space_event;
    }
    waitable_object* queue_non_empty_waitable_object() noexcept
    {
        return &m_non_empty_event;
    }

private:
    objects_waiter*      m_objects_waiter {nullptr};
    waitable_event<true> m_has_space_event;
    waitable_event<true> m_non_empty_event;
};

}
}
