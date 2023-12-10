#pragma once

#include <gkr/api.h>

enum
{
    gkr_log_fo_flag_default         = 0x0000,
    gkr_log_fo_flag_use_utc_time    = 0x0001,
    gkr_log_fo_flag_ignore_inserts  = 0x0002,
    gkr_log_fo_flag_ignore_padding  = 0x0004,
    gkr_log_fo_flag_ignore_coloring = 0x0008,
    gkr_log_fo_flag_ignore_time_fmt = 0x0010,
    gkr_log_fo_flag_ignore_text_fmt = 0x0020,
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned gkr_log_format_output_time(
    char* buf,
    unsigned cch,
    const char* fmt,
    long long stamp,
    int flags
    );
GKR_LOG_API unsigned gkr_log_format_output_text(
    char* buf,
    unsigned cch,
    const char* fmt,
    const struct gkr_log_message* msg,
    int flags,
    const char* const* args,
    unsigned cols,
    unsigned rows
    );

GKR_LOG_API unsigned gkr_log_format_output(const struct gkr_log_message* msg, char* buf, unsigned cch, int type, int color_scheme);

#ifdef __cplusplus
}
#endif

enum {
    FMT_TYPE_PLOG_CSV_HEAD     = 20,
    FMT_TYPE_PLOG_CSV_TEXT     = 21,
    FMT_TYPE_PLOG_CSV_TEXT_UTC = 22,
    FMT_TYPE_PLOG_MSG_ONLY     = 23,
    FMT_TYPE_PLOG_MSG_FUNC     = 24,
    FMT_TYPE_PLOG_MSG_TEXT     = 25,
    FMT_TYPE_PLOG_MSG_TEXT_UTC = 26,
};
enum {
    COLOR_SCHEME_NONE = 0,

    COLOR_SCHEME_PLOG_SEV_NONE    = 100,
    COLOR_SCHEME_PLOG_SEV_FATAL   = 101,
    COLOR_SCHEME_PLOG_SEV_ERROR   = 102,
    COLOR_SCHEME_PLOG_SEV_WARNING = 103,
    COLOR_SCHEME_PLOG_SEV_INFO    = 104,
    COLOR_SCHEME_PLOG_SEV_DEBUG   = 105,
    COLOR_SCHEME_PLOG_SEV_VERBOSE = 106,
};
