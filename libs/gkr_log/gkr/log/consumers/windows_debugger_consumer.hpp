#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/windows_debugger_callbacks.h>

namespace gkr
{
namespace log
{

class windows_debugger_consumer : public consumer
{
    windows_debugger_consumer           (const windows_debugger_consumer&) noexcept = delete;
    windows_debugger_consumer& operator=(const windows_debugger_consumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;

public:
    windows_debugger_consumer(windows_debugger_consumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
    }
    windows_debugger_consumer& operator=(windows_debugger_consumer&& other) noexcept
    {
        m_buf = other.m_buf;
        m_cch = other.m_cch;
        other.m_buf = nullptr;
        other.m_cch = 0;
        return *this;
    }

public:
    GKR_LOG_API windows_debugger_consumer(unsigned buffer_capacity = 2*1024);
    GKR_LOG_API virtual ~windows_debugger_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual unsigned compose_output(char* buf, unsigned cch, const message& msg);
};

}
}
