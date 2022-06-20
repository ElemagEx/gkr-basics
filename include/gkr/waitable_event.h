#pragma once

#include "waitable_object.h"

namespace gkr
{

template<bool ManualReset=false, unsigned MaxWaiters=1>
class waitable_event final : public impl::waiter_registrator<MaxWaiters>
{
    waitable_event           (const waitable_event&) noexcept = delete;
    waitable_event& operator=(const waitable_event&) noexcept = delete;

private:
    using base_t = impl::waiter_registrator<MaxWaiters>;

public:
    waitable_event() noexcept = default;
   ~waitable_event() noexcept = default;

    waitable_event(waitable_event&& other) noexcept : base_t(std::move(other))
    {
        m_signaled.store(other.m_signaled.exchange(false));
    }
    waitable_event& operator=(waitable_event&& other) noexcept
    {
        if(this != &other)
        {
            base_t::operator=(std::move(other));

            m_signaled.store(other.m_signaled.exchange(false));
        }
        return *this;
    }
    void swap(waitable_event& other) noexcept
    {
        if(this != &other)
        {
            base_t::swap(other);

            m_signaled.store(other.m_signaled.exchange(m_signaled.load()));
        }
    }

public:
    void fire()
    {
        bool expected = false;

        if(m_signaled.compare_exchange_strong(expected, true))
        {
            base_t::notify_all_waiters(ManualReset);
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
#ifdef __cpp_if_constexpr
        if constexpr(ManualReset)
#else
        if          (ManualReset)
#endif
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
    std::atomic<bool> m_signaled {false};
};

}
