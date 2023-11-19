#include "windowsDebuggerConsumer.h"

#include <gkr/diagnostics.h>
#include <gkr/log/message.h>
#include <gkr/stamp.h>

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);

static void outputToWindowsDebugger(const char* text)
{
    OutputDebugStringA(text);
    OutputDebugStringA("\n");
}
#else
static void outputToWindowsDebugger(const char* text)
{
}
#endif

extern "C" {

unsigned windowsDebuggerComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg)
{
    struct tm tm;
    int ns = gkr::stamp_decompose(true, msg->info.stamp, tm);

    const int len = std::snprintf(
        buf,
        cch,
        "[%02d:%02d:%02d.%03d][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ns,
        msg->severityName,
        msg->facilityName,
        msg->threadName,
        msg->messageText
        );
    return unsigned(len);
}

struct data_t
{
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*);
    unsigned cch;
    char     buf[1];
};

void* windowsDebuggerCreateConsumerParam(
    unsigned bufferCapacity,
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*)
    )
{
    Check_Arg_IsValid(bufferCapacity > 0, nullptr);
    Check_Arg_NotNull(composeOutput, nullptr);

    data_t* data = static_cast<data_t*>(std::malloc(sizeof(data_t) + bufferCapacity - 1));

    if(data != nullptr)
    {
        data->composeOutput = composeOutput;
        data->cch           = bufferCapacity;
    }
    return data;
}

int windowsDebuggerInitLogging(void* param)
{
#ifdef _WIN32
    return (param != nullptr);
#else
    return false;
#endif
}

void windowsDebuggerDoneLogging(void* param)
{
    if(param != nullptr)
    {
        std::free(param);
    }
}

int windowsDebuggerFilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void windowsDebuggerConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    (*data->composeOutput)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    outputToWindowsDebugger(data->buf);
}

}

windowsDebuggerConsumer::windowsDebuggerConsumer(unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

windowsDebuggerConsumer::~windowsDebuggerConsumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool windowsDebuggerConsumer::init_logging()
{
#ifdef _WIN32
    return (m_buf != nullptr);
#else
    return false;
#endif
}

void windowsDebuggerConsumer::done_logging()
{
}

bool windowsDebuggerConsumer::filter_log_message(const gkr::log::message& msg)
{
    return false;
}

void windowsDebuggerConsumer::consume_log_message(const gkr::log::message& msg)
{
    composeOutput(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    outputToWindowsDebugger(m_buf);
}

unsigned windowsDebuggerConsumer::composeOutput(char* buf, unsigned cch, const gkr::log::message& msg)
{
    return windowsDebuggerComposeOutput(m_buf, m_cch, &msg);
}
