#include <gkr/log/logger.h>

#include <gkr/log/logging.h>
#include <gkr/log/consumer.h>

namespace gkr
{
namespace log
{

logger::logger()
{
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

    return &m_has_entries_event;
}

bool logger::start()
{
    m_log_queue.reset(32);
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
        case ACTION_SET_SEVERITIES   : call_action_method(&logger::set_severities   , param); break;
        case ACTION_SET_FACILITIES   : call_action_method(&logger::set_facilities   , param); break;
        case ACTION_SET_SEVERITY     : call_action_method(&logger::set_severity     , param, result); break;
        case ACTION_SET_FACILITY     : call_action_method(&logger::set_facility     , param, result); break;
        case ACTION_ADD_CONSUMER     : call_action_method(&logger::add_consumer     , param, result); break;
        case ACTION_DEL_CONSUMER     : call_action_method(&logger::del_consumer     , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param); break;
    }
}

void logger::on_wait_timeout()
{
}

void logger::on_wait_success(size_t index)
{
}

bool logger::on_exception(bool can_continue, const std::exception* e) noexcept
{
    return true;
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
    for(auto it = m_consumers.rbegin(); it != m_consumers.rend(); ++it)
    {
        (*it)->done_logging();
        (*it) .reset();
    }
    m_consumers.clear();
}

}
}
