#pragma once

#include <gkr/capi/log/consumer.h>

#include <gkr/api.hpp>

namespace gkr
{
namespace log
{

using message = gkr_log_message;

class consumer
{
public:
    GKR_LOG_API virtual ~consumer();

    virtual bool init_logging() = 0;
    virtual void done_logging() = 0;

    virtual bool  filter_log_message(const message& msg) = 0;
    virtual void consume_log_message(const message& msg) = 0;
};

}
}
