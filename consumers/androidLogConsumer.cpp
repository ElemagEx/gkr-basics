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

androidLogConsumer::androidLogConsumer(std::size_t bufferInitialCapacity)
    : m_buffer(bufferInitialCapacity)
{
    Check_ValidState(m_buffer.capacity() > 0);
}

androidLogConsumer::~androidLogConsumer() = default;

bool androidLogConsumer::init_logging()
{
    Check_ValidState(m_buffer.capacity() > 0, false);

#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

void androidLogConsumer::done_logging()
{
}

bool androidLogConsumer::filter_log_message(const message& msg)
{
    return false;
}

void androidLogConsumer::consume_log_message(const message& msg)
{
    const std::size_t cch = m_buffer.capacity();

    Check_ValidState(cch > 0, );

    char* buffer = m_buffer.data<char>();

    formatText(buffer, cch, msg);

    buffer[cch - 1] = 0;

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

const char* androidLogConsumer::getTag(const message& msg)
{
    return "";
}

void androidLogConsumer::formatText(char* buffer, std::size_t cch, const message& msg)
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

}
}
