#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <functional>

#include "waitable_event.h"
#include "objects_waiter.h"

#ifndef GKR_TWB_API
#define GKR_TWB_API
#endif

namespace gkr
{
namespace impl
{
template<class T>
struct result
{
    T value;
    T* ptr() { return &value; }
    T& val() { return  value; }
};
template<>
struct result<void>
{
    void* ptr() { return nullptr; }
    void  val() { return; }
};
}

class basic_thread_worker
{
    basic_thread_worker           (const basic_thread_worker&) noexcept = delete;
    basic_thread_worker& operator=(const basic_thread_worker&) noexcept = delete;

    basic_thread_worker           (basic_thread_worker&&) noexcept = delete;
    basic_thread_worker& operator=(basic_thread_worker&&) noexcept = delete;

public:
    using action_id_t = std::size_t;

protected:
    GKR_TWB_API          basic_thread_worker();
    GKR_TWB_API virtual ~basic_thread_worker() noexcept(DIAG_NOEXCEPT);

protected:
    virtual const char* get_name() noexcept = 0;

    virtual std::chrono::nanoseconds get_wait_timeout() noexcept = 0;

    virtual std::size_t get_wait_objects_count() noexcept = 0;

    virtual waitable_object* get_wait_object(std::size_t index) = 0;

    virtual bool start() = 0;
    virtual void finish() = 0;

    virtual void on_action(action_id_t action, void* param, void* result) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(std::size_t index) = 0;

    virtual bool on_exception(bool can_continue, const std::exception* e) noexcept = 0;

public:
    GKR_TWB_API bool run();

    GKR_TWB_API bool quit();
    GKR_TWB_API bool join(bool send_quit_signal);

    GKR_TWB_API bool update_wait();

public:
    using action_param_deleter_t = std::function<void(void*)>;

    GKR_TWB_API bool enqueue_action(action_id_t action, void* param = nullptr, action_param_deleter_t deleter = nullptr);
    GKR_TWB_API void execute_action(action_id_t action, void* param = nullptr, void* result = nullptr);

private:
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

    bool safe_start () noexcept;
    void safe_finish() noexcept;

    bool safe_acquire_events() noexcept;

    bool main_loop();

    void dequeue_actions();

    void safe_do_action(action_id_t action, void* param, void* result, bool cross_thread_caller);

    void safe_notify_timeout();
    void safe_notify_complete(std::size_t index);

private:
    enum : std::size_t
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
        std::size_t count;
        void**      result;
    };

    reentrancy_t m_reentrancy = {};
    std::thread  m_thread;
    std::mutex   m_mutex;

    waitable_object** m_objects = nullptr;

    std::size_t m_count  = 0;
    func_t      m_func   = {};

    bool m_running  = false;
    bool m_updating = false;

    waitable_event<> m_wake_event;
    waitable_event<> m_work_event;
    waitable_event<> m_done_event;

    objects_waiter m_waiter;

private:
    struct item_t
    {
        action_id_t            action;
        void*                  param;
        action_param_deleter_t deleter;
    };
    std::queue<item_t> m_sync_queue_items;
    std::mutex         m_sync_queue_mutex;

protected:
    objects_waiter& get_waiter()
    {
        return m_waiter;
    }

    template<typename T>
    void reply_action(T&& value)
    {
        if(can_reply())
        {
            *static_cast<T*>(m_reentrancy.result) = std::move(value);
            reply_action();
        }
    }
    template<typename T>
    void reply_action(const T& value)
    {
        if(can_reply())
        {
            *static_cast<T*>(m_reentrancy.result) = value;
            reply_action();
        }
    }

public:
    template<typename R, typename... Args>
    R execute_action_method(action_id_t action, Args&&... args)
    {
        Assert_Check(!in_worker_thread());

        constexpr std::size_t count = sizeof...(args);

        const void* params[count + 1] = {reinterpret_cast<const void*>(count), static_cast<const void*>(std::addressof(args))...};

        impl::result<R> result;

        execute_action(action, params, result.ptr());

        return result.val();
    }

protected:
#if defined(__clang__) && defined(__linux__)
#define RIGHT_TO_LEFT_ARGS_IN_STACK 0
#else
#define RIGHT_TO_LEFT_ARGS_IN_STACK 1
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
#endif

    template<class R, class C, typename... Args>
    void call_action_method(R (C::*method)(Args...), void* param, void* result)
    {
        void** params = reinterpret_cast<void**>(param);

#if RIGHT_TO_LEFT_ARGS_IN_STACK
        const std::size_t count = reinterpret_cast<std::size_t>(*params);

        params += count;

        auto ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);
#else
        auto ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);
#endif
        if(result != nullptr)
        {
            *static_cast<decltype(ret)*>(result) = ret;
        }
    }
    template<class C, typename... Args>
    void call_action_method(void (C::*method)(Args...), void* param)
    {
        void** params = reinterpret_cast<void**>(param);

#if RIGHT_TO_LEFT_ARGS_IN_STACK
        const std::size_t count = reinterpret_cast<std::size_t>(*params);

        params += count;

        (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);
#else
        (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);
#endif
    }

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#undef RIGHT_TO_LEFT_ARGS_IN_STACK
};

}
