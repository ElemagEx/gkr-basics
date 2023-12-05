#pragma once

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
    int  (*init_logging)(void*);
    void (*done_logging)(void*);

    int  (*filter_log_message)(void*, const struct gkr_log_message*);

    void (*consume_log_message)(void*, const struct gkr_log_message*);
};

#ifdef __cplusplus
}
#endif
