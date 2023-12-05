#include "consumer.hpp"
#include "ids.h"

#include <gkr/diagnostics.h>
#include <gkr/stamp.h>

#include <cstdio>
#include <cstring>

static unsigned format_plog_csv_head    (char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_csv_text    (char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_csv_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_msg_only    (char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_msg_func    (char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_msg_text    (char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);
static unsigned format_plog_msg_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg);

typedef unsigned (*fmt_func_t)(char*, unsigned, int, const struct gkr_log_message&);

static fmt_func_t format_function[] = {
    /*  0*/ nullptr,
    /*  1*/ nullptr,
    /*  2*/ nullptr,
    /*  3*/ nullptr,
    /*  4*/ nullptr,
    /*  5*/ nullptr,
    /*  6*/ nullptr,
    /*  7*/ nullptr,
    /*  8*/ nullptr,
    /*  9*/ nullptr,
    /* 10*/ nullptr,
    /* 11*/ nullptr,
    /* 12*/ nullptr,
    /* 13*/ nullptr,
    /* 14*/ nullptr,
    /* 15*/ nullptr,
    /* 16*/ nullptr,
    /* 17*/ nullptr,
    /* 18*/ nullptr,
    /* 19*/ nullptr,
    /* 20*/ format_plog_csv_head,
    /* 21*/ format_plog_csv_text,
    /* 22*/ format_plog_csv_text_utc,
    /* 23*/ format_plog_msg_only,
    /* 24*/ format_plog_msg_func,
    /* 25*/ format_plog_msg_text,
    /* 26*/ format_plog_msg_text_utc,
    /* 27*/ nullptr,
    /* 28*/ nullptr,
    /* 29*/ nullptr,
    /* 30*/ nullptr,
    /* 31*/ nullptr,
};

extern "C" {

unsigned gkr_log_format_output(char* buf, unsigned cch, int type, int color_scheme, const struct gkr_log_message* msg)
{
    constexpr unsigned functions_count = sizeof(format_function)/sizeof(*format_function);

    Check_Arg_NotNull(msg, 0);
    Check_Arg_NotNull(buf, 0);
    Check_Arg_IsValid(cch >= 8, 0);
    Check_Arg_IsValid(unsigned(type) < functions_count, 0);
    Check_Arg_IsValid(format_function[type] != nullptr, 0);
 
    buf[cch - 1] = 0;

    fmt_func_t function = (unsigned(type) >= functions_count)
        ? nullptr
        : format_function[type]
        ;
    return (function == nullptr)
        ? 0
        : (*function)(buf, cch, color_scheme, *msg)
        ;
}

}

static unsigned seal_buffer(char* buf, unsigned cch)
{
    if(buf[cch - 1] != 0)
    {
        buf[cch - 1] = 0;
        buf[cch - 2] = buf[cch - 3] = buf[cch - 4] = '.';
    }
    return unsigned(std::strlen(buf));
}

inline constexpr const char* format_plog_severity2string(int severity) noexcept
{
    constexpr const char* const SEVERITIES[] = {"NONE", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "VERB"};
    return (unsigned(severity) < unsigned(sizeof(SEVERITIES)/sizeof(char*)))
        ?  SEVERITIES[severity]
        : *SEVERITIES;
}
unsigned format_plog_csv_head(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    std::strncpy(buf, "Date;Time;Severity;TID;This;Function;Message", cch);
    return seal_buffer(buf, cch);
}
static unsigned format_plog_csv_text_both(int local, char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    struct tm tm;
    const int ms = gkr_stamp_decompose(local, msg.stamp, &tm);

    //TODO:double quote not escaped
    int len = std::snprintf(
        buf,
        cch - 2,
        "%04i/%02i/%02i;%02i:%02i:%02i:%03i;%s;%lli;0000000000000000;%s@%u;\"%s",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ms,
        format_plog_severity2string(msg.severity),
        msg.tid,
        msg.sourceFunc,
        msg.sourceLine,
        msg.messageText
        );
    if(len <= 0) return 0;
    buf[len + 0]='\"';
    buf[len + 1]='\0';
    return unsigned(len + 1);
}
unsigned format_plog_csv_text(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    return format_plog_csv_text_both(true, buf, cch, color_scheme, msg);
}
unsigned format_plog_csv_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    return format_plog_csv_text_both(false, buf, cch, color_scheme, msg);
}
unsigned format_plog_msg_only(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    std::strncpy(buf, msg.messageText, cch);
    return seal_buffer(buf, cch);
}
unsigned format_plog_msg_func(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    int len = std::snprintf(
        buf,
        cch,
        "%s@%u: %s",
        msg.sourceFunc,
        msg.sourceLine,
        msg.messageText
        );
    if(len <= 0) return 0;
    return unsigned(len);
}
static unsigned format_plog_msg_text_both(int local, char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    struct tm tm;
    const int ms = gkr_stamp_decompose(local, msg.stamp, &tm);

    int len = std::snprintf(
        buf,
        cch,
        "%04i-%02i-%02i %02i:%02i:%02i:%03i %5s [%lli] [%s@%u] %s",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ms,
        format_plog_severity2string(msg.severity),
        msg.tid,
        msg.sourceFunc,
        msg.sourceLine,
        msg.messageText
        );
    if(len <= 0) return 0;
    return unsigned(len);
}
unsigned format_plog_msg_text(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    return format_plog_msg_text_both(true, buf, cch, color_scheme, msg);
}
unsigned format_plog_msg_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    return format_plog_msg_text_both(false, buf, cch, color_scheme, msg);
}
