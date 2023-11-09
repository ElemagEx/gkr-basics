#include "windowsDebuggerConsumer.h"

#include <gkr/log/message.h>
#include <gkr/log/stamp.h>

#include <cstdio>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif

namespace gkr
{
namespace log
{

windowsDebuggerConsumer::windowsDebuggerConsumer(unsigned bufferCch)
    : m_bufferPtr(nullptr)
    , m_bufferCch(bufferCch)
{
}

windowsDebuggerConsumer::~windowsDebuggerConsumer()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
    }
}

bool windowsDebuggerConsumer::init_logging()
{
    if(m_bufferCch == 0      ) return false;
    if(m_bufferPtr != nullptr) return false;

    m_bufferPtr = new char[m_bufferCch];

#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

void windowsDebuggerConsumer::done_logging()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
        m_bufferPtr = nullptr;
    }
}

bool windowsDebuggerConsumer::filter_log_message(const message& msg)
{
    return false;
}

void windowsDebuggerConsumer::consume_log_message(const message& msg)
{
    if(m_bufferPtr == nullptr) return;

    composeOutput(m_bufferPtr, m_bufferCch, msg);

    m_bufferPtr[m_bufferCch - 1] = 0;

#ifdef _WIN32
    OutputDebugStringA(m_bufferPtr);
    OutputDebugStringA("\n");
#endif
}

void windowsDebuggerConsumer::composeOutput(char* buffer, unsigned cch, const message& msg)
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
}
