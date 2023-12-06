#include "windows_debugger_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);

inline void outputToWindowsDebugger(const char* text)
{
    OutputDebugStringA(text);
    OutputDebugStringA("\n");
}
#else
inline void outputToWindowsDebugger(const char* text)
{
}
#endif

namespace gkr
{
namespace log
{
class c_windows_debugger_consumer : public windows_debugger_consumer
{
    c_windows_debugger_consumer           (const c_windows_debugger_consumer&) noexcept = delete;
    c_windows_debugger_consumer& operator=(const c_windows_debugger_consumer&) noexcept = delete;

    gkr_log_windows_debugger_consumer_callbacks m_callbacks {};

public:
    c_windows_debugger_consumer(c_windows_debugger_consumer&& other) noexcept
        : windows_debugger_consumer(std::move(other))
        , m_callbacks(other.m_callbacks)
    {
        other.m_callbacks = gkr_log_windows_debugger_consumer_callbacks();
    }
    c_windows_debugger_consumer& operator=(c_windows_debugger_consumer&& other) noexcept
    {
        windows_debugger_consumer::operator=(std::move(other));
        m_callbacks = other.m_callbacks;
        other.m_callbacks = gkr_log_windows_debugger_consumer_callbacks();
        return *this;
    }
    c_windows_debugger_consumer(
        gkr_log_windows_debugger_consumer_callbacks* callbacks,
        unsigned bufferCapacity
        )
        : windows_debugger_consumer(bufferCapacity)
    {
        if(callbacks != nullptr) {
            m_callbacks = *callbacks;
        }
    }
    virtual ~c_windows_debugger_consumer() override
    {
    }

protected:
    virtual unsigned compose_output(const message& msg, char* buf, unsigned cch) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, buf, cch);
        } else {
            return windows_debugger_consumer::compose_output(msg, buf, cch);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_windows_debugger_consumer(
    gkr_log_windows_debugger_consumer_callbacks* callbacks,
    unsigned bufferCapacity
    )
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_windows_debugger_consumer>(callbacks, bufferCapacity));
}

}

namespace gkr
{
namespace log
{

windows_debugger_consumer::windows_debugger_consumer(unsigned buffer_capacity)
    : m_buf(nullptr)
    , m_cch(buffer_capacity)
{
    Check_Arg_IsValid(buffer_capacity > 0, );

    m_buf = new char[buffer_capacity];
}

windows_debugger_consumer::~windows_debugger_consumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool windows_debugger_consumer::init_logging()
{
#ifdef _WIN32
    return (m_buf != nullptr);
#else
    return false;
#endif
}

void windows_debugger_consumer::done_logging()
{
}

bool windows_debugger_consumer::filter_log_message(const message& msg)
{
    return false;
}

void windows_debugger_consumer::consume_log_message(const message& msg)
{
    compose_output(msg, m_buf, m_cch);

    m_buf[m_cch - 1] = 0;

    outputToWindowsDebugger(m_buf);
}

unsigned windows_debugger_consumer::compose_output(const message& msg, char* buf, unsigned cch)
{
    struct tm tm;
    int ms = stamp_decompose(true, msg.stamp, tm) / 1000000;

    const int len = std::snprintf(
        buf,
        cch,
        "[%02d:%02d:%02d.%03d][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ms,
        msg.severityName,
        msg.facilityName,
        msg.threadName,
        msg.messageText
        );
    return unsigned(len);
}

}
}
