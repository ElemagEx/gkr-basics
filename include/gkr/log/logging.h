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
        const name_id_pair* facilities = nullptr,
        std::size_t max_queue_entries  = 16
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
    GKR_LOG_API static bool log_simple_message(bool wait, int severity, int facility, const char* message);
    GKR_LOG_API static bool log_format_message(bool wait, int severity, int facility, const char* message, ...);
    GKR_LOG_API static bool log_valist_message(bool wait, int severity, int facility, const char* message, va_list args);
};

}
}
