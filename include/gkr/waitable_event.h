#pragma once

#include "thread_waiting.h"

//#ifndef __cpp_lib_exchange_function
//#include "cpp/lib_exchange_function.h"
//#endif

namespace gkr
{

template<bool ManualReset=false, unsigned MaxWaiters=1>
class waitable_event final : public waitable_registrator<MaxWaiters>
{
    waitable_event           (const waitable_event&) noexcept = delete;
    waitable_event& operator=(const waitable_event&) noexcept = delete;

public:
    virtual
   ~waitable_event() noexcept override = default;
    waitable_event() noexcept          = default;

private:
    std::atomic<bool> m_signaled {false};

private:
    using self_t = waitable_event<ManualReset, MaxWaiters>;
    using base_t = waitable_registrator<MaxWaiters>;

    waitable_event(self_t&& other) noexcept(DIAG_NOEXCEPT) : base_t(std::move(other))
    {
        m_signaled.store(other.m_signaled.exchange(false));
    }
    waitable_event& operator=(self_t&& other) noexcept(DIAG_NOEXCEPT)
    {
        if(this != &other)
        {
            base_t::operator=(std::move(other));

            m_signaled.store(other.m_signaled.exchange(false));
        }
        return *this;
    }
    void swap(waitable_event& other) noexcept(DIAG_NOEXCEPT)
    {
        if(this != &other)
        {
            base_t::swap(other);

            m_signaled.store(other.m_signaled.exchange(m_signaled.load()));
        }
    }

public:
    void fire() noexcept
    {
        bool expected = false;

        if(m_signaled.compare_exchange_strong(expected, true))
        {
            base_t::notify_all_registered_waiters();
        }
    }
    void reset() noexcept
    {
        m_signaled.store(false);
    }

public:
    [[nodiscard]]
    virtual bool try_consume() noexcept override
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
};

}
