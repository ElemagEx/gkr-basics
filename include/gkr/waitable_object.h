#pragma once

#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "diagnostics.h"
#include "lockfree_forward_list.h"

namespace gkr
{

class waitable_object;

namespace impl
{

class basic_objects_waiter
{
    basic_objects_waiter           (const basic_objects_waiter&) noexcept = delete;
    basic_objects_waiter& operator=(const basic_objects_waiter&) noexcept = delete;

    basic_objects_waiter           (basic_objects_waiter&&) noexcept = delete;
    basic_objects_waiter& operator=(basic_objects_waiter&&) noexcept = delete;

protected:
    basic_objects_waiter() noexcept = default;
   ~basic_objects_waiter() noexcept = default;

private:
    friend class ::gkr::waitable_object;

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

   void swap(waitable_object&)
   {
   }

private:
    friend class objects_waiter;

    [[nodiscard]]
    virtual bool try_consume() = 0;

    virtual bool register_waiter(impl::basic_objects_waiter& objects_waiter) = 0;

    virtual bool unregister_waiter(impl::basic_objects_waiter& objects_waiter) = 0;

protected:
    class waiter_t
    {
        std::atomic<impl::basic_objects_waiter*> ptr {nullptr};

    public:
        waiter_t           (const waiter_t&) noexcept = delete;
        waiter_t& operator=(const waiter_t&) noexcept = delete;

        waiter_t() noexcept = default;
       ~waiter_t() noexcept = default;

        waiter_t(waiter_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(other.ptr.load() == nullptr);
        }
        waiter_t& operator=(waiter_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(other.ptr.load() == nullptr);
            Assert_Check(      ptr.load() == nullptr);
            return *this;
        }
        void swap(waiter_t& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(other.ptr.load() == nullptr);
            Assert_Check(      ptr.load() == nullptr);
        }

        bool try_set(impl::basic_objects_waiter& objects_waiter) noexcept
        {
            impl::basic_objects_waiter* expected = nullptr;

            return ptr.compare_exchange_strong(expected, &objects_waiter);
        }
        bool try_reset(impl::basic_objects_waiter& objects_waiter) noexcept
        {
            impl::basic_objects_waiter* expected = &objects_waiter;

            return ptr.compare_exchange_strong(expected, nullptr);
        }
        void notify(bool broadcast)
        {
            impl::basic_objects_waiter* objects_waiter = ptr.load();

            if(objects_waiter != nullptr)
            {
                objects_waiter->notify(broadcast);
            }
        }
    };
};

namespace impl
{

template<unsigned MaxWaiters>
class waiter_registrator : public waitable_object
{
    waiter_registrator           (const waiter_registrator&) noexcept = delete;
    waiter_registrator& operator=(const waiter_registrator&) noexcept = delete;

protected:
    waiter_registrator() noexcept = default;
   ~waiter_registrator() noexcept = default;

    waiter_registrator(waiter_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
    {
        for(unsigned index = 0; index < MaxWaiters; ++index)
        {
            m_waiters[index] = std::move(other.m_waiters[index]);
        }
    }
    waiter_registrator& operator=(waiter_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        for(unsigned index = 0; index < MaxWaiters; ++index)
        {
            m_waiters[index] = std::move(other.m_waiters[index]);
        }
        return *this;
    }
    void swap(waiter_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        for(unsigned index = 0; index < MaxWaiters; ++index)
        {
            m_waiters[index].swap(other.m_waiters[index]);
        }
    }

private:
    waiter_t m_waiters[MaxWaiters] = {};

    virtual bool register_waiter(impl::basic_objects_waiter& objects_waiter) override
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
    virtual bool unregister_waiter(impl::basic_objects_waiter& objects_waiter) override
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
            waiter.notify(broadcast);
        }
    }
};

template<>
class waiter_registrator<0> : public waitable_object
{
    waiter_registrator           (const waiter_registrator&) noexcept = delete;
    waiter_registrator& operator=(const waiter_registrator&) noexcept = delete;

protected:
    waiter_registrator() noexcept = default;
   ~waiter_registrator() noexcept = default;

    waiter_registrator(waiter_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
        , m_waiters(std::move(other.m_waiters))
    {
    }
    waiter_registrator& operator=(waiter_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        m_waiters = std::move(other.m_waiters);

        return *this;
    }

    void swap(waiter_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        m_waiters.swap(other.m_waiters);
    }

private:
    lockfree_forward_list<waiter_t> m_waiters;

    bool register_waiter(impl::basic_objects_waiter& objects_waiter) override
    {
        auto end = m_waiters.end();

        for(auto it = m_waiters.first(); it != end; )
        {
            m_waiters.get_next_or_add_new(it);

            if(it->try_set(objects_waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }
    bool unregister_waiter(impl::basic_objects_waiter& objects_waiter) override
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
            waiter.notify(broadcast);
        }
    }
};

}

}
