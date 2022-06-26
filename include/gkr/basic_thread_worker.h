#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <functional>

#include "waitable_event.h"
#include "objects_waiter.h"

#include <gkr/misc/stack_args_order.h>

#ifndef GKR_BTW_API
#define GKR_BTW_API
#endif

namespace gkr
{
using std::size_t;

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
    using action_id_t = size_t;

protected:
    GKR_BTW_API          basic_thread_worker();
    GKR_BTW_API virtual ~basic_thread_worker() noexcept(DIAG_NOEXCEPT);

protected:
    virtual const char* get_name() noexcept = 0;

    virtual std::chrono::nanoseconds get_wait_timeout() noexcept = 0;

    virtual size_t get_wait_objects_count() noexcept = 0;

    virtual waitable_object* get_wait_object(size_t index) = 0;

    virtual bool start() = 0;
    virtual void finish() = 0;

    virtual void on_action(action_id_t action, void* param, void* result) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(size_t index) = 0;

    virtual bool on_exception(bool can_continue, const std::exception* e) noexcept = 0;

public:
    GKR_BTW_API bool run();

    GKR_BTW_API bool quit();
    GKR_BTW_API bool join(bool send_quit_signal);

    GKR_BTW_API bool update_wait();

public:
    using action_param_deleter_t = std::function<void(void*)>;

    GKR_BTW_API bool enqueue_action(action_id_t action, void* param = nullptr, action_param_deleter_t deleter = nullptr);
    GKR_BTW_API void perform_action(action_id_t action, void* param = nullptr, void* result = nullptr);

private:
    GKR_BTW_API void process_action(action_id_t action, void* param, void* result);

    GKR_BTW_API bool can_reply();
    GKR_BTW_API void reply_action();

public:
    std::thread::id get_thread_id()
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
    void safe_notify_complete(size_t index);

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

    objects_waiter m_waiter;

    using waitable_event_t = waitable_event<false, 1>;

    waitable_event_t m_wake_event;
    waitable_event_t m_work_event;
    waitable_event_t m_done_event;

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
    objects_waiter& get_waiter() noexcept
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

        constexpr size_t count = sizeof...(args);

        const void* params[count + 1] = {reinterpret_cast<const void*>(count), static_cast<const void*>(std::addressof(args))...};

        impl::result<R> result;

        process_action(action, params, result.ptr());

        return result.val();
    }

protected:
#ifndef GKR_RIGHT_TO_LEFT_ARGS_IN_STACK
#if defined(__clang__) && defined(__linux__)
#define GKR_RIGHT_TO_LEFT_ARGS_IN_STACK 0
#else
#define GKR_RIGHT_TO_LEFT_ARGS_IN_STACK 1
#endif
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
#endif

    static void check_args_order()
    {
#if GKR_RIGHT_TO_LEFT_ARGS_IN_STACK
        Assert_Check( method_args_stack_order_is_right_to_left());
#else
        Assert_Check(!method_args_stack_order_is_right_to_left());
#endif
    }

    template<class R, class C, typename... Args>
    void call_action_method(R (C::*method)(Args...), void* param, void* result)
    {
        Assert_Check(in_worker_thread());

        void** params = reinterpret_cast<void**>(param);

#if GKR_RIGHT_TO_LEFT_ARGS_IN_STACK
        const size_t count = reinterpret_cast<size_t>(*params);

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
        Assert_Check(in_worker_thread());

        void** params = reinterpret_cast<void**>(param);

#if GKR_RIGHT_TO_LEFT_ARGS_IN_STACK
        const size_t count = reinterpret_cast<size_t>(*params);

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
};

}
