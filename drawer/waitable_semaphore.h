#pragma once

#ifdef __cpp_lib_semaphore

#include <gkr/thread_waiting.h>

#include <semaphore>

namespace gkr
{

template<std::ptrdiff_t LeastMaxValue = std::counting_semaphore<>::max(), unsigned MaxWaiters = 1>
class waitable_semaphore final : public waitable_registrator<MaxWaiters>
{
    waitable_semaphore           (const waitable_semaphore&) noexcept = delete;
    waitable_semaphore& operator=(const waitable_semaphore&) noexcept = delete;

    waitable_semaphore           (waitable_semaphore&& other) noexcept = delete;
    waitable_semaphore& operator=(waitable_semaphore&& other) noexcept = delete;

public:
    virtual
   ~waitable_semaphore() noexcept override = default;
    waitable_semaphore() noexcept          = delete;

    waitable_semaphore(std::ptrdiff_t desired) : m_semaphore(desired)
    {
    }

private:
    using base_t = waitable_registrator<MaxWaiters>;

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
    virtual bool try_consume() noexcept override
    {
        return m_semaphore.try_acquire();
    }

private:
    semaphore_t m_semaphore;
};

}

#endif