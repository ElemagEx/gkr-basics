#include "consumer.hpp"
#include "galery.hpp"
#include "consumer.hpp"

#include <gkr/diagnostics.h>
#include <gkr/stamp.h>

#include <cstdio>
#include <cstring>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-macros"
#endif

#define STYLE_START                 "\033["
#define STYLE_RESET                 "0"
#define STYLE_END                   "m"
#define STYLE_SEPARATOR             ";"

#define STYLE_ITALIC                "3"
#define STYLE_BOLD                  "2"
#define STYLE_NORMAL                "1"

#define STYLE_TEXT_COLOR_BLACK      "30"
#define STYLE_TEXT_COLOR_DARK_BLUE  "34"
#define STYLE_TEXT_COLOR_DARK_GREEN "32"
#define STYLE_TEXT_COLOR_LIGHT_BLUE "36"
#define STYLE_TEXT_COLOR_DARK_RED   "31"
#define STYLE_TEXT_COLOR_MAGENTA    "35"
#define STYLE_TEXT_COLOR_ORANGE     "33"
#define STYLE_TEXT_COLOR_LIGHT_GRAY "37"
#define STYLE_TEXT_COLOR_GRAY       "90"
#define STYLE_TEXT_COLOR_BLUE       "94"
#define STYLE_TEXT_COLOR_GREEN      "92"
#define STYLE_TEXT_COLOR_CYAN       "96"
#define STYLE_TEXT_COLOR_RED        "91"
#define STYLE_TEXT_COLOR_PINK       "95"
#define STYLE_TEXT_COLOR_YELLOW     "93"
#define STYLE_TEXT_COLOR_WHITE      "97"
#define STYLE_TEXT_COLOR_DEFAULT    STYLE_TEXT_COLOR_LIGHT_GRAY

#define STYLE_BACK_COLOR_BLACK      "40"
#define STYLE_BACK_COLOR_DARK_BLUE  "44"
#define STYLE_BACK_COLOR_DARK_GREEN "42"
#define STYLE_BACK_COLOR_LIGHT_BLUE "46"
#define STYLE_BACK_COLOR_DARK_RED   "41"
#define STYLE_BACK_COLOR_MAGENTA    "45"
#define STYLE_BACK_COLOR_ORANGE     "43"
#define STYLE_BACK_COLOR_LIGHT_GRAY "47"
#define STYLE_BACK_COLOR_GRAY       "100"
#define STYLE_BACK_COLOR_BLUE       "104"
#define STYLE_BACK_COLOR_GREEN      "102"
#define STYLE_BACK_COLOR_CYAN       "106"
#define STYLE_BACK_COLOR_RED        "101"
#define STYLE_BACK_COLOR_PINK       "105"
#define STYLE_BACK_COLOR_YELLOW     "103"
#define STYLE_BACK_COLOR_WHITE      "107"
#define STYLE_BACK_COLOR_DEFAULT    STYLE_BACK_COLOR_BLACK

#define CMD_DEFAULT_STYLE           STYLE_START STYLE_TEXT_COLOR_DEFAULT STYLE_SEPARATOR STYLE_BACK_COLOR_DEFAULT STYLE_END
#define CMD_RESET_STYLE             STYLE_START STYLE_RESET STYLE_END

constexpr unsigned LEN_CMD_RESET_STYLE = sizeof(CMD_RESET_STYLE)-1;

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

unsigned gkr_log_format_output(const struct gkr_log_message* msg, char* buf, unsigned cch, int type, int color_scheme)
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

//
// PLOG
//

#define CMD_PLOG_STYLE_NONE  CMD_DEFAULT_STYLE
#define CMD_PLOG_STYLE_FATAL STYLE_START STYLE_TEXT_COLOR_WHITE  STYLE_SEPARATOR STYLE_BACK_COLOR_RED STYLE_END
#define CMD_PLOG_STYLE_ERROR STYLE_START STYLE_TEXT_COLOR_RED    STYLE_END
#define CMD_PLOG_STYLE_WARN  STYLE_START STYLE_TEXT_COLOR_YELLOW STYLE_END
#define CMD_PLOG_STYLE_INFO  CMD_DEFAULT_STYLE
#define CMD_PLOG_STYLE_DEBUG STYLE_START STYLE_TEXT_COLOR_CYAN   STYLE_END
#define CMD_PLOG_STYLE_VERB  STYLE_START STYLE_TEXT_COLOR_CYAN   STYLE_END

inline const char* plog_severity2string(int severity) noexcept
{
    constexpr const char* const SEVERITIES[] = {"NONE", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "VERB"};
    return (unsigned(severity) < unsigned(sizeof(SEVERITIES)/sizeof(char*)))
        ?  SEVERITIES[severity]
        : *SEVERITIES;
}
inline const char* plog_severity2style(int severity) noexcept
{
    constexpr const char* const STYLES[] = {CMD_PLOG_STYLE_NONE, CMD_PLOG_STYLE_FATAL, CMD_PLOG_STYLE_ERROR, CMD_PLOG_STYLE_WARN, CMD_PLOG_STYLE_INFO, CMD_PLOG_STYLE_DEBUG, CMD_PLOG_STYLE_VERB};
    return (unsigned(severity) < unsigned(sizeof(STYLES)/sizeof(char*)))
        ?  STYLES[severity]
        : *STYLES;
}

unsigned format_plog_csv_head(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    std::strncpy(buf, "Date;Time;Severity;TID;This;Function;Message", cch);
    return seal_buffer(buf, cch);
}
static unsigned format_plog_csv_text_both(int local, char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    struct tm tm;
    const int ms = gkr_stamp_decompose(local, msg.stamp, &tm) / 1000000;

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
        plog_severity2string(msg.severity),
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

static unsigned apply_color_scheme(const char* cmd_str, char* buf, unsigned cch, fmt_func_t func, const struct gkr_log_message& msg)
{
    Assert_NotNullPtr(buf);
    Assert_NotNullPtr(func);
    Assert_NotNullPtr(cmd_str);

    const unsigned cmd_len = unsigned(std::strlen(cmd_str));
    const unsigned fmt_len = cmd_len + LEN_CMD_RESET_STYLE;

    Check_ValidState((cch > fmt_len) && (cch - fmt_len) > 4, 0);

    std::strncpy(buf, cmd_str, cmd_len + 1);

    const unsigned str_len = (*func)(buf + cmd_len, cch - cmd_len, COLOR_SCHEME_NONE, msg);

    if(str_len == 0) return 0;

    const unsigned all_len = fmt_len + str_len;

    Check_ValidState(cch > all_len, 0);

    std::strncpy(buf + cmd_len + str_len, CMD_RESET_STYLE, LEN_CMD_RESET_STYLE + 1);

    return all_len;
}

unsigned format_plog_csv_text(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_csv_text, msg);
    }
    return format_plog_csv_text_both(true, buf, cch, color_scheme, msg);
}
unsigned format_plog_csv_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_csv_text_utc, msg);
    }
    return format_plog_csv_text_both(false, buf, cch, color_scheme, msg);
}
unsigned format_plog_msg_only(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_msg_only, msg);
    }
    std::strncpy(buf, msg.messageText, cch);
    return seal_buffer(buf, cch);
}
unsigned format_plog_msg_func(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_msg_func, msg);
    }
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
    const int ms = gkr_stamp_decompose(local, msg.stamp, &tm) / 1000000;

    int len = std::snprintf(
        buf,
        cch,
        "%04i-%02i-%02i %02i:%02i:%02i:%03i %-5s [%lli] [%s@%u] %s",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ms,
        plog_severity2string(msg.severity),
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
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_msg_text, msg);
    }
    return format_plog_msg_text_both(true, buf, cch, color_scheme, msg);
}
unsigned format_plog_msg_text_utc(char* buf, unsigned cch, int color_scheme, const struct gkr_log_message& msg)
{
    if(color_scheme != COLOR_SCHEME_NONE) {
        return apply_color_scheme(plog_severity2style(msg.severity), buf, cch, format_plog_msg_text_utc, msg);
    }
    return format_plog_msg_text_both(false, buf, cch, color_scheme, msg);
}
