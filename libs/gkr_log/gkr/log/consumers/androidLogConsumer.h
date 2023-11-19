#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int androidLogGetPriority(const struct gkr_log_message* msg);
GKR_LOG_API const char* androidLogGetTag(const struct gkr_log_message* msg);
GKR_LOG_API void androidLogFormatText(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* androidLogCreateConsumerParam(
    unsigned bufferCapacity,
    int (*getPriority)(const struct gkr_log_message*),
    const char* (*getTag)(const struct gkr_log_message*),
    void (*formatText)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int androidLogInitLogging(void* param);

GKR_LOG_API void androidLogDoneLogging(void* param);

GKR_LOG_API int androidLogFilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void androidLogConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

class androidLogConsumer : public gkr::log::consumer
{
    androidLogConsumer           (const androidLogConsumer&) noexcept = delete;
    androidLogConsumer& operator=(const androidLogConsumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;

public:
    androidLogConsumer(androidLogConsumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
    }
    androidLogConsumer& operator=(androidLogConsumer&& other) noexcept
    {
        m_buf = other.m_buf;
        m_cch = other.m_cch;
        other.m_buf = nullptr;
        other.m_cch = 0;
        return *this;
    }

public:
    GKR_LOG_API androidLogConsumer(unsigned bufferCapacity = 2*1024);
    GKR_LOG_API virtual ~androidLogConsumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool filter_log_message(const gkr::log::message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const gkr::log::message& msg) override;

protected:
    GKR_LOG_API virtual int getPriority(const gkr::log::message& msg);

    GKR_LOG_API virtual const char* getTag(const gkr::log::message& msg);

    GKR_LOG_API virtual void formatText(char* buf, unsigned cch, const gkr::log::message& msg);
};

#endif
