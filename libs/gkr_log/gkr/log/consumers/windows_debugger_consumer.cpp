#include <gkr/log/consumers/windows_debugger_consumer.h>

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

unsigned gkr_log_windowsDebugger_ComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg)
{
    struct tm tm;
    int ns = gkr::stamp_decompose(true, msg->stamp, tm);

    const int len = std::snprintf(
        buf,
        cch,
        "[%02d:%02d:%02d.%03d][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ns / 1000000U,
        msg->severityName,
        msg->facilityName,
        msg->threadName,
        msg->messageText
        );
    return unsigned(len);
}

struct data_t
{
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*);
    unsigned cch;
    char     buf[1];
};

void* gkr_log_windowsDebugger_CreateConsumerParam(
    unsigned buffer_capacity,
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    )
{
    Check_Arg_IsValid(buffer_capacity > 0, nullptr);
    Check_Arg_NotNull(compose_output, nullptr);

    data_t* data = static_cast<data_t*>(std::malloc(sizeof(data_t) + buffer_capacity - 1));

    if(data != nullptr)
    {
        data->compose_output = compose_output;
        data->cch            = buffer_capacity;
    }
    return data;
}

int gkr_log_windowsDebugger_InitLogging(void* param)
{
#ifdef _WIN32
    return (param != nullptr);
#else
    return false;
#endif
}

void gkr_log_windowsDebugger_DoneLogging(void* param)
{
    if(param != nullptr)
    {
        std::free(param);
    }
}

int gkr_log_windowsDebugger_FilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void gkr_log_windowsDebugger_ConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    (*data->compose_output)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    outputToWindowsDebugger(data->buf);
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
    compose_output(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    outputToWindowsDebugger(m_buf);
}

unsigned windows_debugger_consumer::compose_output(char* buf, unsigned cch, const message& msg)
{
    return gkr_log_windowsDebugger_ComposeOutput(m_buf, m_cch, &msg);
}

}
}
