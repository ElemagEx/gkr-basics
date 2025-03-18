#include <gkr/defs.hpp>
#include "c_consumer.hxx"

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>

extern "C"
{
int gkr_log_add_raw_consumer(void* channel, void* param, const struct gkr_log_consumer_raw_callbacks* callbacks)
{
    Check_Arg_NotNull(callbacks, -1);
    Check_Arg_NotNull(callbacks->consume_log_message, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer(param, *callbacks));

    return gkr_log_add_consumer(channel, consumer);
}
}

namespace gkr
{
namespace log
{

c_consumer::c_consumer(void* param, const struct gkr_log_consumer_opt_callbacks& opt_callbacks) noexcept
    : m_opt_callbacks(opt_callbacks)
    , m_opt_consume(nullptr)
    , m_param(param)
{
}

c_consumer::c_consumer(void* param, const struct gkr_log_consumer_raw_callbacks& raw_callbacks) noexcept
    : m_opt_callbacks(raw_callbacks.opt_callbacks)
    , m_opt_consume(raw_callbacks.consume_log_message)
    , m_param(param)
{
}

c_consumer::~c_consumer()
{
    if(m_opt_callbacks.destruct != nullptr)
    {
        (*m_opt_callbacks.destruct)(m_param);
    }
}

bool c_consumer::init_logging()
{
    if(m_opt_callbacks.init_logging != nullptr)
    {
        return (0 != (*m_opt_callbacks.init_logging)(m_param));
    }
    else
    {
        return true;
    }
}

void c_consumer::done_logging()
{
    if(m_opt_callbacks.done_logging != nullptr)
    {
        (*m_opt_callbacks.done_logging)(m_param);
    }
}

bool c_consumer::filter_log_message(const message& msg)
{
    if(m_opt_callbacks.filter_log_message != nullptr)
    {
        return (0 != (*m_opt_callbacks.filter_log_message)(m_param, &msg));
    }
    else
    {
        return false;
    }
}

void c_consumer::consume_log_message(const message& msg)
{
    Assert_NotNullPtr(m_opt_consume);

    (*m_opt_consume)(m_param, &msg);
}

}
}
