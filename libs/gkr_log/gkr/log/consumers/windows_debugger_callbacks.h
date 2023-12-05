#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned gkr_log_windowsDebugger_composeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_windowsDebugger_createConsumerParam(
    unsigned buffer_capacity,
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int  gkr_log_windowsDebugger_initLogging(void* param);
GKR_LOG_API void gkr_log_windowsDebugger_doneLogging(void* param);

GKR_LOG_API int  gkr_log_windowsDebugger_filterLogMessage (void* param, const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_windowsDebugger_consumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}
#endif
