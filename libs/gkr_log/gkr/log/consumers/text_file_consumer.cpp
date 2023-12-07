#include "text_file_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>
#include <gkr/sys/process.hpp>

#include <cstdio>
#include <cstdlib>

inline int calcEoln(int eoln)
{
    int   res = 0;
    char* buf = reinterpret_cast<char*>(&res);

    buf[1] = buf[2] = 0;

    switch(eoln)
    {
        default:
        case gkr_log_textFileEoln_LF  : buf[0] = '\n'; break;
        case gkr_log_textFileEoln_CR  : buf[0] = '\r'; break;
        case gkr_log_textFileEoln_CRLF: buf[0] = '\r'; buf[1] = '\n'; break;
    }
    return res;
}
inline unsigned getEolnLen(int eoln)
{
    return (reinterpret_cast<const char*>(&eoln)[1] == 0) ? 1U : 2U;
}
inline void outputToTextFile(std::FILE* file, const char* text, unsigned len, const char* eoln, unsigned cch)
{
    std::fwrite(text, sizeof(char), len, file);
    std::fwrite(eoln, sizeof(char), cch, file);
}

namespace gkr
{
namespace log
{
class c_text_file_consumer : public text_file_consumer
{
    c_text_file_consumer           (const c_text_file_consumer&) noexcept = delete;
    c_text_file_consumer& operator=(const c_text_file_consumer&) noexcept = delete;

    gkr_log_text_file_consumer_callbacks m_callbacks {};

public:
    c_text_file_consumer(c_text_file_consumer&& other) noexcept
        : text_file_consumer(std::move(other))
        , m_callbacks(other.m_callbacks)
    {
        other.m_callbacks = gkr_log_text_file_consumer_callbacks();
    }
    c_text_file_consumer& operator=(c_text_file_consumer&& other) noexcept
    {
        text_file_consumer::operator=(std::move(other));
        m_callbacks = other.m_callbacks;
        other.m_callbacks = gkr_log_text_file_consumer_callbacks();
        return *this;
    }
    c_text_file_consumer(
        const gkr_log_text_file_consumer_callbacks* callbacks,
        const char* filepath = nullptr,
        int eoln = gkr_log_textFileEoln_Default,
        unsigned bufferCapacity = 2 * 1024
        )
        : text_file_consumer(filepath, eoln, bufferCapacity)
    {
        if(callbacks != nullptr) {
            m_callbacks = *callbacks;
        }
    }
    virtual ~c_text_file_consumer() override
    {
    }

protected:
    virtual unsigned compose_output(const message& msg, char* buf, unsigned cch) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, buf, cch);
        } else {
            return text_file_consumer::compose_output(msg, buf, cch);
        }
    }
    virtual void on_file_opened() override
    {
        if(m_callbacks.on_file_opened != nullptr) {
            return (*m_callbacks.on_file_opened)(m_callbacks.param);
        } else {
            return text_file_consumer::on_file_opened();
        }
    }
    virtual void on_file_closing() override
    {
        if(m_callbacks.on_file_closing != nullptr) {
            return (*m_callbacks.on_file_closing)(m_callbacks.param);
        } else {
            return text_file_consumer::on_file_closing();
        }
    }
    virtual void on_enter_file_write() override
    {
        if(m_callbacks.on_enter_file_write != nullptr) {
            return (*m_callbacks.on_enter_file_write)(m_callbacks.param);
        } else {
            return text_file_consumer::on_enter_file_write();
        }
    }
    virtual void on_leave_file_write() override
    {
        if(m_callbacks.on_leave_file_write != nullptr) {
            return (*m_callbacks.on_leave_file_write)(m_callbacks.param);
        } else {
            return text_file_consumer::on_leave_file_write();
        }
    }
};
}
}

extern "C" {

int gkr_log_add_text_file_consumer(
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln,
    unsigned bufferCapacity
    )
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_text_file_consumer>(callbacks, filepath, eoln, bufferCapacity));
}

}

namespace gkr
{
namespace log
{

text_file_consumer::text_file_consumer(
    const char* filepath,
    int eoln,
    unsigned bufferCapacity
    )
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
    , m_eoln(calcEoln(eoln))
    , m_file(nullptr)
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

text_file_consumer::~text_file_consumer()
{
    close_file();
    if(m_buf != nullptr) {
        delete [] m_buf;
    }
}

bool text_file_consumer::init_logging()
{
    if(m_buf == nullptr) return false;

    const unsigned head_len = unsigned(sys::get_current_process_path(m_buf, m_cch));

    if(head_len >= m_cch) {
        return false;
    }
    struct tm tm;
    stamp_decompose(true, stamp_now(), tm);

    const int tail_len = std::snprintf(
        m_buf + head_len,
        m_cch - head_len,
        "_%04i%02i%02i_%02i%02i%02i.log",
        tm.tm_year + 1900,
        tm.tm_mon  + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
        );
    if(tail_len <= 0) {
        return false;
    }
    m_name = m_buf;

    return open_file();
}

void text_file_consumer::done_logging()
{
    close_file();
}

bool text_file_consumer::filter_log_message(const message& msg)
{
    return false;
}

void text_file_consumer::consume_log_message(const message& msg)
{
    const unsigned len = compose_output(msg, m_buf, m_cch);

    m_buf[m_cch - 1] = 0;

    on_enter_file_write();
    outputToTextFile(static_cast<std::FILE*>(m_file), m_buf, len, reinterpret_cast<const char*>(&m_eoln), getEolnLen(m_eoln));
    on_leave_file_write();
}

unsigned text_file_consumer::compose_output(const message& msg, char* buf, unsigned cch)
{
    struct tm tm;
    int ns = stamp_decompose(true, msg.stamp, tm);

    const int len = std::snprintf(
        buf,
        cch,
        "[%02d:%02d:%02d.%03d][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ns / 1000000,
        msg.severityName,
        msg.facilityName,
        msg.threadName,
        msg.messageText
        );
    return unsigned(len);
}

void text_file_consumer::on_file_opened()
{
}

void text_file_consumer::on_file_closing()
{
}

void text_file_consumer::on_enter_file_write()
{
}

void text_file_consumer::on_leave_file_write()
{
}

bool text_file_consumer::open_file()
{
    m_file = std::fopen(m_buf, "wb");

    if(m_file != nullptr)
    {
        return false;
    }
    on_file_opened();
    return true;
}

void text_file_consumer::close_file()
{
    if(m_file != nullptr)
    {
        on_file_closing();
        std::fclose(static_cast<std::FILE*>(m_file));
        m_file = nullptr;
    }
}

}
}
