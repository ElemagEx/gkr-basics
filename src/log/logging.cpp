#include <gkr/log/logging.h>

#include <gkr/log/logger.h>

namespace gkr
{
namespace log
{

static logger s_logger;

bool logging::init(
    const name_id_pair* severities,
    const name_id_pair* facilities
    )
{
    Check_ValidState(!s_logger.running(), false);

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

}
}
