#include <gkr/log/logger.h>

#include <gkr/log/logging.h>
#include <gkr/log/message.h>
#include <gkr/log/consumer.h>
#include <gkr/stamp.h>

#include <gkr/misc/union_cast.h>

#include <cstring>

namespace gkr
{
namespace log
{

inline bool are_equals(const logger::functions_t& functions1, const logger::functions_t& functions2)
{
    return (
        (functions1.init_logging == functions2.init_logging) &&
        (functions1.done_logging == functions2.done_logging) &&
        (functions1. filter_log_message == functions2. filter_log_message) &&
        (functions1.consume_log_message == functions2.consume_log_message) );
}

logger::logger() : m_msg_waiter(gkr::events_waiter::Flag_ForbidMultipleEventsBind)
{
    m_log_queue.bind_with_producer_waiter(m_msg_waiter);

    check_args_order();
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
    return std::chrono::nanoseconds::max();
}

void logger::bind_events(events_waiter& waiter) noexcept(DIAG_NOEXCEPT)
{
    m_log_queue.bind_with_consumer_waiter(waiter);
}

bool logger::on_start()
{
    m_log_queue.threading.any_thread_can_be_producer();
    m_log_queue.threading.set_this_thread_as_exclusive_consumer();
    return true;
}

void logger::on_finish()
{
    while(process_next_message());

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
        case ACTION_ADD_FUNCTIONS    : call_action_method(&logger::add_functions    , param, result); break;
        case ACTION_DEL_FUNCTIONS    : call_action_method(&logger::del_functions    , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param); break;
        case ACTION_SET_THREAD_NAME  : call_action_method(&logger::set_thread_name  , param); break;
        case ACTION_SYNC_LOG_MESSAGE : call_action_method(&logger::sync_log_message , param); break;
    }
}

void logger::on_wait_timeout()
{
    Check_Failure();
}

void logger::on_wait_success(wait_result_t wait_result)
{
    Check_ValidState(m_log_queue.consumer_event_is_signaled(wait_result), );

    [[maybe_unused]] bool processed = process_next_message();

    Check_ValidState(processed, );
}

bool logger::on_exception(except_method_t method, const std::exception* e) noexcept
{
    return true;
}

bool logger::change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_CHANGE_LOG_QUEUE, max_queue_entries, max_message_chars);
    }

    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    if(m_log_queue.capacity() > 0)
    {
        bool result = true;

        if(max_queue_entries != std::size_t(-1)) result = m_log_queue.resize             (max_queue_entries                      ) && result;
        if(max_message_chars != std::size_t(-1)) result = m_log_queue.change_element_size(max_message_chars + 1 + sizeof(message)) && result;

        return result;
    }
    else
    {
        m_log_queue.reset(max_queue_entries, max_message_chars + 1 + sizeof(message), alignof(message));

        Check_ValidState(m_log_queue.capacity() > 0, false);

        Check_ValidState(m_log_queue.element_size() > sizeof(message), false);

        return true;
    }
}

void logger::set_severities(bool clear_existing, const name_id_pair* severities)
{
    Check_ValidState(running(), );

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
    Check_ValidState(running(), );

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
    Check_ValidState(running(), );

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
    Check_ValidState(running(), );

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

bool logger::add_functions(functions_t* functions, void* param)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_ADD_FUNCTIONS, functions, param);
    }
    Check_Arg_NotNull(functions, false);
    Check_Arg_NotNull(functions->init_logging, false);
    Check_Arg_NotNull(functions->done_logging, false);
    Check_Arg_NotNull(functions-> filter_log_message, false);
    Check_Arg_NotNull(functions->consume_log_message, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(are_equals(it->functions, *functions) && (it->param == param))
        {
            Check_Arg_Invalid(consumer, false);
        }
    }
    if(!(*functions->init_logging)(param))
    {
        (*functions->done_logging)(param);
        Check_Failure(false);
    }
    m_consumers.push_back(consumer_data_t{nullptr, *functions, param});

    return true;
}

bool logger::del_functions(functions_t* functions, void* param)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_DEL_FUNCTIONS, functions, param);
    }
    Check_Arg_NotNull(functions, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(are_equals(it->functions, *functions) && (it->param == param))
        {
            m_consumers.erase(it);
            (*functions->done_logging)(param);
            return true;
        }
    }
    Check_Arg_Invalid(functions && param, false);
}

bool logger::add_consumer(consumer_ptr_t consumer)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_ADD_CONSUMER, consumer);
    }
    Check_Arg_NotNull(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(it->consumer == consumer)
        {
            Check_Arg_Invalid(consumer, false);
        }
    }
    if(!init_consumer(*consumer))
    {
        done_consumer(*consumer);
        Check_Failure(false);
    }
    m_consumers.push_back(consumer_data_t{consumer});

    return true;
}

bool logger::del_consumer(consumer_ptr_t consumer)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_DEL_CONSUMER, consumer);
    }
    Check_Arg_NotNull(consumer, false);

    for(auto it = m_consumers.begin(); it != m_consumers.end(); ++it)
    {
        if(it->consumer == consumer)
        {
            m_consumers.erase(it);
            done_consumer(*consumer);
            return true;
        }
    }
    Check_Arg_Invalid(consumer, false);
}

void logger::del_all_consumers()
{
    Check_ValidState(joinable(), );

    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_DEL_ALL_CONSUMERS);
    }
    while(!m_consumers.empty())
    {
        consumer_ptr_t consumer = m_consumers.back().consumer;

        m_consumers.pop_back();

        done_consumer(*consumer);
    }
}

void logger::set_thread_name(const char* name, tid_t tid)
{
    Check_ValidState(running(), );

    if(!in_worker_thread())
    {
        if(tid == 0)
        {
            tid = misc::union_cast<tid_t>(std::this_thread::get_id());
        }
        return execute_action_method<void>(ACTION_SET_THREAD_NAME, name, tid);
    }
    if(name == nullptr)
    {
        m_thread_ids.erase(tid);
    }
    else
    {
        m_thread_ids[tid] = name;
    }
}

bool logger::log_message(bool wait, id_t severity, id_t facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);

    Check_ValidState(running(), false);

    const bool logging_inside_logger = in_worker_thread();

    for( ; ; )
    {
        auto element = logging_inside_logger
            ? m_log_queue.try_start_push()
            : m_log_queue.    start_push();

        if(!element.push_in_progress())
        {
            if(logging_inside_logger)
            {
                [[maybe_unused]] bool processed = process_next_message();
                Check_ValidState(processed, false);
                continue;
            }
            Check_Failure(false);
        }

       const std::size_t size = element.get_element_size();

       Assert_Check(size > sizeof(message));

        const std::size_t cch = size - sizeof(message);

        message_data& msg = element.value<message_data>();

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
}

void logger::sync_log_message(message_data& msg)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SYNC_LOG_MESSAGE, msg);
    }
    process_message(msg);
}

bool logger::compose_message(message_data& msg, std::size_t cch, id_t severity, id_t facility, const char* format, va_list args)
{
    msg.tid      = misc::union_cast<long long>(std::this_thread::get_id());
    msg.stamp    = stamp_now();
    msg.severity = severity;
    msg.facility = facility;

    if(args == nullptr)
    {
        std::strncpy(msg.buffer, format, cch);

        msg.buffer[cch - 1] = 0;

        msg.messageLen = unsigned(std::strlen(msg.buffer));
    }
    else
    {
        const int len = std::vsnprintf(msg.buffer, cch, format, args);

        Check_ValidState(len >= 0, false);

        msg.messageLen = unsigned(len);
    }
    return true;
}

void logger::process_message(message_data& msg)
{
    prepare_message(msg);
    consume_message(msg);
}

void logger::prepare_message(message_data& msg)
{
    msg.messageText = msg.buffer;

    auto itThreadId = m_thread_ids.find(msg.tid);
    auto itSeverity = m_severities.find(msg.severity);
    auto itFacility = m_facilities.find(msg.facility);

    static constexpr char s_empty_name[1] = {0};

    msg.threadName   = (itThreadId == m_thread_ids.end()) ? s_empty_name : itThreadId->second;
    msg.severityName = (itSeverity == m_severities.end()) ? s_empty_name : itSeverity->second;
    msg.facilityName = (itFacility == m_facilities.end()) ? s_empty_name : itFacility->second;
}

void logger::consume_message(const message_data& msg)
{
#ifndef __cpp_exceptions
    for(auto& data : m_consumers)
    {
        if(data.reentry_guard) continue;

        data.reentry_guard = true;
        if(!data.consumer->filter_log_message(msg))
        {
            data.consumer->consume_log_message(msg);
        }
        data.reentry_guard = false;
    }
#else
    std::size_t count = m_consumers.size();
    std::size_t index = 0;

    do
    {
        try
        {
            for( ; index < count; ++index)
            {
                consumer_data_t& data = m_consumers[index];

                if(data.reentry_guard) continue;

                data.reentry_guard = true;
                if(data.consumer != nullptr)
                {
                    if(!data.consumer->filter_log_message(msg))
                    {
                        data.consumer->consume_log_message(msg);
                    }
                }
                else
                {
                    if(!(*data.functions.filter_log_message)(data.param, &msg))
                    {
                        (*data.functions.consume_log_message)(data.param, &msg);
                    }
                }
                data.reentry_guard = false;
            }
            return;
        }
        catch(...)
        {
            consumer_ptr_t consumer_that_throws = m_consumers[index].consumer;
            del_consumer(consumer_that_throws);
            --count;

            Check_Recovery("Log message consumer exception caught - consumer removed");
        }
    }
    while(index < count);
#endif
}

bool logger::process_next_message()
{
    auto element = m_log_queue.start_pop();

    if(!element.pop_in_progress()) return false;

    message_data& msg = element.value<message_data>();

    process_message(msg);

    return true;
}

bool logger::init_consumer(consumer& consumer)
{
#ifndef __cpp_exceptions
    return consumer.init_logging();
#else
    try
    {
        if(consumer.init_logging())
        {
            return true;
        }
    }
    catch(...)
    {
        Check_Recovery("Init consumer exception caught");
    }
    return false;
#endif
}

void logger::done_consumer(consumer& consumer)
{
#ifndef __cpp_exceptions
    consumer.done_logging();
#else
    try
    {
        consumer.done_logging();
    }
    catch(...)
    {
        Check_Recovery("Done consumer exception caught");
    }
#endif
}

}
}
