#include "consumer.hpp"
#include "logging.hpp"

#include <gkr/diagnostics.h>

namespace gkr
{
namespace log
{

class c_consumer : public consumer
{
    gkr_log_consumer_callbacks m_callbacks;

public:
    c_consumer(const struct gkr_log_consumer_callbacks& callbacks) : m_callbacks(callbacks)
    {
    }
    virtual ~c_consumer() override
    {
        Assert_NotNullPtr(m_callbacks.destruct);
        (*m_callbacks.destruct)(m_callbacks.param);
    }

protected:
    virtual bool init_logging() override
    {
        Assert_NotNullPtr(m_callbacks.init_logging);
        return (0 != (*m_callbacks.init_logging)(m_callbacks.param));
    }
    virtual void done_logging() override
    {
        Assert_NotNullPtr(m_callbacks.done_logging);
        (*m_callbacks.done_logging)(m_callbacks.param);
    }
    virtual bool filter_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.filter_log_message);
        return (0 != (*m_callbacks.filter_log_message)(m_callbacks.param, &msg));
    }
    virtual void consume_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.consume_log_message);
        (*m_callbacks.consume_log_message)(m_callbacks.param, &msg);
    }
};

}
}

#ifdef __cplusplus
extern "C" {
#endif

int gkr_log_add_c_consumer(const struct gkr_log_consumer_callbacks* callbacks)
{
    Check_Arg_NotNull(callbacks, 0);
    Check_Arg_NotNull(callbacks->destruct, 0);
    Check_Arg_NotNull(callbacks->init_logging, 0);
    Check_Arg_NotNull(callbacks->done_logging, 0);
    Check_Arg_NotNull(callbacks-> filter_log_message, 0);
    Check_Arg_NotNull(callbacks->consume_log_message, 0);
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_consumer>(*callbacks));
}


#ifdef __cplusplus
}
#endif
