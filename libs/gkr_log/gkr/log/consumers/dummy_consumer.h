#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_dummy_consumer_callbacks {
    void* param;
};

GKR_LOG_API int gkr_log_add_dummy_consumer(
    const gkr_log_dummy_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
