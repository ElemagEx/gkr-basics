#pragma once

#include <gkr/api.h>
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
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos,
    unsigned max_queue_entries,
    unsigned max_message_chars
    );
GKR_LOG_API int gkr_log_done();

GKR_LOG_API int gkr_log_get_severity_threshold();
GKR_LOG_API int gkr_log_set_severity_threshold(int threshold);

GKR_LOG_API int gkr_log_set_max_queue_entries(unsigned max_queue_entries);
GKR_LOG_API int gkr_log_set_max_message_chars(unsigned max_message_chars);

GKR_LOG_API int gkr_log_set_severities(int clear_existing, const struct gkr_log_name_id_pair* severities_infos);
GKR_LOG_API int gkr_log_set_facilities(int clear_existing, const struct gkr_log_name_id_pair* facilities_infos);

GKR_LOG_API int gkr_log_set_severity(const struct gkr_log_name_id_pair* severity_info);
GKR_LOG_API int gkr_log_set_facility(const struct gkr_log_name_id_pair* facility_info);

GKR_LOG_API int gkr_log_del_consumer_by_id(int id);
GKR_LOG_API int gkr_log_del_all_consumers();

#ifdef __cplusplus
}
#endif
