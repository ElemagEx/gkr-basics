#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <functional>

#include "sync_event.h"
#include "objects_waiter.h"

#ifndef GKR_TWB_API
#define GKR_TWB_API
#endif

namespace gkr
{

class thread_worker_base
{
    thread_worker_base           (const thread_worker_base&) noexcept = delete;
    thread_worker_base& operator=(const thread_worker_base&) noexcept = delete;

    thread_worker_base           (thread_worker_base&&) noexcept = delete;
    thread_worker_base& operator=(thread_worker_base&&) noexcept = delete;

public:
    using action_id_t = std::size_t;

protected:
    GKR_TWB_API          thread_worker_base();
    GKR_TWB_API virtual ~thread_worker_base();

protected:
    virtual const char* get_name() = 0;

    virtual std::chrono::nanoseconds get_wait_timeout() = 0;

    virtual size_t get_wait_objects_count() = 0;
    virtual waitable_object& get_wait_object(size_t index) = 0;

    virtual bool start() = 0;
    virtual void finish() = 0;

    virtual void on_action(action_id_t action, void* param, void* result) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(size_t index) = 0;

public:
    GKR_TWB_API bool run();

    GKR_TWB_API bool quit();
    GKR_TWB_API bool join(bool send_quit_signal);

    GKR_TWB_API void update_wait();

public:
    using action_param_deleter_t = std::function<void(void*)>;

    GKR_TWB_API bool enqueue_action(action_id_t action, void* param = nullptr, action_param_deleter_t deleter = nullptr);
    GKR_TWB_API bool execute_action(action_id_t action, void* param = nullptr, void* result = nullptr);

protected:
    GKR_TWB_API bool can_reply();
    GKR_TWB_API void reply_action();

public:
    std::thread::id get_id()
    {
        return m_thread.get_id();
    }
    bool joinable() const
    {
        return m_thread.joinable();
    }
    bool running() const
    {
        return m_running;
    }
    bool in_worker_thread() const
    {
        return (std::this_thread::get_id() == m_thread.get_id());
    }

private:
    void thread_proc();

    bool acquire_events();

    bool main_loop();

    void dequeue_actions();

    bool do_action(action_id_t action, void* param, void* result, bool cross_thread_caller);

private:
    enum : size_t
    {
        OWN_EVENT_HAS_ASYNC_ACTION,
        OWN_EVENT_HAS_SYNC_ACTION,
        OWN_EVENTS_TO_WAIT,
    };
    enum : action_id_t
    {
        ACTION_UPDATE = action_id_t(-2),
        ACTION_QUIT   = action_id_t(-1),
    };
    struct func_t
    {
        action_id_t action;
        void*       param;
        void*       result;
    };
    struct reentrancy_t
    {
        size_t count;
        void** result;
    };

    reentrancy_t m_reentrancy = {};
    std::thread  m_thread;
    std::mutex   m_mutex;

    waitable_object** m_objects = nullptr;

    size_t m_count  = 0;
    func_t m_func   = {};

    bool m_running  = false;
    bool m_updating = false;

    sync_event<> m_wake_event;
    sync_event<> m_work_event;
    sync_event<> m_done_event;

    objects_waiter m_waiter;

private:
    struct item_t
    {
        action_id_t            action;
        void*                  param;
        action_param_deleter_t deleter;
    };
    struct sync_queue_t
    {
        std::queue<item_t> queue;
        std::mutex         mutex;
    };
    sync_queue_t m_sync_queue;

public:
    template<typename R, typename C, typename... Args>
    R execute_action_ex(int action, /*C* obj, */R (C::*method)(Args...), Args... args)
    {
        if(in_worker_thread())
        {
            return (static_cast<C*>(this)->*method)(args...);
        }

		constexpr size_t count = sizeof...(args);
#ifdef _WIN32
        const void* params[count + 1] = { reinterpret_cast<const void*>(count), static_cast<const void*>(&args)... };
#else
        const void* params[count + 0] = {                                       static_cast<const void*>(&args)... };
#endif
        R result;

        execute_action(action, params, &result);

        return result;
    }
#if 0
    template<typename C, typename... Args>
    void execute_action_ex(int action, /*C* obj, */auto (C::*method)(Args...), Args&&... args)
    {
        if(in_worker_thread())
        {
            (static_cast<C*>(this)->*method)(args...);
            return;
        }

        constexpr size_t count = sizeof...(args);
#ifdef _WIN32
        const void* params[count + 1] = { reinterpret_cast<const void*>(count), static_cast<const void*>(&args)... };
#else
        const void* params[count + 0] = {                                       static_cast<const void*>(&args)... };
#endif
        execute_action(action, params);
    }
#endif
protected:
    template<class R, class C, typename ... Args>
    void call_action_method(R (C::*method)(Args...), void* param, void* result)
    {
        void** params = reinterpret_cast<void**>(param);
#ifdef _WIN32
        const size_t count = reinterpret_cast<size_t>(*params);

        params += count;

        auto ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<std::remove_reference_t<Args>*>(*params--)...);
#else
        auto ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<std::remove_reference_t<Args>*>(*params++)...);
#endif
        if(result != nullptr)
        {
            *static_cast<decltype(ret)*>(result) = ret;
        }
    }

    template<typename T>
    void reply_action(T&& value)
    {
        if(can_reply())
        {
            *static_cast<T*>(m_reentrancy->result) = std::move(value);
            reply_action();
        }
    }
    template<typename T>
    void reply_action(const T& value)
    {
        if(can_reply())
        {
            *static_cast<T*>(m_reentrancy->result) = value;
            reply_action();
        }
    }
};

}
