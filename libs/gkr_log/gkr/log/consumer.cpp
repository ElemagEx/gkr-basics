#include <gkr/defs.hpp>
#include <gkr/log/consumer.hpp>

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

#include <cstring>
#include <ctime>

namespace gkr
{
namespace log
{
consumer::~consumer()
{
}
}
}
extern "C"
int gkr_log_add_raw_consumer(void* channel, void* param, const struct gkr_log_consumer_raw_callbacks* callbacks)
{
    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::log::consumer>(param, callbacks));

    return gkr_log_add_consumer(channel, consumer);
}

namespace
{
using ulonglong = unsigned long long;

constexpr char CHAR_FMT_ENTER = '<';
constexpr char CHAR_FMT_LEAVE = '>';
constexpr char CHAR_INS_ARG   = 'I';
constexpr char CHAR_CONSOLE   = 'C';
constexpr char CHAR_LPADDING  = 'P';
constexpr char CHAR_RPADDING  = 'R';

constexpr int make_four_cc(const char* str)
{
    return int(
        (unsigned(str[1]) << 24) |
        (unsigned(str[2]) << 16) |
        (unsigned(str[3]) <<  8) |
        (unsigned(str[4]) <<  0) );
}
constexpr int FOUR_CC_TEXT = make_four_cc("<TEXT>"); // message text

constexpr int FOUR_CC_CNAM = make_four_cc("<CNAM>"); // channel name
constexpr int FOUR_CC_TNAM = make_four_cc("<TNAM>"); // thread name
constexpr int FOUR_CC_SNAM = make_four_cc("<SNAM>"); // severity name
constexpr int FOUR_CC_FNAM = make_four_cc("<FNAM>"); // facility name

constexpr int FOUR_CC_T_ID = make_four_cc("<T_ID>"); // thread name
constexpr int FOUR_CC_S_ID = make_four_cc("<S_ID>"); // severity name
constexpr int FOUR_CC_F_ID = make_four_cc("<F_ID>"); // facility name

constexpr int FOUR_CC_FUNC = make_four_cc("<FUNC>"); // source function
constexpr int FOUR_CC_FILE = make_four_cc("<FILE>"); // source file
constexpr int FOUR_CC_LINE = make_four_cc("<LINE>"); // source line

constexpr int FOUR_CC_MSEC = make_four_cc("<MSEC>"); // milliseconds
constexpr int FOUR_CC_USEC = make_four_cc("<USEC>"); // microseconds
constexpr int FOUR_CC_NSEC = make_four_cc("<NSEC>"); //  nanoseconds

constexpr int FOUR_CC_STMP = make_four_cc("<STMP>"); // raw stamp

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
bool copy_msg_txt(std::size_t count, char*& buf, std::size_t& cap, const char* text, int flags)
{
    if((flags & gkr_log_fo_flag_escape_text_dquote) == 0)
    {
        return copy_fmt_text(count, buf, cap, text, count);
    }
    for( ; ; )
    {
        const char* found = std::strchr(text, '\"');

        if((found == nullptr) || std::size_t(found - text) >= count)
        {
            return copy_fmt_text(count, buf, cap, text, count);
        }
        if(!copy_fmt_text(std::size_t(found - text), buf, cap, text, count)) return false;
        if(!copy_str_text(2, buf, cap, "\\\"")) return false;
        advance_format(1, text, count);
    }
}
bool copy_pad_txt(char ch, int& padding, char*& buf, std::size_t& cap, const char* text, std::size_t len, int flags)
{
    Check_Arg_NotNull(text, false);

    if((flags & gkr_log_fo_flag_use_padding) == 0) padding = 0;

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

    if((fmt[4] == CHAR_FMT_ENTER) && (fmt[5] == CHAR_CONSOLE)) return true;

    if(!copy_str_text(1, pch, scl, "m")) return false;

    std::size_t
    len = (CCH_STYLE_BUF - scl);
    scl = (CCH_STYLE_BUF - 2);

    if((flags & gkr_log_fo_flag_use_coloring) == 0) return true;

    return copy_str_text(len, buf, cap, style);
}
bool parse_ins_arg(const char* fmt, char*& buf, std::size_t& cap, const struct gkr_log_message* msg, int flags, const char* const* args, unsigned rows, unsigned cols)
{
    bool use_severity;
    switch(*fmt)
    {
        case 'S': use_severity = true ; break;
        case 'F': use_severity = false; break;
        default : return void(errno = ENOTSUP), false;
    }
    const int digit1 = (fmt[1] - '0'); if((digit1 < 0) || (digit1 > 9)) return void(errno = ENOTSUP), false;
    const int digit2 = (fmt[2] - '0'); if((digit2 < 0) || (digit2 > 9)) return void(errno = ENOTSUP), false;

    if((flags & gkr_log_fo_flag_use_inserts) == 0) return true;

    const std::size_t row = std::size_t((digit1 * 10) + digit2);
    const std::size_t col = use_severity
        ? unsigned(msg->severity)
        : unsigned(msg->facility)
        ;
    if((row >= rows) || (col >= cols)) return void(errno = ENOTSUP), false;

    const std::size_t index = (row * cols) + col;

    Check_Arg_NotNull(args       , false);
    Check_Arg_IsValid(rows > row , false);
    Check_Arg_IsValid(cols > col , false);
    Check_Arg_NotNull(args[index], false);

    const char* arg = args[index];

    flags &= ~(gkr_log_fo_flag_remove_trail_space | gkr_log_fo_flag_append_eoln_mask);

    unsigned cch = gkr_log_apply_text_format(buf, unsigned(cap), arg, msg, flags, args, rows, cols);

    if(errno != 0) return false;

    buf += cch;
    cap -= cch;
    return true;
}
}

extern "C"
{

unsigned gkr_log_apply_time_format(char* buf, unsigned cch, const char* fmt, long long stamp, int flags)
{
    Check_Arg_NotNull(fmt    , 0U);
    Check_Arg_NotNull(buf    , 0U);
    Check_Arg_IsValid(cch > 0, 0U);

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

unsigned gkr_log_apply_text_format(char* buf, unsigned cch, const char* fmt, const struct gkr_log_message* msg, int flags, const char* const* args, unsigned rows, unsigned cols)
{
    Check_Arg_NotNull(msg    , 0U);
    Check_Arg_NotNull(fmt    , 0U);
    Check_Arg_NotNull(buf    , 0U);
    Check_Arg_IsValid(cch > 0, 0U);

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
        const char* found = std::strchr(fmt, CHAR_FMT_ENTER);

        if(found == nullptr)
        {
            if(copy_fmt_text(len, buf, cap, fmt, len)) errno = 0;
            break;
        }
        if(!copy_fmt_text(std::size_t(found - fmt), buf, cap, fmt, len)) break;

        if(fmt[1] == CHAR_FMT_ENTER)
        {
            advance_format(1, fmt, len);
            if(copy_fmt_text(1, buf, cap, fmt, len)) continue;
            break;
        }
        if((len < 6) || (fmt[5] != CHAR_FMT_LEAVE))
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

            case CHAR_INS_ARG : if(parse_ins_arg(fmt - 4, buf, cap, msg, flags, args, rows, cols)) continue; break;

            default:
                switch(make_four_cc(fmt - 6))
                {
                    case FOUR_CC_TEXT: if(copy_msg_txt(msg->messageLen, buf, cap, msg->messageText, flags)) continue; break;

                    case FOUR_CC_CNAM: if(copy_pad_txt(ch, padding, buf, cap, msg->channelName , 0, flags)) continue; break;
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
                    case FOUR_CC_MSEC: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1000000   ), flags)) continue; break;
                    case FOUR_CC_USEC: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1000      ), flags)) continue; break;
                    case FOUR_CC_NSEC: if(copy_pad_num(ch, padding, buf, cap, ulonglong(ns / 1         ), flags)) continue; break;

                    default:
                        errno = ENOTSUP;
                        break;
                }
                break;
        }
        break;
    }
    if(flags & gkr_log_fo_flag_remove_trail_space)
    {
        for( ; (buf > start) && std::isspace(*(buf - 1)); --buf, ++cap);
    }
    switch(flags & gkr_log_fo_flag_append_eoln_mask)
    {
        case gkr_log_fo_flag_append_eoln_lf  : if(copy_str_text(1, buf, cap, "\n"  )) break; errno = ENOTSUP; return 0;
        case gkr_log_fo_flag_append_eoln_cr  : if(copy_str_text(1, buf, cap, "\r"  )) break; errno = ENOTSUP; return 0;
        case gkr_log_fo_flag_append_eoln_crlf: if(copy_str_text(2, buf, cap, "\r\n")) break; errno = ENOTSUP; return 0;
    }
    if(cap == 0) return 0;
    *buf = 0;
    return unsigned(buf - start);
}

}
