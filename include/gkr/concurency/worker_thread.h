#pragma once

#include <gkr/concurency/events_waiting.h>
#include <gkr/concurency/waitable_lockfree_queue.h>
#include <gkr/misc/stack_args_order.h>

#include <mutex>
#include <thread>
#include <functional>

#ifndef GKR_CWT_API
#define GKR_CWT_API
#endif

namespace gkr
{
namespace impl
{
template<class T>
struct result_t
{
    T value;
    T* ptr() { return &value; }
    T& val() { return  value; }
};
template<>
struct result_t<void>
{
    void* ptr() { return nullptr; }
    void  val() { return; }
};
}

class worker_thread
{
    worker_thread           (const worker_thread&) noexcept = delete;
    worker_thread& operator=(const worker_thread&) noexcept = delete;

    worker_thread           (worker_thread&&) noexcept = delete;
    worker_thread& operator=(worker_thread&&) noexcept = delete;

public:
    using action_id_t = std::size_t;

    using action_param_deleter_t = std::function<void(void*)>;

protected:
    GKR_CWT_API          worker_thread(std::size_t initial_action_queue_capacity = 8) noexcept(false);
    GKR_CWT_API virtual ~worker_thread() noexcept(DIAG_NOEXCEPT);

protected:
    virtual const char* get_name() noexcept = 0;

    virtual std::chrono::nanoseconds get_wait_timeout() noexcept = 0;

    virtual void bind_events(events_waiter& waiter) noexcept(DIAG_NOEXCEPT) = 0;

    virtual bool on_start() = 0;
    virtual void on_finish() = 0;

    virtual void on_action(action_id_t action, void* param, void* result) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(wait_result_t wait_result) = 0;

    enum class except_method_t {on_start, on_finish, on_action, on_wait_timeout, on_wait_success};
    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept = 0;

public:
    GKR_CWT_API bool run() noexcept(DIAG_NOEXCEPT);
    GKR_CWT_API bool join(bool send_quit_signal) noexcept(DIAG_NOEXCEPT);

public:
    GKR_CWT_API bool quit() noexcept(DIAG_NOEXCEPT);

    GKR_CWT_API bool update_wait() noexcept(DIAG_NOEXCEPT);

    GKR_CWT_API bool resize_actions_queue(std::size_t capacity) noexcept(false);

public:
    GKR_CWT_API bool enqueue_action(action_id_t id, void* param = nullptr, action_param_deleter_t deleter = nullptr) noexcept(DIAG_NOEXCEPT);
    GKR_CWT_API void perform_action(action_id_t id, void* param = nullptr, void* result = nullptr) noexcept(DIAG_NOEXCEPT);

private:
    GKR_CWT_API void forward_action(action_id_t id, void* param, void* result) noexcept(DIAG_NOEXCEPT);

    GKR_CWT_API bool can_reply() noexcept;
    GKR_CWT_API bool reply_action() noexcept;

public:
    std::thread::id get_thread_id() const noexcept
    {
        return m_thread.get_id();
    }
    bool joinable() const noexcept
    {
        return m_thread.joinable();
    }
    bool running() const noexcept
    {
        return m_running;
    }
    bool in_worker_thread() const noexcept
    {
        return (std::this_thread::get_id() == m_thread.get_id());
    }

private:
    void thread_proc() noexcept(DIAG_NOEXCEPT);

    bool safe_start () noexcept;
    void safe_finish() noexcept;

    bool main_loop() noexcept(DIAG_NOEXCEPT);

    void dequeue_actions(bool all) noexcept(DIAG_NOEXCEPT);

    void safe_do_action(action_id_t id, void* param, void* result, bool cross_thread_caller) noexcept(DIAG_NOEXCEPT);

    void safe_notify_wait_timeout() noexcept;
    void safe_notify_wait_success(wait_result_t wait_result) noexcept;

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
        action_id_t id;
        void*       param;
        void*       result;
    };
    struct reentrancy_t
    {
        std::size_t count;
        void**      result;
    };
    struct queued_action_t
    {
        action_id_t            id;
        void*                  param;
        action_param_deleter_t deleter;
    };
    using actions_queue_t = waitable_lockfree_queue<queued_action_t, true>;

    reentrancy_t m_reentrancy = {};
    func_t       m_func       = {};

    std::mutex  m_mutex;
    std::thread m_thread;

    events_waiter m_queue_waiter;
    events_waiter m_outer_waiter;
    events_waiter m_inner_waiter;

    actions_queue_t m_actions_queue;

    event_controller m_work_event;
    event_controller m_done_event;

    bool m_running  = false;
    bool m_updating = false;

protected:
    template<typename T>
    bool reply_action(T&& value)
    {
        Check_ValidState(in_worker_thread(), false);

        if(!can_reply())
        {
            return false;
        }
        if(*m_reentrancy.result != nullptr)
        {
            *static_cast<T*>(*m_reentrancy.result) = std::move(value);
        }
        return reply_action();
    }
    template<typename T>
    bool reply_action(const T& value)
    {
        Check_ValidState(in_worker_thread(), false);

        if(!can_reply())
        {
            return false;
        }
        if(*m_reentrancy.result != nullptr)
        {
            *static_cast<T*>(*m_reentrancy.result) = value;
        }
        return reply_action();
    }

protected:
    template<typename R, typename... Args>
    R execute_action_method(action_id_t action, Args&&... args) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check(!in_worker_thread());

        constexpr size_t count = sizeof...(args);

        const void* params[count + 1] = {reinterpret_cast<const void*>(count), static_cast<const void*>(std::addressof(args))...};

        impl::result_t<R> result;

        forward_action(action, params, result.ptr());

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

        R ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);
#else
        R ret = (static_cast<C*>(this)->*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);
#endif
        if(result != nullptr)
        {
            *static_cast<R*>(result) = ret;
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
