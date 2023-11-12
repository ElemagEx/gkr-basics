#include "consoleConsumer.h"

#include <gkr/log/message.h>
#include <gkr/log/stamp.h>

#include <iostream>

namespace gkr
{
namespace log
{

consoleConsumer::consoleConsumer(std::size_t bufferCapacity)
    : m_buffer(bufferCapacity)
{
    Check_ValidState(m_buffer.capacity() > 0);
}

consoleConsumer::~consoleConsumer() = default;

bool consoleConsumer::init_logging()
{
    Check_ValidState(m_buffer.capacity() > 0, false);

    return true;
}

void consoleConsumer::done_logging()
{
}

bool consoleConsumer::filter_log_message(const message& msg)
{
    return false;
}

void consoleConsumer::consume_log_message(const message& msg)
{
    const std::size_t cch = m_buffer.capacity();

    Check_ValidState(cch > 0, );

    char* buffer = m_buffer.data<char>();

    composeOutput(buffer, cch, msg);

    buffer[cch - 1] = 0;

    std::cerr << buffer << std::endl;
}

void consoleConsumer::composeOutput(char* buffer, std::size_t cch, const message& msg)
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
