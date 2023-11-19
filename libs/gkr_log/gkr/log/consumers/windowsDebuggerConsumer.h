#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned windowsDebuggerComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* windowsDebuggerCreateConsumerParam(
    unsigned bufferCapacity,
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int windowsDebuggerInitLogging(void* param);

GKR_LOG_API void windowsDebuggerDoneLogging(void* param);

GKR_LOG_API int windowsDebuggerFilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void windowsDebuggerConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

class windowsDebuggerConsumer : public gkr::log::consumer
{
    windowsDebuggerConsumer           (const windowsDebuggerConsumer&) noexcept = delete;
    windowsDebuggerConsumer& operator=(const windowsDebuggerConsumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;

public:
    windowsDebuggerConsumer(windowsDebuggerConsumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
    }
    windowsDebuggerConsumer& operator=(windowsDebuggerConsumer&& other) noexcept
    {
        m_buf = other.m_buf;
        m_cch = other.m_cch;
        other.m_buf = nullptr;
        other.m_cch = 0;
        return *this;
    }

public:
    GKR_LOG_API windowsDebuggerConsumer(unsigned bufferCapacity = 2*1024);
    GKR_LOG_API virtual ~windowsDebuggerConsumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool filter_log_message(const gkr::log::message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const gkr::log::message& msg) override;

protected:
    GKR_LOG_API virtual unsigned composeOutput(char* buf, unsigned cch, const gkr::log::message& msg);
};

#endif
