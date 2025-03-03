#pragma once

#include <gkr/capi/api.h>
#include <gkr/log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_name_id_pair
{
    const char* name;
    int         id;
};
struct gkr_log_consumer_callbacks;

GKR_LOG_API int gkr_log_init(
    const char* name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    );
GKR_LOG_API int gkr_log_done();

GKR_LOG_API void* gkr_log_add_instance(
    const char* name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    );
GKR_LOG_API int gkr_log_del_instance(void* instance);

GKR_LOG_API unsigned gkr_log_get_max_queue_entries();
GKR_LOG_API unsigned gkr_log_get_max_message_chars();

GKR_LOG_API int gkr_log_set_max_queue_entries(unsigned max_queue_entries);
GKR_LOG_API int gkr_log_set_max_message_chars(unsigned max_message_chars);

GKR_LOG_API int gkr_log_get_severity_threshold(void* instance);
GKR_LOG_API int gkr_log_set_severity_threshold(void* instance, int threshold);

GKR_LOG_API int gkr_log_set_severities(void* instance, int clear_existing, const struct gkr_log_name_id_pair* severities_infos);
GKR_LOG_API int gkr_log_set_facilities(void* instance, int clear_existing, const struct gkr_log_name_id_pair* facilities_infos);

GKR_LOG_API int gkr_log_set_severity(void* instance, const struct gkr_log_name_id_pair* severity_info);
GKR_LOG_API int gkr_log_set_facility(void* instance, const struct gkr_log_name_id_pair* facility_info);

GKR_LOG_API int gkr_log_del_consumer_by_id(void* instance, int id);
GKR_LOG_API int gkr_log_del_all_consumers(void* instance);

#ifdef __cplusplus
}
#endif
