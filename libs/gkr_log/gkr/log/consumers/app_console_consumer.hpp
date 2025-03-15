#pragma once

#include <gkr/capi/log/consumers/app_console_consumer.h>

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>

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
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len = nullptr, bool colored = false);
};

}
}
