#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

enum
{
    gkr_log_appConsoleWriteMethod_printf,
    gkr_log_appConsoleWriteMethod_fputs2stderr,
    gkr_log_appConsoleWriteMethod_fputs2stdout,
#ifdef __cplusplus
    gkr_log_appConsoleWriteMethod_stream2cerr,
    gkr_log_appConsoleWriteMethod_stream2clog,
    gkr_log_appConsoleWriteMethod_stream2cout,
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned gkr_log_appConsole_ComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_appConsole_CreateConsumerParam(
    int method,
    unsigned bufferCapacity,
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int gkr_log_appConsole_InitLogging(void* param);

GKR_LOG_API void gkr_log_appConsole_DoneLogging(void* param);

GKR_LOG_API int gkr_log_appConsole_FilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void gkr_log_appConsole_ConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

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
    GKR_LOG_API app_console_consumer(int method = gkr_log_appConsoleWriteMethod_stream2cerr, unsigned bufferCapacity = 2*1024);
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

#endif
