#pragma once

#include <gkr/api.hpp>
#include <gkr/diagnostics.hpp>

#include <utility>
#include <chrono>

namespace gkr
{

using wait_result_t = unsigned long long;

static constexpr std::size_t WAIT_MAX_OBJECTS = sizeof(wait_result_t) * 8 - 1;

static constexpr long long WAIT_INFINITE = -1;

static constexpr wait_result_t WAIT_RESULT_ERROR   = wait_result_t(1) << WAIT_MAX_OBJECTS;
static constexpr wait_result_t WAIT_RESULT_TIMEOUT = wait_result_t(0);

#ifndef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#define GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
#endif
#endif

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
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    std::size_t m_wait_count = 0;
#endif

protected:
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    std::size_t wait_count() const noexcept { return m_wait_count; }
#else
    static constexpr std::size_t wait_count() noexcept { return 0; }
#endif

    waitable_object(object_handle_t handle) noexcept : m_handle(handle)
    {
    }
    waitable_object(waitable_object&& other) noexcept
        : m_handle(std::exchange(other.m_handle, INVALID_OBJECT_HANDLE_VALUE))
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
        , m_wait_count(std::exchange(other.m_wait_count, 0U))
#endif
    {
    }
    waitable_object& operator=(waitable_object&& other) noexcept
    {
        m_handle = std::exchange(other.m_handle, INVALID_OBJECT_HANDLE_VALUE);
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
        m_wait_count = std::exchange(other.m_wait_count, 0U);
#endif
        return *this;
    }
    void swap(waitable_object& other)
    {
        std::swap(m_handle, other.m_handle);
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
        std::swap(m_wait_count, other.m_wait_count);
#endif
    }

public:
    bool is_valid() const noexcept
    {
        return (handle() != INVALID_OBJECT_HANDLE_VALUE);
    }

public:
    GKR_INNER_API virtual ~waitable_object();

public:
    GKR_INNER_API void close();

    GKR_INNER_API bool wait(long long timeout_ns = WAIT_INFINITE);

public:
    GKR_INNER_API static wait_result_t wait_many(long long timeout_ns, waitable_object** objects, std::size_t count);

public:
    bool consume()
    {
        return wait(WAIT_INFINITE);
    }
    bool try_consume()
    {
        return wait(0);
    }

public:
    template<typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout_duration)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_duration).count();

        return wait(timeout_ns);
    }
    template<typename Clock, typename Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - timeout_time).count();

        return wait(timeout_ns);
    }

public:
    template<typename... Objects>
    static wait_result_t wait_many(long long timeout_ns, Objects&&... objects)
    {
        constexpr std::size_t count = sizeof...(objects);

        static_assert(count >  0               , "At least one waitable object must be passed");
        static_assert(count <= WAIT_MAX_OBJECTS, "Too many waitable objects are passed");

        waitable_object* objs[count] = { &objects... };

        return wait_many(timeout_ns, objs, count);
    }

public:
    template<typename Rep, typename Period, typename... Objects>
    static wait_result_t wait_many_for(const std::chrono::duration<Rep, Period>& timeout_duration, Objects&&... objects)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_duration).count();

        return wait_many(timeout_ns, std::forward<Objects>(objects)...);
    }
    template<typename Clock, typename Duration, typename... Objects>
    static wait_result_t wait_many_until(const std::chrono::time_point<Clock, Duration>& timeout_time, Objects&&... objects)
    {
        const long long timeout_ns = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - timeout_time).count();

        return wait_many(timeout_ns, std::forward<Objects>(objects)...);
    }

public:
    static bool is_signaled(wait_result_t wait_result, std::size_t index) noexcept
    {
        return ((wait_result & (wait_result_t(1) << index)) != 0);
    }

public:
    GKR_INNER_API static waitable_object& null_ref() noexcept;
};

}
