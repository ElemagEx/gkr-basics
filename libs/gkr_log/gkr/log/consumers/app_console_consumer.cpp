#include "app_console_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>
#include <gkr/sys/file.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>

inline void outputToConsole(int method, const char* text)
{
    switch(method)
    {
        default:
            printf("%s\n", text);
            break;

        case gkr_log_appConsoleWriteMethod_fputs2stderr:
            std::fputs(text, stderr);
            std::fputs("\n", stderr);
            break;
        case gkr_log_appConsoleWriteMethod_fputs2stdout:
            std::fputs(text, stdout);
            std::fputs("\n", stdout);
            break;

        case gkr_log_appConsoleWriteMethod_stream2cout: std::cout << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2cerr: std::cerr << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2clog: std::clog << text << std::endl; break;
    }
}

inline bool isOutputAtty(int method)
{
    switch(method)
    {
        case gkr_log_appConsoleWriteMethod_fputs2stderr:
        case gkr_log_appConsoleWriteMethod_stream2cerr:
        case gkr_log_appConsoleWriteMethod_stream2clog:
            return gkr::sys::file_is_atty(stderr);

        case gkr_log_appConsoleWriteMethod_fputs2stdout:
        case gkr_log_appConsoleWriteMethod_stream2cout:
        default:
            return gkr::sys::file_is_atty(stdout);
    }
}

namespace gkr
{
namespace log
{
class c_app_console_consumer : public app_console_consumer
{
    c_app_console_consumer           (const c_app_console_consumer&) noexcept = delete;
    c_app_console_consumer& operator=(const c_app_console_consumer&) noexcept = delete;

    gkr_log_app_console_consumer_callbacks m_callbacks {};

public:
    c_app_console_consumer(c_app_console_consumer&& other) noexcept
        : app_console_consumer(std::move(other))
        , m_callbacks(other.m_callbacks)
    {
        other.m_callbacks = gkr_log_app_console_consumer_callbacks();
    }
    c_app_console_consumer& operator=(c_app_console_consumer&& other) noexcept
    {
        app_console_consumer::operator=(std::move(other));
        m_callbacks = other.m_callbacks;
        other.m_callbacks = gkr_log_app_console_consumer_callbacks();
        return *this;
    }
    c_app_console_consumer(
        const gkr_log_app_console_consumer_callbacks* callbacks,
        int method,
        unsigned bufferCapacity
        )
        : app_console_consumer(method, bufferCapacity)
    {
        if(callbacks != nullptr) {
            m_callbacks = *callbacks;
        }
    }
    virtual ~c_app_console_consumer() override
    {
    }

protected:
    virtual unsigned compose_output(const message& msg, char* buf, unsigned cch) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, buf, cch);
        } else {
            return app_console_consumer::compose_output(msg, buf, cch);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_app_console_consumer(
    const gkr_log_app_console_consumer_callbacks* callbacks,
    int method,
    unsigned bufferCapacity
    )
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_app_console_consumer>(callbacks, method, bufferCapacity));
}

}

namespace gkr
{
namespace log
{

app_console_consumer::app_console_consumer(int method, unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
    , m_method(method)
    , m_isAtty(isOutputAtty(method))
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

app_console_consumer::~app_console_consumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool app_console_consumer::init_logging()
{
#ifdef _WIN32
    return (m_buf != nullptr);
#else
    return false;
#endif
}

void app_console_consumer::done_logging()
{
}

bool app_console_consumer::filter_log_message(const message& msg)
{
    return false;
}

void app_console_consumer::consume_log_message(const message& msg)
{
    compose_output(msg, m_buf, m_cch);

    m_buf[m_cch - 1] = 0;

    outputToConsole(m_method, m_buf);
}

unsigned app_console_consumer::compose_output(const message& msg, char* buf, unsigned cch)
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

}
}
