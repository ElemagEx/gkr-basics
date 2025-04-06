#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_name_id_pair
{
    const char* name;
    int         id;
};
struct gkr_log_consumer_callbacks;

#define GKR_LOG_CHANNEL_NAME_DIAGNOSTICS    "GKR-Diagnostics"

GKR_LOG_API int gkr_log_init(
    const char* primary_channel_name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos,
    int add_diag_channel
    );
GKR_LOG_API int gkr_log_done();

GKR_LOG_API void* gkr_log_add_channel(
    const char* name,
    unsigned max_queue_entries,
    unsigned max_message_chars,
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos
    );
GKR_LOG_API int gkr_log_del_channel(void* channel);

GKR_LOG_API void* gkr_log_get_channel(const char* name);

GKR_LOG_API unsigned gkr_log_get_max_queue_entries();
GKR_LOG_API unsigned gkr_log_get_max_message_chars();

GKR_LOG_API int gkr_log_set_max_queue_entries(unsigned max_queue_entries);
GKR_LOG_API int gkr_log_set_max_message_chars(unsigned max_message_chars);

GKR_LOG_API int gkr_log_get_severity_threshold(void* channel);
GKR_LOG_API int gkr_log_set_severity_threshold(void* channel, int threshold);

GKR_LOG_API int gkr_log_set_severities(void* channel, int clear_existing, const struct gkr_log_name_id_pair* severities_infos);
GKR_LOG_API int gkr_log_set_facilities(void* channel, int clear_existing, const struct gkr_log_name_id_pair* facilities_infos);

GKR_LOG_API int gkr_log_set_severity(void* channel, const struct gkr_log_name_id_pair* severity_info);
GKR_LOG_API int gkr_log_set_facility(void* channel, const struct gkr_log_name_id_pair* facility_info);

GKR_LOG_API int gkr_log_del_consumer_by_id(void* channel, int id);
GKR_LOG_API int gkr_log_del_all_consumers(void* channel);

#ifdef __cplusplus
}
#endif
