#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_androidLog_getPriority(const struct gkr_log_message* msg);
GKR_LOG_API const char* gkr_log_androidLog_getTag(const struct gkr_log_message* msg);
GKR_LOG_API unsigned gkr_log_androidLog_composeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_androidLog_createConsumerParam(
    unsigned buffer_capacity,
    int (*get_priority)(const struct gkr_log_message*),
    const char* (*get_tag)(const struct gkr_log_message*),
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int  gkr_log_androidLog_initLogging(void* param);
GKR_LOG_API void gkr_log_androidLog_doneLogging(void* param);

GKR_LOG_API int  gkr_log_androidLog_filterLogMessage (void* param, const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_androidLog_consumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}
#endif
