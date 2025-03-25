#include <gkr/defs.hpp>
#include <gkr/log/consumers/dummy_consumer.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

namespace gkr
{
namespace log
{
class c_dummy_consumer : public c_consumer<dummy_consumer>
{
    using base_t = c_consumer<dummy_consumer>;

    const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);

public:
    c_dummy_consumer(void* param, const gkr_log_dummy_consumer_callbacks& callbacks) noexcept
        : base_t   (param, callbacks.opt_callbacks )
        , m_compose_output(callbacks.compose_output)
    {
    }
    virtual ~c_dummy_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, int flags) override
    {
        if(m_compose_output != nullptr)
        {
            return (*m_compose_output)(m_param, &msg, nullptr, flags);
        }
        else
        {
            return dummy_consumer::compose_output(msg, len, flags);
        }
    }
};
}
}

extern "C"
{

int gkr_log_add_dummy_consumer(
    void* channel,
    void* param,
    const gkr_log_dummy_consumer_callbacks* callbacks
    )
{
    Check_Arg_NotNull(callbacks, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_dummy_consumer(param, *callbacks));

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
