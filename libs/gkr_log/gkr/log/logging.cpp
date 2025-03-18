#include <gkr/defs.hpp>
#include <gkr/log/logging.hpp>
#include "logger.hxx"

#include <gkr/sys/thread.hpp>

#include <exception>

#ifdef GENERIC_LOG_SKIP_STREAM
#error This project must be compiled w/out this define
#endif

namespace
{
gkr::log::logger& get_logger()
{
    static gkr::log::logger s_logger;
    return s_logger;
}

inline bool I2B(int val)
{
    return (val != 0);
}

struct buffer_t
{
    char*       ptr = nullptr;
    std::size_t cch = 0;
};
thread_local buffer_t thread_local_buffer;

struct logger_thread_data
{
    bool registered  = false;
    int  last_msg_id = 0;
    char buff[56]    ={0};

    ~logger_thread_data()
    {
        if(registered && get_logger().running())
        {
            get_logger().set_thread_name(nullptr, nullptr);
        }
    }
};
static_assert(sizeof(logger_thread_data) == 64, "");

int check_thread_name(const char* name)
{
    static thread_local logger_thread_data thread_data {};

    if(name != nullptr)
    {
        thread_data.registered = true;

        constexpr unsigned cch = sizeof(thread_data.buff);

        std::strncpy(thread_data.buff, name, cch);

        thread_data.buff[cch - 1] = 0;

        get_logger().set_thread_name(&thread_data.last_msg_id, name);
    }
    else if(!thread_data.registered)
    {
        thread_data.registered = true;

        gkr::sys::get_current_thread_name(thread_data.buff);

        get_logger().set_thread_name(&thread_data.last_msg_id, thread_data.buff);
    }
    return thread_data.last_msg_id;
}
}

extern "C"
{

int gkr_log_init(
    const char* primary_channel_name,
    unsigned max_queue_entries, // = 32
    unsigned max_message_chars, // = 427 [512 bytes - sizeof(gkr_log_message) - 17] //1 for NTS and 16 for msg id/channel (64bit)
    const struct gkr_log_name_id_pair* severities_infos, // = nullptr - no severity names
    const struct gkr_log_name_id_pair* facilities_infos  // = nullptr - no facility names
    )
{
    return (nullptr != get_logger().add_channel(true, primary_channel_name, max_queue_entries, max_message_chars, severities_infos, facilities_infos));
}

int gkr_log_done()
{
    return get_logger().del_channel(nullptr);
}

void* gkr_log_add_channel(
    const char* name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    )
{
    return get_logger().add_channel(false, name, max_queue_entries, max_message_chars, severities_infos, facilities_infos);
}

int gkr_log_del_channel(void* channel)
{
    return get_logger().del_channel(channel);
}

unsigned gkr_log_get_max_queue_entries()
{
    return get_logger().get_max_queue_entries();
}

unsigned gkr_log_get_max_message_chars()
{
    return get_logger().get_max_message_chars();
}

int gkr_log_set_max_queue_entries(unsigned max_queue_entries)
{
    return get_logger().change_log_queue(max_queue_entries, 0);
}

int gkr_log_set_max_message_chars(unsigned max_message_chars)
{
    return get_logger().change_log_queue(0, max_message_chars);
}

int gkr_log_get_severity_threshold(void* channel)
{
    return (channel == nullptr)
        ? get_logger().get_severity_threshold()
        : static_cast<gkr::log::channel_info*>(channel)->threshold;
}

int gkr_log_set_severity_threshold(void* channel, int threshold)
{
    if(channel == nullptr)
    {
        get_logger().set_severity_threshold(threshold);
    }
    else
    {
        static_cast<gkr::log::channel_info*>(channel)->threshold = threshold;
    }
    return true;
}

int gkr_log_set_severities(void* channel, int clear_existing, const struct gkr_log_name_id_pair* severities_infos)
{
    return get_logger().set_severities(channel, I2B(clear_existing), severities_infos);
}

int gkr_log_set_facilities(void* channel, int clear_existing, const struct gkr_log_name_id_pair* facilities_infos)
{
    return get_logger().set_facilities(channel, I2B(clear_existing), facilities_infos);
}

int gkr_log_set_severity(void* channel, const struct gkr_log_name_id_pair* severity_info)
{
    Check_Arg_NotNull(severity_info, false);
    return get_logger().set_severity(channel, *severity_info);
}

int gkr_log_set_facility(void* channel, const struct gkr_log_name_id_pair* facility_info)
{
    Check_Arg_NotNull(facility_info, false);
    return get_logger().set_facility(channel, *facility_info);
}

int gkr_log_del_consumer_by_id(void* channel, int id)
{
    return get_logger().del_consumer(channel, nullptr, id);
}

int gkr_log_del_all_consumers(void* channel)
{
    return get_logger().del_all_consumers(channel);
}

int gkr_log_set_current_thread_name(const char* name)
{
    Check_Arg_NotNull(name, false);
    check_thread_name(name);
    return true;
}

int gkr_log_get_current_thread_llm_id(int id)
{
    if(id == 0) return check_thread_name(nullptr);

    while(id != check_thread_name(nullptr))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return id;
}

int gkr_log_simple_message(void* channel, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);
    check_thread_name(nullptr);
    return get_logger().log_message(channel, nullptr, severity, facility, text, nullptr);
}

int gkr_log_printf_message(void* channel, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const int id = get_logger().log_message(channel, nullptr, severity, facility, format, args);

    va_end(args);
    return id;
}

int gkr_log_valist_message(void* channel, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);
    return get_logger().log_message(channel, nullptr, severity, facility, format, args);
}

int gkr_log_simple_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().log_message(channel, &location, severity, facility, text, nullptr);
}

int gkr_log_printf_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const gkr::log::source_location location{func, file, line};
    const int id = get_logger().log_message(channel, &location, severity, facility, format, args);

    va_end(args);
    return id;
}

int gkr_log_valist_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().log_message(channel, &location, severity, facility, format, args);
}

int gkr_log_custom_message_start(void* channel, int severity, char** buf, unsigned* cch)
{
    Check_Arg_NotNull(buf, 0);
    Check_Arg_NotNull(cch, 0);

    return get_logger().start_log_message(channel, severity, *buf, *cch);
}

int gkr_log_custom_message_cancel(void* channel)
{
    return get_logger().cancel_log_message(channel);
}

int gkr_log_custom_message_finish(void* channel, int severity, int facility)
{
    check_thread_name(nullptr);

    return get_logger().finish_log_message(channel, nullptr, severity, facility);
}

int gkr_log_custom_message_finish_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility)
{
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().finish_log_message(channel, &location, severity, facility);
}

const char* gkr_log_format_output(
    const char* fmt,
    const struct gkr_log_message* msg,
    int flags,
    const char* const* args,
    unsigned rows,
    unsigned cols,
    unsigned* len
    )
{
    return get_logger().format_output(fmt, *msg, flags, args, rows, cols, len);
}

}

int gkr_log_add_consumer(void* channel, std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    return get_logger().add_consumer(channel, consumer);
}

int gkr_log_del_consumer(void* channel, std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    return get_logger().del_consumer(channel, consumer, 0);
}

namespace gkr
{
namespace log
{
namespace impl
{

void init_ostringstream_allocator(char* buf, std::size_t cch)
{
    Assert_NotNullPtr(buf);
    Assert_Check(cch > 0);

    thread_local_buffer.ptr = buf;
    thread_local_buffer.cch = cch;
}
void done_ostringstream_allocator(std::size_t len)
{
    Assert_Check(len < thread_local_buffer.cch);

    thread_local_buffer.ptr[len] = 0;

    thread_local_buffer.ptr = nullptr;
    thread_local_buffer.cch = 0;
}
void* allocate_bytes(std::size_t& cb)
{
    Assert_NotNullPtr(thread_local_buffer.ptr);

    if(cb > thread_local_buffer.cch) throw std::bad_alloc();

    cb = thread_local_buffer.cch;

    return thread_local_buffer.ptr;
}
void deallocate_bytes(void* p, std::size_t cb)
{
}

}
}
}
