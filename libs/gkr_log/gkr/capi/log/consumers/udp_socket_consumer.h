#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/consumer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_udp_socket_consumer_callbacks
{
    struct gkr_log_consumer_opt_callbacks opt_callbacks;
};

GKR_LOG_API int gkr_log_add_udp_socket_consumer(
    void* channel,
    void* param,
    const gkr_log_udp_socket_consumer_callbacks* callbacks,
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize
    );

#ifdef __cplusplus
}
#endif
