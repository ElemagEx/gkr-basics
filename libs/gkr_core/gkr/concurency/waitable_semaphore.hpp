#pragma once

#include <gkr/api.hpp>
#include <gkr/concurency/waitable_object.hpp>

namespace gkr
{

class waitable_semaphore : public waitable_object
{
    waitable_semaphore           (const waitable_semaphore&) noexcept = delete;
    waitable_semaphore& operator=(const waitable_semaphore&) noexcept = delete;

    unsigned m_max_count;

public:
    static constexpr unsigned MAXIMUM_COUNT = 0x7FFFFFFEU;

public:
    waitable_semaphore(unsigned max_count = MAXIMUM_COUNT)
        : waitable_object(create(max_count, max_count))
        , m_max_count(max_count)
    {
    }
    waitable_semaphore(unsigned max_count, unsigned init_count)
        : waitable_object(create(max_count, init_count))
        , m_max_count(max_count)
    {
    }
    waitable_semaphore(waitable_semaphore&& other) noexcept
        : waitable_object(std::move(other))
        , m_max_count(std::exchange(other.m_max_count, 0U))
    {
    }
    waitable_semaphore& operator=(waitable_semaphore&& other) noexcept
    {
        waitable_object::operator=(std::move(other));
        m_max_count = std::exchange(other.m_max_count, 0U);
        return *this;
    }
    void swap(waitable_semaphore& other)
    {
        waitable_object::swap(other);
        std::swap(m_max_count, other.m_max_count);
    }

    GKR_INNER_API virtual ~waitable_semaphore() override;

protected:
    GKR_INNER_API virtual bool handle_poll_data(unsigned long long value) override;

    GKR_INNER_API static object_handle_t create(unsigned max_count, unsigned init_count);

public:
    GKR_INNER_API bool release(unsigned count = 1);

    GKR_INNER_API void set_max_count(unsigned max_count);

public:
    unsigned get_max_count() const noexcept
    {
        return m_max_count;
    }
};

}
