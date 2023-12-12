#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/text_file_consumer.h>

#include <string>

namespace gkr
{
namespace log
{

class text_file_consumer : public consumer
{
    int         m_eoln;
    void*       m_file;
    std::string m_name;

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
