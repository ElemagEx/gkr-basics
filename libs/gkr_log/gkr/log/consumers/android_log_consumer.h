#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_android_log_consumer_callbacks
{
    void         *param;
    const char* (*compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
    const char* (*get_tag       )(void*, const struct gkr_log_message*);
    int         (*get_priority  )(void*, const struct gkr_log_message*);
};

GKR_LOG_API int gkr_log_add_android_log_consumer(
    void* instance,
    const gkr_log_android_log_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
