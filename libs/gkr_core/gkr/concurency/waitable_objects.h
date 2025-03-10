#pragma once

#include <gkr/api.hpp>
#include <gkr/diagnostics.hpp>

#include <utility>
#include <chrono>

namespace gkr
{
namespace impl
{

class waitable_object
{
    waitable_object           (const waitable_object&) noexcept = delete;
    waitable_object& operator=(const waitable_object&) noexcept = delete;

protected:
#ifdef _WIN32
    using object_handle_t = void*;
#else
    using object_handle_t = int;
#endif
    static constexpr object_handle_t INVALID_OBJECT_HANDLE_VALUE = object_handle_t(-1);

    object_handle_t handle() const noexcept
    {
        return m_handle;
    }
    virtual bool handle_poll_data(unsigned long long value) = 0;

private:
    object_handle_t m_handle = INVALID_OBJECT_HANDLE_VALUE;

protected:
    waitable_object(object_handle_t handle) noexcept(DIAG_NOEXCEPT) : m_handle(handle)
    {
    }
    waitable_object(waitable_object&& other) noexcept : m_handle(std::exchange(other.m_handle, INVALID_OBJECT_HANDLE_VALUE))
    {
    }
    waitable_object& operator=(waitable_object&& other) noexcept
    {
        if(this != &other)
        {
            m_handle = std::exchange(other.m_handle, INVALID_OBJECT_HANDLE_VALUE);
        }
        return *this;
    }
    void swap(waitable_object& other)
    {
        std::swap(m_handle, other.m_handle);
    }

public:
    GKR_INNER_API virtual ~waitable_object() noexcept(DIAG_NOEXCEPT);

public:
    GKR_INNER_API void close() noexcept(DIAG_NOEXCEPT);

    GKR_INNER_API bool wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT);

public:
    bool is_valid() const noexcept
    {
        return (handle() != INVALID_OBJECT_HANDLE_VALUE);
    }

public:
    bool consume() noexcept(DIAG_NOEXCEPT)
    {
        return wait(-1);
    }
    bool try_consume() noexcept(DIAG_NOEXCEPT)
    {
        return wait(0);
    }

public:
    template<typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) noexcept(DIAG_NOEXCEPT)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_duration).count();

        return wait(timeout_ns);
    }
    template<typename Clock, typename Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept(DIAG_NOEXCEPT)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - timeout_time).count();

        return wait(timeout_ns);
    }
};

}

class waitable_event : public impl::waitable_object
{
    waitable_event           (const waitable_event&) noexcept = delete;
    waitable_event& operator=(const waitable_event&) noexcept = delete;

    using base_t = impl::waitable_object;

    bool m_manual_reset;

public:
    waitable_event(bool manual_reset = false, bool create_fired = false) noexcept(DIAG_NOEXCEPT)
        : base_t(create(manual_reset, create_fired))
        , m_manual_reset(manual_reset)
    {
    }
    waitable_event(waitable_event&& other) noexcept
        : base_t(std::move(other))
        , m_manual_reset(std::exchange(other.m_manual_reset, false))
    {
    }
    waitable_event& operator=(waitable_event&& other) noexcept
    {
        base_t::operator=(std::move(other));
        m_manual_reset = std::exchange(other.m_manual_reset, false);
        return *this;
    }
    void swap(waitable_event& other)
    {
        base_t::swap(other);
        std::swap(m_manual_reset, other.m_manual_reset);
    }

    GKR_INNER_API virtual ~waitable_event() noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_INNER_API virtual bool handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT) override;

    GKR_INNER_API static object_handle_t create(bool manual_reset, bool create_fired) noexcept(DIAG_NOEXCEPT);

public:
    GKR_INNER_API void fire() noexcept(DIAG_NOEXCEPT);

    GKR_INNER_API void reset() noexcept(DIAG_NOEXCEPT);

public:
    GKR_INNER_API void set_manual_reset(bool manual_reset) noexcept(DIAG_NOEXCEPT);

    bool is_manual_reset() const noexcept { return m_manual_reset; }
};

class waitable_semaphore : public impl::waitable_object
{
    waitable_semaphore           (const waitable_semaphore&) noexcept = delete;
    waitable_semaphore& operator=(const waitable_semaphore&) noexcept = delete;

    using base_t = impl::waitable_object;

    unsigned m_max_count;

public:
    waitable_semaphore(unsigned max_count) noexcept(DIAG_NOEXCEPT)
        : base_t(create(max_count, max_count))
        , m_max_count(max_count)
    {
    }
    waitable_semaphore(unsigned max_count, unsigned init_count) noexcept(DIAG_NOEXCEPT)
        : base_t(create(max_count, init_count))
        , m_max_count(max_count)
    {
    }
    waitable_semaphore(waitable_semaphore&& other) noexcept
        : base_t(std::move(other))
        , m_max_count(std::exchange(other.m_max_count, 0U))
    {
    }
    waitable_semaphore& operator=(waitable_semaphore&& other) noexcept
    {
        base_t::operator=(std::move(other));
        m_max_count = std::exchange(other.m_max_count, 0U);
        return *this;
    }
    void swap(waitable_semaphore& other)
    {
        base_t::swap(other);
        std::swap(m_max_count, other.m_max_count);
    }

    GKR_INNER_API virtual ~waitable_semaphore() noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_INNER_API virtual bool handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT) override;

    GKR_INNER_API static object_handle_t create(unsigned max_count, unsigned init_count) noexcept(DIAG_NOEXCEPT);

public:
    GKR_INNER_API bool release(unsigned count = 1) noexcept(DIAG_NOEXCEPT);

    GKR_INNER_API void set_max_count(unsigned max_count) noexcept(DIAG_NOEXCEPT);

    unsigned get_max_count() const noexcept
    {
        return m_max_count;
    }
};

}
