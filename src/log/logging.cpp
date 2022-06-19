#include <gkr/log/logging.h>

#include <gkr/log/logger.h>

namespace gkr
{
namespace log
{

static logger s_logger;

bool logging::init(
    const name_id_pair* severities,
    const name_id_pair* facilities,
    std::size_t max_queue_entries,
    std::size_t max_message_chars
    )
{
    Check_ValidState(!s_logger.running(), false);

    s_logger.change_log_queue(max_queue_entries, max_message_chars);

    if(!s_logger.run()) return false;

    s_logger.set_severities(false, severities);
    s_logger.set_facilities(false, facilities);

    return true;
}

void logging::done()
{
    if(!s_logger.running()) return;

    s_logger.join(true);
}

bool logging::change_log_queue(
    std::size_t max_queue_entries,
    std::size_t max_message_chars
    )
{
    return s_logger.change_log_queue(max_queue_entries, max_message_chars);
}

bool logging::set_severities(bool clear_existing, const name_id_pair* severities)
{
    Check_ValidState(s_logger.running(), false);

    s_logger.set_severities(clear_existing, severities);

    return true;
}

bool logging::set_facilities(bool clear_existing, const name_id_pair* facilities)
{
    Check_ValidState(s_logger.running(), false);

    s_logger.set_facilities(clear_existing, facilities);

    return true;
}

bool logging::set_severity(const name_id_pair& severity)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.set_severity(severity);
}

bool logging::set_facility(const name_id_pair& facility)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.set_facility(facility);
}

bool logging::add_consumer(std::shared_ptr<consumer> consumer)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.add_consumer(consumer);
}

bool logging::del_consumer(std::shared_ptr<consumer> consumer)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.del_consumer(consumer);
}

bool logging::del_all_consumers()
{
    Check_ValidState(s_logger.running(), false);

    s_logger.del_all_consumers();

    return true;
}

bool logging::log_simple_message(bool wait, int severity, int facility, const char* message)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.log_message(wait, severity, facility, message, nullptr);
}

bool logging::log_format_message(bool wait, int severity, int facility, const char* message, ...)
{
    Check_ValidState(s_logger.running(), false);

    va_list args;
    va_start(args, message);

    return s_logger.log_message(wait, severity, facility, message, args);
}

bool logging::log_valist_message(bool wait, int severity, int facility, const char* message, va_list args)
{
    Check_ValidState(s_logger.running(), false);

    return s_logger.log_message(wait, severity, facility, message, args);
}

}
}
