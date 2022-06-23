#pragma once

#include "diagnostics.h"
#include "waitable_object.h"

namespace gkr
{

using wait_result_t = unsigned;

constexpr std::size_t maximum_wait_objects = sizeof(wait_result_t) * 8 - 1;

constexpr wait_result_t wait_result_error   = wait_result_t(1U << maximum_wait_objects);
constexpr wait_result_t wait_result_timeout = wait_result_t(0);

inline constexpr bool waitable_object_wait_is_completed(wait_result_t wait_result, std::size_t index) noexcept
{
    return (wait_result & (wait_result_t(1) << index)) != 0;
}

constexpr std::chrono::nanoseconds timeout_infinite = std::chrono::nanoseconds::max ();
constexpr std::chrono::nanoseconds timeout_ignore   = std::chrono::nanoseconds::zero();

class objects_waiter : public impl::basic_objects_waiter
{
    objects_waiter           (const objects_waiter&) noexcept = delete;
    objects_waiter& operator=(const objects_waiter&) noexcept = delete;

    objects_waiter           (objects_waiter&&) noexcept = delete;
    objects_waiter& operator=(objects_waiter&&) noexcept = delete;

public:
    objects_waiter() noexcept = default;
   ~objects_waiter() noexcept = default;

private:
    struct node_t
    {
        waitable_object** objects;
        std::size_t       count;
        node_t*           next;
    };
    node_t *m_first = nullptr;

    bool find_object(waitable_object* object) noexcept
    {
        for(node_t* node = m_first; node != nullptr; node = node->next)
        {
            for(std::size_t index = 0; index < node->count; ++index)
            {
                if(object == node->objects[index])
                {
                    return true;
                }
            }
        }
        return false;
    }
    std::size_t register_self(std::size_t count, waitable_object** objects) noexcept
    {
        for(std::size_t index = 0; index < count; ++index)
        {
            if(!find_object(objects[index]))
            {
                if(!objects[index]->register_waiter(*this))
                {
                    return index;
                }
            }
        }
        return count;
    }
    void unregiser_self(std::size_t count, waitable_object** objects) noexcept
    {
        for(std::size_t index = 0; index < count; ++index)
        {
            if(!find_object(objects[index]))
            {
                objects[index]->unregister_waiter(*this);
            }
        }
    }

private:
    class waiter_guard
    {
        objects_waiter* m_parent;
        node_t          m_node;
    public:
        waiter_guard(objects_waiter& waiter, std::size_t count, waitable_object** objects) noexcept
            : m_parent(nullptr)
            , m_node  {objects, count, waiter.m_first}
        {
            const std::size_t registered = waiter.register_self(count, objects);

            if(registered < count)
            {
                waiter.unregiser_self(registered, objects);
                return;
            }
            waiter.m_first = &m_node;

            m_parent = &waiter;
        }
        ~waiter_guard() noexcept
        {
            if(initialization_failed()) return;

            objects_waiter& waiter = *m_parent;

            node_t** prev;
            for(prev = &waiter.m_first; *prev != &m_node; prev = &(*prev)->next);
            *prev = m_node.next;

            waiter.unregiser_self(m_node.count, m_node.objects);
        }
        bool initialization_failed() const noexcept
        {
            return (m_parent == nullptr);
        }
    };
    friend class exception_guard;

private:
    static bool collect_result(wait_result_t& result, std::size_t count, waitable_object** objects) noexcept
    {
        result = 0;
        for(std::size_t index = 0; index < count; ++index)
        {
            if(objects[index]->try_consume())
            {
                result |= (1U << index);
            }
        }
        return (result != 0);
    }

public:
    template<typename... WaitableObjects>
    static wait_result_t check(WaitableObjects&... waitable_objects)
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        waitable_object* objects[count] = {&waitable_objects...};

        return check(count, objects);
    }
    static wait_result_t check(std::size_t count, waitable_object** objects)
    {
        Check_ValidArg(count > 0, wait_result_error);
        Check_ValidArg(count < maximum_wait_objects, wait_result_error);

        Check_NotNullArg(objects, wait_result_error);

        Check_ValidArrayArg(index, count, objects[index] != nullptr, wait_result_error);

        wait_result_t result;

        collect_result(result, count, objects);

        return result;
    }

    template<typename... WaitableObjects>
    wait_result_t wait(WaitableObjects&... waitable_objects)
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        waitable_object* objects[count] = {&waitable_objects...};

        return wait(count, objects);
    }
    wait_result_t wait(std::size_t count, waitable_object** objects)
    {
        Check_ValidArg(count > 0, wait_result_error);
        Check_ValidArg(count < maximum_wait_objects, wait_result_error);

        Check_NotNullArg(objects, wait_result_error);

        Check_ValidArrayArg(index, count, objects[index] != nullptr, wait_result_error);

        wait_result_t result;

        if(collect_result(result, count, objects))
        {
            return result;
        }
        std::unique_lock<std::mutex> lock(m_mutex);

        waiter_guard guard(*this, count, objects);

        if(guard.initialization_failed())
        {
            return wait_result_error;
        }
        for( ; ; )
        {
            m_cvar.wait(lock);

            if(collect_result(result, count, objects)) break;
        }

        return result;
    }

    template<typename Rep, typename Period, typename... WaitableObjects>
    wait_result_t wait(std::chrono::duration<Rep, Period> timeout, WaitableObjects&... waitable_objects)
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        waitable_object* objects[count] = {&waitable_objects...};

        return wait(timeout, count, objects);
    }
    template<typename Rep, typename Period>
    wait_result_t wait(std::chrono::duration<Rep, Period> timeout, std::size_t count, waitable_object** objects)
    {
        Check_ValidArg(count > 0, wait_result_error);
        Check_ValidArg(count < maximum_wait_objects, wait_result_error);

        Check_NotNullArg(objects, wait_result_error);

        Check_ValidArrayArg(index, count, objects[index] != nullptr, wait_result_error);

        using duration_t = std::chrono::duration<Rep, Period>;

        wait_result_t result;

        if(collect_result(result, count, objects))
        {
            return result;
        }
        if(timeout == duration_t::zero())
        {
            return wait_result_timeout;
        }

        std::unique_lock<std::mutex> lock(m_mutex);

        waiter_guard guard(*this, count, objects);

        if(guard.initialization_failed())
        {
            return wait_result_error;
        }
        if(timeout == duration_t::max())
        {
            for( ; ; )
            {
                m_cvar.wait(lock);

                if(collect_result(result, count, objects)) break;
            }
        }
        else
        {
            for(auto start_time = std::chrono::steady_clock::now(); ; )
            {
                if(m_cvar.wait_for(lock, timeout) == std::cv_status::timeout) break;

                if(collect_result(result, count, objects)) break;

                const auto now = std::chrono::steady_clock::now();

                const duration_t elapsed_time = std::chrono::duration_cast<duration_t>(now - start_time);

                if(elapsed_time >= timeout) break;

                timeout -= elapsed_time;

                start_time = now;
            }
        }

        return result;
    }
};

template<typename WaitableObject>
inline waitable_object_guard<WaitableObject> guard_waitable_object(
    wait_result_t wait_result,
    std::size_t index,
    WaitableObject& object
    ) noexcept
{
    return waitable_object_guard<WaitableObject>(
        object,
        waitable_object_wait_is_completed(wait_result, index)
        );
}

}
