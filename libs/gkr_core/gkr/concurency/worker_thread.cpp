#include <gkr/defs.hpp>
#include <gkr/concurency/worker_thread.hpp>

#include <gkr/stack_alloc.hpp>
#include <gkr/sys/thread.hpp>

namespace gkr
{

worker_thread::worker_thread(std::size_t initial_actions_queue_capacity, std::size_t initial_actions_queue_element_size)
{
    m_actions_queue.reset(initial_actions_queue_capacity, initial_actions_queue_element_size);
}

worker_thread::~worker_thread()
{
    Assert_CheckMsg(
        !joinable(),
        "The running thread must be joined before being destructed or call Join method in leaf child class destructor"
        );
}

bool worker_thread::run()
{
    Check_ValidState(m_actions_queue.element_size() >= sizeof(actions_queue_element_header_t), false);

    std::lock_guard<std::mutex> lock(m_mutex);

    if(running())
    {
        errno = EEXIST;
        return false;
    }
    Check_ValidState(!m_thread.joinable(), false);

    m_thread = std::thread([this] () { thread_proc(); });

    m_work_event.fire();
    m_done_event.wait();

    if(running()) return true;
    errno = ECANCELED;
    return false;
}

bool worker_thread::join(bool send_quit_signal)
{
    if(!m_thread.joinable()) return false;

    Check_ValidState(!in_worker_thread(), false);

    if(send_quit_signal) quit();

    m_thread.join();

    return true;
}

bool worker_thread::quit()
{
    Check_ValidState(joinable(), false);

    if(in_worker_thread())
    {
        m_running = false;
        return true;
    }
    else
    {
        return running() ? enqueue_action(ACTION_QUIT) : true;
    }
}

bool worker_thread::update_wait()
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

bool worker_thread::resize_actions_queue(size_t capacity)
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

void worker_thread::forward_action(action_id_t id, void* param, void* result)
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

void worker_thread::thread_proc()
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
    m_actions_queue.threading.any_thread_can_be_producer();
    m_actions_queue.threading.set_current_thread_as_exclusive_consumer();

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
}

bool worker_thread::main_loop()
{
    const long long timeout_ns = get_wait_timeout_ns();

    const std::size_t count = get_waitable_objects_count();

    Check_ValidState(count < WAIT_MAX_OBJECTS, false);

    constexpr std::size_t SELF_OBJECTS_TO_WAIT = 2;

    GKR_STACK_ARRAY(waitable_object*, objects, count + SELF_OBJECTS_TO_WAIT);

    objects[0] = &m_work_event;
    objects[1] = &m_actions_queue.get_consumer_waitable_object();

    for(std::size_t index = 0; index < count; ++index)
    {
        objects[index + SELF_OBJECTS_TO_WAIT] = &get_waitable_object(index + SELF_OBJECTS_TO_WAIT);
    }

    while(running())
    {
        if(m_updating) return true;

        const wait_result_t wait_result = waitable_object::wait_many(timeout_ns, objects, count + SELF_OBJECTS_TO_WAIT);

        if(wait_result == WAIT_RESULT_ERROR) return false;

        if(wait_result == WAIT_RESULT_TIMEOUT)
        {
            safe_notify_wait_timeout();
        }
        else
        {
            constexpr wait_result_t OTHER_EVENTS_MASK = ~((wait_result_t(1) << SELF_OBJECTS_TO_WAIT) - 1);

            if((wait_result & OTHER_EVENTS_MASK) != 0)
            {
                safe_notify_wait_success(wait_result >> SELF_OBJECTS_TO_WAIT, objects + SELF_OBJECTS_TO_WAIT, count);
            }
            if(waitable_object::is_signaled(wait_result, 0))
            {
                safe_do_cross_thread_action();
            }
            if(waitable_object::is_signaled(wait_result, 1))
            {
                safe_dequeue_actions(false);
            }
        }
    }
    return false;
}

void worker_thread::safe_dequeue_actions(bool all)
{
    do
    {
        auto element = m_actions_queue.try_start_pop();

        if(!element.pop_in_progress()) break;

        actions_queue_element_header_t& header = element.as<actions_queue_element_header_t>();

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

void worker_thread::safe_do_cross_thread_action()
{
    m_reply = &m_func.result;

#ifndef __cpp_exceptions
    on_cross_action(m_func.id, m_func.param, m_func.result);
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

void worker_thread::safe_notify_wait_success(wait_result_t wait_result, waitable_object**, std::size_t count) noexcept
{
#ifndef __cpp_exceptions
    for(std::size_t index = 0; index < count; ++index)
    {
        if(waitable_object_is_signaled(wait_result, index))
        {
            on_wait_success(index);
        }
    }
#else
    try
    {
        for(std::size_t index = 0; index < count; ++index)
        {
            if(waitable_object::is_signaled(wait_result, index))
            {
                on_wait_success(index);
            }
        }
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
