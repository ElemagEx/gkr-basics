#pragma once

#include "waitable_object.h"

namespace gkr
{

template<unsigned MaxWaiters = 1>
class waitable_event final : public impl::waiter_registrator<MaxWaiters>
{
    waitable_event           (const waitable_event&) noexcept = delete;
    waitable_event& operator=(const waitable_event&) noexcept = delete;

public:
    waitable_event() noexcept = default;
   ~waitable_event() noexcept = default;

    waitable_event(bool manual_reset) noexcept : m_manual_reset(manual_reset)
    {
    }
    waitable_event(waitable_event&& other) noexcept : m_manual_reset(other.m_manual_reset)
    {
        m_signaled.store(other.m_signaled.exchange(false));
    }
    waitable_event& operator=(waitable_event&& other) noexcept
    {
        m_manual_reset = other.m_manual_reset;

        m_signaled.store(other.m_signaled.exchange(false));

        return *this;
    }

private:
    using base_t = impl::waiter_registrator<MaxWaiters>;

public:
    void fire()
    {
        bool expected = false;

        if(m_signaled.compare_exchange_strong(expected, true))
        {
            base_t::notify_all_waiters(m_manual_reset);
        }
    }
    void reset()
    {
        m_signaled.store(false);
    }

public:
    [[nodiscard]]
    bool try_consume() override
    {
        if(m_manual_reset)
        {
            return m_signaled.load();
        }
        else
        {
            bool expected = true;

            return m_signaled.compare_exchange_strong(expected, false);
        }
    }

private:
    std::atomic<bool> m_signaled      {false};
    bool              m_manual_reset = false;
};

}
