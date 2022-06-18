#pragma once

#include "waitable_object.h"

#include <mutex>

namespace gkr
{
namespace impl
{
template<bool> struct std_mutex;
template<> struct std_mutex<false> { using type = std::mutex; };
template<> struct std_mutex<true > { using type = std::recursive_mutex; };
}

template<unsigned MaxWaiters = 1, bool Recursive=false>
class waitable_mutex final : public impl::waiter_registrator<MaxWaiters>
{
    waitable_mutex           (const waitable_mutex&) noexcept = delete;
    waitable_mutex& operator=(const waitable_mutex&) noexcept = delete;

    waitable_mutex           (waitable_mutex&& other) noexcept = delete;
    waitable_mutex& operator=(waitable_mutex&& other) noexcept = delete;

public:
    waitable_mutex() noexcept = default;
   ~waitable_mutex() noexcept = default;

private:
    using mutex_t = typename impl::std_mutex<Recursive>::type;

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
