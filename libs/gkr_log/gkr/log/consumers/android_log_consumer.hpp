#pragma once

#include <gkr/log/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/android_log_callbacks.h>

namespace gkr
{
namespace log
{

class android_log_consumer : public consumer
{
    android_log_consumer           (const android_log_consumer&) noexcept = delete;
    android_log_consumer& operator=(const android_log_consumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;

public:
    android_log_consumer(android_log_consumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
    }
    android_log_consumer& operator=(android_log_consumer&& other) noexcept
    {
        m_buf = other.m_buf;
        m_cch = other.m_cch;
        other.m_buf = nullptr;
        other.m_cch = 0;
        return *this;
    }

public:
    GKR_LOG_API android_log_consumer(unsigned bufferCapacity = 2*1024);
    GKR_LOG_API virtual ~android_log_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool filter_log_message(const message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual int get_priority(const message& msg);

    GKR_LOG_API virtual const char* get_tag(const message& msg);

    GKR_LOG_API virtual unsigned compose_output(char* buf, unsigned cch, const message& msg);
};

}
}
