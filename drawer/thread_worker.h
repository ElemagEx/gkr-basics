#pragma once

#include <mutex>
#include <thread>
#include <functional>

#include <gkr/thread_waiting.h>
#include <gkr/waitable_event.h>
#include <gkr/waitable_lockfree_queue.h>

#include <gkr/misc/stack_args_order.h>

#ifndef GKR_BTW_API
#define GKR_BTW_API
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

class thread_worker
{
    thread_worker           (const thread_worker&) noexcept = delete;
    thread_worker& operator=(const thread_worker&) noexcept = delete;

    thread_worker           (thread_worker&&) noexcept = delete;
    thread_worker& operator=(thread_worker&&) noexcept = delete;

public:
    using action_id_t = std::size_t;

    using action_param_deleter_t = std::function<void(void*)>;

protected:
    GKR_BTW_API          thread_worker(std::size_t initial_action_queue_capacity = 8) noexcept(false);
    GKR_BTW_API virtual ~thread_worker() noexcept(DIAG_NOEXCEPT);

protected:
    virtual const char* get_name() noexcept = 0;

    virtual std::chrono::nanoseconds get_wait_timeout() noexcept = 0;

    virtual std::size_t get_wait_objects_count() noexcept = 0;

    virtual waitable_object* get_wait_object(std::size_t index) noexcept = 0;

    virtual bool on_start() = 0;
    virtual void on_finish() = 0;

    virtual void on_action(action_id_t action, void* param, void* result) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(std::size_t index) = 0;

    enum class except_method_t {on_start, on_finish, on_action, on_wait_timeout, on_wait_success};
    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept = 0;

public:
    GKR_BTW_API bool run() noexcept(DIAG_NOEXCEPT);
    GKR_BTW_API bool join(bool send_quit_signal) noexcept(DIAG_NOEXCEPT);

public:
    GKR_BTW_API bool quit() noexcept(DIAG_NOEXCEPT);

    GKR_BTW_API bool update_wait() noexcept(DIAG_NOEXCEPT);

    GKR_BTW_API bool resize_actions_queue(std::size_t capacity) noexcept(false);

public:
    GKR_BTW_API bool enqueue_action(action_id_t id, void* param = nullptr, action_param_deleter_t deleter = nullptr) noexcept(DIAG_NOEXCEPT);
    GKR_BTW_API void perform_action(action_id_t id, void* param = nullptr, void* result = nullptr) noexcept(DIAG_NOEXCEPT);

private:
    GKR_BTW_API void forward_action(action_id_t id, void* param, void* result) noexcept(DIAG_NOEXCEPT);

    GKR_BTW_API bool can_reply() noexcept;
    GKR_BTW_API bool reply_action() noexcept;

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

    void safe_notify_timeout() noexcept;
    void safe_notify_complete(std::size_t index) noexcept;

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

    reentrancy_t m_reentrancy = {};

    std::thread  m_thread;
    std::mutex   m_mutex;

    waitable_object** m_objects = nullptr;

    std::size_t m_count  = 0;
    func_t      m_func   = {};

    bool m_running  = false;
    bool m_updating = false;

    using waitable_event_t = waitable_event<false, 1>;

    waitable_event_t m_work_event;
    waitable_event_t m_done_event;

private:
    struct queued_action
    {
        action_id_t            id;
        void*                  param;
        action_param_deleter_t deleter;
    };
    using actions_queue_t = lockfree_queue<queued_action, true, impl::queue_wait_support<0,1>>;

    actions_queue_t m_actions_queue;

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
