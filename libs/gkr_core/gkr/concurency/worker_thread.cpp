#include <gkr/concurency/worker_thread.h>

#include <gkr/sys/thread_name.h>

namespace gkr
{

worker_thread::worker_thread(std::size_t initial_actions_queue_capacity, std::size_t initial_actions_queue_element_size) noexcept(false)
    : m_queue_waiter (gkr::events_waiter::Flag_ForbidMultipleEventsBind)
    , m_outer_waiter (gkr::events_waiter::Flag_ForbidMultipleEventsBind)
    , m_inner_waiter (gkr::events_waiter::Flag_ForbidMultipleThreadsWait | gkr::events_waiter::Flag_AllowPartialEventsWait)
{
    m_done_event.bind_with(m_outer_waiter, false, false);
    m_work_event.bind_with(m_inner_waiter, false, false);

    m_actions_queue.bind_with_producer_waiter(m_queue_waiter);
    m_actions_queue.bind_with_consumer_waiter(m_inner_waiter);

    m_actions_queue.reset(
        initial_actions_queue_capacity,
        initial_actions_queue_element_size,
        alignof(actions_queue_element_header_t)
        );
}

worker_thread::~worker_thread() noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(
        !joinable(),
        "The running thread must be joined before being destructed or call Join method in leaf child class destructor"
        );
}

bool worker_thread::run() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(m_actions_queue.element_size() >= sizeof(actions_queue_element_header_t), false);

    std::lock_guard<std::mutex> lock(m_mutex);

    Check_ValidState(!m_thread.joinable(), false);

    m_thread = std::thread([this] () { thread_proc(); });

    m_work_event.fire();
    m_done_event.wait();

    return running();
}

bool worker_thread::join(bool send_quit_signal) noexcept(DIAG_NOEXCEPT)
{
    if(!m_thread.joinable()) return false;

    Check_ValidState(!in_worker_thread(), false);

    if(send_quit_signal) quit();

    m_thread.join();

    return true;
}

bool worker_thread::quit() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(running(), false);

    if(in_worker_thread())
    {
        m_running = false;
        return true;
    }
    else
    {
        return enqueue_action(ACTION_QUIT);
    }
}

bool worker_thread::update_wait() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(running(), false);

    if(in_worker_thread())
    {
        m_updating = true;
        return true;
    }
    else
    {
        return enqueue_action(ACTION_UPDATE);
    }
}

bool worker_thread::resize_actions_queue(size_t capacity) noexcept(false)
{
    if(running())
    {
        Check_ValidState(!in_worker_thread(), false);
        return m_actions_queue.resize(capacity);
    }
    else
    {
        m_actions_queue.reset(capacity);
        return true;
    }
}

void worker_thread::forward_action(action_id_t id, void* param, void* result) noexcept(DIAG_NOEXCEPT)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Check_ValidState(running(), );

    m_func = func_t{id, param, result};

    m_work_event.fire();
    m_done_event.wait();
}

bool worker_thread::reply_action() noexcept
{
    if(m_reply == nullptr) return false;

    m_reply = nullptr;

    m_done_event.fire();

    return true;
}

void worker_thread::thread_proc() noexcept(DIAG_NOEXCEPT)
{
    m_work_event.wait();

    const char* name = get_name();

    sys::set_current_thread_name(name);

    m_running = safe_start();

    m_done_event.fire();

    if(running())
    {
        do
        {
            m_updating = false;
        }
        while(main_loop());

        safe_dequeue_actions(true);
    }
    safe_finish();
}

bool worker_thread::safe_start() noexcept
{
    Check_ValidState(m_inner_waiter.events_count() == OWN_EVENTS_TO_WAIT, false);

    m_actions_queue.threading.any_thread_can_be_producer();
    m_actions_queue.threading.set_this_thread_as_exclusive_consumer();

#ifndef __cpp_exceptions
    return on_start();
#else
    try
    {
        return on_start();
    }
    catch(const std::exception& e)
    {
        return on_exception(except_method_t::on_start, &e);
    }
    catch(...)
    {
        return on_exception(except_method_t::on_start, nullptr);
    }
#endif
}

void worker_thread::safe_finish() noexcept
{
#ifndef __cpp_exceptions
    on_finish();
#else
    try
    {
        on_finish();
    }
    catch(const std::exception& e)
    {
        on_exception(except_method_t::on_finish, &e);
    }
    catch(...)
    {
        on_exception(except_method_t::on_finish, nullptr);
    }
#endif

    m_inner_waiter.pop_events(OWN_EVENTS_TO_WAIT);
}

bool worker_thread::main_loop() noexcept(DIAG_NOEXCEPT)
{
    const std::chrono::nanoseconds timeout = get_wait_timeout();

    m_inner_waiter.pop_events(OWN_EVENTS_TO_WAIT);

    bind_events(m_inner_waiter);

    Check_ValidState(m_inner_waiter.events_count() >= OWN_EVENTS_TO_WAIT, false);

    while(running())
    {
        if(m_updating) return true;

        wait_result_t wait_result;

        if(timeout == std::chrono::nanoseconds::max())
        {
            wait_result = m_inner_waiter.wait();
        }
        else if(timeout == std::chrono::nanoseconds::zero())
        {
            wait_result = m_inner_waiter.check();
        }
        else
        {
            wait_result = m_inner_waiter.wait_for(timeout);
        }

        Check_ValidState(wait_result != WAIT_RESULT_ERROR, false);

        if(wait_result == WAIT_RESULT_TIMEOUT)
        {
            safe_notify_wait_timeout();
        }
        else
        {
            constexpr wait_result_t OTHER_EVENTS_MASK = ~((wait_result_t(1) << OWN_EVENTS_TO_WAIT) - 1);

            if((wait_result & OTHER_EVENTS_MASK) != 0)
            {
                safe_notify_wait_success(wait_result & OTHER_EVENTS_MASK);
            }
            if(m_work_event.is_signaled(wait_result))
            {
                safe_do_cross_action();
            }
            if(m_actions_queue.consumer_event_is_signaled(wait_result))
            {
                safe_dequeue_actions(false);
            }
        }
    }
    return false;
}

void worker_thread::safe_dequeue_actions(bool all) noexcept(DIAG_NOEXCEPT)
{
    do
    {
        auto element = m_actions_queue.try_start_pop();

        if(!element.pop_in_progress()) break;

        actions_queue_element_header_t& header = element.value<actions_queue_element_header_t>();

        switch(header.id)
        {
            case ACTION_UPDATE: m_updating = true ; break;
            case ACTION_QUIT  : m_running  = false; break;
            default:
#ifndef __cpp_exceptions
                on_queue_action(header.id, element.data());
#else
                try
                {
                    on_queue_action(header.id, element.data());
                }
                catch(const std::exception& e)
                {
                    m_running = on_exception(except_method_t::on_queue_action, &e);
                }
                catch(...)
                {
                    m_running = on_exception(except_method_t::on_queue_action, nullptr);
                }
#endif
                break;
        }
    }
    while(all);
}

void worker_thread::safe_do_cross_action() noexcept(DIAG_NOEXCEPT)
{
    m_reply = &m_func.result;

#ifndef __cpp_exceptions
    on_action(id, param, result);
#else
    try
    {
        on_cross_action(m_func.id, m_func.param, m_func.result);
    }
    catch(const std::exception& e)
    {
        m_running = on_exception(except_method_t::on_cross_action, &e);
    }
    catch(...)
    {
        m_running = on_exception(except_method_t::on_cross_action, nullptr);
    }
#endif

    reply_action();
}

void worker_thread::safe_notify_wait_timeout() noexcept
{
#ifndef __cpp_exceptions
    on_wait_timeout();
#else
    try
    {
        on_wait_timeout();
    }
    catch(const std::exception& e)
    {
        m_running = on_exception(except_method_t::on_wait_timeout, &e);
    }
    catch(...)
    {
        m_running = on_exception(except_method_t::on_wait_timeout, nullptr);
    }
#endif
}

void worker_thread::safe_notify_wait_success(wait_result_t wait_result) noexcept
{
#ifndef __cpp_exceptions
    on_wait_success(wait_result);
#else
    try
    {
        on_wait_success(wait_result);
    }
    catch(const std::exception& e)
    {
        m_running = on_exception(except_method_t::on_wait_success, &e);
    }
    catch(...)
    {
        m_running = on_exception(except_method_t::on_wait_success, nullptr);
    }
#endif
}

}
