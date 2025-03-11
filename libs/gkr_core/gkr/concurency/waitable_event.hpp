#pragma once

#include <gkr/api.hpp>
#include <gkr/concurency/waitable_object.h>

namespace gkr
{

class waitable_event : public waitable_object
{
    waitable_event           (const waitable_event&) noexcept = delete;
    waitable_event& operator=(const waitable_event&) noexcept = delete;

    bool m_manual_reset;

public:
    waitable_event(bool manual_reset = false, bool create_fired = false)
        : waitable_object(create(manual_reset, create_fired))
        , m_manual_reset(manual_reset)
    {
    }
    waitable_event(waitable_event&& other) noexcept
        : waitable_object(std::move(other))
        , m_manual_reset(std::exchange(other.m_manual_reset, false))
    {
    }
    waitable_event& operator=(waitable_event&& other) noexcept
    {
        waitable_object::operator=(std::move(other));
        m_manual_reset = std::exchange(other.m_manual_reset, false);
        return *this;
    }
    void swap(waitable_event& other)
    {
        waitable_object::swap(other);
        std::swap(m_manual_reset, other.m_manual_reset);
    }

    GKR_INNER_API virtual ~waitable_event() override;

protected:
    GKR_INNER_API virtual bool handle_poll_data(unsigned long long value) override;

    GKR_INNER_API static object_handle_t create(bool manual_reset, bool create_fired);

public:
    GKR_INNER_API void fire();

    GKR_INNER_API void reset();

public:
    GKR_INNER_API void set_manual_reset(bool manual_reset);

    bool is_manual_reset() const noexcept
    {
        return m_manual_reset;
    }
};

}
