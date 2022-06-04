#include <gkr/thread_worker_base.h>

#include <gkr/sys/stack_alloc.h>
#include <gkr/sys/thread_name.h>

namespace gkr
{

thread_worker_base::thread_worker_base()
{
}

thread_worker_base::~thread_worker_base()
{
    Assert_CheckMsg(
        !joinable(),
        "The running thread must be joined before being destructed or call Join method in leaf child class destructor"
        );
}

bool thread_worker_base::run()
{
    std::lock_guard<decltype(m_mutex)> lock(m_mutex);

    Check_ValidState(!m_thread.joinable(), false);

    m_thread = std::thread([this] () { thread_proc(); });

    m_work_event.fire();

    m_waiter.wait(timeout_infinite, m_done_event);

    return running();
}

bool thread_worker_base::quit()
{
    return enqueue_action(ACTION_QUIT);
}

bool thread_worker_base::join(bool send_quit_signal)
{
    if(!m_thread.joinable()) return false;

    Check_ValidState(!in_worker_thread(), false);

    if(send_quit_signal) quit();

    m_thread.join();

    return true;
}

void thread_worker_base::update_wait()
{
    m_updating = true;

    if(in_worker_thread()) return;

    enqueue_action(ACTION_UPDATE);
}

bool thread_worker_base::enqueue_action(action_id_t action, void* param, action_param_deleter_t deleter)
{
    std::lock_guard<decltype(m_sync_queue_mutex)> lock(m_sync_queue_mutex);

    Check_ValidState(running(), false);

    m_sync_queue_items.push({action, param, deleter});

    m_wake_event.fire();

    return true;
}

bool thread_worker_base::execute_action(action_id_t action, void* param, void* result)
{
    bool succeeded;

    if(in_worker_thread())
    {
        succeeded = do_action(action, param, result, false);
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        Check_ValidState(running(), false);

        m_func = func_t{action, param, result};

        m_work_event.fire();

        succeeded = (wait_result_error != m_waiter.wait(timeout_infinite, m_done_event));
    }

    return succeeded;
}

bool thread_worker_base::can_reply()
{
    Check_ValidState(in_worker_thread(), false);
    Check_ValidState(m_reentrancy.count > 0, false);

    return ((m_reentrancy.count == 1) && (m_reentrancy.result != nullptr));
}

void thread_worker_base::reply_action()
{
    if(can_reply())
    {
        m_reentrancy.result = nullptr;

        m_done_event.fire();
    }
}

void thread_worker_base::thread_proc()
{
    m_waiter.wait(timeout_infinite, m_work_event);

    m_reentrancy = {0, nullptr};

    const char* name = get_name();

    sys::set_current_thread_name(name);

    m_running = start();
    m_done_event.fire();

    do
    {
        m_updating = false;
        m_count    = OWN_EVENTS_TO_WAIT + get_wait_objects_count();
    }
    while(main_loop());

    dequeue_actions();

    finish();
}

bool thread_worker_base::acquire_events()
{
    m_objects[OWN_EVENT_HAS_ASYNC_ACTION] = &m_wake_event;
    m_objects[OWN_EVENT_HAS_SYNC_ACTION ] = &m_work_event;

    for(size_t index = OWN_EVENTS_TO_WAIT; index < m_count; ++index)
    {
        m_objects[index] = get_wait_object(index - OWN_EVENTS_TO_WAIT);

        Check_NotNullPtr(m_objects[index], false);
    }
    return true;
}

bool thread_worker_base::main_loop()
{
    m_objects = static_cast<waitable_object**>(stack_alloc(m_count * sizeof(waitable_object*)));

    if(!acquire_events()) return false;

    const auto timeout = get_wait_timeout();

    while(running())
    {
        if(m_updating) return true;

        const wait_result_t wait_result = m_waiter.wait(timeout, m_count, m_objects);

        Check_ValidState(wait_result != wait_result_error, false);

        if(wait_result == wait_result_timeout)
        {
            on_wait_timeout();
        }
        else
        {
            for(size_t index = OWN_EVENTS_TO_WAIT; index < m_count; ++index)
            {
                if(wait_object_is_signalled(wait_result, index))
                {
                    on_wait_success(index - OWN_EVENTS_TO_WAIT);
                }
            }
            if(wait_object_is_signalled(wait_result, OWN_EVENT_HAS_ASYNC_ACTION))
            {
                dequeue_actions();
            }
            if(wait_object_is_signalled(wait_result, OWN_EVENT_HAS_SYNC_ACTION))
            {
                do_action(m_func.action, m_func.param, m_func.result, true);
            }
        }
    }
    return false;
}

void thread_worker_base::dequeue_actions()
{
    for(item_t item; ; )
    {
        {
            std::lock_guard<decltype(m_sync_queue_mutex)> lock(m_sync_queue_mutex);
            if(m_sync_queue_items.empty()) return;
            item = m_sync_queue_items.front();
            m_sync_queue_items.pop();
        }

        do_action(item.action, item.param, nullptr, false);

        if(item.deleter)
        {
            item.deleter(item.param);
        }
    }
}

bool thread_worker_base::do_action(action_id_t action, void* param, void* result, bool cross_thread_caller)
{
    Assert_Check(!cross_thread_caller || (m_reentrancy.count == 0));

    ++m_reentrancy.count;

    if(cross_thread_caller)
    {
        m_reentrancy.result = &result;
    }
    switch(action)
    {
        case ACTION_UPDATE: m_running = true ; break;
        case ACTION_QUIT  : m_running = false; break;
        default:
            on_action(action, param, result);
            break;
    }
    if(cross_thread_caller)
    {
        m_reentrancy.result = nullptr;

        m_done_event.fire();
    }

    --m_reentrancy.count;

    return true;
}

}
