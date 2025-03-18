#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/consumer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_windows_debugger_consumer_callbacks {
    struct gkr_log_consumer_opt_callbacks opt_callbacks;

    const char* (*compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
};

GKR_LOG_API int gkr_log_add_windows_debugger_consumer(
    void* channel,
    void* param,
    const gkr_log_windows_debugger_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
