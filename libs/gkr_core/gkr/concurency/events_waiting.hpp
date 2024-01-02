#pragma once

#include <chrono>
#include <climits>
#include <utility>

#ifndef GKR_NO_STL_EVENTS_WAITER
#include <mutex>
#include <condition_variable>
#endif

#ifndef GKR_EVENTS_WAITING_SINGLE_HEADER

#include <gkr/diagnostics.hpp>

#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.hpp>
#endif

#else

#ifndef __cpp_lib_exchange_function
namespace std
{
template<class T, class U=T>
T exchange(T& obj, U&& new_value) noexcept(
    std::is_nothrow_move_constructible<T>::value &&
    std::is_nothrow_assignable<T&, U>::value
    )
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
}
#endif

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Check_ValidState
#define Check_ValidState    if(check, ...) if(!(check)) return __VA_ARGS__
#endif
#ifndef Check_Arg_IsValid
#define Check_Arg_IsValid   if(check, ...) if(!(check)) return __VA_ARGS__
#endif
#ifndef Assert_Check
#include <cassert>
#define Assert_Check assert
#endif

#endif

namespace gkr
{

using wait_result_t = unsigned long long;

constexpr std::size_t WAIT_MAX_OBJECTS = sizeof(wait_result_t) * CHAR_BIT - 1;

constexpr wait_result_t WAIT_RESULT_ERROR   = wait_result_t(1) << WAIT_MAX_OBJECTS;
constexpr wait_result_t WAIT_RESULT_TIMEOUT = wait_result_t(0);

class event_controller;

class events_waiter
{
    events_waiter           (const events_waiter&) noexcept = delete;
    events_waiter& operator=(const events_waiter&) noexcept = delete;

    events_waiter           (events_waiter&&) noexcept = delete;
    events_waiter& operator=(events_waiter&&) noexcept = delete;

protected:
    events_waiter(std::size_t flags) : m_flags(flags)
    {
    }
    virtual ~events_waiter()
    {
    }

private:
    std::size_t m_flags;

    friend class event_controller;

public:
    enum Flag
    {
        Flag_ForbidMultipleThreadsWait = 0x01,
        Flag_ForbidMultipleEventsBind  = 0x02,
        Flag_AllowPartialEventsWait    = 0x04,
    };
    bool flag_is_set(std::size_t flag) const noexcept
    {
        return ((m_flags & flag) != 0);
    }

public:
    virtual std::size_t events_count() const noexcept = 0;

    virtual bool pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT) = 0;

    virtual bool remove_all_events() noexcept(DIAG_NOEXCEPT) = 0;

protected:
    virtual bool add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false) = 0;

    virtual bool fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT) = 0;

    virtual bool reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT) = 0;

protected:
    virtual wait_result_t check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT) = 0;

    virtual wait_result_t wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT) = 0;

public:
    virtual wait_result_t check_all_events() noexcept(DIAG_NOEXCEPT) = 0;

    virtual wait_result_t wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT) = 0;

private:
    static bool event_is_signaled(wait_result_t wait_result, std::size_t index) noexcept
    {
        const wait_result_t bit = (wait_result_t(1) << index);

        return ((wait_result & bit) != 0);
    }

protected:
    static long long calc_timeout() noexcept
    {
        return -1;
    }
    template<typename Rep, typename Period>
    static long long calc_timeout(const std::chrono::duration<Rep, Period>& timeout_duration) noexcept
    {
        const long long timeout = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_duration).count();
        return (timeout <= 0)
            ? 0
            : timeout;
    }
    template<typename Clock, typename Duration>
    static long long calc_timeout(const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept
    {
        const long long timeout = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - timeout_time).count();
        return (timeout <= 0)
            ? 0
            : timeout;
    }

private:
    wait_result_t single_check(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        return check_single_event(index);
    }
    wait_result_t single_wait(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        return wait_single_event(index, calc_timeout());
    }
    template<typename Rep, typename Period>
    wait_result_t single_wait_for(std::size_t index, const std::chrono::duration<Rep, Period>& timeout_duration) noexcept(DIAG_NOEXCEPT)
    {
        return wait_single_event(index, calc_timeout(timeout_duration));
    }
    template<typename Clock, typename Duration>
    wait_result_t single_wait_until(std::size_t index, const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept(DIAG_NOEXCEPT)
    {
        return wait_single_event(index, calc_timeout(timeout_time));
    }

public:
    wait_result_t check() noexcept(DIAG_NOEXCEPT)
    {
        return check_all_events();
    }
    wait_result_t wait() noexcept(DIAG_NOEXCEPT)
    {
        return wait_all_events(calc_timeout());
    }
    template<typename Rep, typename Period>
    wait_result_t wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) noexcept(DIAG_NOEXCEPT)
    {
        return wait_all_events(calc_timeout(timeout_duration));
    }
    template<typename Clock, typename Duration>
    wait_result_t wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept(DIAG_NOEXCEPT)
    {
        return wait_all_events(calc_timeout(timeout_time));
    }
};

class event_controller final
{
    event_controller           (const event_controller&) noexcept = delete;
    event_controller& operator=(const event_controller&) noexcept = delete;

private:
    events_waiter* m_waiter {nullptr};
    std::size_t    m_index  {std::size_t(-1)};

public:
    event_controller(events_waiter& waiter, bool manual_reset, bool initial_state) noexcept(DIAG_NOEXCEPT)
    {
        bind_with(waiter, manual_reset, initial_state);
        Assert_Check(is_bound());
    }

    event_controller() noexcept = default;
   ~event_controller() noexcept = default;

    event_controller(event_controller&& other) noexcept
        : m_waiter(std::exchange(other.m_waiter, nullptr))
        , m_index (std::exchange(other.m_index , std::size_t(-1)))
    {
    }
    event_controller& operator=(event_controller&& other) noexcept
    {
        m_waiter = std::exchange(other.m_waiter, nullptr);
        m_index  = std::exchange(other.m_index , std::size_t(-1));
        return *this;
    }

    void swap(event_controller& other) noexcept
    {
        std::swap(m_waiter, other.m_waiter);
        std::swap(m_index , other.m_index );
    }

public:
    bool bind_with(events_waiter& waiter, bool manual_reset, bool initial_state) noexcept(DIAG_NOEXCEPT)
    {
        if(!waiter.add_event(manual_reset, initial_state, m_index)) return false;
        m_waiter = &waiter;
        return true;
    }
    void unbind() noexcept
    {
        m_waiter = nullptr;
        m_index  = std::size_t(-1);
    }
    bool is_bound() const noexcept
    {
        return (m_waiter != nullptr);
    }

public:
    bool fire() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(is_bound(), false);

        return m_waiter->fire_event(m_index);
    }
    bool reset() noexcept(DIAG_NOEXCEPT)
    {
        Check_ValidState(is_bound(), false);

        return m_waiter->reset_event(m_index);
    }

public:
    bool is_signaled(wait_result_t wait_result) const noexcept
    {
        return events_waiter::event_is_signaled(wait_result, m_index);
    }

public:
    bool try_consume() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_bound());

        wait_result_t wait_result = m_waiter->single_check(m_index);

        Assert_Check(wait_result != WAIT_RESULT_ERROR);

        return is_signaled(wait_result);
    }
    void wait() noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_bound());

        [[maybe_unused]] wait_result_t wait_result = m_waiter->single_wait(m_index);

        Assert_Check(wait_result != WAIT_RESULT_ERROR);
        Assert_Check(is_signaled(wait_result));
    }
    template<typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_bound());

        wait_result_t wait_result = m_waiter->single_wait_for(m_index, timeout_duration);

        Assert_Check(wait_result != WAIT_RESULT_ERROR);

        return is_signaled(wait_result);
    }
    template<typename Clock, typename Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(is_bound());

        wait_result_t wait_result = m_waiter->single_wait_until(m_index, timeout_time);

        Assert_Check(wait_result != WAIT_RESULT_ERROR);

        return is_signaled(wait_result);
    }
};

class mutex_controller final
{
    mutex_controller           (const mutex_controller&) noexcept = delete;
    mutex_controller& operator=(const mutex_controller&) noexcept = delete;

private:
    event_controller m_event_controller;

public:
    mutex_controller(events_waiter& waiter) noexcept(DIAG_NOEXCEPT)
        : m_event_controller(waiter, false, true)
    {
    }

    mutex_controller() noexcept = default;
   ~mutex_controller() noexcept = default;

    mutex_controller(mutex_controller&& other) noexcept
        : m_event_controller(std::move(other.m_event_controller))
    {
    }
    mutex_controller& operator=(mutex_controller&& other) noexcept
    {
        m_event_controller = std::move(other.m_event_controller);
        return *this;
    }

    void swap(mutex_controller& other) noexcept
    {
        m_event_controller.swap(other.m_event_controller);
    }

public:
    bool bind_with(events_waiter& waiter) noexcept(DIAG_NOEXCEPT)
    {
        return m_event_controller.bind_with(waiter, false, true);
    }
    void unbind() noexcept
    {
        return m_event_controller.unbind();
    }
    bool is_bound() const noexcept
    {
        return m_event_controller.is_bound();
    }

public:
    bool is_locked(wait_result_t wait_result) const noexcept
    {
        return m_event_controller.is_signaled(wait_result);
    }

public:
    [[nodiscard]]
    bool try_lock()
    {
        Assert_Check(is_bound());
        return m_event_controller.try_consume();
    }
    void lock()
    {
        Assert_Check(is_bound());
        m_event_controller.wait();
    }
    void unlock()
    {
        Assert_Check(is_bound());
        [[maybe_unused]] bool fire_result = m_event_controller.fire();
        Assert_Check(fire_result);
    }
};

#ifndef GKR_NO_STL_EVENTS_WAITER

class stl_events_waiter final : public events_waiter
{
    stl_events_waiter           (const stl_events_waiter&) noexcept = delete;
    stl_events_waiter& operator=(const stl_events_waiter&) noexcept = delete;

    stl_events_waiter           (stl_events_waiter&&) noexcept = delete;
    stl_events_waiter& operator=(stl_events_waiter&&) noexcept = delete;

public:
    stl_events_waiter(std::size_t flags = 0) noexcept : events_waiter(flags)
    {
    }
    virtual ~stl_events_waiter() noexcept
    {
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cvar;

    wait_result_t m_bits_manual_reset {0};
    wait_result_t m_bits_event_state  {0};
    std::size_t   m_events_count      {0};
    std::size_t   m_waiting_threads   {0};

public:
    virtual std::size_t events_count() const noexcept override
    {
        return m_events_count;
    }

public:
    virtual bool pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT) override
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_Arg_IsValid(min_count <= m_events_count, false);

        Check_ValidState(m_waiting_threads == 0, false);

        const wait_result_t mask = (wait_result_t(1) << min_count) - 1;

        m_bits_manual_reset &= ~mask;
        m_bits_event_state  &= ~mask;
        m_events_count       = min_count;

        return true;
    }
    virtual bool remove_all_events() noexcept(DIAG_NOEXCEPT) override
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_waiting_threads == 0, false);

        m_bits_manual_reset = 0;
        m_bits_event_state  = 0;
        m_events_count      = 0;

        return true;
    }

private:
    virtual bool add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false) override
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_waiting_threads == 0, false);
        Check_ValidState(m_events_count    <  WAIT_MAX_OBJECTS, false);

        Check_ValidState(!flag_is_set(Flag_ForbidMultipleEventsBind) || (m_events_count == 0), false);

        index = m_events_count++;

        const wait_result_t bit = wait_result_t(1) << index;

        if(manual_reset ) m_bits_manual_reset |= bit;
        if(initial_state) m_bits_event_state  |= bit;

        return true;
    }
    virtual bool fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override
    {
        bool notify_all;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            Check_Arg_IsValid(index < m_events_count, false);

            const wait_result_t bit = (wait_result_t(1) << index);

            if((m_bits_event_state & bit) != 0) return true;

            m_bits_event_state |= bit;

            if(m_waiting_threads == 0) return true;

            notify_all =
                ((m_bits_manual_reset & bit) != 0)
                ||
                flag_is_set(Flag_AllowPartialEventsWait);
        }
        if(notify_all)
        {
            m_cvar.notify_all();
        }
        else
        {
            m_cvar.notify_one();
        }
        return true;
    }
    virtual bool reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_Arg_IsValid(index < m_events_count, false);

        const wait_result_t bit = (wait_result_t(1) << index);

        m_bits_event_state &= ~bit;

        return true;
    }

private:
    wait_result_t all_events_mask() const noexcept
    {
        return ((wait_result_t(1) << m_events_count) - 1);
    }
    bool mask_is_valid(wait_result_t mask, bool check_only) const noexcept
    {
        if(check_only || flag_is_set(Flag_AllowPartialEventsWait))
        {
            return ((mask & all_events_mask()) != 0);
        }
        else
        {
            return (mask == all_events_mask());
        }
    }
    bool this_thread_can_wait() const noexcept
    {
        return !flag_is_set(Flag_ForbidMultipleThreadsWait) || (m_waiting_threads == 0);
    }

private:
    bool has_signaled_event(wait_result_t& wait_result, wait_result_t mask) noexcept
    {
        wait_result = m_bits_event_state & mask;
        m_bits_event_state &= (~mask | m_bits_manual_reset);
        return (wait_result != 0);
    }

private:
    wait_result_t masked_check(wait_result_t mask) noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_events_count > 0, WAIT_RESULT_ERROR);
        Check_ValidState(mask_is_valid(mask, true), WAIT_RESULT_ERROR);

        wait_result_t wait_result = WAIT_RESULT_ERROR;
        has_signaled_event(wait_result, mask);
        return wait_result;
    }
    wait_result_t masked_wait(wait_result_t mask, long long nsec) noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_events_count > 0, WAIT_RESULT_ERROR);
        Check_ValidState(mask_is_valid(mask, false), WAIT_RESULT_ERROR);
        Check_ValidState(this_thread_can_wait(), WAIT_RESULT_ERROR);

        wait_result_t wait_result = WAIT_RESULT_ERROR;

        ++m_waiting_threads;
        if(nsec < 0)
        {
            m_cvar.wait(lock, [this, &wait_result, mask] () noexcept { return has_signaled_event(wait_result, mask); });
        }
        else
        {
            const auto duration = std::chrono::nanoseconds(nsec);
            m_cvar.wait_for(lock, duration, [this, &wait_result, mask] () noexcept { return has_signaled_event(wait_result, mask); });
        }
        --m_waiting_threads;

        return wait_result;
    }

private:
    virtual wait_result_t check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override
    {
        const wait_result_t bit = wait_result_t(1) << index;

        return masked_check(bit);
    }
    virtual wait_result_t wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT) override
    {
        const wait_result_t bit = wait_result_t(1) << index;

        return masked_wait(bit, nsec);
    }

private:
    virtual wait_result_t check_all_events() noexcept(DIAG_NOEXCEPT) override
    {
        return masked_check(all_events_mask());
    }
    virtual wait_result_t wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT) override
    {
        return masked_wait(all_events_mask(), nsec);
    }
};

#endif /*ndef GKR_NO_STL_EVENTS_WAITER*/

}
