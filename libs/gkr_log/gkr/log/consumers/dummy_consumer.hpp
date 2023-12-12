#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/dummy_consumer.h>

namespace gkr
{
namespace log
{

class dummy_consumer : public consumer
{
public:
    GKR_LOG_API dummy_consumer();
    GKR_LOG_API virtual ~dummy_consumer() override;

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
