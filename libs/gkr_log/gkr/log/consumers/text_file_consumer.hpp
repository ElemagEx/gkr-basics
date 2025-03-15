#pragma once

#include <gkr/capi/log/consumers/text_file_consumer.h>

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>

#include <string>

namespace gkr
{
namespace log
{

class text_file_consumer : public consumer
{
    std::string m_path;
    void*       m_file  = nullptr;
    int         m_eoln  = 0;
    int         m_flags = false;
    gkr_log_tfs m_size  = 0;

public:
    GKR_LOG_API text_file_consumer(
        const char* filepath = nullptr,
        int eoln = gkr_log_tf_eoln_default
        );
    GKR_LOG_API virtual ~text_file_consumer() override;

protected:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len = nullptr, bool colored = false);

protected:
    GKR_LOG_API virtual void on_file_opened();
    GKR_LOG_API virtual void on_file_closing();

    GKR_LOG_API virtual void on_enter_file_write();
    GKR_LOG_API virtual void on_leave_file_write();

public:
    gkr_log_tfs get_size() const
    {
        return m_size;
    }
    const char* get_path() const
    {
        return m_path.c_str();
    }

public:
    GKR_LOG_API void write_line(const char* line, unsigned len);

    GKR_LOG_API void roll(unsigned max_files);

private:
    void  open();
    void close();
};

}
}
