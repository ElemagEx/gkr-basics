#include <catch2/catch_test_macros.hpp>

#include <gkr/thread_waiting.h>
#include <gkr/waitable_event.h>

#include <thread>

#include <mutex>
#include <chrono>
#include <condition_variable>

namespace gkr
{

class events_waiter;

class event_controller final
{
    event_controller           (const event_controller&) noexcept = delete;
    event_controller& operator=(const event_controller&) noexcept = delete;

private:
    events_waiter* m_waiter {nullptr};
    std::size_t    m_index  {0};

    friend class events_waiter;

public:
    event_controller() noexcept = default;
   ~event_controller() noexcept = default;

    event_controller(event_controller&& other) noexcept
        : m_waiter(std::exchange(other.m_waiter, nullptr))
        , m_index (std::exchange(other.m_index , 0))
    {
    }
    event_controller& operator=(event_controller&& other) noexcept
    {
        m_waiter = std::exchange(other.m_waiter, nullptr);
        m_index  = std::exchange(other.m_index , 0);
        return *this;
    }

public:
    bool bond_with(events_waiter& waiter, bool manual_reset, bool initial_state) noexcept(DIAG_NOEXCEPT)
    {
        return waiter.add_event(manual_reset, initial_state, *this);
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
};

class events_waiter final
{
    events_waiter           (const events_waiter&) noexcept = delete;
    events_waiter& operator=(const events_waiter&) noexcept = delete;

    events_waiter           (events_waiter&&) noexcept = delete;
    events_waiter& operator=(events_waiter&&) noexcept = delete;

public:
    events_waiter() noexcept = default;
   ~events_waiter() noexcept = default;

private:
    std::mutex m_mutex;
    std::condition_variable m_cvar;

    wait_result_t m_bits_manual_reset {0};
    wait_result_t m_bits_signaled     {0};
    std::size_t   m_events_count      {0};
    std::size_t   m_waiting_threads   {0};

public:
    bool clear_all_events() noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_waiting_threads == 0, false);

        m_bits_manual_reset = 0;
        m_bits_signaled     = 0;
        m_events_count      = 0;

        return true;
    }

private:
    friend class event_controller;

    bool add_event(bool manual_reset, bool initial_state, event_controller& controller) noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_waiting_threads == 0, false);
        Check_ValidState(m_events_count < WAIT_MAX_OBJECTS, false);

        const std::size_t index = ++m_events_count;

        const wait_result_t bit = wait_result_t(1) << index;

        if(manual_reset ) m_bits_manual_reset |= bit;
        if(initial_state) m_bits_signaled     |= bit;

        controller.m_waiter = this;
        controller.m_index  = index;
        return true;
    }
    bool fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        bool notify, manual_reset;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            Check_Arg_IsValid(index < m_events_count, false);

            const wait_result_t bit = (wait_result_t(1) << index);

            if((m_bits_signaled & bit) != 0)
            {
                return true;
            }
            m_bits_signaled |= bit;

            notify       = (m_waiting_threads > 0);
            manual_reset = ((m_bits_manual_reset & bit) != 0);
        }
        if(notify)
        {
            if(manual_reset)
            {
                m_cvar.notify_all();
            }
            else
            {
                m_cvar.notify_one();
            }
        }
        return true;
    }
    bool reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_Arg_IsValid(index < m_events_count, false);

        const wait_result_t bit = (wait_result_t(1) << index);

        m_bits_signaled &= ~bit;

        return true;
    }

private:
    bool stop_waiting(wait_result_t& result) noexcept
    {
        result = m_bits_signaled;
        m_bits_signaled &= m_bits_manual_reset;
        return (result != 0);
    }

public:
    wait_result_t check() noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_events_count > 0, WAIT_RESULT_ERROR);

        wait_result_t result;
        stop_waiting(result);
        return result;
    }
    wait_result_t wait() noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_events_count > 0, WAIT_RESULT_ERROR);

        wait_result_t result;

        ++m_waiting_threads;
        m_cvar.wait(lock, [this, &result] () { return stop_waiting(result); });
        --m_waiting_threads;

        return result;
    }
    template<typename Rep, typename Period>
    wait_result_t wait_for(const std::chrono::duration<Rep, Period>& rel_time) noexcept(DIAG_NOEXCEPT)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        Check_ValidState(m_events_count > 0, WAIT_RESULT_ERROR);

        wait_result_t result;

        ++m_waiting_threads;
        m_cvar.wait(lock, rel_time, [this, &result] () { return stop_waiting(result); });
        --m_waiting_threads;

        return result;
    }
};

}

void test()
{
}

TEST_CASE("thread.waiting. main")
{
    using namespace gkr;
    using namespace std::chrono_literals;

    waitable_event<> e1;

    auto t1 = std::thread([&e1] () {
        objects_waiter& waiter = this_thread_objects_waiter();

        auto result = waiter.wait(e1);

        CHECK(waitable_object_wait_is_completed(result, 0));
    });

    std::this_thread::sleep_for(1s);

    e1.fire();
    t1.join();
}
