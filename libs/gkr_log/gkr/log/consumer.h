#pragma once

#include <gkr/api.h>

struct gkr_log_message
{
    long long   tid;
    long long   stamp;
    int         severity;
    int         facility;
    const char* sourceFunc;
    const char* sourceFile;
    unsigned    sourceLine;
    unsigned    messageLen;
    const char* messageText;
    const char* threadName;
    const char* severityName;
    const char* facilityName;
};

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_consumer_callbacks
{
    void  *param;
    void (*destruct)(void*);

    int  (*init_logging)(void*);
    void (*done_logging)(void*);

    int  (* filter_log_message)(void*, const struct gkr_log_message*);
    void (*consume_log_message)(void*, const struct gkr_log_message*);
};

GKR_LOG_API int gkr_log_add_c_consumer(const struct gkr_log_consumer_callbacks* callbacks);

#ifdef __cplusplus
}
#endif
