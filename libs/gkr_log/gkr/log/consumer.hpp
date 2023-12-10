#pragma once

#include <gkr/log/consumer.h>

namespace gkr
{
namespace log
{

using message = gkr_log_message;

class consumer
{
public:
    virtual ~consumer() = default;

    virtual bool init_logging() = 0;
    virtual void done_logging() = 0;

    virtual bool  filter_log_message(const message& msg) = 0;
    virtual void consume_log_message(const message& msg) = 0;
};

}
}

#define PLOG_TEXT