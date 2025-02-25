#include <gkr/defs.hpp>
#include "logger.hxx"

#include <gkr/stamp.hpp>

#include <gkr/misc/union_cast.hpp>

#include <cstring>

namespace
{
inline std::size_t calc_queue_element_size(std::size_t extra_data_size, std::size_t max_message_chars)
{
    constexpr std::size_t granularity = 256;

    const std::size_t queue_element_size = (extra_data_size + max_message_chars);

    return ((queue_element_size % granularity) == 0)
        ? (queue_element_size)
        : (queue_element_size / granularity) * granularity + granularity;
}
}

namespace gkr
{
namespace log
{

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
    process_all_messages();

    for(auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        del_all_consumers(&*it);
    }
    m_instances.clear();

    del_all_consumers(nullptr);
}

void logger::on_cross_action(action_id_t action, void* param, void* result)
{
    switch(action)
    {
        case ACTION_ADD_INSTANCE     : call_action_method(&logger::add_instance     , param, result); break;
        case ACTION_DEL_INSTANCE     : call_action_method(&logger::del_instance     , param, result); break;
        case ACTION_CHANGE_LOG_QUEUE : call_action_method(&logger::change_log_queue , param, result); break;
        case ACTION_SET_SEVERITIES   : call_action_method(&logger::set_severities   , param, result); break;
        case ACTION_SET_FACILITIES   : call_action_method(&logger::set_facilities   , param, result); break;
        case ACTION_SET_SEVERITY     : call_action_method(&logger::set_severity     , param, result); break;
        case ACTION_SET_FACILITY     : call_action_method(&logger::set_facility     , param, result); break;
        case ACTION_ADD_CONSUMER     : call_action_method(&logger::add_consumer     , param, result); break;
        case ACTION_DEL_CONSUMER     : call_action_method(&logger::del_consumer     , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param, result); break;
        case ACTION_SET_THREAD_NAME  : call_action_method(&logger::set_thread_name  , param, result); break;
        case ACTION_SYNC_LOG_MESSAGE : call_action_method(&logger::sync_log_message , param); break;
    }
}

void logger::on_queue_action(action_id_t action, void* data)
{
    Check_Failure();
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

void* logger::add_instance(
    bool primary,
    const char* name,
    std::size_t max_queue_entries,
    std::size_t max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    )
{
    if(!in_worker_thread())
    {
        if(!run())
        {
            if(errno != EEXIST)
            {
                gkr_log_done();
                return nullptr;
            }
        }
        return execute_action_method<void*>(ACTION_ADD_INSTANCE, primary, name, max_queue_entries, max_message_chars, severities_infos, facilities_infos);
    }
    ++m_ref_count;

    void* instance = primary
        ? &m_primary
        : ((void)m_instances.emplace_back(), static_cast<void*>(&m_instances.back()))
        ;
    change_log_queue(max_queue_entries, max_message_chars);

    change_name(instance, name);

    set_severities(instance, false, severities_infos);
    set_facilities(instance, false, facilities_infos);

    return instance;
}

bool logger::del_instance(void* instance)
{
    if(!running()) return false;

    if(!in_worker_thread())
    {
        if(execute_action_method<bool>(ACTION_DEL_INSTANCE, instance)) return false;

        if(m_ref_count == 0) join(false);

        return true;
    }
    if(m_ref_count == 0) return false;

    if(--m_ref_count == 0) quit();

    if(instance == nullptr) return true;

    process_all_messages();

    for(auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        if(&*it == instance)
        {
            del_all_consumers(instance);
            m_instances.erase(it);
            return true;
        }
    }
    return false;
}

bool logger::change_name(void* instance, const char* name)
{
    Check_ValidState(running(), false);

    instance_data& data = get_data(instance);
    data.name = (name == nullptr) ? "" : name;
    return true;
}

bool logger::change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_CHANGE_LOG_QUEUE, max_queue_entries, max_message_chars);
    }
    if(m_log_queue.capacity() > 0)
    {
        if(max_queue_entries > get_max_queue_entries())
        {
            m_log_queue.resize(max_queue_entries);
        }
        if(max_message_chars > get_max_message_chars())
        {
            m_log_queue.change_element_size(calc_queue_element_size(sizeof(message_head) + 1, max_message_chars));
        }
    }
    else
    {
        if(max_queue_entries == 0) max_queue_entries = 32;
        if(max_message_chars == 0) max_message_chars = (512 - sizeof(message_head) + 1);

        m_log_queue.reset(max_queue_entries, calc_queue_element_size(sizeof(message_head) + 1, max_message_chars));

        Check_ValidState(m_log_queue.capacity() > 0, false);

        Check_ValidState(m_log_queue.element_size() > sizeof(message_data), false);
    }
    return true;
}

bool logger::set_severities(void* instance, bool clear_existing, const name_id_pair* severities_infos)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_SEVERITIES, instance, clear_existing, severities_infos);
    }
    instance_data& data = get_data(instance);

    if(clear_existing)
    {
        data.severities.clear();
    }
    if(severities_infos != nullptr) for( ; severities_infos->name != nullptr; ++severities_infos)
    {
        data.severities[severities_infos->id] = severities_infos->name;
    }
    return true;
}

bool logger::set_facilities(void* instance, bool clear_existing, const name_id_pair* facilities_infos)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_FACILITIES, instance, clear_existing, facilities_infos);
    }
    instance_data& data = get_data(instance);

    if(clear_existing)
    {
        data.facilities.clear();
    }
    if(facilities_infos != nullptr) for( ; facilities_infos->name != nullptr; ++facilities_infos)
    {
        data.facilities[facilities_infos->id] = facilities_infos->name;
    }
    return true;
}

bool logger::set_severity(void* instance, const name_id_pair& severity_info)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_SEVERITY, instance, severity_info);
    }
    instance_data& data = get_data(instance);

    if(severity_info.name == nullptr)
    {
        data.severities.erase(severity_info.id);
    }
    else
    {
        data.severities[severity_info.id] = severity_info.name;
    }
    return true;
}

bool logger::set_facility(void* instance, const name_id_pair& facility_info)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_SET_FACILITY, instance, facility_info);
    }
    instance_data& data = get_data(instance);

    if(facility_info.name == nullptr)
    {
        data.facilities.erase(facility_info.id);
    }
    else
    {
        data.facilities[facility_info.id] = facility_info.name;
    }
    return true;
}

int logger::add_consumer(void* instance, consumer_ptr_t consumer)
{
    Check_ValidState(running(), 0);

    if(!in_worker_thread())
    {
        return execute_action_method<int>(ACTION_ADD_CONSUMER, instance, consumer);
    }
    Check_Arg_NotNull(consumer, 0);

    instance_data& data = get_data(instance);

    for(auto it = data.consumers.begin(); it != data.consumers.end(); ++it)
    {
        if(it->consumer == consumer)
        {
            Check_Arg_Invalid(consumer, 0);
        }
    }
    if(!init_consumer(*consumer))
    {
        done_consumer(*consumer);
        Check_Failure(0);
    }
    data.consumers.emplace_back();

    consumer_info& info = data.consumers.back();
    info.consumer = consumer;
    info.id       = ++m_consumer_id;
    return info.id;
}

bool logger::del_consumer(void* instance, consumer_ptr_t consumer, int id)
{
    Check_ValidState(running(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_DEL_CONSUMER, instance, consumer, id);
    }
    instance_data& data = get_data(instance);

    for(auto it = data.consumers.begin(); it != data.consumers.end(); ++it)
    {
        if((it->consumer == consumer) || (it->id == id))
        {
            data.consumers.erase(it);
            done_consumer(*consumer);
            return true;
        }
    }
    return false;
}

bool logger::del_all_consumers(void* instance)
{
    Check_ValidState(joinable(), false);

    if(!in_worker_thread())
    {
        return execute_action_method<bool>(ACTION_DEL_ALL_CONSUMERS, instance);
    }
    instance_data& data = get_data(instance);

    while(!data.consumers.empty())
    {
        consumer_ptr_t consumer = data.consumers.back().consumer;

        data.consumers.pop_back();

        done_consumer(*consumer);
    }
    return true;
}

bool logger::set_thread_name(int* ptr, const char* name, tid_t tid)
{
    if(!running()) return false;

    if(!in_worker_thread())
    {
        if(tid == 0)
        {
            tid = misc::union_cast<tid_t>(std::this_thread::get_id());
        }
        return execute_action_method<bool>(ACTION_SET_THREAD_NAME, ptr, name, tid);
    }
    if(name == nullptr)
    {
        m_thread_ids.erase(tid);
    }
    else
    {
        m_thread_ids[tid] = name;
    }
    if(ptr == nullptr)
    {
        m_msg_id_ptr.erase(tid);
    }
    else
    {
        m_msg_id_ptr[tid] = ptr;
    }
    return true;
}

static thread_local void* thread_local_element = nullptr;

bool logger::start_log_message(void* instance, int severity, char*& buf, unsigned& cch)
{
    buf = nullptr;
    cch = 0;

    Check_ValidState(thread_local_element == nullptr, false);

    Check_ValidState(running(), false);

    Check_ValidState(!in_worker_thread(), false);//TODO:???

    instance_data& data = get_data(instance);

    if(severity >= data.threshold) return false;

    auto element = m_log_queue.start_push();

    Check_ValidState(element.push_in_progress(), false);

    Assert_Check(element.size() > sizeof(message_data));

    message_data& msg = element.as<message_data>();

    buf = msg.buf;
    cch = unsigned(element.size() - sizeof(message_head));

    thread_local_element = element.detach();
    return true;
}

int logger::cancel_log_message(void* instance)
{
    Check_NotNullPtr(thread_local_element, -1);

    queue_producer_element<log_queue_t> element(m_log_queue, thread_local_element);

    thread_local_element = nullptr;

    element.cancel_push();
    return 0;
}

int logger::finish_log_message(void* instance, const source_location* location, int severity, int facility)
{
    Check_NotNullPtr(thread_local_element, 0);

    queue_producer_element<log_queue_t> element(m_log_queue, thread_local_element);

    thread_local_element = nullptr;

    message_data& msg = element.as<message_data>();

    if(!compose_message(msg, 0, instance, location, severity, facility, nullptr, nullptr))
    {
        element.cancel_push();
        return 0;
    }
    return msg.id;
}

int logger::log_message(void* instance, const source_location* location, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, 0);

    Check_ValidState(running(), 0);

    Check_ValidState(thread_local_element == nullptr, 0);

    instance_data& data = get_data(instance);

    if(severity >= data.threshold) return 0;

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
                Check_ValidState(processed, 0);
                continue;
            }
            Check_Failure(0);
        }

        Assert_Check(m_log_queue.element_size() > sizeof(message_data));

        message_data& msg = element.as<message_data>();

        const std::size_t cch = std::size_t(msg.buf - element.data<char>());

        if(!compose_message(msg, cch, instance, location, severity, facility, format, args))
        {
            element.cancel_push();
            return 0;
        }
        return msg.id;
    }
}

const char* logger::format_output(
    const char* fmt,
    const struct gkr_log_message& msg,
    int flags,
    const char* const* args,
    unsigned rows,
    unsigned cols,
    unsigned* len
    )
{
    Check_Arg_NotNull(fmt, nullptr);

//  Check_ValidState(running(), nullptr);

    Check_ValidState(in_worker_thread(), nullptr);

    constexpr std::size_t CCH_MAX_BUFFER = 64 * 1024;     // 64K
    constexpr std::size_t CCH_FMT_BUFFER =      1024 / 4; // 256 bytes
    constexpr std::size_t CCH_TXT_BUFFER =  1 * 1024;     // 1K

    if(m_fmt.capacity() == 0) m_fmt.reserve(CCH_FMT_BUFFER);
    if(m_txt.capacity() == 0) m_txt.reserve(CCH_TXT_BUFFER);

    unsigned length;

    for( ; ; )
    {
        length = gkr_log_apply_time_format(m_fmt.data<char>(), unsigned(m_fmt.capacity()), fmt, msg.stamp, flags);

        if(length > 0) break;

        if((errno != ENOBUFS) || (m_fmt.capacity() >= CCH_MAX_BUFFER)) return nullptr;

        m_fmt.reserve(2 * m_fmt.capacity());
    }
    for( ; ; )
    {
        length = gkr_log_apply_text_format(m_txt.data<char>(), unsigned(m_txt.capacity()), m_fmt.data<char>(), &msg, flags, args, rows, cols);

        if(length > 0) break;

        if((errno != ENOBUFS) || (m_txt.capacity() >= CCH_MAX_BUFFER)) return nullptr;

        m_txt.reserve(2 * m_txt.capacity());
    }
    if(len != nullptr) *len = length;

    return m_txt.data<char>();
}

void logger::sync_log_message(message_data& msg)
{
    if(!in_worker_thread())
    {
        return execute_action_method<void>(ACTION_SYNC_LOG_MESSAGE, msg);
    }
    process_message(msg);
}

bool logger::compose_message(message_data& msg, std::size_t cch, void* instance, const source_location* location, int severity, int facility, const char* format, va_list args)
{
    msg.id       = ++m_msg_id;
    msg.instance = instance;

    msg.tid      = misc::union_cast<long long>(std::this_thread::get_id());
    msg.stamp    = stamp_now();
    msg.severity = severity;
    msg.facility = facility;

    if(location == nullptr)
    {
        msg.sourceFunc = "";
        msg.sourceFile = "";
        msg.sourceLine = 0;
    }
    else
    {
        msg.sourceFunc = (location->func != nullptr) ? location->func : "";
        msg.sourceFile = (location->file != nullptr) ? location->file : "";
        msg.sourceLine = (location->line);
    }
    if(format == nullptr)
    {
        msg.messageLen = unsigned(std::strlen(msg.buf));
    }
    else if(args == nullptr)
    {
        std::strncpy(msg.buf, format, cch);

        msg.buf[cch - 1] = 0;

        msg.messageLen = unsigned(std::strlen(msg.buf));
    }
    else
    {
        const int len = std::vsnprintf(msg.buf, cch, format, args);

        Check_ValidState(len >= 0, false);

        msg.messageLen = unsigned(len);
    }
    return true;
}

void logger::process_message(message_data& msg)
{
    prepare_message(msg);
    consume_message(msg);

    auto it = m_msg_id_ptr.find(msg.tid);
    if(it  != m_msg_id_ptr.end()) *it->second = msg.id;
}

void logger::prepare_message(message_data& msg)
{
    instance_data& data = get_data(msg.instance);

    msg.messageText = msg.buf;
    msg.moduleName  = data.name;

    auto itThreadId =    m_thread_ids.find(msg.tid);
    auto itSeverity = data.severities.find(msg.severity);
    auto itFacility = data.facilities.find(msg.facility);

    msg.threadName   = (itThreadId ==    m_thread_ids.end()) ? "" : itThreadId->second;
    msg.severityName = (itSeverity == data.severities.end()) ? "" : itSeverity->second;
    msg.facilityName = (itFacility == data.facilities.end()) ? "" : itFacility->second;
}

void logger::consume_message(const message_data& msg)
{
    instance_data& data = get_data(msg.instance);

    for(instance_data* instance = &data; instance != nullptr; instance = (instance != &m_primary) ? &m_primary : nullptr)
    {
        std::size_t count = instance->consumers.size();
        std::size_t index = 0;
#ifndef __cpp_exceptions
        for( ; index < count; ++index)
        {
            consumer_info& info = instance->consumers[index];

            if(info.reentry_guard) continue;

            info.reentry_guard = true;
            if(!info.consumer->filter_log_message(msg))
            {
                info.consumer->consume_log_message(msg);
            }
            info.reentry_guard = false;
        }
    }
#else
        do
        {
            try
            {
                for( ; index < count; ++index)
                {
                    consumer_info& info = instance->consumers[index];

                    if(info.reentry_guard) continue;

                    info.reentry_guard = true;
                    if(!info.consumer->filter_log_message(msg))
                    {
                        info.consumer->consume_log_message(msg);
                    }
                    info.reentry_guard = false;
                }
                return;
            }
            catch(...)
            {
                consumer_ptr_t consumer_that_throws = instance->consumers[index].consumer;
                del_consumer(instance, consumer_that_throws, 0);
                --count;

                Check_Recovery("Log message consumer exception caught - consumer removed");
            }
        }
        while(index < count);
#endif
    }
}

void logger::process_all_messages()
{
    while(process_next_message());
}

bool logger::process_next_message()
{
    auto element = m_log_queue.try_start_pop();

    if(!element.pop_in_progress()) return false;

    message_data& msg = element.as<message_data>();

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
