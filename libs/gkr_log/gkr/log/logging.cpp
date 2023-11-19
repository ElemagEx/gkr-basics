#include <gkr/log/logging.h>

#include <gkr/log/logger.h>
#include <gkr/sys/thread_name.h>

using logger = gkr::log::logger;

static logger* s_logger = nullptr;

alignas(logger)
static char s_storage_for_logger[sizeof(logger)] {0};

static void check_thread_name(const char* name);

inline bool I2B(int val)
{
    return (val != 0);
}

extern "C"
{

int gkr_log_init(
    const struct gkr_log_name_id_pair* severities, // = nullptr - no severity names
    const struct gkr_log_name_id_pair* facilities, // = nullptr - no facility names
    unsigned max_queue_entries, // = 16
    unsigned max_message_chars  // = 968 [1024 - sizeof(gkr_log_message)]
    )
{
    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    Check_ValidState(s_logger == nullptr, false);

    s_logger = new (s_storage_for_logger) logger();

    if(!s_logger->run())
    {
        gkr_log_done();
        return false;
    }
    s_logger->set_severities(false, severities);
    s_logger->set_facilities(false, facilities);

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

int gkr_log_set_severities(int clear_existing, const struct gkr_log_name_id_pair* severities)
{
    if(s_logger == nullptr) return false;

    s_logger->set_severities(I2B(clear_existing), severities);

    return true;
}

int gkr_log_set_facilities(int clear_existing, const struct gkr_log_name_id_pair* facilities)
{
    if(s_logger == nullptr) return false;

    s_logger->set_facilities(I2B(clear_existing), facilities);

    return true;
}

int gkr_log_set_severity(const struct gkr_log_name_id_pair* severity)
{
    Check_Arg_NotNull(severity, false);

    if(s_logger == nullptr) return false;

    s_logger->set_severity(*severity);

    return true;
}

int gkr_log_set_facility(const struct gkr_log_name_id_pair* facility)
{
    Check_Arg_NotNull(facility, false);

    if(s_logger == nullptr) return false;

    s_logger->set_facility(*facility);

    return true;
}

int gkr_log_add_consumer(gkr_log_consumer* consumer, void* param)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->add_functions(consumer, param);
}

int gkr_log_del_consumer(gkr_log_consumer* consumer, void* param)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->del_functions(consumer, param);
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

int gkr_log_simple_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* text)
{
    Check_Arg_NotNull(text, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(I2B(wait), severity, facility, text, nullptr);
}

int gkr_log_printf_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    va_list args;
    va_start(args, format);

    const bool result = s_logger->log_message(I2B(wait), severity, facility, format, args);

    va_end(args);
    return result;
}

int gkr_log_valist_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* format, va_list args)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(I2B(wait), severity, facility, format, args);
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

struct thread_name_t
{
    bool registered = false;
#ifndef NDEBUG
    char buff[gkr::sys::MAX_THREAD_NAME_CCH] = {0};
#endif
    ~thread_name_t()
    {
        if(registered && (s_logger != nullptr))
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
