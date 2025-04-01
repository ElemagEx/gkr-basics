#include <gkr/defs.hpp>
#include <gkr/log/consumers/dummy_consumer.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

extern "C"
{

int gkr_log_add_dummy_consumer(
    void* channel,
    void* param,
    const gkr_log_dummy_consumer_callbacks* callbacks
    )
{
    const struct gkr_log_consumer_aid_callbacks* aid_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->aid_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::log::dummy_consumer>(param, aid_callbacks));

    return gkr_log_add_consumer(channel, consumer);
}

}

namespace gkr
{
namespace log
{

dummy_consumer::dummy_consumer() noexcept
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
    compose_output(msg, nullptr, 0);
}

const char* dummy_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

}
}
