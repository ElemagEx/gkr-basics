#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/text_file_callbacks.h>

#include <string>

namespace gkr
{
namespace log
{

class text_file_consumer : public consumer
{
    text_file_consumer           (const text_file_consumer&) noexcept = delete;
    text_file_consumer& operator=(const text_file_consumer&) noexcept = delete;

private:
    char*       m_buf;
    unsigned    m_cch;
    int         m_eoln;
    void*       m_file;
    std::string m_name;

public:
    text_file_consumer(text_file_consumer&& other) noexcept
        : m_buf (other.m_buf)
        , m_cch (other.m_cch)
        , m_eoln(other.m_eoln)
        , m_file(other.m_file)
        , m_name(std::move(other.m_name))
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

        m_name = std::move(other.m_name);

        other.m_buf  = nullptr;
        other.m_cch  = 0;
        other.m_eoln = m_eoln;
        other.m_file = m_file;
        return *this;
    }

public:
    GKR_LOG_API text_file_consumer(
        const char* filepath = nullptr,
        int eoln = gkr_log_textFileEoln_Default,
        unsigned bufferCapacity = 2*1024
        );
    GKR_LOG_API virtual ~text_file_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual unsigned compose_output(const message& msg, char* buf, unsigned cch);

    GKR_LOG_API virtual void on_file_opened();
    GKR_LOG_API virtual void on_file_closing();

    GKR_LOG_API virtual void on_enter_file_write();
    GKR_LOG_API virtual void on_leave_file_write();

private:
    GKR_LOG_API unsigned    get_file_size();
    GKR_LOG_API const char* get_file_path();

    GKR_LOG_API void roll_file();

private:
    bool  open_file();
    void close_file();
};

}
}
