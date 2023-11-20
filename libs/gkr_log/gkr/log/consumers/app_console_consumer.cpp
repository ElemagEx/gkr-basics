#include <gkr/log/consumers/app_console_consumer.h>

#include <gkr/diagnostics.h>
#include <gkr/log/message.h>
#include <gkr/stamp.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

static void outputToConsole(int method, const char* text)
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

        case gkr_log_appConsoleWriteMethod_stream2cerr: std::cerr << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2clog: std::clog << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2cout: std::cout << text << std::endl; break;
    }
}

extern "C" {

unsigned gkr_log_appConsole_ComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg)
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
        ns / 1000000,
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
    int      method;
    unsigned cch;
    char     buf[1];
};

void* gkr_log_appConsole_CreateConsumerParam(
    int method,
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
        data->method        = method;
        data->cch           = bufferCapacity;
    }
    return data;
}

int gkr_log_appConsole_InitLogging(void* param)
{
    return (param != nullptr);
}

void gkr_log_appConsole_DoneLogging(void* param)
{
    if(param != nullptr)
    {
        std::free(param);
    }
}

int gkr_log_appConsole_FilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void gkr_log_appConsole_ConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    (*data->composeOutput)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    outputToConsole(data->method, data->buf);
}

}

namespace gkr
{
namespace log
{

app_console_consumer::app_console_consumer(int method, unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
    , m_mth(method)
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
    compose_output(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    outputToConsole(m_mth, m_buf);
}

unsigned app_console_consumer::compose_output(char* buf, unsigned cch, const message& msg)
{
    return gkr_log_appConsole_ComposeOutput(m_buf, m_cch, &msg);
}

}
}
