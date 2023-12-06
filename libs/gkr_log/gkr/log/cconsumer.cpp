#pragma once

#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace log
{

template<class base_consumer>
class c_consumer_wrapper : base_consumer
{
    c_consumer_wrapper           (const c_consumer_wrapper&) noexcept = delete;
    c_consumer_wrapper& operator=(const c_consumer_wrapper&) noexcept = delete;

private:
    gkr_log_consumer_callbacks* m_callbacks;

protected:
    template<typename... Args>
    c_consumer_wrapper(Args&&... args) : base_consumer(args...)
    {
    }
    virtual ~c_consumer_wrapper() override
    {
        if((m_callbacks != nullptr) && (m_callbacks->destruct != nullptr))
        {
            (*m_callbacks->destruct)(m_callbacks->param);
        }
    }

    virtual bool init_logging() override
    {
        
    }
    virtual void done_logging() override
    {
    }

    virtual bool  filter_log_message(const message& msg) = 0;
    virtual void consume_log_message(const message& msg) = 0;

};

}
}
