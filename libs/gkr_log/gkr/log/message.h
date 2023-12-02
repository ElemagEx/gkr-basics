#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_message
{
    long long   tid;
    long long   stamp;
    int         severity;
    int         facility;
    unsigned    messageLen;
    const char* messageText;
    const char* threadName;
    const char* severityName;
    const char* facilityName;
};

#ifdef __cplusplus
}
#endif
