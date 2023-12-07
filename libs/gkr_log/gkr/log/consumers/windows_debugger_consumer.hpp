#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/windows_debugger_callbacks.h>

namespace gkr
{
namespace log
{

class windows_debugger_consumer : public consumer
{
    char*    m_buf;
    unsigned m_cch;

public:
    GKR_LOG_API windows_debugger_consumer(unsigned buffer_capacity = 2*1024);
    GKR_LOG_API virtual ~windows_debugger_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual unsigned compose_output(const message& msg, char* buf, unsigned cch);
};

}
}
