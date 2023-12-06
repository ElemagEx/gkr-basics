#include "logging.hpp"
#include "logger.hpp"

#include <gkr/sys/thread.hpp>

#include <exception>

#ifdef GKR_NO_OSTREAM_LOGGING
#error This project must be compiled w.out this define
#endif

using logger = gkr::log::logger;

static logger*  s_logger = nullptr;
static unsigned s_refCnt = 0;

alignas(logger)
static char s_storage_for_logger[sizeof(logger)] {0};

static void check_thread_name(const char* name);

inline bool I2B(int val)
{
    return (val != 0);
}

extern "C"
{
//
//TODO:Made initialization/deinitialization thread-safe
//
int gkr_log_init(
    const struct gkr_log_name_id_pair* severities_infos, // = nullptr - no severity names
    const struct gkr_log_name_id_pair* facilities_infos, // = nullptr - no facility names
    unsigned max_queue_entries, // = 16
    unsigned max_message_chars  // = 959 [1024 - sizeof(gkr_log_message) - 1] //1 for null terminated character
    )
{
    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    if(s_logger != nullptr)
    {
        ++s_refCnt;
        return true;
    }
    s_logger = new (s_storage_for_logger) logger();
    s_refCnt = 1;

    if(!s_logger->run())
    {
        gkr_log_done();
        return false;
    }
    s_logger->set_severities(false, severities_infos);
    s_logger->set_facilities(false, facilities_infos);

    if(!s_logger->change_log_queue(max_queue_entries, max_message_chars))
    {
        gkr_log_done();
        return false;
    }
    return true;
}

int gkr_log_done()
{
    if(s_logger == nullptr) return true;

    if(--s_refCnt > 1) return true;

    s_logger->~logger();

    s_logger = nullptr;

    return true;
}

int gkr_log_set_max_queue_entries(unsigned max_queue_entries)
{
    Check_Arg_IsValid(max_queue_entries > 0, false);

    if(s_logger == nullptr) return false;

    return s_logger->change_log_queue(max_queue_entries, std::size_t(-1));
}

int gkr_log_set_max_message_chars(unsigned max_message_chars)
{
    Check_Arg_IsValid(max_message_chars > 0, false);

    if(s_logger == nullptr) return false;

    return s_logger->change_log_queue(std::size_t(-1), max_message_chars);
}

int gkr_log_set_severities(int clear_existing, const struct gkr_log_name_id_pair* severities_infos)
{
    if(s_logger == nullptr) return false;

    s_logger->set_severities(I2B(clear_existing), severities_infos);

    return true;
}

int gkr_log_set_facilities(int clear_existing, const struct gkr_log_name_id_pair* facilities_infos)
{
    if(s_logger == nullptr) return false;

    s_logger->set_facilities(I2B(clear_existing), facilities_infos);

    return true;
}

int gkr_log_set_severity(const struct gkr_log_name_id_pair* severity_info)
{
    Check_Arg_NotNull(severity_info, false);

    if(s_logger == nullptr) return false;

    s_logger->set_severity(*severity_info);

    return true;
}

int gkr_log_set_facility(const struct gkr_log_name_id_pair* facility_info)
{
    Check_Arg_NotNull(facility_info, false);

    if(s_logger == nullptr) return false;

    s_logger->set_facility(*facility_info);

    return true;
}

int gkr_log_add_consumer_callbacks(gkr_log_consumer_callbacks* callbacks, void* param)
{
    Check_Arg_NotNull(callbacks, false);

    if(s_logger == nullptr) return false;

    return s_logger->add_callbacks(callbacks, param);
}

int gkr_log_del_consumer_callbacks(gkr_log_consumer_callbacks* callbacks, void* param)
{
    Check_Arg_NotNull(callbacks, false);

    if(s_logger == nullptr) return false;

    return s_logger->del_callbacks(callbacks, param);
}

int gkr_log_del_all_consumers()
{
    if(s_logger == nullptr) return false;

    s_logger->del_all_consumers();

    return true;
}

int gkr_log_set_this_thread_name(const char* name)
{
    if(s_logger == nullptr) return false;

    check_thread_name(name);

    return true;
}

int gkr_log_simple_message(int wait, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(nullptr, I2B(wait), severity, facility, text, nullptr);
}

int gkr_log_printf_message(int wait, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const bool result = s_logger->log_message(nullptr, I2B(wait), severity, facility, format, args);

    va_end(args);
    return result;
}

int gkr_log_valist_message(int wait, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(nullptr, I2B(wait), severity, facility, format, args);
}

int gkr_log_simple_message_ex(const char* func, const char* file, unsigned line, int wait, int severity, int facility, const char* text)
{
    Check_Arg_NotNull(text, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};

    return s_logger->log_message(&location, I2B(wait), severity, facility, text, nullptr);
}

int gkr_log_printf_message_ex(const char* func, const char* file, unsigned line, int wait, int severity, int facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const gkr::log::source_location location{func, file, line};

    const bool result = s_logger->log_message(&location, I2B(wait), severity, facility, format, args);

    va_end(args);
    return result;
}

int gkr_log_valist_message_ex(const char* func, const char* file, unsigned line, int wait, int severity, int facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};

    return s_logger->log_message(&location, I2B(wait), severity, facility, format, args);
}

int gkr_log_custom_message_start(char** buf, unsigned* cch)
{
    Check_Arg_NotNull(buf, nullptr);
    Check_Arg_NotNull(cch, nullptr);

    if(s_logger == nullptr) return false;

    return s_logger->start_log_message(*buf, *cch);
}

int gkr_log_custom_message_finish(int wait, int severity, int facility)
{
    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->finish_log_message(nullptr, I2B(wait), severity, facility);
}

int gkr_log_custom_message_finish_ex(const char* func, const char* file, unsigned line, int wait, int severity, int facility)
{
    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    const gkr::log::source_location location{func, file, line};

    return s_logger->finish_log_message(&location, I2B(wait), severity, facility);
}


}

int gkr_log_add_consumer(std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->add_consumer(consumer);
}

int gkr_log_del_consumer(std::shared_ptr<gkr::log::consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->del_consumer(consumer);
}

namespace gkr
{
namespace log
{
namespace impl
{

struct buffer_t
{
    char*       ptr = nullptr;
    std::size_t cch = 0;
};
static thread_local buffer_t thread_local_buffer;

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

struct thread_name_t
{
    bool registered = false;
#ifndef NDEBUG
    char buff[gkr::sys::MAX_THREAD_NAME_CCH] = {0};
#endif
    ~thread_name_t()
    {
        if(registered && (s_logger != nullptr) && s_logger->running())
        {
            s_logger->set_thread_name(nullptr);
        }
    }
};

void check_thread_name(const char* name)
{
    static thread_local thread_name_t thread_name {};

    if(name != nullptr)
    {
        thread_name.registered = true;

        s_logger->set_thread_name(name);

#ifndef NDEBUG
        std::strncpy(thread_name.buff, name, gkr::sys::MAX_THREAD_NAME_CCH);

        thread_name.buff[gkr::sys::MAX_THREAD_NAME_CCH - 1] = 0;
#endif
        return;
    }
    thread_name.registered = true;

#ifndef NDEBUG
    char* buff;
    buff = thread_name.buff;
#else
    char buff[gkr::sys::MAX_THREAD_NAME_CCH] = {0};
#endif
    if(gkr::sys::get_current_thread_name(buff))
    {
        s_logger->set_thread_name(buff);
    }
}
