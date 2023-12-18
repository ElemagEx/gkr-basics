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
    const char* moduleName;
    const char* threadName;
    const char* severityName;
    const char* facilityName;
};
enum
{
    gkr_log_fo_flag_default            = 0x0000,
    gkr_log_fo_flag_use_inserts        = 0x0001,
    gkr_log_fo_flag_use_padding        = 0x0002,
    gkr_log_fo_flag_use_coloring       = 0x0004,
    gkr_log_fo_flag_use_utc_time       = 0x0008,
    gkr_log_fo_flag_ignore_time_fmt    = 0x0010,
    gkr_log_fo_flag_ignore_text_fmt    = 0x0020,
    gkr_log_fo_flag_unescape_text      = 0x0100,
    gkr_log_fo_flag_escape_text_dquote = 0x0200,
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

GKR_LOG_API int gkr_log_add_c_consumer(
    void* instance,
    const struct gkr_log_consumer_callbacks* callbacks
    );

GKR_LOG_API unsigned gkr_log_apply_time_format(
    char* buf,
    unsigned cch,
    const char* fmt,
    long long stamp,
    int flags
    );
GKR_LOG_API unsigned gkr_log_apply_text_format(
    char* buf,
    unsigned cch,
    const char* fmt,
    const struct gkr_log_message* msg,
    int flags,
    const char* const* args,
    unsigned rows,
    unsigned cols
    );

GKR_LOG_API const char* gkr_log_format_output(
    const char* fmt,
    const struct gkr_log_message* msg,
    int flags,
    const char* const* args,
    unsigned rows,
    unsigned cols,
    unsigned* len
    );

#ifdef __cplusplus
}
#endif
