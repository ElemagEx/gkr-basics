#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>

#include "diag/diagnostics.h"

#include "lockfree_forward_list.h"

namespace gkr
{
using std::size_t;

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

        if(broadcast || m_always_broadcast)
        {
            m_cvar.notify_all();
        }
        else
        {
            m_cvar.notify_one();
        }
    }

public:
    void set_always_broadcast(bool always_broadcast) noexcept
    {
        m_always_broadcast = always_broadcast;
    }

protected:
    std::mutex m_mutex;
    std::condition_variable m_cvar;
    bool m_always_broadcast = false;
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
        waiter_t           (const waiter_t&) noexcept = delete;
        waiter_t& operator=(const waiter_t&) noexcept = delete;

        std::atomic<impl::basic_objects_waiter*> m_objects_waiter {nullptr};

    public:
        waiter_t() noexcept
        {
        }
        ~waiter_t() noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_objects_waiter.load() == nullptr);
        }
        waiter_t([[maybe_unused]] waiter_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(other.m_objects_waiter.load() == nullptr);
        }
        waiter_t& operator=([[maybe_unused]] waiter_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_objects_waiter.load() == nullptr);
            Assert_Check(other.m_objects_waiter.load() == nullptr);
            return *this;
        }
        void swap([[maybe_unused]] waiter_t& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_objects_waiter.load() == nullptr);
            Assert_Check(other.m_objects_waiter.load() == nullptr);
        }

        bool try_set(impl::basic_objects_waiter& objects_waiter) noexcept
        {
            impl::basic_objects_waiter* expected = nullptr;

            return m_objects_waiter.compare_exchange_strong(expected, &objects_waiter);
        }
        bool try_reset(impl::basic_objects_waiter& objects_waiter) noexcept
        {
            impl::basic_objects_waiter* expected = &objects_waiter;

            return m_objects_waiter.compare_exchange_strong(expected, nullptr);
        }
        void notify(bool broadcast)
        {
            impl::basic_objects_waiter* objects_waiter = m_objects_waiter.load();

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
class waitable_registrator : public waitable_object
{
    waitable_registrator           (const waitable_registrator&) noexcept = delete;
    waitable_registrator& operator=(const waitable_registrator&) noexcept = delete;

protected:
    waitable_registrator() noexcept = default;
   ~waitable_registrator() noexcept = default;

    waitable_registrator(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
    {
        for(size_t index = 0; index < size_t(MaxWaiters); ++index)
        {
            m_waiters[index] = std::move(other.m_waiters[index]);
        }
    }
    waitable_registrator& operator=(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        for(size_t index = 0; index < size_t(MaxWaiters); ++index)
        {
            m_waiters[index] = std::move(other.m_waiters[index]);
        }
        return *this;
    }
    void swap(waitable_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        for(size_t index = 0; index < size_t(MaxWaiters); ++index)
        {
            m_waiters[index].swap(other.m_waiters[index]);
        }
    }

public:
    static constexpr unsigned max_waiters()
    {
        return MaxWaiters;
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
class waitable_registrator<0> : public waitable_object
{
    waitable_registrator           (const waitable_registrator&) noexcept = delete;
    waitable_registrator& operator=(const waitable_registrator&) noexcept = delete;

protected:
    waitable_registrator() noexcept = default;
   ~waitable_registrator() noexcept = default;

    waitable_registrator(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
        , m_waiters(std::move(other.m_waiters))
    {
    }
    waitable_registrator& operator=(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        m_waiters = std::move(other.m_waiters);

        return *this;
    }

    void swap(waitable_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        m_waiters.swap(other.m_waiters);
    }

public:
    static constexpr unsigned max_waiters()
    {
        return 0;
    }

private:
    lockfree_forward_list<waiter_t> m_waiters;

    virtual bool register_waiter(impl::basic_objects_waiter& objects_waiter) override
    {
        auto end = m_waiters.end();

        for(auto it = m_waiters.before_begin(); it != end; )
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

template<typename WaitableObject>
class waitable_object_guard
{
    waitable_object_guard           (const waitable_object_guard&) noexcept = delete;
    waitable_object_guard& operator=(const waitable_object_guard&) noexcept = delete;
#ifdef __cpp_guaranteed_copy_elision
private:
    waitable_object_guard           (waitable_object_guard&&) noexcept = delete;
    waitable_object_guard& operator=(waitable_object_guard&&) noexcept = delete;
#else
public:
    waitable_object_guard           (waitable_object_guard&&) noexcept = default;
    waitable_object_guard& operator=(waitable_object_guard&&) noexcept = default;
#endif
public:
    explicit waitable_object_guard(WaitableObject& object, bool set = true) noexcept
        : m_object(set ? &object : nullptr)
    {
    }
    waitable_object_guard() noexcept : m_object(nullptr)
    {
    }
    ~waitable_object_guard() noexcept(false)
    {
        if(m_object != nullptr)
        {
            m_object->unlock();
        }
    }

public:
    bool wait_is_completed() const noexcept
    {
        return (m_object != nullptr);
    }
    explicit operator bool() const noexcept
    {
        return (m_object != nullptr);
    }

private:
    WaitableObject* m_object;
};

}
