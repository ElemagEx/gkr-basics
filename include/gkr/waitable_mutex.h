#pragma once

#include <gkr/thread_waiting.h>

#include <mutex>

namespace gkr
{

template<bool Recursive=false, unsigned MaxWaiters = 1>
class waitable_mutex final : public waitable_registrator<MaxWaiters>
{
    waitable_mutex           (const waitable_mutex&) noexcept = delete;
    waitable_mutex& operator=(const waitable_mutex&) noexcept = delete;

    waitable_mutex           (waitable_mutex&& other) noexcept = delete;
    waitable_mutex& operator=(waitable_mutex&& other) noexcept = delete;

public:
    waitable_mutex() noexcept = default;
   ~waitable_mutex() noexcept = default;

private:
    template<bool> struct std_mutex;
    template<> struct std_mutex<false> { using type = std::mutex; };
    template<> struct std_mutex<true > { using type = std::recursive_mutex; };

    using mutex_t = typename std_mutex<Recursive>::type;

    using base_t = waitable_registrator<MaxWaiters>;

public:
    void lock()
    {
        m_mutex.lock();
    }
    void unlock()
    {
        m_mutex.unlock();

        base_t::notify_all_registered_waiters();
    }
    [[nodiscard]]
    bool try_lock()
    {
        return m_mutex.try_lock();
    }

private:
    [[nodiscard]]
    virtual bool try_consume() noexcept override
    {
        return m_mutex.try_lock();
    }

private:
    mutex_t m_mutex;
};

}
