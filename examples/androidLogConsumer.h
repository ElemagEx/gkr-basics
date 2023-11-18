#pragma once

#include <gkr/log/consumer.h>

#ifndef GKR_SAMPLE_API
#define GKR_SAMPLE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

GKR_SAMPLE_API int androidLogGetPriority(const struct gkr_log_message* msg);
GKR_SAMPLE_API const char* androidLogGetTag(const struct gkr_log_message* msg);
GKR_SAMPLE_API void androidLogFormatText(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_SAMPLE_API void* androidLogCreateParam(
    unsigned bufferCapacity,
    int (*getPriority)(const struct gkr_log_message*),
    const char* (*getTag)(const struct gkr_log_message*),
    void (*formatText)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_SAMPLE_API int androidLogInitLogging(void* param);

GKR_SAMPLE_API void androidLogDoneLogging(void* param);

GKR_SAMPLE_API int androidLogFilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_SAMPLE_API void androidLogConsumeLogMessage(void* param, const struct gkr_log_message* msg);

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
    GKR_SAMPLE_API androidLogConsumer(unsigned bufferCapacity = 2*1024);
    GKR_SAMPLE_API virtual ~androidLogConsumer() override;

protected:
    GKR_SAMPLE_API virtual bool init_logging() override;
    GKR_SAMPLE_API virtual void done_logging() override;

    GKR_SAMPLE_API virtual bool filter_log_message(const gkr::log::message& msg) override;

    GKR_SAMPLE_API virtual void consume_log_message(const gkr::log::message& msg) override;

protected:
    GKR_SAMPLE_API virtual int getPriority(const gkr::log::message& msg);

    GKR_SAMPLE_API virtual const char* getTag(const gkr::log::message& msg);

    GKR_SAMPLE_API virtual void formatText(char* buf, unsigned cch, const gkr::log::message& msg);
};

#endif
