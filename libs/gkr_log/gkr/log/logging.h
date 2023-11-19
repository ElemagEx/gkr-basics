#pragma once

#include <gkr/log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_name_id_pair
{
    const char*  name;
    gkr_log_id_t id;
};

struct gkr_log_consumer;

GKR_CORE_API int gkr_log_init(
    const struct gkr_log_name_id_pair* severities,
    const struct gkr_log_name_id_pair* facilities,
    unsigned max_queue_entries,
    unsigned max_message_chars
    );
GKR_CORE_API int gkr_log_done();

GKR_CORE_API int gkr_log_set_max_queue_entries(unsigned max_queue_entries);
GKR_CORE_API int gkr_log_set_max_message_chars(unsigned max_message_chars);

GKR_CORE_API int gkr_log_set_severities(int clear_existing, const struct gkr_log_name_id_pair* severities);
GKR_CORE_API int gkr_log_set_facilities(int clear_existing, const struct gkr_log_name_id_pair* facilities);

GKR_CORE_API int gkr_log_set_severity(const struct gkr_log_name_id_pair* severity);
GKR_CORE_API int gkr_log_set_facility(const struct gkr_log_name_id_pair* facility);

GKR_CORE_API int gkr_log_add_consumer(struct gkr_log_consumer* consumer, void* param);
GKR_CORE_API int gkr_log_del_consumer(struct gkr_log_consumer* consumer, void* param);

GKR_CORE_API int gkr_log_del_all_consumers();

#ifdef __cplusplus
}

#include <memory>

namespace gkr
{
namespace log
{

using name_id_pair = gkr_log_name_id_pair;
class consumer;

struct logging final
{
    const bool initialized;

    logging(
        const name_id_pair* severities = nullptr,
        const name_id_pair* facilities = nullptr,
        unsigned max_queue_entries = 16,
        unsigned max_message_chars = 968 // 1024 - sizeof(log::message)
        )
        : initialized(0 != gkr_log_init(severities, facilities, max_queue_entries, max_message_chars))
    {
    }
    ~logging()
    {
        gkr_log_done();
    }

    logging           (const logging& other) noexcept : initialized(other.initialized) {}
    logging& operator=(const logging&      ) noexcept { return *this; }

    logging           (logging&& other) noexcept : initialized(other.initialized) {}
    logging& operator=(logging&&      ) noexcept { return *this; }
};

}
}

GKR_CORE_API int gkr_log_add_consumer(std::shared_ptr<gkr::log::consumer> consumer);
GKR_CORE_API int gkr_log_del_consumer(std::shared_ptr<gkr::log::consumer> consumer);

#endif
