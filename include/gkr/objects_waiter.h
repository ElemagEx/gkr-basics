#pragma once

#include "diagnostics.h"
#include "waitable_object.h"

namespace gkr
{

using wait_result_t = unsigned;

constexpr std::size_t maximum_wait_objects = sizeof(wait_result_t) * 8 - 1;

constexpr wait_result_t wait_result_error   = wait_result_t(0x80000000);
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

    objects_waiter           (objects_waiter&& other) noexcept = delete;
    objects_waiter& operator=(objects_waiter&& other) noexcept = delete;

public:
    objects_waiter() noexcept = default;
   ~objects_waiter() noexcept = default;

private:
    struct node_t
    {
        waitable_object** objects;
        std::size_t       count;
        node_t*           next;
    } *first = nullptr;

    bool find_object(waitable_object* object) noexcept
    {
        for(node_t* node = first; node != nullptr; node = node->next)
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
    bool register_self(std::size_t count, waitable_object** objects) noexcept
    {
        bool result = true;

        for(std::size_t index = 0; index < count; ++index)
        {
            if(!find_object(objects[index]))
            {
                result = objects[index]->register_waiter(*this) && result;
            }
        }
        return result;
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
    class exception_guard
    {
        objects_waiter* parent;
        node_t          node;
    public:
        exception_guard(objects_waiter& waiter, std::size_t count, waitable_object** objects) noexcept
            : parent(nullptr)
            , node  {objects, count, waiter.first}
        {
            if(!waiter.register_self(count, objects)) return;

            waiter.first = &node;

            parent = &waiter;
        }
        ~exception_guard() noexcept
        {
            if(initialization_failed()) return;

            objects_waiter& waiter = *parent;

            node_t** prev;
            for(prev = &waiter.first; *prev != &node; prev = &(*prev)->next);
            *prev = node.next;

            waiter.unregiser_self(node.count, node.objects);
        }
        bool initialization_failed() const noexcept
        {
            return (parent == nullptr);
        }
    };
    friend class exception_guard;

private:
    bool collect_result(wait_result_t& result, std::size_t count, waitable_object** objects) noexcept
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
    wait_result_t check(WaitableObjects&... waitable_objects)
    {
        constexpr std::size_t count = sizeof...(waitable_objects);

        waitable_object* objects[count] = {&waitable_objects...};

        return check(count, objects);
    }
    wait_result_t check(std::size_t count, waitable_object** objects)
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

        exception_guard guard(*this, count, objects);

        if(guard.initialization_failed())
        {
            return wait_result_error;
        }
        do
        {
            m_cvar.wait(lock);
        }
        while(!collect_result(result, count, objects));

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

        exception_guard guard(*this, count, objects);

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
inline waitable_object_guard<WaitableObject> guard_waitable_object(wait_result_t wait_result, std::size_t index, WaitableObject& object) noexcept
{
    if(waitable_object_wait_is_completed(wait_result, index))
    {
        return waitable_object_guard<WaitableObject>(object);
    }
    else
    {
        return waitable_object_guard<WaitableObject>();
    }
}

}
