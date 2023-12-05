#pragma once

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API void* gkr_log_udpSocket_createConsumerParam(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    );

GKR_LOG_API int  gkr_log_udpSocket_initLogging(void* param);
GKR_LOG_API void gkr_log_udpSocket_doneLogging(void* param);

GKR_LOG_API int  gkr_log_udpSocket_filterLogMessage (void* param, const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_udpSocket_consumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}
#endif
