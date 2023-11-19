#include <gkr/log/consumers/androidLogConsumer.h>

#include <gkr/diagnostics.h>
#include <gkr/log/message.h>
#include <gkr/comm/udpMessagePacket.h>

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

int androidLogGetPriority(const struct gkr_log_message* msg)
{
#ifdef __ANDROID__
    return ANDROID_LOG_VERBOSE;
#else
    return 0;
#endif
}

const char* androidLogGetTag(const struct gkr_log_message* msg)
{
    return "";
}

void androidLogFormatText(char* buf, unsigned cch, const struct gkr_log_message* msg)
{
    std::snprintf(
        buf,
        cch,
        "[%s][%s][%s] - %s",
        msg->severityName,
        msg->facilityName,
        msg->threadName,
        msg->messageText
        );
}

struct data_t
{
    int         (*getPriority)(const struct gkr_log_message*);
    const char* (*getTag)(const struct gkr_log_message*);
    void        (*formatText)(char*, unsigned, const struct gkr_log_message*);
    unsigned    cch;
    char        buf[1];
};

void* androidLogCreateConsumerParam(
    unsigned bufferCapacity,
    int (*getPriority)(const struct gkr_log_message*),
    const char* (*getTag)(const struct gkr_log_message*),
    void (*formatText)(char*, unsigned, const struct gkr_log_message*)
    )
{
    Check_Arg_IsValid(bufferCapacity > 0, NULL);
    Check_Arg_NotNull(getPriority, NULL);
    Check_Arg_NotNull(getTag, NULL);
    Check_Arg_NotNull(formatText, NULL);

    data_t* data = static_cast<data_t*>(std::malloc(sizeof(data_t) + bufferCapacity - 1));

    if(data != nullptr)
    {
        data->getPriority = getPriority;
        data->getTag      = getTag;
        data->formatText  = formatText;
        data->cch         = bufferCapacity;
    }
    return data;
}

int androidLogInitLogging(void* param)
{
#ifdef __ANDROID__
    return (param != nullptr);
#else
    return false;
#endif
}

void androidLogDoneLogging(void* param)
{
    if(param != nullptr)
    {
        std::free(param);
    }
}

int androidLogFilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void androidLogConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    (*data->formatText)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    const char* tag      = (*data->getTag)(msg);
    const int   priority = (*data->getPriority)(msg);

    writeToAndroidLog(priority, tag, data->buf);
}

}

androidLogConsumer::androidLogConsumer(unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

androidLogConsumer::~androidLogConsumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool androidLogConsumer::init_logging()
{
#ifdef __ANDROID__
    return (param != nullptr);
#else
    return false;
#endif
}

void androidLogConsumer::done_logging()
{
}

bool androidLogConsumer::filter_log_message(const gkr::log::message& msg)
{
    return false;
}

void androidLogConsumer::consume_log_message(const gkr::log::message& msg)
{
    formatText(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    const char* tag      = getTag(msg);
    const int   priority = getPriority(msg);

    writeToAndroidLog(priority, tag, m_buf);
}

int androidLogConsumer::getPriority(const gkr::log::message& msg)
{
    return androidLogGetPriority(&msg);
}

const char* androidLogConsumer::getTag(const gkr::log::message& msg)
{
    return androidLogGetTag(&msg);
}

void androidLogConsumer::formatText(char* buf, unsigned cch, const gkr::log::message& msg)
{
    androidLogFormatText(buf, cch, &msg);
}
