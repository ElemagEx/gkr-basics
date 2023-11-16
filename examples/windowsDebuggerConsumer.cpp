#include "windowsDebuggerConsumer.h"

#include <gkr/log/message.h>
#include <gkr/stamp.h>

#include <cstdio>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif

namespace gkr
{

windowsDebuggerConsumer::windowsDebuggerConsumer(std::size_t bufferCapacity)
    : m_buffer(bufferCapacity)
{
    Check_ValidState(m_buffer.capacity() > 0);
}

windowsDebuggerConsumer::~windowsDebuggerConsumer() = default;

bool windowsDebuggerConsumer::init_logging()
{
    Check_ValidState(m_buffer.capacity() > 0, false);

#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

void windowsDebuggerConsumer::done_logging()
{
}

bool windowsDebuggerConsumer::filter_log_message(const log::message& msg)
{
    return false;
}

void windowsDebuggerConsumer::consume_log_message(const log::message& msg)
{
    const std::size_t cch = m_buffer.capacity();

    Check_ValidState(cch > 0, );

    char* buffer = m_buffer.data<char>();

    composeOutput(buffer, cch, msg);

    buffer[cch - 1] = 0;

#ifdef _WIN32
    OutputDebugStringA(buffer);
    OutputDebugStringA("\n");
#endif
}

void windowsDebuggerConsumer::composeOutput(char* buffer, std::size_t cch, const log::message& msg)
{
    std::tm  tm;
    unsigned ns;

    gkr::log::decompose_stamp(true, msg.stamp, tm, ns);

    std::snprintf(
        buffer,
        cch,
        "[%02d:%02d:%02d.%03u][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ns,
        msg.severityName,
        msg.facilityName,
        msg.threadName,
        msg.messageText
        );
}

}
