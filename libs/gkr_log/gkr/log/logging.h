#pragma once

#include <gkr/log/api.h>
#include <gkr/log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_name_id_pair
{
    const char* name;
    int         id;
};
struct gkr_log_functions;

GKR_LOG_API int gkr_log_init(
    const struct gkr_log_name_id_pair* severities_infos,
    const struct gkr_log_name_id_pair* facilities_infos,
    unsigned max_queue_entries,
    unsigned max_message_chars
    );
GKR_LOG_API int gkr_log_done();

GKR_LOG_API int gkr_log_set_max_queue_entries(unsigned max_queue_entries);
GKR_LOG_API int gkr_log_set_max_message_chars(unsigned max_message_chars);

GKR_LOG_API int gkr_log_set_severities(int clear_existing, const struct gkr_log_name_id_pair* severities_infos);
GKR_LOG_API int gkr_log_set_facilities(int clear_existing, const struct gkr_log_name_id_pair* facilities_infos);

GKR_LOG_API int gkr_log_set_severity(const struct gkr_log_name_id_pair* severity_info);
GKR_LOG_API int gkr_log_set_facility(const struct gkr_log_name_id_pair* facility_info);

GKR_LOG_API int gkr_log_add_functions(struct gkr_log_functions* functions, void* param);
GKR_LOG_API int gkr_log_del_functions(struct gkr_log_functions* functions, void* param);

GKR_LOG_API int gkr_log_del_all_consumers();

#ifdef __cplusplus
}
#endif
