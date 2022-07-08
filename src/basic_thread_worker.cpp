#include <gkr/basic_thread_worker.h>

#include <gkr/sys/stack_alloc.h>
#include <gkr/sys/thread_name.h>

namespace gkr
{

basic_thread_worker::basic_thread_worker() noexcept(false)
{
    m_assist_waiter.set_always_broadcast(true);

    m_actions_queue.reset(initial_actions_queue_capacity);

    m_actions_queue.set_producer_waiter(m_assist_waiter);
    m_actions_queue.set_consumer_waiter(m_thread_waiter);
}

basic_thread_worker::~basic_thread_worker() noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(
        !joinable(),
        "The running thread must be joined before being destructed or call Join method in leaf child class destructor"
        );
}

bool basic_thread_worker::run()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Check_ValidState(!m_thread.joinable(), false);

    m_thread = std::thread([this] () { thread_proc(); });

    m_work_event.fire();

    m_assist_waiter.wait(m_done_event);

    return running();
}

bool basic_thread_worker::join(bool send_quit_signal)
{
    if(!m_thread.joinable()) return false;

    Check_ValidState(!in_worker_thread(), false);

    if(send_quit_signal) quit();

    m_thread.join();

    return true;
}

bool basic_thread_worker::quit()
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

bool basic_thread_worker::update_wait()
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

bool basic_thread_worker::resize_actions_queue(size_t capacity)
{
    Check_ValidState(running(), false);

    return m_actions_queue.resize(capacity);
}

bool basic_thread_worker::enqueue_action(action_id_t id, void* param, action_param_deleter_t deleter)
{
    Check_ValidState(running(), false);

    return m_actions_queue.push({id, param, deleter});
}

void basic_thread_worker::perform_action(action_id_t id, void* param, void* result)
{
    if(in_worker_thread())
    {
        safe_do_action(id, param, result, false);
    }
    else
    {
        forward_action(id, param, result);
    }
}

void basic_thread_worker::forward_action(action_id_t id, void* param, void* result)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    Check_ValidState(running(),);

    m_func = func_t{id, param, result};

    m_work_event.fire();

    m_assist_waiter.wait(m_done_event);
}

bool basic_thread_worker::can_reply() noexcept
{
    return ((m_reentrancy.count == 1) && (m_reentrancy.result != nullptr));
}

bool basic_thread_worker::reply_action() noexcept
{
    if(!can_reply()) return false;

    m_reentrancy.result = nullptr;

    m_done_event.fire();

    return true;
}

void basic_thread_worker::thread_proc()
{
    m_thread_waiter.wait(m_work_event);

    m_reentrancy = {0, nullptr};

    const char* name = get_name();

    sys::set_current_thread_name(name);

    m_running = safe_start();

    m_done_event.fire();

    if(running())
    {
        do
        {
            m_updating = false;
            m_count    = OWN_EVENTS_TO_WAIT + get_wait_objects_count();
        }
        while(main_loop());

        dequeue_actions(true);
    }
    safe_finish();
}

bool basic_thread_worker::safe_start() noexcept
{
    m_actions_queue.threading.any_thread_can_be_producer();
    m_actions_queue.threading.set_this_thread_as_exclusive_consumer();

#ifndef __cpp_exceptions
    return start();
#else
    try
    {
        return start();
    }
    catch(const std::exception& e)
    {
        on_exception(false, &e);
    }
    catch(...)
    {
        on_exception(false, nullptr);
    }
    return false;
#endif
}

void basic_thread_worker::safe_finish() noexcept
{
#ifndef __cpp_exceptions
    finish();
#else
    try
    {
        finish();
    }
    catch(const std::exception& e)
    {
        on_exception(false, &e);
    }
    catch(...)
    {
        on_exception(false, nullptr);
    }
#endif
}

bool basic_thread_worker::safe_acquire_events() noexcept
{
    m_objects[OWN_EVENT_HAS_ASYNC_ACTION] =  m_actions_queue.queue_has_items_waitable_object();
    m_objects[OWN_EVENT_HAS_SYNC_ACTION ] = &m_work_event;

    for(size_t index = OWN_EVENTS_TO_WAIT; index < m_count; ++index)
    {
#ifndef __cpp_exceptions
        m_objects[index] = get_wait_object(index - OWN_EVENTS_TO_WAIT);

        if(m_objects[index] == nullptr)
        {
            m_count = index;
            Check_Failure(true);
        }
#else
        try
        {
            m_objects[index] = get_wait_object(index - OWN_EVENTS_TO_WAIT);

            if(m_objects[index] == nullptr)
            {
                m_count = index;
                Check_Failure(true);
            }
        }
        catch(const std::exception& e)
        {
            m_count = index;
            return on_exception(true, &e);
        }
        catch(...)
        {
            m_count = index;
            return on_exception(true, nullptr);
        }
#endif
    }
    return true;
}

bool basic_thread_worker::main_loop()
{
    m_objects = static_cast<waitable_object**>(stack_alloc(m_count * sizeof(waitable_object*)));

    if(!safe_acquire_events()) return false;

    const auto timeout = get_wait_timeout();

    while(running())
    {
        if(m_updating) return true;

        const wait_result_t wait_result = m_thread_waiter.wait(timeout, m_count, m_objects);

        Check_ValidState(wait_result != wait_result_error, false);

        if(wait_result == wait_result_timeout)
        {
            safe_notify_timeout();
        }
        else
        {
            for(size_t index = OWN_EVENTS_TO_WAIT; index < m_count; ++index)
            {
                if(waitable_object_wait_is_completed(wait_result, index))
                {
                    safe_notify_complete(index - OWN_EVENTS_TO_WAIT);
                }
            }
            if(waitable_object_wait_is_completed(wait_result, OWN_EVENT_HAS_ASYNC_ACTION))
            {
                dequeue_actions(false);
            }
            if(waitable_object_wait_is_completed(wait_result, OWN_EVENT_HAS_SYNC_ACTION))
            {
                safe_do_action(m_func.id, m_func.param, m_func.result, true);
            }
        }
    }
    return false;
}

void basic_thread_worker::dequeue_actions(bool all)
{
    do
    {
        auto element = m_actions_queue.try_start_pop();

        if(!element.pop_in_progress()) break;

        queued_action& action = *element;

        safe_do_action(action.id, action.param, nullptr, false);

        if(action.deleter)
        {
            action.deleter(action.param);
        }
    }
    while(all);
}

void basic_thread_worker::safe_do_action(action_id_t id, void* param, void* result, bool cross_thread_caller)
{
    Assert_Check(!cross_thread_caller || (m_reentrancy.count == 0));

    ++m_reentrancy.count;

    if(cross_thread_caller)
    {
        m_reentrancy.result = &result;
    }
    switch(id)
    {
        case ACTION_UPDATE: m_updating = true ; break;
        case ACTION_QUIT  : m_running  = false; break;
        default:
#ifndef __cpp_exceptions
            on_action(id, param, result);
#else
            try
            {
                on_action(id, param, result);
            }
            catch(const std::exception& e)
            {
                m_running = on_exception(true, &e);
            }
            catch(...)
            {
                m_running = on_exception(true, nullptr);
            }
#endif
            break;
    }
    if(cross_thread_caller)
    {
        reply_action();
    }

    --m_reentrancy.count;
}

void basic_thread_worker::safe_notify_timeout()
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
        m_running = on_exception(true, &e);
    }
    catch(...)
    {
        m_running = on_exception(true, nullptr);
    }
#endif
}

void basic_thread_worker::safe_notify_complete(size_t index)
{
#ifndef __cpp_exceptions
    on_wait_success(index);
#else
    try
    {
        on_wait_success(index);
    }
    catch(const std::exception& e)
    {
        m_running = on_exception(true, &e);
    }
    catch(...)
    {
        m_running = on_exception(true, nullptr);
    }
#endif
}

}
