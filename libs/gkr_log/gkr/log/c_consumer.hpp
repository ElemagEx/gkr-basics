#pragma once

#include <gkr/log/consumer.hpp>

#include <utility>

namespace gkr
{
namespace log
{

template<class ParentConsumer>
class c_consumer : public ParentConsumer
{
    struct gkr_log_consumer_opt_callbacks m_opt_callbacks;

    void (*m_opt_consume)(void*, const struct gkr_log_message*);

protected:
    void* m_param = nullptr;

    template<typename... Args>
    c_consumer(void* param, const struct gkr_log_consumer_opt_callbacks& opt_callbacks, Args&&... args)
        : ParentConsumer (std::forward<Args>(args)...)
        , m_opt_callbacks(opt_callbacks)
        , m_opt_consume  (nullptr)
        , m_param        (param)
    {
    }

public:
    virtual ~c_consumer() override
    {
        if(m_opt_callbacks.destruct != nullptr)
        {
            (*m_opt_callbacks.destruct)(m_param);
        }
    }

protected:
    virtual bool init_logging() override
    {
        if(!ParentConsumer::init_logging()) return false;

        if(m_opt_callbacks.init_logging != nullptr)
        {
            return (0 != (*m_opt_callbacks.init_logging)(m_param));
        }
        else
        {
            return true;
        }
    }
    virtual void done_logging() override
    {
        if(m_opt_callbacks.init_logging != nullptr)
        {
            (*m_opt_callbacks.done_logging)(m_param);
        }
        ParentConsumer::done_logging();
    }
    virtual bool filter_log_message(const message& msg) override
    {
        if(ParentConsumer::filter_log_message(msg)) return true;

        if(m_opt_callbacks.filter_log_message != nullptr)
        {
            return (0 != (*m_opt_callbacks.filter_log_message)(m_param, &msg));
        }
        else
        {
            return false;
        }
    }
    virtual void consume_log_message(const message& msg) override
    {
        if(m_opt_consume != nullptr)
        {
            (*m_opt_consume)(m_param, &msg);
        }
        else
        {
            ParentConsumer::consume_log_message(msg);
        }
    }
};
template<>
class c_consumer<consumer> : public consumer
{
    struct gkr_log_consumer_opt_callbacks m_opt_callbacks;

    void (*m_opt_consume)(void*, const struct gkr_log_message*);

protected:
    void* m_param = nullptr;

    template<typename... Args>
    c_consumer(void* param, const struct gkr_log_consumer_opt_callbacks& opt_callbacks, Args&&... args) noexcept
        : m_opt_callbacks(opt_callbacks)
        , m_opt_consume  (nullptr)
        , m_param        (param)
    {
    }

public:
    template<typename... Args>
    c_consumer(void* param, const struct gkr_log_consumer_raw_callbacks& raw_callbacks, Args&&... args) noexcept
        : m_opt_callbacks(raw_callbacks.opt_callbacks)
        , m_opt_consume  (raw_callbacks.consume_log_message)
        , m_param        (param)
    {
    }
    virtual ~c_consumer() override
    {
        if(m_opt_callbacks.destruct != nullptr)
        {
            (*m_opt_callbacks.destruct)(m_param);
        }
    }

protected:
    virtual bool init_logging() override
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
    virtual void done_logging() override
    {
        if(m_opt_callbacks.init_logging != nullptr)
        {
            (*m_opt_callbacks.done_logging)(m_param);
        }
    }
    virtual bool filter_log_message(const message& msg) override
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
    virtual void consume_log_message(const message& msg) override
    {
        if(m_opt_consume != nullptr)
        {
            (*m_opt_consume)(m_param, &msg);
        }
    }
};

}
}
