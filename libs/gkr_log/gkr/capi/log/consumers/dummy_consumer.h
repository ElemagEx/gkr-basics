#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/consumer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_dummy_consumer_callbacks
{
    struct gkr_log_consumer_aid_callbacks aid_callbacks;
};

GKR_LOG_API int gkr_log_add_dummy_consumer(
    void* channel,
    void* param,
    const gkr_log_dummy_consumer_callbacks* callbacks
    );

#ifdef __cplusplus
}
#endif
