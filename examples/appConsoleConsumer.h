#pragma once

#include <gkr/log/consumer.h>

#ifndef GKR_SAMPLE_API
#define GKR_SAMPLE_API
#endif

enum appConsoleWriteMethod
{
    appConsoleWriteMethod_printf,
    appConsoleWriteMethod_fputs2stderr,
    appConsoleWriteMethod_fputs2stdout,
#ifdef __cplusplus
    appConsoleWriteMethod_stream2cerr,
    appConsoleWriteMethod_stream2clog,
    appConsoleWriteMethod_stream2cout,
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_SAMPLE_API unsigned appConsoleComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_SAMPLE_API void* appConsoleCreateParam(
    int method,
    unsigned bufferCapacity,
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_SAMPLE_API int appConsoleInitLogging(void* param);

GKR_SAMPLE_API void appConsoleDoneLogging(void* param);

GKR_SAMPLE_API int appConsoleFilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_SAMPLE_API void appConsoleConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

class appConsoleConsumer : public gkr::log::consumer
{
    appConsoleConsumer           (const appConsoleConsumer&) noexcept = delete;
    appConsoleConsumer& operator=(const appConsoleConsumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;
    int      m_mth;

public:
    appConsoleConsumer(appConsoleConsumer&& other) noexcept
        : m_buf(other.m_buf)
        , m_cch(other.m_cch)
        , m_mth(other.m_mth)
    {
        other.m_buf = nullptr;
        other.m_cch = 0;
        other.m_mth = 0;
    }
    appConsoleConsumer& operator=(appConsoleConsumer&& other) noexcept
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
    GKR_SAMPLE_API appConsoleConsumer(int method = appConsoleWriteMethod_stream2cerr, unsigned bufferCapacity = 2*1024);
    GKR_SAMPLE_API virtual ~appConsoleConsumer() override;

protected:
    GKR_SAMPLE_API virtual bool init_logging() override;
    GKR_SAMPLE_API virtual void done_logging() override;

    GKR_SAMPLE_API virtual bool filter_log_message(const gkr::log::message& msg) override;

    GKR_SAMPLE_API virtual void consume_log_message(const gkr::log::message& msg) override;

protected:
    GKR_SAMPLE_API virtual unsigned composeOutput(char* buf, unsigned cch, const gkr::log::message& msg);
};

#endif
