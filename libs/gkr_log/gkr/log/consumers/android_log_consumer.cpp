#include "android_log_consumer.hpp"

#include <gkr/diagnostics.h>

#include <cstdio>
#include <cstdlib>

#ifdef __ANDROID__
#include <android/log.h>
static void writeToAndroidLog(int priority, const char* tag, const char* text)
{
    __android_log_write(priority, tag, text);
}
#else
static void writeToAndroidLog(int priority, const char* tag, const char* text)
{
}
#endif

extern "C" {

int gkr_log_androidLog_getPriority(const struct gkr_log_message* msg)
{
#ifdef __ANDROID__
    return ANDROID_LOG_VERBOSE;
#else
    return 0;
#endif
}

const char* gkr_log_androidLog_getTag(const struct gkr_log_message* msg)
{
    return "";
}

unsigned gkr_log_androidLog_composeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg)
{
    const int len = std::snprintf(
        buf,
        cch,
        "[%s][%s][%s] - %s",
        msg->severityName,
        msg->facilityName,
        msg->threadName,
        msg->messageText
        );
    return unsigned(len);
}

struct data_t
{
    int         (*get_priority)(const struct gkr_log_message*);
    const char* (*get_tag)(const struct gkr_log_message*);
    unsigned    (*compose_output)(char*, unsigned, const struct gkr_log_message*);
    unsigned    cch;
    char        buf[1];
};

void* gkr_log_androidLog_createConsumerParam(
    unsigned buffer_capacity,
    int (*get_priority)(const struct gkr_log_message*),
    const char* (*get_tag)(const struct gkr_log_message*),
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    )
{
    Check_Arg_IsValid(buffer_capacity > 0, NULL);
    Check_Arg_NotNull(get_priority, NULL);
    Check_Arg_NotNull(get_tag, NULL);
    Check_Arg_NotNull(compose_output, NULL);

    data_t* data = static_cast<data_t*>(std::malloc(sizeof(data_t) + buffer_capacity - 1));

    if(data != nullptr)
    {
        data->get_priority   = get_priority;
        data->get_tag        = get_tag;
        data->compose_output = compose_output;
        data->cch            = buffer_capacity;
    }
    return data;
}

int gkr_log_androidLog_initLogging(void* param)
{
#ifdef __ANDROID__
    return (param != nullptr);
#else
    return false;
#endif
}

void gkr_log_androidLog_doneLogging(void* param)
{
    if(param != nullptr)
    {
        std::free(param);
    }
}

int gkr_log_androidLog_filterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void gkr_log_androidLog_consumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    (*data->compose_output)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    const char* tag      = (*data->get_tag)(msg);
    const int   priority = (*data->get_priority)(msg);

    writeToAndroidLog(priority, tag, data->buf);
}

}

namespace gkr
{
namespace log
{

android_log_consumer::android_log_consumer(unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

android_log_consumer::~android_log_consumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool android_log_consumer::init_logging()
{
#ifdef __ANDROID__
    return (param != nullptr);
#else
    return false;
#endif
}

void android_log_consumer::done_logging()
{
}

bool android_log_consumer::filter_log_message(const message& msg)
{
    return false;
}

void android_log_consumer::consume_log_message(const message& msg)
{
    compose_output(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    const char* tag      = get_tag(msg);
    const int   priority = get_priority(msg);

    writeToAndroidLog(priority, tag, m_buf);
}

int android_log_consumer::get_priority(const message& msg)
{
    return gkr_log_androidLog_getPriority(&msg);
}

const char* android_log_consumer::get_tag(const message& msg)
{
    return gkr_log_androidLog_getTag(&msg);
}

unsigned android_log_consumer::compose_output(char* buf, unsigned cch, const message& msg)
{
    return gkr_log_androidLog_composeOutput(buf, cch, &msg);
}

}
}
