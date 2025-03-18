#pragma once

#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace log
{

class c_consumer : public consumer
{
    struct gkr_log_consumer_opt_callbacks m_opt_callbacks = { nullptr };

    void (*m_opt_consume)(void*, const struct gkr_log_message*) = nullptr;

protected:
    void* m_param = nullptr;

    c_consumer(void* param, const struct gkr_log_consumer_opt_callbacks& opt_callbacks) noexcept;

public:
    c_consumer(void* param, const struct gkr_log_consumer_raw_callbacks& raw_callbacks) noexcept;

    virtual ~c_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const message& msg) override;
    virtual void consume_log_message(const message& msg) override;
};

}
}
