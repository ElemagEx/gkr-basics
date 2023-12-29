#pragma once

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.h>

namespace gkr
{
namespace log
{

class windows_debugger_consumer : public consumer
{
public:
    GKR_LOG_API windows_debugger_consumer();
    GKR_LOG_API virtual ~windows_debugger_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len = nullptr, bool colored = false);
};

}
}
