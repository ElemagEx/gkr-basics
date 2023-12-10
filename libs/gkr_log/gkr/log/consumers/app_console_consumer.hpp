#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.h>

namespace gkr
{
namespace log
{

class app_console_consumer : public consumer
{
    int  m_method;
    bool m_isAtty;

public:
    GKR_LOG_API app_console_consumer(int method = gkr_log_appConsoleWriteMethod_stream2cout);
    GKR_LOG_API virtual ~app_console_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, bool colored);
};

}
}
