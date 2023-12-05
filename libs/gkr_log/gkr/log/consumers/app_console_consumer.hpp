#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/app_console_callbacks.h>

namespace gkr
{
namespace log
{

class app_console_consumer : public consumer
{
    app_console_consumer           (const app_console_consumer&) noexcept = delete;
    app_console_consumer& operator=(const app_console_consumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;
    int      m_mth;

public:
    app_console_consumer(app_console_consumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
        , m_mth(other.m_mth)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
        other.m_mth = 0;
    }
    app_console_consumer& operator=(app_console_consumer&& other) noexcept
    {
        m_buf = other.m_buf;
        m_cch = other.m_cch;
        m_mth = other.m_mth;
        other.m_buf = nullptr;
        other.m_cch = 0;
        other.m_mth = 0;
        return *this;
    }

public:
    GKR_LOG_API app_console_consumer(int method = gkr_log_appConsoleWriteMethod_stream2cout, unsigned bufferCapacity = 2*1024);
    GKR_LOG_API virtual ~app_console_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool filter_log_message(const message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual unsigned compose_output(char* buf, unsigned cch, const gkr::log::message& msg);
};

}
}
