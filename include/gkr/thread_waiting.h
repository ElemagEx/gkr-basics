#pragma once

#include <mutex>
#include <chrono>
#include <atomic>
#include <climits>
#include <condition_variable>

#include <gkr/diag/diagnostics.h>

#include <gkr/lockfree_grow_only_bag.h>

namespace gkr
{
using wait_result_t = unsigned long long;

constexpr std::size_t WAIT_MAX_OBJECTS = sizeof(wait_result_t) * CHAR_BIT - 1;

constexpr wait_result_t WAIT_RESULT_ERROR   = wait_result_t(1) << WAIT_MAX_OBJECTS;
constexpr wait_result_t WAIT_RESULT_TIMEOUT = wait_result_t(0);

class waitable_object;

class objects_waiter
{
    objects_waiter           (const objects_waiter&) noexcept = delete;
    objects_waiter& operator=(const objects_waiter&) noexcept = delete;

    objects_waiter           (objects_waiter&&) noexcept = delete;
    objects_waiter& operator=(objects_waiter&&) noexcept = delete;

private:
    objects_waiter() noexcept = default;
   ~objects_waiter() noexcept = default;

    friend objects_waiter& this_thread_objects_waiter() noexcept;

private:
    std::mutex m_mutex;
    std::condition_variable m_cvar;

private:
    void notify() noexcept
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cvar.notify_one();
    }
    friend class waitable_object;

private:
    static bool collect_result(wait_result_t& result, std::size_t count, waitable_object** objects) noexcept;

    std::size_t register_self(std::size_t count, waitable_object** objects) noexcept;

    void unregiser_self(std::size_t count, waitable_object** objects) noexcept;

    class registrator_t
    {
        registrator_t           (registrator_t&&) noexcept = delete;
        registrator_t& operator=(registrator_t&&) noexcept = delete;

        registrator_t           (const registrator_t&) noexcept = delete;
        registrator_t& operator=(const registrator_t&) noexcept = delete;

    private:
        objects_waiter*    m_waiter;
        std::size_t&       m_count;
        waitable_object**& m_objects;

    public:
        registrator_t(objects_waiter& waiter, std::size_t& count, waitable_object**& objects) noexcept
            : m_waiter (nullptr)
            , m_count  (count)
            , m_objects(objects)
        {
            const std::size_t registered = waiter.register_self(count, objects);

            if(registered < count)
            {
                waiter.unregiser_self(registered, objects);
                return;
            }

            m_waiter = &waiter;
        }
        ~registrator_t() noexcept
        {
            if(m_waiter != nullptr)
            {
                m_waiter->unregiser_self(m_count, m_objects);
            }
        }
        explicit operator bool() const noexcept
        {
            return (m_waiter != nullptr);
        }
    };
    friend class registrator_t;

private:
    wait_result_t wait_for(std::size_t count, waitable_object** objects) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        wait_result_t result;

        if(collect_result(result, count, objects)) return result;

        registrator_t registrator(*this, count, objects);

        if(registrator)
        {
            for( ; ; )
            {
                m_cvar.wait(lock);

                if(collect_result(result, count, objects)) return result;
            }
        }
        return WAIT_RESULT_ERROR;
    }
    template<typename Rep, typename Period, typename... WaitableObjects>
    wait_result_t wait_for(std::chrono::duration<Rep, Period> timeout, std::size_t count, waitable_object** objects) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        wait_result_t result;

        if(collect_result(result, count, objects)) return result;

        registrator_t registrator(*this, count, objects);

        if(registrator)
        {
            for(auto time_point = std::chrono::system_clock::now() + timeout; ; )
            {
                if(m_cvar.wait_until(lock, time_point) == std::cv_status::timeout) break;

                if(collect_result(result, count, objects)) return result;
            }
            if(collect_result(result, count, objects)) return result;

            return WAIT_RESULT_TIMEOUT;
        }
        return WAIT_RESULT_ERROR;
    }

public:
    template<typename... WaitableObjects>
    static wait_result_t check(WaitableObjects&... waitable_objects) noexcept
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        static_assert(count >= 1, "At least one wait object must be provided");
        static_assert(count <= WAIT_MAX_OBJECTS, "Too many wait objects are provided");

        waitable_object* objects[count] = {&waitable_objects...};

        wait_result_t result;

        collect_result(result, count, objects);

        return result;
    }
    static wait_result_t check(std::size_t count, waitable_object** objects) noexcept(DIAG_NOEXCEPT)
    {
        Check_Arg_IsValid(count >= 1, WAIT_RESULT_ERROR);
        Check_Arg_IsValid(count <= WAIT_MAX_OBJECTS, WAIT_RESULT_ERROR);

        Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);

        Check_Arg_Array(index, count, objects[index] != nullptr, WAIT_RESULT_ERROR);//???

        wait_result_t result;

        collect_result(result, count, objects);

        return result;
    }

public:
    template<typename... WaitableObjects>
    wait_result_t wait(WaitableObjects&... waitable_objects) noexcept
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        static_assert(count >= 1, "At least one wait object must be provided");
        static_assert(count <= WAIT_MAX_OBJECTS, "Too many wait objects are provided");

        waitable_object* objects[count] = {&waitable_objects...};

        return wait_for(count, objects);
    }
    wait_result_t wait(std::size_t count, waitable_object** objects) noexcept(DIAG_NOEXCEPT)
    {
        Check_Arg_IsValid(count >= 1, WAIT_RESULT_ERROR);
        Check_Arg_IsValid(count <= WAIT_MAX_OBJECTS, WAIT_RESULT_ERROR);

        Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);

        Check_Arg_Array(index, count, objects[index] != nullptr, WAIT_RESULT_ERROR);//???

        return wait_for(count, objects);
    }

public:
    template<typename Rep, typename Period, typename... WaitableObjects>
    wait_result_t wait(std::chrono::duration<Rep, Period> timeout, WaitableObjects&... waitable_objects) noexcept
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        static_assert(count >= 1, "At least one wait object must be provided");
        static_assert(count <= WAIT_MAX_OBJECTS, "Too many wait objects are provided");

        waitable_object* objects[count] = {&waitable_objects...};

        using duration_t = std::chrono::duration<Rep, Period>;

        if(timeout == duration_t::max())
        {
            return wait_for(count, objects);
        }
        else
        {
            return wait_for(timeout, count, objects);
        }
    }
    template<typename Rep, typename Period, typename... WaitableObjects>
    wait_result_t wait(std::chrono::duration<Rep, Period> timeout, std::size_t count, waitable_object** objects) noexcept(DIAG_NOEXCEPT)
    {
        Check_Arg_IsValid(count >= 1, WAIT_RESULT_ERROR);
        Check_Arg_IsValid(count <= WAIT_MAX_OBJECTS, WAIT_RESULT_ERROR);

        Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);

        Check_Arg_Array(index, count, objects[index] != nullptr, WAIT_RESULT_ERROR);

        using duration_t = std::chrono::duration<Rep, Period>;

        if(timeout == duration_t::max())
        {
            return wait_for(count, objects);
        }
        else
        {
            return wait_for(timeout, count, objects);
        }
    }
};

class waitable_object
{
    waitable_object           (const waitable_object&) noexcept = delete;
    waitable_object& operator=(const waitable_object&) noexcept = delete;

protected:
    waitable_object           (waitable_object&&) noexcept = default;
    waitable_object& operator=(waitable_object&&) noexcept = default;

    virtual
   ~waitable_object() noexcept = default;
    waitable_object() noexcept = default;

    void swap(waitable_object&)
    {
    }

protected:
    void notify_waiter(objects_waiter& waiter) noexcept
    {
        waiter.notify();
    }

private:
    friend class objects_waiter;

    [[nodiscard]]
    virtual bool try_consume() noexcept = 0;

    virtual bool register_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) = 0;

    virtual bool unregister_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) = 0;

protected:
    class waiter_registration_t
    {
        waiter_registration_t           (const waiter_registration_t&) noexcept = delete;
        waiter_registration_t& operator=(const waiter_registration_t&) noexcept = delete;

        std::atomic<objects_waiter*> m_waiter {nullptr};

    public:
        waiter_registration_t() noexcept
        {
        }
        ~waiter_registration_t() noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_waiter == nullptr);
        }
        waiter_registration_t([[maybe_unused]] waiter_registration_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(other.m_waiter == nullptr);
        }
        waiter_registration_t& operator=([[maybe_unused]] waiter_registration_t&& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_waiter == nullptr);
            Assert_Check(other.m_waiter == nullptr);
            return *this;
        }
        void swap([[maybe_unused]] waiter_registration_t& other) noexcept(DIAG_NOEXCEPT)
        {
            Assert_Check(      m_waiter == nullptr);
            Assert_Check(other.m_waiter == nullptr);
        }

    public:
        enum { Registered, Duplicated, Occupied };

    public:
        int try_register(objects_waiter& waiter) noexcept
        {
            objects_waiter* expected = nullptr;

            if(m_waiter.compare_exchange_strong(expected, &waiter)) return Registered;

            if(expected == &waiter) return Duplicated;

            return Occupied;
        }
        bool try_unregister(objects_waiter& waiter) noexcept
        {
            objects_waiter* expected = &waiter;

            return m_waiter.compare_exchange_strong(expected, nullptr);
        }
        void notify_waiter() noexcept
        {
            objects_waiter* waiter = m_waiter;

            if(waiter != nullptr)
            {
                waiter->notify();
            }
        }
    };
};

template<unsigned MaxWaiters>
class waitable_registrator : public waitable_object
{
    waitable_registrator           (const waitable_registrator&) noexcept = delete;
    waitable_registrator& operator=(const waitable_registrator&) noexcept = delete;

protected:
    virtual
   ~waitable_registrator() noexcept override = default;
    waitable_registrator() noexcept          = default;

    waitable_registrator(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
    {
        for(std::size_t index = 0; index < std::size_t(MaxWaiters); ++index)
        {
            m_registrations[index] = std::move(other.m_registrations[index]);
        }
    }
    waitable_registrator& operator=(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        for(std::size_t index = 0; index < std::size_t(MaxWaiters); ++index)
        {
            m_registrations[index] = std::move(other.m_registrations[index]);
        }

        return *this;
    }
    void swap(waitable_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        for(std::size_t index = 0; index < std::size_t(MaxWaiters); ++index)
        {
            m_registrations[index].swap(other.m_registrations[index]);
        }
    }

public:
    static constexpr unsigned max_waiters()
    {
        return MaxWaiters;
    }

private:
    waiter_registration_t m_registrations[MaxWaiters] = {};

    virtual bool register_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) override
    {
        for(waiter_registration_t& registration : m_registrations)
        {
            switch(registration.try_register(waiter))
            {
                case waiter_registration_t::Registered: return true;
                case waiter_registration_t::Duplicated: Check_Failure(false);
            }
        }
        Check_Failure(false);
    }
    virtual bool unregister_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) override
    {
        for(waiter_registration_t& registration : m_registrations)
        {
            if(registration.try_unregister(waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }

protected:
    void notify_all_registered_waiters() noexcept
    {
        for(waiter_registration_t& registration : m_registrations)
        {
            registration.notify_waiter();
        }
    }
};
template<>
class waitable_registrator<0> : public waitable_object
{
    waitable_registrator           (const waitable_registrator&) noexcept = delete;
    waitable_registrator& operator=(const waitable_registrator&) noexcept = delete;

protected:
    virtual
   ~waitable_registrator() noexcept override = default;
    waitable_registrator() noexcept          = default;

    waitable_registrator(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
        : waitable_object(std::move(other))
        , m_registrations(std::move(other.m_registrations))
    {
    }
    waitable_registrator& operator=(waitable_registrator&& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::operator=(std::move(other));

        m_registrations = std::move(other.m_registrations);

        return *this;
    }
    void swap(waitable_registrator& other) noexcept(DIAG_NOEXCEPT)
    {
        waitable_object::swap(other);

        m_registrations.swap(other.m_registrations);
    }

public:
    static constexpr unsigned max_waiters()
    {
        return unsigned(-1);
    }

private:
    lockfree_grow_only_bag<waiter_registration_t> m_registrations;

    virtual bool register_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) override
    {
        for( ; ; )
        {
            for(waiter_registration_t& registration : m_registrations)
            {
                switch(registration.try_register(waiter))
                {
                    case waiter_registration_t::Registered: return true;
                    case waiter_registration_t::Duplicated: Check_Failure(false);
                }
            }
            switch(m_registrations.insert()->try_register(waiter))
            {
                case waiter_registration_t::Registered: return true;
                case waiter_registration_t::Duplicated: Check_Failure(false);
            }
        }
    }
    virtual bool unregister_waiter(objects_waiter& waiter) noexcept(DIAG_NOEXCEPT) override
    {
        for(waiter_registration_t& registration : m_registrations)
        {
            if(registration.try_unregister(waiter))
            {
                return true;
            }
        }
        Check_Failure(false);
    }

protected:
    void notify_all_registered_waiters() noexcept
    {
        for(waiter_registration_t& registration : m_registrations)
        {
            registration.notify_waiter();
        }
    }
};

inline objects_waiter& this_thread_objects_waiter() noexcept
{
    static thread_local objects_waiter tl_objects_waiter;
    return tl_objects_waiter;
}

inline bool waitable_object_wait_is_completed(wait_result_t result, std::size_t index)
{
    return ((result & (wait_result_t(1) << index)) != 0);
}

inline bool objects_waiter::collect_result(wait_result_t& result, std::size_t count, waitable_object** objects) noexcept
{
    result = 0;

    for(std::size_t index = 0; index < count; ++index)
    {
        if(objects[index]->try_consume())
        {
            result |= (wait_result_t(1) << index);
        }
    }
    return (result != 0);
}

inline std::size_t objects_waiter::register_self(std::size_t count, waitable_object** objects) noexcept
{
    for(std::size_t index = 0; index < count; ++index)
    {
        if(!objects[index]->register_waiter(*this))
        {
            return index;
        }
    }
    return count;
}
inline void objects_waiter::unregiser_self(std::size_t count, waitable_object** objects) noexcept
{
    for(std::size_t index = count; index-- > 0; )
    {
        objects[index]->unregister_waiter(*this);
    }
}

}
