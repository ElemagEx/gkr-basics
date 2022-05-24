#pragma once

#include "waitable_object.h"

#include <mutex>

namespace gkr
{

template<unsigned MaxWaiters = 1, bool Recursive=false>
class sync_mutex final : public impl::waiter_registrator<MaxWaiters>
{
    sync_mutex           (const sync_mutex&) noexcept = delete;
    sync_mutex& operator=(const sync_mutex&) noexcept = delete;

    sync_mutex           (sync_mutex&& other) noexcept = delete;
    sync_mutex& operator=(sync_mutex&& other) noexcept = delete;

public:
    sync_mutex() noexcept = default;
   ~sync_mutex() noexcept = default;

private:
    template<bool> struct std_mutex;
    template<> struct std_mutex<false> { using type = std::mutex; };
    template<> struct std_mutex<true > { using type = std::recursive_mutex; };

    using mutex_t = typename std_mutex<Recursive>::type;

    using base_t = impl::waiter_registrator<MaxWaiters>;

public:
    void lock()
    {
        m_mutex.lock();
    }
    void unlock()
    {
        m_mutex.unlock();

        base_t::notify_all_waiters(false);
    }
    [[nodiscard]]
    bool try_lock()
    {
        return m_mutex.try_lock();
    }

private:
    [[nodiscard]]
    bool try_consume() override
    {
        return m_mutex.try_lock();
    }

private:
    mutex_t m_mutex;
};

}
