#include "text_file_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/galery.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/sys/process.hpp>

#include <cstdio>

namespace gkr
{
namespace log
{
class c_text_file_consumer : public text_file_consumer
{
    gkr_log_text_file_consumer_callbacks m_callbacks {};

public:
    c_text_file_consumer(
        const gkr_log_text_file_consumer_callbacks* callbacks,
        const char* filepath = nullptr,
        int eoln = gkr_log_tf_eoln_default
        )
        : text_file_consumer(filepath, eoln)
    {
        if(callbacks != nullptr) m_callbacks = *callbacks;
    }
    virtual ~c_text_file_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned& len) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg);
        } else {
            return text_file_consumer::compose_output(msg, len);
        }
    }

protected:
    virtual void on_file_opened() override
    {
        if(m_callbacks.on_file_opened != nullptr) {
            return (*m_callbacks.on_file_opened)(m_callbacks.param);
        } else {
            return text_file_consumer::on_file_opened();
        }
    }
    virtual void on_file_closing() override
    {
        if(m_callbacks.on_file_closing != nullptr) {
            return (*m_callbacks.on_file_closing)(m_callbacks.param);
        } else {
            return text_file_consumer::on_file_closing();
        }
    }
    virtual void on_enter_file_write() override
    {
        if(m_callbacks.on_enter_file_write != nullptr) {
            return (*m_callbacks.on_enter_file_write)(m_callbacks.param);
        } else {
            return text_file_consumer::on_enter_file_write();
        }
    }
    virtual void on_leave_file_write() override
    {
        if(m_callbacks.on_leave_file_write != nullptr) {
            return (*m_callbacks.on_leave_file_write)(m_callbacks.param);
        } else {
            return text_file_consumer::on_leave_file_write();
        }
    }
};
}
}

extern "C" {

int gkr_log_add_text_file_consumer(
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln
    )
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_text_file_consumer>(callbacks, filepath, eoln));
}

}

namespace
{
int calc_eoln(int eoln)
{
    int   res = 0;
    char* buf = reinterpret_cast<char*>(&res);

    buf[1] = buf[2] = 0;

    switch(eoln)
    {
        default:
        case gkr_log_tf_eoln_lf  : buf[0] = '\n'; break;
        case gkr_log_tf_eoln_cr  : buf[0] = '\r'; break;
        case gkr_log_tf_eoln_crlf: buf[0] = '\r'; buf[1] = '\n'; break;
    }
    return res;
}
inline unsigned get_eoln_len(int eoln)
{
    return (reinterpret_cast<const char*>(&eoln)[1] == 0) ? 1U : 2U;
}
inline void output_to_text_file(std::FILE* file, const char* text, unsigned len)
{
    std::fwrite(text, sizeof(char), len, file);
}
}

namespace gkr
{
namespace log
{

text_file_consumer::text_file_consumer(
    const char* filepath,
    int eoln
    )
    : m_eoln(calc_eoln(eoln))
    , m_file(nullptr)
{
}

text_file_consumer::~text_file_consumer()
{
    close_file();
}

bool text_file_consumer::init_logging()
{
    constexpr unsigned cch = 256;
    char buf[cch];

    const unsigned head_len = unsigned(sys::get_current_process_path(buf, cch));
    if(head_len >= cch) return false;

    unsigned tail_len = gkr_log_apply_time_format(buf + head_len, cch - head_len, "_%F_%T.log", stamp_now(), 0);
    if(tail_len == 0) return false;

    m_name = buf;
    return open_file();
}

void text_file_consumer::done_logging()
{
    close_file();
}

bool text_file_consumer::filter_log_message(const message& msg)
{
    return false;
}

void text_file_consumer::consume_log_message(const message& msg)
{
    unsigned len;
    const char* output = compose_output(msg, len);

    std::FILE* file = static_cast<std::FILE*>(m_file);

    on_enter_file_write();
    output_to_text_file(file, output, len);
    output_to_text_file(file, reinterpret_cast<const char*>(&m_eoln), get_eoln_len(m_eoln));
    on_leave_file_write();
}

const char* text_file_consumer::compose_output(const message& msg, unsigned& len)
{
    return gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, 0, nullptr, 0, 0, &len);
}

void text_file_consumer::on_file_opened()
{
}

void text_file_consumer::on_file_closing()
{
}

void text_file_consumer::on_enter_file_write()
{
}

void text_file_consumer::on_leave_file_write()
{
}

bool text_file_consumer::open_file()
{
    m_file = std::fopen(m_name.c_str(), "wb");

    if(m_file != nullptr)
    {
        return false;
    }
    on_file_opened();
    return true;
}

void text_file_consumer::close_file()
{
    if(m_file != nullptr)
    {
        on_file_closing();
        std::fclose(static_cast<std::FILE*>(m_file));
        m_file = nullptr;
    }
}

}
}
