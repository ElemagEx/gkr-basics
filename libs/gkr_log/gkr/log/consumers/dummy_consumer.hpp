#pragma once

#include <gkr/capi/log/consumers/dummy_consumer.h>

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace log
{

class dummy_consumer : public consumer
{
public:
    GKR_LOG_API dummy_consumer() noexcept;
    GKR_LOG_API virtual ~dummy_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len, int flags);
};

}
}
