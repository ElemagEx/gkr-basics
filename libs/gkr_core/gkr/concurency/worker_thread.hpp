#pragma once

#include <gkr/api.hpp>
#include <gkr/concurency/waitable_object.hpp>

#include <gkr/concurency/waitable_lockfree_queue.hpp>
#include <gkr/misc/stack_args_order.hpp>
#include <gkr/diagnostics.hpp>

#include <cstddef>
#include <thread>
#include <mutex>

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

    struct alignas(std::max_align_t) actions_queue_element_header_t { action_id_t id {0}; };

protected:
    GKR_INNER_API worker_thread(
        std::size_t initial_actions_queue_capacity     = 8,
        std::size_t initial_actions_queue_element_size = sizeof(actions_queue_element_header_t)
        );
    GKR_INNER_API virtual ~worker_thread();

protected:
    virtual const char* get_name() noexcept = 0;

    virtual long long get_wait_timeout_ns() noexcept = 0;

    virtual std::size_t get_waitable_objects_count() noexcept = 0;

    virtual waitable_object& get_waitable_object(std::size_t index) noexcept = 0;

    virtual bool on_start() = 0;
    virtual void on_finish() = 0;

    virtual void on_cross_action(action_id_t action, void* param, void* result) = 0;
    virtual void on_queue_action(action_id_t action, void* data) = 0;

    virtual void on_wait_timeout() = 0;
    virtual void on_wait_success(std::size_t index) = 0;

    enum class except_method_t {on_start, on_finish, on_cross_action, on_queue_action, on_wait_timeout, on_wait_success};
    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept = 0;

public:
    GKR_INNER_API bool run();
    GKR_INNER_API bool join(bool send_quit_signal);

public:
    GKR_INNER_API bool quit();

    GKR_INNER_API bool update_wait();

    GKR_INNER_API bool resize_actions_queue(std::size_t capacity);

private:
    GKR_INNER_API void forward_action(action_id_t id, void* param, void* result);

    GKR_INNER_API bool reply_action() noexcept;

public:
    std::thread::id thread_id() const noexcept
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
    void thread_proc();

    bool safe_start () noexcept;
    void safe_finish() noexcept;

    bool main_loop();

    void safe_dequeue_actions(bool all);

    void safe_do_cross_thread_action();

    void safe_notify_wait_timeout() noexcept;
    void safe_notify_wait_success(wait_result_t wait_result, waitable_object**, std::size_t count) noexcept;

private:
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
    using actions_queue_t = waitable_lockfree_queue<void, true, true>;

    func_t m_func  = {};
    void** m_reply = nullptr;

    std::mutex  m_mutex;
    std::thread m_thread;

    actions_queue_t m_actions_queue;

    waitable_event m_work_event;
    waitable_event m_done_event;

    bool m_running  = false;
    bool m_updating = false;

protected:
    unsigned get_actions_queue_fill_percentage() const noexcept
    {
        return unsigned((100 * m_actions_queue.count()) / m_actions_queue.capacity());
    }
    bool enqueue_action(action_id_t id)
    {
        Check_ValidState(!in_worker_thread(), false);
        Check_ValidState(running(), false);

        auto element = m_actions_queue.start_push();

        Check_ValidState(element.push_in_progress(), false);

        element.as<actions_queue_element_header_t>().id = id;
        return true;
    }
    template<typename T>
    bool enqueue_action(action_id_t id, T&& data) noexcept(DIAG_NOEXCEPT && std::is_nothrow_move_constructible<T>::value)
    {
        Check_ValidState(running() && !in_worker_thread(), false);

        static_assert(sizeof (T) >= sizeof(actions_queue_element_header_t), "Your data must inherit or start with actions_queue_element_header_t");
        static_assert(alignof(T) >= sizeof(actions_queue_element_header_t), "Your data must have alignment greater or equal to the alignement of actions_queue_element_header_t");

        auto element = m_actions_queue.start_push();

        Check_ValidState(element.push_in_progress(), false);

        new (element.data()) T(std::move(data));

        element.as<actions_queue_element_header_t>().id = id;
        return true;
    }
    template<typename T>
    bool enqueue_action(action_id_t id, const T& data) noexcept(DIAG_NOEXCEPT && std::is_nothrow_copy_constructible<T>::value)
    {
        Check_ValidState(running() && !in_worker_thread(), false);

        static_assert(sizeof (T) >= sizeof(actions_queue_element_header_t), "Your data must inherit or start with actions_queue_element_header_t");
        static_assert(alignof(T) >= sizeof(actions_queue_element_header_t), "Your data must have alignment greater or equal to the alignement of actions_queue_element_header_t");

        auto element = m_actions_queue.start_push();

        Check_ValidState(element.push_in_progress(), false);

        new (element.data()) T(data);

        element.as<actions_queue_element_header_t>().id = id;
        return true;
    }

protected:
    template<typename T>
    bool reply_action(T&& value)
    {
        Check_ValidState(in_worker_thread(), false);

        if(m_reply == nullptr)
        {
            return false;
        }
        if(*m_reply != nullptr)
        {
            *static_cast<T*>(*m_reply) = std::move(value);
        }
        return reply_action();
    }
    template<typename T>
    bool reply_action(const T& value)
    {
        Check_ValidState(in_worker_thread(), false);

        if(m_reply == nullptr)
        {
            return false;
        }
        if(*m_reply != nullptr)
        {
            *static_cast<T*>(*m_reply) = value;
        }
        return reply_action();
    }

protected:
    template<typename R, typename... Args>
    R execute_action_method(action_id_t action, Args&&... args)
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
        Assert_Check( misc::method_args_stack_order_is_right_to_left());
#else
        Assert_Check(!misc::method_args_stack_order_is_right_to_left());
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
