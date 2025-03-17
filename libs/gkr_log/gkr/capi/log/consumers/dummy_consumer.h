#pragma once

#include <gkr/capi/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_dummy_consumer_callbacks
{
    void         *param;
    const char* (*compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
};

GKR_LOG_API int gkr_log_add_dummy_consumer(
    void* channel,
    const gkr_log_dummy_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
