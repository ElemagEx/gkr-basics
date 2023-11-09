#include "consoleConsumer.h"

#include <gkr/log/message.h>
#include <gkr/log/stamp.h>

#include <iostream>

namespace gkr
{
namespace log
{

consoleConsumer::consoleConsumer(unsigned bufferCch)
    : m_bufferPtr(nullptr)
    , m_bufferCch(bufferCch)
{
}

consoleConsumer::~consoleConsumer()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
    }
}

bool consoleConsumer::init_logging()
{
    if(m_bufferCch == 0      ) return false;
    if(m_bufferPtr != nullptr) return false;

    m_bufferPtr = new char[m_bufferCch];

    return true;
}

void consoleConsumer::done_logging()
{
    if(m_bufferPtr != nullptr)
    {
        delete [] m_bufferPtr;
        m_bufferPtr = nullptr;
    }
}

bool consoleConsumer::filter_log_message(const message& msg)
{
    return false;
}

void consoleConsumer::consume_log_message(const message& msg)
{
    if(m_bufferPtr == nullptr) return;

    composeOutput(m_bufferPtr, m_bufferCch, msg);

    m_bufferPtr[m_bufferCch - 1] = 0;

    std::cerr << m_bufferPtr << std::endl;
}

void consoleConsumer::composeOutput(char* buffer, unsigned cch, const message& msg)
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
