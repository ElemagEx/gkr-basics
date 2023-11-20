#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_androidLog_GetPriority(const struct gkr_log_message* msg);
GKR_LOG_API const char* gkr_log_androidLog_GetTag(const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_androidLog_FormatText(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_androidLog_CreateConsumerParam(
    unsigned buffer_capacity,
    int (*get_priority)(const struct gkr_log_message*),
    const char* (*get_tag)(const struct gkr_log_message*),
    void (*format_text)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int gkr_log_androidLog_InitLogging(void* param);

GKR_LOG_API void gkr_log_androidLog_DoneLogging(void* param);

GKR_LOG_API int gkr_log_androidLog_FilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void gkr_log_androidLog_ConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

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

    GKR_LOG_API virtual void format_text(char* buf, unsigned cch, const message& msg);
};

}
}

#endif
