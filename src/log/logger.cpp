#include <gkr/log/logger.h>

#include <gkr/log/logging.h>
#include <gkr/log/consumer.h>

#include <gkr/sys/thread_name.h>
#include <gkr/misc/union_cast.h>
#include <gkr/misc/stamp.h>

#include <cstring>

namespace gkr
{
namespace log
{

logger::logger()
{
    m_log_queue.get_synchronization().set_producer_waiter(m_producer_waiter);
    m_log_queue.get_synchronization().set_consumer_waiter(get_waiter());
}

logger::~logger() noexcept(DIAG_NOEXCEPT)
{
    join(true);
}

const char* logger::get_name() noexcept
{
    return "gkr-logger";
}

std::chrono::nanoseconds logger::get_wait_timeout() noexcept
{
    return timeout_infinite;
}

size_t logger::get_wait_objects_count() noexcept
{
    return 1;
}

waitable_object* logger::get_wait_object(size_t index)
{
    Check_ValidState(index == 0, nullptr);

    return m_log_queue.get_synchronization().queue_non_empty_waitable_object();
}

bool logger::start()
{
    m_log_queue.threading.any_thread_can_be_producer();
    m_log_queue.threading.set_this_thread_as_exclusive_consumer();
    return true;
}

void logger::finish()
{
    del_all_consumers();
}

void logger::on_action(action_id_t action, void* param, void* result)
{
    switch(action)
    {
        case ACTION_CHANGE_LOG_QUEUE : call_action_method(&logger::change_log_queue , param, result); break;
        case ACTION_SET_SEVERITIES   : call_action_method(&logger::set_severities   , param); break;
        case ACTION_SET_FACILITIES   : call_action_method(&logger::set_facilities   , param); break;
        case ACTION_SET_SEVERITY     : call_action_method(&logger::set_severity     , param); break;
        case ACTION_SET_FACILITY     : call_action_method(&logger::set_facility     , param); break;
        case ACTION_ADD_CONSUMER     : call_action_method(&logger::add_consumer     , param, result); break;
        case ACTION_DEL_CONSUMER     : call_action_method(&logger::del_consumer     , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param); break;
        case ACTION_REGISTER_THREAD  : call_action_method(&logger::register_thread  , param); break;
        case ACTION_SYNC_LOG_MESSAGE : call_action_method(&logger::sync_log_message , param); break;
    }
}

void logger::on_wait_timeout()
{
    Check_Failure();
}

void logger::on_wait_success(size_t index)
{
    Check_ValidState(index == 0, );

    auto element = m_log_queue.start_pop<message_data>(timeout_infinite);

    Check_ValidState(element.pop_in_progress(), );

    message_data& msg = *element;

    prepare_message(msg);
    consume_message(msg);
}

bool logger::on_exception(bool, const std::exception*) noexcept
{
    return true;
}

bool logger::change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars)
{
    if(running() && !in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_CHANGE_LOG_QUEUE, max_queue_entries, max_message_chars);
    }
    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    const std::size_t queue_capacity   = (max_queue_entries == std::size_t(-1))
        ? lockfree_queue_t::npos
        : max_queue_entries
        ;
    const std::size_t queue_entry_size = (max_message_chars == std::size_t(-1))
        ? lockfree_queue_t::npos
        : max_message_chars + sizeof(message)
        ;
    if(!running())
    {
        m_log_queue.reset(queue_capacity, queue_entry_size, alignof(message));
        return true;
    }
    else
    {
    //  m_log_queue.resize(queue_capacity, queue_entry_size, alignof(message));
        Check_Failure(false);
    }
}

void logger::set_severities(bool clear_existing, const name_id_pair* severities)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SET_SEVERITIES, clear_existing, severities);
    }
    if(clear_existing)
    {
        m_severities.clear();
    }
    if(severities != nullptr) for( ; severities->name != nullptr; ++severities)
    {
        m_severities[severities->id] = severities->name;
    }
}

void logger::set_facilities(bool clear_existing, const name_id_pair* facilities)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SET_FACILITIES, clear_existing, facilities);
    }
    if(clear_existing)
    {
        m_facilities.clear();
    }
    if(facilities != nullptr) for( ; facilities->name != nullptr; ++facilities)
    {
        m_facilities[facilities->id] = facilities->name;
    }
}

void logger::set_severity(const name_id_pair& severity)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SET_SEVERITY, severity);
    }
    if(severity.name == nullptr)
    {
        m_severities.erase(severity.id);
    }
    else
    {
        m_severities[severity.id] = severity.name;
    }
}

void logger::set_facility(const name_id_pair& facility)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SET_FACILITY, facility);
    }
    if(facility.name == nullptr)
    {
        m_facilities.erase(facility.id);
    }
    else
    {
        m_facilities[facility.id] = facility.name;
    }
}

bool logger::add_consumer(std::shared_ptr<consumer> consumer)
{
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_ADD_CONSUMER, consumer);
    }
    Check_Arg_NotNull(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(*it == consumer)
        {
            Check_Arg_Invalid(consumer, false);
        }
    }
    if(!consumer->init_logging())
    {
        Check_Failure(false);
    }
    m_consumers.push_back(consumer);

    return true;
}

bool logger::del_consumer(std::shared_ptr<consumer> consumer)
{
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_DEL_CONSUMER, consumer);
    }
    Check_Arg_NotNull(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(*it == consumer)
        {
            m_consumers.erase(it);
            consumer->done_logging();
            return true;
        }
    }
    Check_Arg_Invalid(consumer, false);
}

void logger::del_all_consumers()
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_DEL_ALL_CONSUMERS);
    }
    for(auto it = m_consumers.rbegin(); it != m_consumers.rend(); ++it)
    {
        (*it)->done_logging();
        (*it) .reset();
    }
    m_consumers.clear();
}

bool logger::log_message(bool wait, int severity, int facility, const char* format, std::va_list args)
{
    Check_Arg_NotNull(format, false);

    check_thread_registered();

    const std::size_t size = m_log_queue.element_size();

    Assert_Check(size > sizeof(message));

    const std::size_t cch = size - sizeof(message);

    auto element = m_log_queue.start_push<message_data>(timeout_infinite);

    Check_ValidState(element.push_in_progress(), false);

    message_data& msg = *element;

    if(!compose_message(msg, cch, severity, facility, format, args))
    {
        element.cancel_push();
        return false;
    }
    if(wait)
    {
        sync_log_message(msg);
        element.cancel_push();
    }
    return true;
}

void logger::register_thread(std::thread::id tid, const char* name)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_REGISTER_THREAD, tid, name);
    }
    thread_name_t& thread_name = m_thread_names[tid];

    if(name != nullptr)
    {
        std::strncpy(thread_name.name, name, max_name_cch);

        thread_name.name[max_name_cch-1] = 0;
    }
}

void logger::sync_log_message(message_data& msg)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SYNC_LOG_MESSAGE, msg);
    }
    prepare_message(msg);
    consume_message(msg);
}

void logger::check_thread_registered()
{
    static thread_local bool thread_registered = false;

    if(thread_registered) return;

    thread_registered = true;

    char name[sys::max_thread_name_cch];

    if(!sys::get_current_thread_name(name)) return;

    register_thread(std::this_thread::get_id(), name);
}

bool logger::compose_message(message_data& msg, std::size_t cch, int severity, int facility, const char* format, std::va_list args)
{
    msg.tid      = misc::union_cast<std::int64_t>(std::this_thread::get_id());
    msg.stamp    = calc_stamp();
    msg.severity = std::uint16_t(severity);
    msg.facility = std::uint16_t(facility);

    if(args == nullptr)
    {
        std::strncpy(msg.buffer, format, cch);

        msg.buffer[cch - 1] = 0;

        const std::size_t len = std::strlen(msg.buffer);

        msg.mesageLen = std::uint16_t(len);
    }
    else
    {
        const int len = std::vsnprintf(msg.buffer, cch, format, args);

        Check_ValidState(len >= 0, false);

        msg.mesageLen = std::uint16_t(len);
    }

    return true;
}

void logger::prepare_message(message_data& msg)
{
    auto tid = misc::union_cast<std::thread::id>(msg.tid);

    auto itSeverity = m_severities.find(msg.severity);
    auto itFacility = m_facilities.find(msg.facility);

    msg.threadName   = m_thread_names[tid].name;
    msg.messageText  = msg.buffer;
    msg.severityName = (itSeverity == m_severities.end()) ? nullptr : itSeverity->second;
    msg.facilityName = (itFacility == m_facilities.end()) ? nullptr : itFacility->second;
}

void logger::consume_message(const message_data& msg)
{
#ifdef __cpp_exceptions
    std::size_t count = m_consumers.size();
    std::size_t index = 0;

    do
    {
        try
        {
            for( ; index < count; ++index)
            {
                m_consumers[index]->consume_log_message(msg);
            }
            return;
        }
        catch(...)
        {
            std::shared_ptr<consumer> consumer_that_throws = m_consumers[index];
            del_consumer(consumer_that_throws);
            --count;

            Check_Recovery("Log message consumer exception caught - consumer removed");
        }
    }
    while(index < count);
#else
    for(auto& consumer : m_consumers)
    {
        consumer->consume_log_message(msg);
    }
#endif
}

}
}
