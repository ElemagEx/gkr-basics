#include <gkr/defs.hpp>
#include <gkr/log/logging.hpp>
#include "logger.hxx"

#include <gkr/sys/thread.hpp>

#include <exception>

#ifdef GKR_NO_STREAM_LOGGING
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
    const char* name,
    unsigned max_queue_entries, // = 32
    unsigned max_message_chars, // = 427 [512 bytes - sizeof(gkr_log_message) - 17] //1 for NTS and 16 for msg id/instance (64bit)
    const struct gkr_log_name_id_pair* severities_infos, // = nullptr - no severity names
    const struct gkr_log_name_id_pair* facilities_infos  // = nullptr - no facility names
    )
{
    return (nullptr != get_logger().add_instance(true, name, max_queue_entries, max_message_chars, severities_infos, facilities_infos));
}

int gkr_log_done()
{
    return get_logger().del_instance(nullptr);
}

void* gkr_log_add_instance(
    const char* name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    )
{
    return get_logger().add_instance(false, name, max_queue_entries, max_message_chars, severities_infos, facilities_infos);
}

int gkr_log_del_instance(void* instance)
{
    return get_logger().del_instance(instance);
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

int gkr_log_get_severity_threshold(void* instance)
{
    return (instance == nullptr)
        ? get_logger().get_severity_threshold()
        : static_cast<gkr::log::instance_info*>(instance)->threshold;
}

int gkr_log_set_severity_threshold(void* instance, int threshold)
{
    if(instance == nullptr)
    {
        get_logger().set_severity_threshold(threshold);
    }
    else
    {
        static_cast<gkr::log::instance_info*>(instance)->threshold = threshold;
    }
    return true;
}

int gkr_log_set_severities(void* instance, int clear_existing, const struct gkr_log_name_id_pair* severities_infos)
{
    return get_logger().set_severities(instance, I2B(clear_existing), severities_infos);
}

int gkr_log_set_facilities(void* instance, int clear_existing, const struct gkr_log_name_id_pair* facilities_infos)
{
    return get_logger().set_facilities(instance, I2B(clear_existing), facilities_infos);
}

int gkr_log_set_severity(void* instance, const struct gkr_log_name_id_pair* severity_info)
{
    Check_Arg_NotNull(severity_info, false);
    return get_logger().set_severity(instance, *severity_info);
}

int gkr_log_set_facility(void* instance, const struct gkr_log_name_id_pair* facility_info)
{
    Check_Arg_NotNull(facility_info, false);
    return get_logger().set_facility(instance, *facility_info);
}

int gkr_log_del_consumer_by_id(void* instance, int id)
{
    return get_logger().del_consumer(instance, nullptr, id);
}

int gkr_log_del_all_consumers(void* instance)
{
    return get_logger().del_all_consumers(instance);
}

int gkr_log_set_this_thread_name(const char* name)
{
    Check_Arg_NotNull(name, false);
    check_thread_name(name);
    return true;
}

int gkr_log_get_this_thread_llm_id(int id)
{
    if(id == 0) return check_thread_name(nullptr);

    while(id != check_thread_name(nullptr))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return id;
}

int gkr_log_simple_message(void* instance, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);
    check_thread_name(nullptr);
    return get_logger().log_message(instance, nullptr, severity, facility, text, nullptr);
}

int gkr_log_printf_message(void* instance, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const int id = get_logger().log_message(instance, nullptr, severity, facility, format, args);

    va_end(args);
    return id;
}

int gkr_log_valist_message(void* instance, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);
    return get_logger().log_message(instance, nullptr, severity, facility, format, args);
}

int gkr_log_simple_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().log_message(instance, &location, severity, facility, text, nullptr);
}

int gkr_log_printf_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const gkr::log::source_location location{func, file, line};
    const int id = get_logger().log_message(instance, &location, severity, facility, format, args);

    va_end(args);
    return id;
}

int gkr_log_valist_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().log_message(instance, &location, severity, facility, format, args);
}

int gkr_log_custom_message_start(void* instance, int severity, char** buf, unsigned* cch)
{
    Check_Arg_NotNull(buf, 0);
    Check_Arg_NotNull(cch, 0);

    return get_logger().start_log_message(instance, severity, *buf, *cch);
}

int gkr_log_custom_message_cancel(void* instance)
{
    return get_logger().cancel_log_message(instance);
}

int gkr_log_custom_message_finish(void* instance, int severity, int facility)
{
    check_thread_name(nullptr);

    return get_logger().finish_log_message(instance, nullptr, severity, facility);
}

int gkr_log_custom_message_finish_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility)
{
    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};
    return get_logger().finish_log_message(instance, &location, severity, facility);
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

int gkr_log_add_consumer(void* instance, std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    return get_logger().add_consumer(instance, consumer);
}

int gkr_log_del_consumer(void* instance, std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    return get_logger().del_consumer(instance, consumer, 0);
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
