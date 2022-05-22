#pragma once

#include <utility>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "common.h"

namespace gkr
{

class waitable_object;

namespace impl
{

class base_objects_waiter
{
    base_objects_waiter           (const base_objects_waiter&) noexcept = delete;
    base_objects_waiter& operator=(const base_objects_waiter&) noexcept = delete;

    base_objects_waiter           (base_objects_waiter&&) noexcept = delete;
    base_objects_waiter& operator=(base_objects_waiter&&) noexcept = delete;

protected:
    base_objects_waiter() noexcept = default;
   ~base_objects_waiter() noexcept = default;

private:
    friend class waitable_object;

    void notify(bool broadcast)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(broadcast)
        {
            m_cvar.notify_all();
        }
        else
        {
            m_cvar.notify_one();
        }
    }

protected:
    std::mutex m_mutex;
    std::condition_variable m_cvar;
};

}

class objects_waiter;

class waitable_object
{
    waitable_object           (const waitable_object&) noexcept = delete;
    waitable_object& operator=(const waitable_object&) noexcept = delete;

    waitable_object           (waitable_object&&) noexcept = delete;
    waitable_object& operator=(waitable_object&&) noexcept = delete;

protected:
    using capacity_t = unsigned short;

    struct waiter_t
    {
        waiter_t           (const waiter_t&) noexcept = delete;
        waiter_t& operator=(const waiter_t&) noexcept = delete;

        waiter_t() noexcept = default;
       ~waiter_t() noexcept = default;

        waiter_t(waiter_t&& other) noexcept
        {
            Assert_Check(other.ptr.load() == nullptr);
        }
        waiter_t& operator=(waiter_t&& other) noexcept
        {
            Assert_Check(other.ptr.load() == nullptr);
            Assert_Check(      ptr.load() == nullptr);
            return *this;
        }

        std::atomic<impl::base_objects_waiter*> ptr = nullptr;
    };

protected:
   ~waitable_object() noexcept = default;

    waitable_object(bool manual_reset, capacity_t capacity, waiter_t* waiters) noexcept
        : m_manual_reset(manual_reset)
        , m_capacity(capacity)
        , m_waiters (waiters )
    {
        Assert_Check(m_waiters != nullptr);
    }
    waitable_object(waitable_object&& other, capacity_t capacity, waiter_t* waiters) noexcept
        : m_manual_reset(std::exchange(other.m_manual_reset, false))
        , m_capacity(capacity)
        , m_waiters (waiters )
    {
        m_signaled.store(other.m_signaled.exchange(false));

        Assert_Check(m_waiters != nullptr);
    }
    void assignment(waitable_object&& other, capacity_t capacity, waiter_t* waiters) noexcept
    {
        m_manual_reset = std::exchange(other.m_manual_reset, false);

        m_capacity = capacity;
        m_waiters  = waiters ;

        m_signaled.store(other.m_signaled.exchange(false));

        Assert_Check(m_waiters != nullptr);
    }

protected:
    void fire()
    {
        bool expected = false;

        if(m_signaled.compare_exchange_strong(expected, true))
        {
            for(size_t index = 0; index < m_capacity; ++index)
            {
                impl::base_objects_waiter* waiter = m_waiters[index].ptr.load();

                if(waiter != nullptr)
                {
                    waiter->notify(m_manual_reset);
                }
            }
        }
    }
    void reset()
    {
        m_signaled.store(false);
    }
    bool try_consume() noexcept
    {
        if(m_manual_reset)
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
    friend class objects_waiter;

    bool register_waiter(impl::base_objects_waiter& waiter) noexcept
    {
        for(size_t index = 0; index < m_capacity; ++index)
        {
            impl::base_objects_waiter* expected = nullptr;

            if(m_waiters[index].ptr.compare_exchange_strong(expected, &waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }
    bool unregister_waiter(impl::base_objects_waiter& waiter) noexcept
    {
        for(size_t index = 0; index < m_capacity; ++index)
        {
            impl::base_objects_waiter* expected = &waiter;

            if(m_waiters[index].ptr.compare_exchange_strong(expected, nullptr))
            {
                return true;
            }
        }
        Check_Failure(false);
    }

private:
    std::atomic<bool> m_signaled;
    bool              m_manual_reset;
    capacity_t        m_capacity;
    waiter_t*         m_waiters;
};

}
