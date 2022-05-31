#pragma once

#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "common.h"
#include "lockfree_forward_list.h"

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

protected:
    waitable_object           (waitable_object&&) noexcept = default;
    waitable_object& operator=(waitable_object&&) noexcept = default;

    waitable_object() noexcept = default;
   ~waitable_object() noexcept = default;

private:
    friend class objects_waiter;

    [[nodiscard]]
    virtual bool try_consume() = 0;

    virtual bool register_waiter(impl::base_objects_waiter& objects_waiter) = 0;

    virtual bool unregister_waiter(impl::base_objects_waiter& objects_waiter) = 0;

protected:
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

        bool try_set(impl::base_objects_waiter& objects_waiter)
        {
            impl::base_objects_waiter* expected = nullptr;

            return ptr.compare_exchange_strong(expected, &objects_waiter);
        }
        bool try_reset(impl::base_objects_waiter& objects_waiter)
        {
            impl::base_objects_waiter* expected = &objects_waiter;

            return ptr.compare_exchange_strong(expected, nullptr);
        }

        std::atomic<impl::base_objects_waiter*> ptr = nullptr;
    };

    void notify_waiter(waiter_t& waiter, bool broadcast)
    {
        impl::base_objects_waiter* objects_waiter = waiter.ptr.load();

        if(objects_waiter != nullptr)
        {
            objects_waiter->notify(broadcast);
        }
    }
};

namespace impl
{

template<unsigned MaxWaiters>
class waiter_registrator : public waitable_object
{
    waiter_registrator           (const waiter_registrator&) noexcept = delete;
    waiter_registrator& operator=(const waiter_registrator&) noexcept = delete;

protected:
    waiter_registrator           (waiter_registrator&&) noexcept = default;
    waiter_registrator& operator=(waiter_registrator&&) noexcept = default;

    waiter_registrator() noexcept = default;
   ~waiter_registrator() noexcept = default;

private:
    waiter_t m_waiters[MaxWaiters] = {};

    bool register_waiter(impl::base_objects_waiter& objects_waiter) override
    {
        for(waiter_t& waiter : m_waiters)
        {
            if(waiter.try_set(objects_waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }
    bool unregister_waiter(impl::base_objects_waiter& objects_waiter) override
    {
        for(waiter_t& waiter : m_waiters)
        {
            if(waiter.try_reset(objects_waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }

protected:
    void notify_all_waiters(bool broadcast)
    {
        for(waiter_t& waiter : m_waiters)
        {
            waitable_object::notify_waiter(waiter, broadcast);
        }
    }
};

template<>
class waiter_registrator<0> : public waitable_object
{
    waiter_registrator           (const waiter_registrator&) noexcept = delete;
    waiter_registrator& operator=(const waiter_registrator&) noexcept = delete;

protected:
    waiter_registrator           (waiter_registrator&&) noexcept = default;
    waiter_registrator& operator=(waiter_registrator&&) noexcept = default;

    waiter_registrator() noexcept = default;
   ~waiter_registrator() noexcept = default;

private:
    lockfree_forward_list<waiter_t> m_waiters;

    bool register_waiter(impl::base_objects_waiter& objects_waiter) override
    {
        for(waiter_t& waiter : m_waiters)
        {
            if(waiter.try_set(objects_waiter))
            {
                return true;
            }
        }
        Verify_BoolRes(m_waiters.emplace_front().try_set(objects_waiter), false);
        return true;
    }
    bool unregister_waiter(impl::base_objects_waiter& objects_waiter) override
    {
        for(waiter_t& waiter : m_waiters)
        {
            if(waiter.try_reset(objects_waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }

protected:
    void notify_all_waiters(bool broadcast)
    {
        for(waiter_t& waiter : m_waiters)
        {
            waitable_object::notify_waiter(waiter, broadcast);
        }
    }
};

}

}
