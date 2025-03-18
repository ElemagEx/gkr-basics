#pragma once

#include <gkr/capi/log/consumers/android_log_consumer.h>

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace log
{

class android_log_consumer : public consumer
{
public:
    GKR_LOG_API android_log_consumer() noexcept;
    GKR_LOG_API virtual ~android_log_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len = nullptr, int flags = 0);

    GKR_LOG_API virtual const char* get_tag(const message& msg);

    GKR_LOG_API virtual int get_priority(const message& msg);
};

class android_log_consumer_ex : public android_log_consumer
{
    const char* m_tag;
    const int*  m_levels;
    unsigned    m_count;

public:
    android_log_consumer_ex(const char* tag, const int* levels, unsigned count) noexcept : m_tag(tag), m_levels(levels), m_count(count)
    {
    }

private:
    virtual const char* get_tag(const message& msg) override
    {
        return m_tag;
    }
    virtual int get_priority(const message& msg) override
    {
        return (unsigned(msg.severity) < m_count)
            ? 1
            : m_levels[msg.severity];
    }
};

}
}
