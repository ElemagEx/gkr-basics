#include "androidLogConsumer.h"

#include <gkr/log/message.h>

#include <cstdio>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

namespace gkr
{
namespace log
{

androidLogConsumer::androidLogConsumer(unsigned bufferCch)
    : m_bufferPtr(nullptr)
    , m_bufferCch(bufferCch)
{
}

androidLogConsumer::~androidLogConsumer()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
    }
}

bool androidLogConsumer::init_logging()
{
    if(m_bufferCch == 0      ) return false;
    if(m_bufferPtr != nullptr) return false;

    m_bufferPtr = new char[m_bufferCch];

#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

void androidLogConsumer::done_logging()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
        m_bufferPtr = nullptr;
    }
}

bool androidLogConsumer::filter_log_message(const message& msg)
{
    return false;
}

void androidLogConsumer::consume_log_message(const message& msg)
{
    if(m_bufferPtr == nullptr) return;

    formatText(m_bufferPtr, m_bufferCch, msg);

    m_bufferPtr[m_bufferCch - 1] = 0;

#if defined(__ANDROID__)
    const char* tag      = getTag(msg);
    const int   priority = getPriority(msg);

    __android_log_write(priority, tag, m_bufferPtr);
#endif
}

int androidLogConsumer::getPriority(const message& msg)
{
#if defined(__ANDROID__)
    return ANDROID_LOG_VERBOSE;
#else
    return 0;
#endif
}

void androidLogConsumer::formatText(char* buffer, unsigned cch, const message& msg)
{
    std::snprintf(
        buffer,
        cch,
        "[%s][%s][%s] - %s",
        msg.severityName,
        msg.facilityName,
        msg.threadName,
        msg.messageText
        );
}

const char* androidLogConsumer::getTag(const message& msg)
{
    return "";
}

}
}
