#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

enum
{
    gkr_log_textFileEoln_LF,
    gkr_log_textFileEoln_CR,
    gkr_log_textFileEoln_CRLF,
#if   defined(_WIN32)
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_CRLF
#elif defined(__APPLE__)
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_CR
#else
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_LF
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned gkr_log_textFile_makeFilePath(char* buf, unsigned cch);
GKR_LOG_API unsigned gkr_log_textFile_ComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_textFile_CreateConsumerParam(
    int eoln,
    unsigned buffer_capacity,
    unsigned (*make_file_path)(char*, unsigned),
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int gkr_log_textFile_InitLogging(void* param);

GKR_LOG_API void gkr_log_textFile_DoneLogging(void* param);

GKR_LOG_API int gkr_log_textFile_FilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void gkr_log_textFile_ConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

namespace gkr
{
namespace log
{

class text_file_consumer : public consumer
{
    text_file_consumer           (const text_file_consumer&) noexcept = delete;
    text_file_consumer& operator=(const text_file_consumer&) noexcept = delete;

private:
    char*    m_buf;
    unsigned m_cch;
    int      m_eoln;
    void*    m_file;

public:
    text_file_consumer(text_file_consumer&& other) noexcept
        : m_buf (other.m_buf)
        , m_cch (other.m_cch)
        , m_eoln(other.m_eoln)
        , m_file(other.m_file)
    {
        other.m_buf  = nullptr;
        other.m_cch  = 0;
        other.m_eoln = m_eoln;
        other.m_file = m_file;
    }
    text_file_consumer& operator=(text_file_consumer&& other) noexcept
    {
        m_buf  = other.m_buf;
        m_cch  = other.m_cch;
        m_eoln = other.m_eoln;
        m_file = other.m_file;
        other.m_buf  = nullptr;
        other.m_cch  = 0;
        other.m_eoln = m_eoln;
        other.m_file = m_file;
        return *this;
    }

public:
    GKR_LOG_API text_file_consumer(int eoln = gkr_log_textFileEoln_Default, unsigned buffer_capacity = 2*1024);
    GKR_LOG_API virtual ~text_file_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool filter_log_message(const message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual unsigned make_file_path(char* buf, unsigned cch);
    GKR_LOG_API virtual unsigned compose_output(char* buf, unsigned cch, const message& msg);

private:
    void close_file();
};

}
}
#endif