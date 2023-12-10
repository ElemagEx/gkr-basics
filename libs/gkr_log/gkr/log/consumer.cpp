#include "consumer.hpp"
#include "logging.hpp"

#include <gkr/diagnostics.h>
#include <gkr/stamp.hpp>

namespace gkr
{
namespace log
{

class c_consumer : public consumer
{
    gkr_log_consumer_callbacks m_callbacks;

public:
    c_consumer(const struct gkr_log_consumer_callbacks& callbacks) : m_callbacks(callbacks)
    {
    }
    virtual ~c_consumer() override
    {
        Assert_NotNullPtr(m_callbacks.destruct);
        (*m_callbacks.destruct)(m_callbacks.param);
    }

protected:
    virtual bool init_logging() override
    {
        Assert_NotNullPtr(m_callbacks.init_logging);
        return (0 != (*m_callbacks.init_logging)(m_callbacks.param));
    }
    virtual void done_logging() override
    {
        Assert_NotNullPtr(m_callbacks.done_logging);
        (*m_callbacks.done_logging)(m_callbacks.param);
    }
    virtual bool filter_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.filter_log_message);
        return (0 != (*m_callbacks.filter_log_message)(m_callbacks.param, &msg));
    }
    virtual void consume_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.consume_log_message);
        (*m_callbacks.consume_log_message)(m_callbacks.param, &msg);
    }
};

}
}

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

    if((fmt[4] == CHAR_FORMAT) && (fmt[5] == CHAR_CONSOLE)) return true;

    if(!copy_str_text(1, pch, scl, "m")) return false;

    std::size_t
    len = (CCH_STYLE_BUF - scl);
    scl = (CCH_STYLE_BUF - 2);

    if((flags & gkr_log_fo_flag_use_coloring) == 0) return true;

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

    if((flags & gkr_log_fo_flag_use_inserts) == 0) return true;

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

    unsigned cch = gkr_log_apply_text_format(buf, unsigned(cap), arg, msg, flags, args, cols, rows);

    if(errno != 0) return false;

    buf += cch;
    cap -= cch;
    return true;
}
}

extern "C" {

int gkr_log_add_c_consumer(const struct gkr_log_consumer_callbacks* callbacks)
{
    Check_Arg_NotNull(callbacks, 0);
    Check_Arg_NotNull(callbacks->destruct, 0);
    Check_Arg_NotNull(callbacks->init_logging, 0);
    Check_Arg_NotNull(callbacks->done_logging, 0);
    Check_Arg_NotNull(callbacks-> filter_log_message, 0);
    Check_Arg_NotNull(callbacks->consume_log_message, 0);
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_consumer>(*callbacks));
}

unsigned gkr_log_apply_time_format(char* buf, unsigned cch, const char* fmt, long long stamp, int flags)
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

unsigned gkr_log_apply_text_format(char* buf, unsigned cch, const char* fmt, const struct gkr_log_message* msg, int flags, const char* const* args, unsigned cols, unsigned rows)
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

}
