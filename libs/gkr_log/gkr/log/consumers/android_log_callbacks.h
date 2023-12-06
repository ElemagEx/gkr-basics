#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_android_log_consumer_callbacks {
    void         *param;
    int         (*get_priority  )(void*, const struct gkr_log_message*);
    const char* (*get_tag       )(void*, const struct gkr_log_message*);
    unsigned    (*compose_output)(void*, const struct gkr_log_message*, char*, unsigned);
};

GKR_LOG_API int gkr_log_add_android_log_consumer(
    gkr_log_android_log_consumer_callbacks* callbacks,
    unsigned bufferCapacity
    );

#ifdef __cplusplus
}
#endif
