#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_udp_socket_consumer_callbacks {
    void *param;
};

GKR_LOG_API int gkr_log_add_udp_socket_consumer(
    void* instance,
    const gkr_log_udp_socket_consumer_callbacks* callbacks,
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize
    );

#ifdef __cplusplus
}
#endif
