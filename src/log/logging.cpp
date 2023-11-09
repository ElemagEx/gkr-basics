#include <gkr/log/logging.h>

#include <gkr/log/logger.h>
#include <gkr/sys/thread_name.h>

namespace gkr
{
namespace log
{

static logger* s_logger = nullptr;

alignas(logger)
static char s_storage_for_logger[sizeof(logger)] {0};

bool logging::init(
    const name_id_pair_t* severities,
    const name_id_pair_t* facilities,
    std::size_t max_queue_entries,
    std::size_t max_message_chars
    )
{
    Check_ValidState(s_logger == nullptr, false);

    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    s_logger = new (s_storage_for_logger) logger();

    if(!s_logger->run())
    {
        done();
        return false;
    }
    s_logger->set_severities(false, severities);
    s_logger->set_facilities(false, facilities);

    if(!s_logger->change_log_queue(max_queue_entries, max_message_chars))
    {
        done();
        return false;
    }
    return true;
}

void logging::done()
{
    if(s_logger == nullptr) return;

    s_logger->~logger();

    s_logger = nullptr;
}

bool logging::change_log_queue(
    std::size_t max_queue_entries,
    std::size_t max_message_chars
    )
{
    if(s_logger == nullptr) return false;

    Check_Arg_IsValid(max_queue_entries > 0, false);
    Check_Arg_IsValid(max_message_chars > 0, false);

    return s_logger->change_log_queue(max_queue_entries, max_message_chars);
}

bool logging::set_severities(bool clear_existing, const name_id_pair_t* severities)
{
    if(s_logger == nullptr) return false;

    s_logger->set_severities(clear_existing, severities);

    return true;
}

bool logging::set_facilities(bool clear_existing, const name_id_pair_t* facilities)
{
    if(s_logger == nullptr) return false;

    s_logger->set_facilities(clear_existing, facilities);

    return true;
}

bool logging::set_severity(const name_id_pair_t& severity)
{
    if(s_logger == nullptr) return false;

    s_logger->set_severity(severity);

    return true;
}

bool logging::set_facility(const name_id_pair_t& facility)
{
    if(s_logger == nullptr) return false;

    s_logger->set_facility(facility);

    return true;
}

bool logging::add_consumer(std::shared_ptr<consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->add_consumer(consumer);
}

bool logging::del_consumer(std::shared_ptr<consumer> consumer)
{
    Check_Arg_NotNull(consumer, false);

    if(s_logger == nullptr) return false;

    return s_logger->del_consumer(consumer);
}

bool logging::del_all_consumers()
{
    if(s_logger == nullptr) return false;

    s_logger->del_all_consumers();

    return true;
}

bool logging::set_this_thread_name(const char* name)
{
    if(s_logger == nullptr) return false;

    check_thread_name(name);

    return true;
}

bool logging::log_simple_message(bool wait, id_t severity, id_t facility, const char* text)
{
    Check_Arg_NotNull(text, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(wait, severity, facility, text, nullptr);
}

bool logging::log_format_message(bool wait, id_t severity, id_t facility, const char* format, ...)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    std::va_list args;
    va_start(args, format);

    const bool result = s_logger->log_message(wait, severity, facility, format, args);

    va_end(args);
    return result;
}

bool logging::log_valist_message(bool wait, id_t severity, id_t facility, const char* format, std::va_list args)
{
    Check_Arg_NotNull(format, false);

    if(s_logger == nullptr) return false;

    check_thread_name(nullptr);

    return s_logger->log_message(wait, severity, facility, format, args);
}

struct thread_name_t
{
    bool registered = false;
#ifndef NDEBUG
    char buff[sys::MAX_THREAD_NAME_CCH] = {0};
#endif

    ~thread_name_t()
    {
        if(registered && (s_logger != nullptr))
        {
            s_logger->set_thread_name(nullptr);
        }
    }
};

void logging::check_thread_name(const char* name)
{
    static thread_local thread_name_t thread_name{};

    if(name != nullptr)
    {
        thread_name.registered = true;

        s_logger->set_thread_name(name);

#ifndef NDEBUG
        std::strncpy(thread_name.buff, name, sys::MAX_THREAD_NAME_CCH);

        thread_name.buff[sys::MAX_THREAD_NAME_CCH - 1] = 0;
#endif
        return;
    }
    if(thread_name.registered)
    {
        return;
    }
    thread_name.registered = true;

#ifndef NDEBUG
    char* buff;
    buff = thread_name.buff;
#else
    char buff[sys::MAX_THREAD_NAME_CCH] = {0};
#endif
    if(sys::get_current_thread_name(buff))
    {
        s_logger->set_thread_name(buff);
    }
}

}
}
