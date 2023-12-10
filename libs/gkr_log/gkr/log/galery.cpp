#include "galery.hpp"
#include "consumer.hpp"

#include <gkr/diagnostics.h>
#include <gkr/stamp.hpp>

#include <ctime>
#include <cerrno>
#include <cstdio>
#include <cstring>

namespace
{
using ulonglong = unsigned long long;

constexpr char CHAR_FORMAT   = '$';
constexpr char CHAR_INS_ARG  = 'I';
constexpr char CHAR_CONSOLE  = 'C';
constexpr char CHAR_LPADDING = 'P';
constexpr char CHAR_RPADDING = 'R';

constexpr int make_four_cc(const char* str)
{
    return int(
        (unsigned(str[1]) << 24) |
        (unsigned(str[2]) << 16) |
        (unsigned(str[3]) <<  8) |
        (unsigned(str[4]) <<  0) );
}
constexpr int FOUR_CC_TEXT = make_four_cc("$TEXT$"); // Message text

constexpr int FOUR_CC_TNAM = make_four_cc("$TNAM$"); // thread name
constexpr int FOUR_CC_SNAM = make_four_cc("$SNAM$"); // severity name
constexpr int FOUR_CC_FNAM = make_four_cc("$FNAM$"); // facility name

constexpr int FOUR_CC_T_ID = make_four_cc("$T_ID$"); // thread name
constexpr int FOUR_CC_S_ID = make_four_cc("$S_ID$"); // severity name
constexpr int FOUR_CC_F_ID = make_four_cc("$F_ID$"); // facility name

constexpr int FOUR_CC_FUNC = make_four_cc("$FUNC$"); // source function
constexpr int FOUR_CC_FILE = make_four_cc("$FILE$"); // source file
constexpr int FOUR_CC_LINE = make_four_cc("$LINE$"); // source line

constexpr int FOUR_CC_S_MS = make_four_cc("$S_MS$"); // milliseconds
constexpr int FOUR_CC_S_US = make_four_cc("$S_US$"); // microseconds
constexpr int FOUR_CC_S_NS = make_four_cc("$S_NS$"); //  nanoseconds

constexpr int FOUR_CC_STMP = make_four_cc("$STMP$"); // raw stamp

constexpr std::size_t CCH_STYLE_BUF  = 24;
constexpr std::size_t CCH_NUMBER_BUF = 24;

void advance_format(std::size_t count, const char*& fmt, std::size_t& len)
{
    Assert_Check(count <= len);
    fmt += count; len -= count;
}
bool copy_str_text(std::size_t count, char*& buf, std::size_t& cap, const char* str)
{
    if(count >= cap) return false;

    for( ; count-- > 0; --cap) *buf++ = *str++;

    return true;
}
bool copy_fmt_text(std::size_t count, char*& buf, std::size_t& cap, const char*& fmt, std::size_t& len)
{
    if(count >= cap) return false;

    std::memcpy(buf, fmt, count);

    buf += count; cap -= count;
    fmt += count; len -= count;
    return true;
}
bool copy_msg_txt(std::size_t count, char*& buf, std::size_t& cap, const char* text)
{
    if(count >= cap) return false;

    std::memcpy(buf, text, count);

    buf += count; cap -= count;
    return true;
}
bool copy_pad_txt(char ch, int& padding, char*& buf, std::size_t& cap, const char* text, std::size_t len, int flags)
{
    Check_Arg_NotNull(text, (errno = EINVAL), false);

    if((flags & gkr_log_fo_flag_ignore_padding) != 0) padding = 0;

    if(len == 0) len = std::strlen(text);

    if(padding > int(len))
    {
        std::size_t size = std::size_t(padding);
        if(size >= cap) return 0;
        size -= len;
        std::memcpy(buf, text, len);
        buf += len; cap -= len;
        std::memset(buf, ch, size);
        buf += size; cap -= size;
    }
    else if(-padding > int(len))
    {
        std::size_t size = std::size_t(-padding);
        if(size >= cap) return 0;
        size -= len;
        std::memset(buf, ch, size);
        buf += size; cap -= size;
        std::memcpy(buf, text, len);
        buf += len; cap -= len;
    }
    else
    {
        if(len >= cap) return 0;
        std::memcpy(buf, text, len);
        buf += len; cap -= len;
    }
    padding = 0;
    return true;
}
bool copy_pad_num(char ch, int& padding, char*& buf, std::size_t cap, ulonglong number, int flags)
{
    char text[CCH_NUMBER_BUF];
    const std::size_t len = std::size_t(std::snprintf(text, 24, "%llu", number));
    return copy_pad_txt(ch, padding, buf, cap, text, len, flags);
}
bool parse_padding(const char* fmt, char& ch, int& padding, int factor)
{
    ch = fmt[0]; if((ch < 32) || (ch > 126)) return false;

    const int digit1 = (fmt[1] - '0'); if((digit1 < 0) || (digit1 > 9)) return false;
    const int digit2 = (fmt[2] - '0'); if((digit2 < 0) || (digit2 > 9)) return false;

    padding = ((digit1 * 10) + digit2) * factor;
    return true;
}
bool parse_styling(const char* fmt, char* style, std::size_t& scl, char*& buf, std::size_t& cap, int flags)
{
    const int digit1 = (fmt[0] - '0'); if((digit1 < 0) || (digit1 > 9)) return false;
    const int digit2 = (fmt[1] - '0'); if((digit2 < 0) || (digit2 > 9)) return false;
    const int digit3 = (fmt[2] - '0'); if((digit3 < 0) || (digit3 > 9)) return false;

    Assert_Check(scl < CCH_STYLE_BUF);

    char* pch = style + (CCH_STYLE_BUF - scl);

    if(scl < (CCH_STYLE_BUF - 2)) if(!copy_str_text(1, pch, scl, ";")) return false;

    if     (digit1 > 0) { if(!copy_str_text(3, pch, scl, fmt + 0)) return false; }
    else if(digit2 > 0) { if(!copy_str_text(2, pch, scl, fmt + 1)) return false; }
    else                { if(!copy_str_text(1, pch, scl, fmt + 2)) return false; }

    if((fmt[4] == CHAR_FORMAT) && (fmt[5] == CHAR_CONSOLE)) return true;

    if(!copy_str_text(1, pch, scl, "m")) return false;

    std::size_t
    len = (CCH_STYLE_BUF - scl);
    scl = (CCH_STYLE_BUF - 2);

    if((flags & gkr_log_fo_flag_ignore_coloring) != 0) return true;

    return copy_str_text(len, buf, cap, style);
}
bool parse_ins_arg(const char* fmt, char*& buf, std::size_t& cap, const struct gkr_log_message* msg, int flags, const char* const* args, unsigned cols, unsigned rows)
{
    bool use_severity;
    switch(*fmt)
    {
        case 'S': use_severity = true ; break;
        case 'F': use_severity = false; break;
        default : return (errno = ENOTSUP), false;
    }
    const int digit1 = (fmt[1] - '0'); if((digit1 < 0) || (digit1 > 9)) return (errno = ENOTSUP), false;
    const int digit2 = (fmt[2] - '0'); if((digit2 < 0) || (digit2 > 9)) return (errno = ENOTSUP), false;

    if((flags & gkr_log_fo_flag_ignore_inserts) != 0) return true;

    const std::size_t col = std::size_t((digit1 * 10) + digit2);
    const std::size_t row = use_severity
        ? unsigned(msg->severity)
        : unsigned(msg->facility)
        ;
    if((col >= cols) || (row >= rows)) return (errno = ENOTSUP), false;

    const std::size_t index = (row * cols) + col;

    Check_Arg_NotNull(args       , (errno = EINVAL), false);
    Check_Arg_IsValid(cols > col , (errno = EINVAL), false);
    Check_Arg_IsValid(rows > row , (errno = EINVAL), false);
    Check_Arg_NotNull(args[index], (errno = EINVAL), false);

    const char* arg = args[index];

    unsigned cch = gkr_log_format_output_text(buf, unsigned(cap), arg, msg, flags, args, cols, rows);

    if(errno != 0) return false;

    buf += cch;
    cap -= cch;
    return true;
}
}

extern "C" {

unsigned gkr_log_format_output_time(char* buf, unsigned cch, const char* fmt, long long stamp, int flags)
{
    Check_Arg_NotNull(fmt    , (errno = EINVAL), 0);
    Check_Arg_NotNull(buf    , (errno = EINVAL), 0);
    Check_Arg_IsValid(cch > 0, (errno = EINVAL), 0);

    if((*fmt == 0) || ((flags & gkr_log_fo_flag_ignore_time_fmt) != 0))
    {
        *buf  = 0;
        errno = 0;
        return 0;
    }
    const bool local = ((flags & gkr_log_fo_flag_use_utc_time) == 0);

    struct tm tm;
    if(0 > gkr_stamp_decompose(local, stamp, &tm)) return 0;

    const std::size_t len = std::strftime(buf, cch, fmt, &tm);

    if(len == 0)
    {
        *buf  = 0;
        errno = ENOBUFS;
        return 0;
    }
    return unsigned(len);
}

unsigned gkr_log_format_output_text(char* buf, unsigned cch, const char* fmt, const struct gkr_log_message* msg, int flags, const char* const* args, unsigned cols, unsigned rows)
{
    Check_Arg_NotNull(msg    , (errno = EINVAL), 0);
    Check_Arg_NotNull(fmt    , (errno = EINVAL), 0);
    Check_Arg_NotNull(buf    , (errno = EINVAL), 0);
    Check_Arg_IsValid(cch > 0, (errno = EINVAL), 0);

    const bool local = ((flags & gkr_log_fo_flag_use_utc_time) == 0);

    struct tm tm;
    const int ns = gkr_stamp_decompose(local, msg->stamp, &tm);
    if(ns < 0) return 0;

    char ch = ' ';
    int padding = 0;
    char* start = buf;
    std::size_t cap = cch;
    std::size_t len = std::strlen(fmt);
    std::size_t scl = (CCH_STYLE_BUF - 2);
    char style[CCH_STYLE_BUF] = "\033[";

    if((flags & gkr_log_fo_flag_ignore_text_fmt) != 0)
    {
        fmt += len;
        len  = 0;
    }
    for(errno = ENOBUFS; ; )
    {
        const char* found = std::strchr(fmt, CHAR_FORMAT);

        if(found == nullptr)
        {
            if(copy_fmt_text(len, buf, cap, fmt, len)) errno = 0;
            break;
        }
        if(!copy_fmt_text(std::size_t(found - fmt), buf, cap, fmt, len)) break;

        if(fmt[1] == CHAR_FORMAT)
        {
            advance_format(1, fmt, len);
            if(copy_fmt_text(1, buf, cap, fmt, len)) continue;
            break;
        }
        if((len < 6) || (fmt[5] != CHAR_FORMAT))
        {
            errno = ENOTSUP;
            break;
        }
        advance_format(6, fmt, len);
        switch(*(fmt - 5))
        {
            case CHAR_LPADDING: if(parse_padding(fmt - 4, ch, padding, +1)) continue; errno = ENOTSUP; break;
            case CHAR_RPADDING: if(parse_padding(fmt - 4, ch, padding, -1)) continue; errno = ENOTSUP; break;

            case CHAR_CONSOLE : if(parse_styling(fmt - 4, style, scl, buf, cap, flags)) continue; errno = ENOTSUP; break;

            case CHAR_INS_ARG : if(parse_ins_arg(fmt - 4, buf, cap, msg, flags, args, cols, rows)) continue; break;

            default:
                switch(make_four_cc(fmt - 6))
                {
                    case FOUR_CC_TEXT: if(copy_msg_txt(msg->messageLen, buf, cap, msg->messageText)) continue; break;

                    case FOUR_CC_TNAM: if(copy_pad_txt(ch, padding, buf, cap, msg->threadName  , 0, flags)) continue; break;
                    case FOUR_CC_SNAM: if(copy_pad_txt(ch, padding, buf, cap, msg->severityName, 0, flags)) continue; break;
                    case FOUR_CC_FNAM: if(copy_pad_txt(ch, padding, buf, cap, msg->facilityName, 0, flags)) continue; break;
                    case FOUR_CC_FUNC: if(copy_pad_txt(ch, padding, buf, cap, msg->sourceFunc  , 0, flags)) continue; break;
                    case FOUR_CC_FILE: if(copy_pad_txt(ch, padding, buf, cap, msg->sourceFile  , 0, flags)) continue; break;

                    case FOUR_CC_T_ID: if(copy_pad_num(ch, padding, buf, cap, ulonglong(msg->tid       ), flags)) continue; break;
                    case FOUR_CC_S_ID: if(copy_pad_num(ch, padding, buf, cap, ulonglong(msg->severity  ), flags)) continue; break;
                    case FOUR_CC_F_ID: if(copy_pad_num(ch, padding, buf, cap, ulonglong(msg->facility  ), flags)) continue; break;
                    case FOUR_CC_LINE: if(copy_pad_num(ch, padding, buf, cap, ulonglong(msg->sourceLine), flags)) continue; break;
                    case FOUR_CC_STMP: if(copy_pad_num(ch, padding, buf, cap, ulonglong(msg->stamp     ), flags)) continue; break;
                    case FOUR_CC_S_MS: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1000000   ), flags)) continue; break;
                    case FOUR_CC_S_US: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1000      ), flags)) continue; break;
                    case FOUR_CC_S_NS: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1         ), flags)) continue; break;

                    default:
                        errno = ENOTSUP;
                        break;
                }
                break;
        }
        break;
    }
    if(cch == 0) return 0;
    *buf = 0;
    return unsigned(buf - start);
}

unsigned gkr_log_format_output(const struct gkr_log_message* msg, char* buf, unsigned cch, int type, int color_scheme)
{
    return 0;
}

}

#if 0
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
#endif
