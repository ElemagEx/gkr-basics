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
    m_log_queue.get_synchronization().set_waiter(get_waiter());
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

    return m_log_queue.queue_non_empty_waitable_object();
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
        case ACTION_SET_SEVERITY     : call_action_method(&logger::set_severity     , param, result); break;
        case ACTION_SET_FACILITY     : call_action_method(&logger::set_facility     , param, result); break;
        case ACTION_ADD_CONSUMER     : call_action_method(&logger::add_consumer     , param, result); break;
        case ACTION_DEL_CONSUMER     : call_action_method(&logger::del_consumer     , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param); break;
        case ACTION_REGISTER_THREAD  : call_action_method(&logger::register_thread  , param); break;
        case ACTION_SYNC_LOG_MESSAGE : call_action_method(&logger::sync_log_message , param); break;
    }
}

void logger::on_wait_timeout()
{
}

void logger::on_wait_success(size_t index)
{
    Check_ValidState(index == 0, );

    auto element = m_log_queue.start_pop<msg_entry>(timeout_infinite);

    Check_ValidState(element.pop_in_progress(), );

    msg_entry& entry = *element;

    consume_msg_entry(entry);
}

bool logger::on_exception(bool, const std::exception*) noexcept
{
    return true;
}

bool logger::change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars)
{
    Check_ValidArg(max_queue_entries > 0, false);
    Check_ValidArg(max_message_chars > 0, false);

    const std::size_t queue_entry_size = (max_message_chars == std::size_t(-1))
        ? max_message_chars
        : max_message_chars + sizeof(entry_info)
        ;
    if(!running())
    {
        m_log_queue.reset(max_queue_entries, queue_entry_size, alignof(entry_info));
        return true;
    }
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_CHANGE_LOG_QUEUE, max_queue_entries, max_message_chars);
    }
    Check_Failure(false);
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

bool logger::set_severity(const name_id_pair& severity)
{
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_SEVERITY, severity);
    }
    Check_NotNullArg(severity.name, false);

    m_severities[severity.id] = severity.name;

    return true;
}

bool logger::set_facility(const name_id_pair& facility)
{
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_FACILITY, facility);
    }
    Check_NotNullArg(facility.name, false);

    m_facilities[facility.id] = facility.name;

    return true;
}

bool logger::add_consumer(std::shared_ptr<consumer> consumer)
{
    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_ADD_CONSUMER, consumer);
    }
    Check_NotNullArg(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(*it == consumer)
        {
            Check_InvalidArg(consumer, false);
        }
    }
    if(!Verify_BoolRes(consumer->init_logging()))
    {
        return false;
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
    Check_NotNullArg(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(*it == consumer)
        {
            m_consumers.erase(it);
            consumer->done_logging();
            return true;
        }
    }
    Check_InvalidArg(consumer, false);
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

bool logger::log_message(bool wait, int severity, int facility, const char* message, va_list args)
{
    Check_NotNullArg(message, false);

    check_thread_registered();

    const std::size_t cch = m_log_queue.element_size() - sizeof(entry_info);

    auto element = m_log_queue.start_push<msg_entry>(timeout_infinite);

    Check_ValidState(element.push_in_progress(), false);

    msg_entry& entry = *element;

    if(!prepare_msg_entry(entry, cch, severity, facility, message, args))
    {
        element.cancel_push();
        return false;
    }
    if(wait)
    {
        sync_log_message(entry);
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
        std::strncpy(thread_name.name, name, max_name_cch - 1);
    }
}

void logger::sync_log_message(const msg_entry& entry)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SYNC_LOG_MESSAGE, entry);
    }
    consume_msg_entry(entry);
}

void logger::check_thread_registered()
{
    static thread_local bool thread_registered = false;

    if(thread_registered) return;

    thread_registered = true;

    char name[sys::max_thread_name_cch];

    if(!Verify_BoolRes(sys::get_current_thread_name(name))) return;

    register_thread(std::this_thread::get_id(), name);
}

bool logger::prepare_msg_entry(msg_entry& entry, std::size_t cch, int severity, int facility, const char* message, va_list args)
{
    if(args == nullptr)
    {
        const std::size_t len = std::strlen(std::strncpy(entry.message, message, cch));

        entry.mesageLen = std::uint16_t(len);
    }
    else
    {
        const int len = std::vsnprintf(entry.message, cch, message, args);

        Check_ValidState(len >= 0, false);

        entry.mesageLen = std::uint16_t(len);
    }

    auto tid = std::this_thread::get_id();

    entry.tid      = misc::union_cast<std::int64_t>(tid);
    entry.stamp    = misc::get_stamp();
    entry.severity = std::uint16_t(severity);
    entry.facility = std::uint16_t(facility);

    entry.threadName  = m_thread_names[tid].name;
    entry.messageText = entry.message;

    auto itSeverity = m_severities.find(entry.severity);
    auto itFacility = m_facilities.find(entry.facility);

    entry.severityName = (itSeverity == m_severities.end()) ? nullptr : itSeverity->second;
    entry.facilityName = (itFacility == m_facilities.end()) ? nullptr : itFacility->second;

    return true;
}

void logger::consume_msg_entry(const msg_entry& entry)
{
    for(auto& consumer : m_consumers)
    {
        consumer->consume_log_message(entry);
    }
}

}
}
