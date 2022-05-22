#pragma once

#include "waitable_object.h"
#include "common.h"

namespace gkr
{

template<size_t MaxWaiters = 1>
class sync_event : public waitable_object
{
    static_assert(MaxWaiters > 0, "MaxWaiters Must be larger than 0");

    sync_event           (const sync_event&) noexcept = delete;
    sync_event& operator=(const sync_event&) noexcept = delete;

public:
    sync_event(bool manual_reset = false) noexcept
        : waitable_object(manual_reset, capacity_t(MaxWaiters), m_waiters_storage)
    {
    }
    ~sync_event() noexcept
    {
    }

    sync_event(sync_event&& other) noexcept
        : sync_event(other, capacity_t(MaxWaiters), m_waiters_storage)
    {
    }
    sync_event& operator=(sync_event&& other) noexcept
    {
        waitable_object::assignment(other, capacity_t(MaxWaiters), m_waiters_storage);
        m_waiters_storage = std::move(other.m_waiters_storage);
        return *this;
    }

public:
    using waitable_object::fire;
    using waitable_object::reset;
    using waitable_object::try_consume;

private:
    waiter_t m_waiters_storage[MaxWaiters] = {};
};

}
