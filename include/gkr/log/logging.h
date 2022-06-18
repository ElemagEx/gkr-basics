#pragma once

#include <memory>
#include <cstdarg>

#ifndef GKR_LOG_API
#define GKR_LOG_API
#endif

namespace gkr
{
namespace log
{

struct name_id_pair
{
    const char*    name;
    unsigned short id;
};

class consumer;

class logging
{
public:
    GKR_LOG_API static bool init(
        const name_id_pair* severities = nullptr,
        const name_id_pair* facilities = nullptr
        );
    GKR_LOG_API static void done();

public:
    GKR_LOG_API static bool set_severities(bool clear_existing, const name_id_pair* severities);
    GKR_LOG_API static bool set_facilities(bool clear_existing, const name_id_pair* facilities);

    GKR_LOG_API static bool set_severity(const name_id_pair& severity);
    GKR_LOG_API static bool set_facility(const name_id_pair& facility);

public:
    GKR_LOG_API static bool add_consumer(std::shared_ptr<consumer> consumer);
    GKR_LOG_API static bool del_consumer(std::shared_ptr<consumer> consumer);

    GKR_LOG_API static bool del_all_consumers();

public:
    GKR_LOG_API static void log_message  (bool instant, int severity, int facility, void* data, const char* message, ...);
    GKR_LOG_API static void log_message_v(bool instant, int severity, int facility, void* data, const char* message, va_list);
};

}
}

#ifndef GKR_LOG
#define GKR_LOG(severity, facility, message, ...) ::gkr::log::logging::log_message(false, severity, facility, message, __VA_ARGS__)
#endif
