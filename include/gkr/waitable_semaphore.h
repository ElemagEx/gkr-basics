#pragma once

#include "waitable_object.h"

#ifdef __cpp_lib_semaphore

#include <semaphore>

namespace gkr
{

namespace impl
{
#if defined(_MSC_VER)
constexpr std::ptrdiff_t Default_LeastMaxValue = std::_Semaphore_max;
#elif defined(__GNUC__)
constexpr std::ptrdiff_t Default_LeastMaxValue = 0;
#elif defined(__clang__)
constexpr std::ptrdiff_t Default_LeastMaxValue = 0;
#else
constexpr std::ptrdiff_t Default_LeastMaxValue = 0;
#endif
}

template<unsigned MaxWaiters = 1, std::ptrdiff_t LeastMaxValue = impl::Default_LeastMaxValue>
class waitable_semaphore final : public impl::waiter_registrator<MaxWaiters>
{
    waitable_semaphore           (const waitable_semaphore&) noexcept = delete;
    waitable_semaphore& operator=(const waitable_semaphore&) noexcept = delete;

    waitable_semaphore           (waitable_semaphore&& other) noexcept = delete;
    waitable_semaphore& operator=(waitable_semaphore&& other) noexcept = delete;

public:
    waitable_semaphore() noexcept = delete;
   ~waitable_semaphore() noexcept = default;

    waitable_semaphore(std::ptrdiff_t desired) : m_semaphore(desired)
    {
    }

private:
    using base_t = impl::waiter_registrator<MaxWaiters>;

    using semaphore_t = std::counting_semaphore<LeastMaxValue>;

public:
    void acquire()
    {
        m_semaphore.acquire();
    }
    void release(std::ptrdiff_t update = 1)
    {
        m_semaphore.release();

        base_t::notify_all_waiters(update > 1);
    }
    [[nodiscard]]
    bool try_acquire()
    {
        return m_semaphore.try_acquire();
    }

    constexpr std::ptrdiff_t max() noexcept
    {
        return semaphore_t::max();
    }

private:
    [[nodiscard]]
    bool try_consume() override
    {
        return m_semaphore.try_acquire();
    }

private:
    semaphore_t m_semaphore;
};

}

#endif