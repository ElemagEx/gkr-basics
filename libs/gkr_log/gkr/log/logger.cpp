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

logger::logger()
{
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

long long logger::get_wait_timeout_ns() noexcept
{
    return WAIT_INFINITE;
}

std::size_t logger::get_waitable_objects_count() noexcept
{
    return 1;
}

waitable_object& logger::get_waitable_object(std::size_t index) noexcept
{
    return m_log_queue.get_consumer_waitable_object();
}

bool logger::on_start()
{
    m_log_queue.threading.any_thread_can_be_producer();
    m_log_queue.threading.set_current_thread_as_exclusive_consumer();
    return true;
}

void logger::on_finish()
{
    process_all_messages();

    for(auto it = m_channels.begin(); it != m_channels.end(); ++it)
    {
        del_channel(&*it);
    }
    m_channels.clear();

    del_channel(nullptr);

    m_primary.name = "";
}

void logger::on_cross_action(action_id_t action, void* param, void* result)
{
    switch(action)
    {
        case ACTION_ADD_CHANNEL      : call_action_method(&logger::add_channel      , param, result); break;
        case ACTION_DEL_CHANNEL      : call_action_method(&logger::del_channel      , param, result); break;
        case ACTION_GET_CHANNEL      : call_action_method(&logger::get_channel      , param, result); break;
        case ACTION_CHANGE_LOG_QUEUE : call_action_method(&logger::change_log_queue , param, result); break;
        case ACTION_SET_SEVERITIES   : call_action_method(&logger::set_severities   , param, result); break;
        case ACTION_SET_FACILITIES   : call_action_method(&logger::set_facilities   , param, result); break;
        case ACTION_SET_SEVERITY     : call_action_method(&logger::set_severity     , param, result); break;
        case ACTION_SET_FACILITY     : call_action_method(&logger::set_facility     , param, result); break;
        case ACTION_ADD_CONSUMER     : call_action_method(&logger::add_consumer     , param, result); break;
        case ACTION_DEL_CONSUMER     : call_action_method(&logger::del_consumer     , param, result); break;
        case ACTION_DEL_ALL_CONSUMERS: call_action_method(&logger::del_all_consumers, param, result); break;
        case ACTION_SET_THREAD_NAME  : call_action_method(&logger::set_thread_name  , param, result); break;
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

void logger::on_wait_success(std::size_t index)
{
    Check_ValidState(index == 0, );

    DIAG_VAR(bool, processed)
    process_next_message();

    Check_ValidState(processed, );
}

bool logger::on_exception(except_method_t method, const std::exception* e) noexcept
{
    return true;
}

void* logger::add_channel(
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
                return nullptr;
            }
        }
        return execute_action_method<void*>(ACTION_ADD_CHANNEL, primary, name, max_queue_entries, max_message_chars, severities_infos, facilities_infos);
    }
    void* channel = primary
        ? &m_primary
        : ((void)m_channels.emplace_back(), static_cast<void*>(&m_channels.back()))
        ;
    change_log_queue(max_queue_entries, max_message_chars);

    change_name(channel, name);

    set_severities(channel, true, severities_infos);
    set_facilities(channel, true, facilities_infos);

    return channel;
}

bool logger::del_channel(void* channel)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_DEL_CHANNEL, channel);
    }
    channel_data& data = get_data(channel);
    if(data.name == nullptr) return false;

    del_all_consumers(channel);
    set_severities(channel, true, nullptr);
    set_facilities(channel, true, nullptr);
    data.name = nullptr;

    return true;
}

void* logger::get_channel(const char* name)
{
    Check_Arg_NotNull(name, nullptr);

    if(!in_worker_thread())
    {
        Check_ValidState(running(), nullptr);

        return execute_action_method<void*>(ACTION_GET_CHANNEL, name);
    }
    for(auto it = m_channels.begin(); it != m_channels.end(); ++it)
    {
        channel_data& data = *it;

        if(data.name == nullptr) continue;

        if(!std::strcmp(data.name, name)) return &data;
    }
    return nullptr;
}

bool logger::change_name(void* channel, const char* name)
{
    Check_ValidState(running(), false);

    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

    data.name = (name == nullptr) ? "" : name;
    return true;
}

bool logger::change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_CHANGE_LOG_QUEUE, max_queue_entries, max_message_chars);
    }
    if(m_log_queue.capacity() > 0)
    {
        if(max_queue_entries > get_max_queue_entries())
        {
            m_log_queue.reserve(max_queue_entries);
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

bool logger::set_severities(void* channel, bool clear_existing, const name_id_pair* severities_infos)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_SET_SEVERITIES, channel, clear_existing, severities_infos);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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

bool logger::set_facilities(void* channel, bool clear_existing, const name_id_pair* facilities_infos)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_SET_FACILITIES, channel, clear_existing, facilities_infos);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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

bool logger::set_severity(void* channel, const name_id_pair& severity_info)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_SET_SEVERITY, channel, severity_info);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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

bool logger::set_facility(void* channel, const name_id_pair& facility_info)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_SET_FACILITY, channel, facility_info);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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

int logger::add_consumer(void* channel, consumer_ptr_t consumer, int id)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), 0);

        return execute_action_method<int>(ACTION_ADD_CONSUMER, channel, consumer, id);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, 0);

    if(consumer == nullptr)
    {
        consumer = find_consumer(id);
        if(consumer == nullptr) return 0;
    }
    for(auto it = data.consumers.begin(); it != data.consumers.end(); ++it)
    {
        if(it->consumer == consumer)
        {
            return 0;
        }
    }
    if(!init_consumer(*consumer))
    {
        done_consumer(*consumer);
        return 0;
    }
    data.consumers.emplace_back();

    consumer_info& info = data.consumers.back();
    info.consumer = consumer;
    info.id       = ++m_consumer_id;
    return info.id;
}

bool logger::del_consumer(void* channel, consumer_ptr_t consumer, int id)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_DEL_CONSUMER, channel, consumer, id);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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

bool logger::del_all_consumers(void* channel)
{
    if(!in_worker_thread())
    {
        Check_ValidState(running(), false);

        return execute_action_method<bool>(ACTION_DEL_ALL_CONSUMERS, channel);
    }
    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, false);

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
    if(!in_worker_thread())
    {
        if(!running()) return false;

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

void* logger::start_log_message(void* channel, int severity, char*& buf, unsigned& cch)
{
    buf = nullptr;
    cch = 0;

    Check_ValidState(running(), nullptr);

    Check_ValidState(!in_worker_thread(), nullptr);//TODO:???

    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, nullptr);

    if(severity >= data.threshold) return nullptr;

    auto element = m_log_queue.start_push();

    Check_ValidState(element.push_in_progress(), nullptr);

    Assert_Check(element.size() > sizeof(message_data));

    message_data& msg = element.as<message_data>();

    buf = msg.buf;
    cch = unsigned(element.size() - sizeof(message_head));

    return element.detach();
}

int logger::cancel_log_message(void* context, void* channel)
{
    if(context == nullptr) return 0;

    queue_producer_element<log_queue_t> element(m_log_queue, context);

    element.cancel_push();
    return 0;
}

int logger::finish_log_message(void* context, void* channel, const source_location* location, int severity, int facility)
{
    if(context == nullptr) return 0;

    queue_producer_element<log_queue_t> element(m_log_queue, context);

    message_data& msg = element.as<message_data>();

    compose_message(msg, 0, channel, location, severity, facility, nullptr, nullptr);

    return msg.id;
}

int logger::log_message(void* channel, const source_location* location, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, 0);

    Check_ValidState(running(), 0);

    channel_data& data = get_data(channel);
    Check_NotNullPtr(data.name, 0);

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
                DIAG_VAR(bool, processed)
                process_next_message();
                Check_ValidState(processed, 0);
                continue;
            }
            Check_Failure(0);
        }

        Assert_Check(m_log_queue.element_size() > sizeof(message_data));

        message_data& msg = element.as<message_data>();

        const std::size_t cch = std::size_t(m_log_queue.element_size() - (msg.buf - element.data<char>()));

        compose_message(msg, cch, channel, location, severity, facility, format, args);

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

    constexpr std::size_t CCH_MAX_BUFFER = 64 * std::size_t(1024);      // 64K
    constexpr std::size_t CCH_FMT_BUFFER =      std::size_t(1024) / 4U; // 256 bytes
    constexpr std::size_t CCH_TXT_BUFFER =  1 * std::size_t(1024);      // 1K

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

void logger::compose_message(message_data& msg, std::size_t cch, void* channel, const source_location* location, int severity, int facility, const char* format, va_list args)
{
    msg.id       = ++m_msg_id;
    msg.channel  = channel;

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
    }
    else if(args == nullptr)
    {
        std::strncpy(msg.buf, format, cch);
        msg.buf[cch - 1] = 0;
    }
    else
    {
        std::vsnprintf(msg.buf, cch, format, args);
    }
    msg.messageLen = unsigned(std::strlen(msg.buf));
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
    channel_data& data = get_data(msg.channel);

    msg.messageText = msg.buf;
    msg.channelName = data.name;

    auto itThreadId =    m_thread_ids.find(msg.tid);
    auto itSeverity = data.severities.find(msg.severity);
    auto itFacility = data.facilities.find(msg.facility);

    msg.threadName   = (itThreadId ==    m_thread_ids.end()) ? "" : itThreadId->second;
    msg.severityName = (itSeverity == data.severities.end()) ? "" : itSeverity->second;
    msg.facilityName = (itFacility == data.facilities.end()) ? "" : itFacility->second;
}

void logger::consume_message(const message_data& msg)
{
    channel_data& data = get_data(msg.channel);

    for(channel_data* channel = &data; channel != nullptr; channel = (channel != &m_primary) ? &m_primary : nullptr)
    {
        std::size_t count = channel->consumers.size();
        std::size_t index = 0;
#ifndef __cpp_exceptions
        for( ; index < count; ++index)
        {
            consumer_info& info = channel->consumers[index];

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
                    consumer_info& info = channel->consumers[index];

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
                consumer_ptr_t consumer_that_throws = channel->consumers[index].consumer;
                del_consumer(channel, consumer_that_throws, 0);
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

consumer_ptr_t logger::find_consumer(int id)
{
    for(auto& info : m_primary.consumers)
    {
        if(info.id == id) return info.consumer;
    }
    for(auto& data : m_channels)
    {
        for(auto& info : data.consumers)
        {
            if(info.id == id) return info.consumer;
        }
    }
    return nullptr;
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
