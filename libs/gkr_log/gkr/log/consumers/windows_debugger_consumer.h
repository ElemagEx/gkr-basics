#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_windows_debugger_consumer_callbacks {
    void         *param;
    const char* (*compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
};

GKR_LOG_API int gkr_log_add_windows_debugger_consumer(
    void* instance,
    const gkr_log_windows_debugger_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
