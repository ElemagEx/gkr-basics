#pragma once

#include "message.h"

#include <memory>
#include <cstdarg>

#ifndef GKR_LOG_API
#define GKR_LOG_API
#endif

namespace gkr
{
using std::size_t;

namespace log
{

struct name_id_pair
{
    const char*    name;
    unsigned short id;
};

class consumer;

class logging final
{
public:
    GKR_LOG_API static bool init(
        const name_id_pair* severities = nullptr,
        const name_id_pair* facilities = nullptr,
        size_t max_queue_entries  = 16,
        size_t max_message_chars  = (1024 - sizeof(message))
        );
    GKR_LOG_API static void done();

public:
    GKR_LOG_API static bool change_log_queue(
        size_t max_queue_entries = size_t(-1),
        size_t max_message_chars = size_t(-1)
        );

public:
    GKR_LOG_API static bool set_severities(bool clear_existing, const name_id_pair* severities = nullptr);
    GKR_LOG_API static bool set_facilities(bool clear_existing, const name_id_pair* facilities = nullptr);

    GKR_LOG_API static bool set_severity(const name_id_pair& severity);
    GKR_LOG_API static bool set_facility(const name_id_pair& facility);

public:
    GKR_LOG_API static bool add_consumer(std::shared_ptr<consumer> consumer);
    GKR_LOG_API static bool del_consumer(std::shared_ptr<consumer> consumer);

    GKR_LOG_API static bool del_all_consumers();

public:
    GKR_LOG_API static bool log_simple_message(bool wait, int severity, int facility, const char* format);
    GKR_LOG_API static bool log_format_message(bool wait, int severity, int facility, const char* format, ...);
    GKR_LOG_API static bool log_valist_message(bool wait, int severity, int facility, const char* format, std::va_list args);

public:
    logging(
        const name_id_pair* severities = nullptr,
        const name_id_pair* facilities = nullptr,
        size_t max_queue_entries  = 16,
        size_t max_message_chars  = (1024 - sizeof(message))
        )
    {
        m_initialized = init(severities, facilities, max_queue_entries, max_message_chars);
    }
    ~logging()
    {
        if(m_initialized) done();
    }

private:
    bool m_initialized = false;

private:
    logging           (const logging&) noexcept = delete;
    logging& operator=(const logging&) noexcept = delete;

    logging           (logging&&) noexcept = delete;
    logging& operator=(logging&&) noexcept = delete;

private:
    static void check_thread();
};

}
}
