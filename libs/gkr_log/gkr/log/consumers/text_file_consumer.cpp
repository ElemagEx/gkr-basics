#include "text_file_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/galery.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/sys/process.hpp>
#include <gkr/sys/path.hpp>

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
    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        if(m_callbacks.compose_output != nullptr)
        {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, len, colored);
        }
        else
        {
            return text_file_consumer::compose_output(msg, len, colored);
        }
    }

protected:
    virtual void on_file_opened() override
    {
        if(m_callbacks.on_file_opened != nullptr)
        {
            return (*m_callbacks.on_file_opened)(m_callbacks.param, this);
        }
    }
    virtual void on_file_closing() override
    {
        if(m_callbacks.on_file_closing != nullptr)
        {
            return (*m_callbacks.on_file_closing)(m_callbacks.param, this);
        }
    }
    virtual void on_enter_file_write() override
    {
        if(m_callbacks.on_enter_file_write != nullptr)
        {
            return (*m_callbacks.on_enter_file_write)(m_callbacks.param, this);
        }
    }
    virtual void on_leave_file_write() override
    {
        if(m_callbacks.on_leave_file_write != nullptr)
        {
            return (*m_callbacks.on_leave_file_write)(m_callbacks.param, this);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_text_file_consumer(
    void* instance,
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln
    )
{
    return gkr_log_add_consumer(instance, std::make_shared<gkr::log::c_text_file_consumer>(callbacks, filepath, eoln));
}

gkr_log_tfs gkr_log_text_file_get_size(void* arg)
{
    return static_cast<gkr::log::text_file_consumer*>(arg)->get_size();
}

const char* gkr_log_text_file_get_path(void* arg)
{
    return static_cast<gkr::log::text_file_consumer*>(arg)->get_path();
}

void gkr_log_text_file_write_line(void* arg, const char* line, unsigned len)
{
    return static_cast<gkr::log::text_file_consumer*>(arg)->write_line(line, len);
}

void gkr_log_text_file_roll(void* arg, unsigned max_files)
{
    return static_cast<gkr::log::text_file_consumer*>(arg)->roll(max_files);
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
enum
{
    text_file_flag_prevent_open_close_events = 0x0001,
    text_file_flag_prevent_write_line_events = 0x0002,
    text_file_flag_prevent_recursive_roll    = 0x0004,
};
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
{
}

text_file_consumer::~text_file_consumer()
{
    close();
}

bool text_file_consumer::init_logging()
{
    constexpr unsigned cch = 256;
    char buf[cch];

    const unsigned head_len = unsigned(sys::get_current_process_path(buf, cch));
    if(head_len >= cch) return false;

    unsigned tail_len = gkr_log_apply_time_format(buf + head_len, cch - head_len, "_%F_%T.log", stamp_now(), 0);
    if(tail_len == 0) return false;

    m_path = buf;
    open();
    return true;
}

void text_file_consumer::done_logging()
{
    close();
}

bool text_file_consumer::filter_log_message(const message& msg)
{
    return false;
}

void text_file_consumer::consume_log_message(const message& msg)
{
    unsigned len;
    const char* output = compose_output(msg, &len);

    write_line(output, len);
}

const char* text_file_consumer::compose_output(const message& msg, unsigned* len, bool colored)
{
    return gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, 0, nullptr, 0, 0, len);
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

void text_file_consumer::write_line(const char* line, unsigned len)
{
    Check_Arg_IsValid((line != nullptr) || (len == 0), );

    if(m_file == nullptr) return;

    std::FILE* file = static_cast<std::FILE*>(m_file);

    if((m_flags & text_file_flag_prevent_write_line_events) == 0)
    {
        m_flags |=  text_file_flag_prevent_write_line_events;
        on_enter_file_write();
        m_flags &= ~text_file_flag_prevent_write_line_events;
    }
    output_to_text_file(file, line, len);
    m_size += len;

    len = get_eoln_len(m_eoln);
    output_to_text_file(file, reinterpret_cast<const char*>(&m_eoln), len);
    m_size += len;

    if((m_flags & text_file_flag_prevent_write_line_events) == 0)
    {
        m_flags |=  text_file_flag_prevent_write_line_events;
        on_leave_file_write();
        m_flags &= ~text_file_flag_prevent_write_line_events;
    }
}

void text_file_consumer::roll(unsigned max_files)
{
    Check_Arg_IsValid(max_files > 0, );

    if((m_flags & text_file_flag_prevent_recursive_roll) != 0) return;

    m_flags |= text_file_flag_prevent_recursive_roll;

    close();

    if(max_files <= 1)
    {
        remove(m_path.c_str());
    }
    else
    {
        char num[12];
        snprintf(num, sizeof(num), "%u", --max_files);
        std::string next_path = sys::path_insert_ext(m_path.c_str(), num, 1);

        remove(next_path.c_str());

        while(max_files > 1)
        {
            snprintf(num, sizeof(num), "%u", --max_files);
            std::string prev_path = sys::path_insert_ext(m_path.c_str(), num, 1);
            if(std::rename(prev_path.c_str(), next_path.c_str())) std::remove(prev_path.c_str()); //TODO:LOG and/or CHECK - https://learn.microsoft.com/en-us/cpp/code-quality/c6031
            next_path = std::move(prev_path);
        }
        if(!std::rename(m_path.c_str(), next_path.c_str())) std::remove(m_path.c_str()); //TODO:LOG and/or CHECK - https://learn.microsoft.com/en-us/cpp/code-quality/c6031
    }
    open();

    m_flags &= ~text_file_flag_prevent_recursive_roll;
}

void text_file_consumer::open()
{
    m_file = std::fopen(m_path.c_str(), "ab");

    if(m_file == nullptr) return; //TODO:Log

    fpos_t pos;
    fgetpos(static_cast<std::FILE*>(m_file), &pos);
    m_size = gkr_log_tfs(pos);

    if((m_flags & text_file_flag_prevent_open_close_events) == 0)
    {
        m_flags |=  text_file_flag_prevent_open_close_events;
        on_file_opened();
        m_flags &= ~text_file_flag_prevent_open_close_events;
    }
}

void text_file_consumer::close()
{
    if(m_file == nullptr) return;

    if((m_flags & text_file_flag_prevent_open_close_events) == 0)
    {
        m_flags |=  text_file_flag_prevent_open_close_events;
        on_file_closing();
        m_flags &= ~text_file_flag_prevent_open_close_events;
    }
    std::fclose(static_cast<std::FILE*>(m_file));

    m_file = nullptr;
}

}
}
