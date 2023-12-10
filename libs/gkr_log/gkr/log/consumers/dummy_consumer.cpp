#include "dummy_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>

#include <cstdio>
#include <cstdlib>

namespace gkr
{
namespace log
{
class c_dummy_consumer : public dummy_consumer
{
    gkr_log_dummy_consumer_callbacks m_callbacks {};

public:
    c_dummy_consumer(const gkr_log_dummy_consumer_callbacks* callbacks)
        : dummy_consumer()
    {
        if(callbacks != nullptr) m_callbacks = *callbacks;
    }
    virtual ~c_dummy_consumer() override
    {
    }
};
}
}

extern "C" {

int gkr_log_add_dummy_consumer(const gkr_log_dummy_consumer_callbacks* callbacks)
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_dummy_consumer>(callbacks));
}

}

namespace gkr
{
namespace log
{

dummy_consumer::dummy_consumer()
{
}

dummy_consumer::~dummy_consumer()
{
}

bool dummy_consumer::init_logging()
{
    return true;
}

void dummy_consumer::done_logging()
{
}

bool dummy_consumer::filter_log_message(const message& msg)
{
    return false;
}

void dummy_consumer::consume_log_message(const message& msg)
{
}

}
}
