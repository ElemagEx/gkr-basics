#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_windows_debugger_consumer_callbacks {
    void      *param;
    unsigned (*compose_output)(void*, const struct gkr_log_message*, char*, unsigned);
};

GKR_LOG_API int gkr_log_add_windows_debugger_consumer(
    gkr_log_windows_debugger_consumer_callbacks* callbacks,
    unsigned bufferCapacity
    );

#ifdef __cplusplus
}
#endif
