#include <gkr/defs.hpp>
#include <gkr/log/consumers/text_file_consumer.hpp>

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/log/c_consumer.hpp>

#include <gkr/sys/process.hpp>
#include <gkr/sys/path.hpp>
#include <gkr/sys/file.hpp>

#include <cstdio>

namespace
{
template<unsigned CCH>
unsigned fill_eoln(int flags, char (&buf)[CCH])
{
    static_assert(CCH >= 2, "Must be at least 2");
    switch(flags & gkr_log_fo_flag_append_eoln_mask)
    {
        case gkr_log_fo_flag_append_eoln_lf  : buf[0] = '\n'; return 1;
        case gkr_log_fo_flag_append_eoln_cr  : buf[0] = '\r'; return 1;
        case gkr_log_fo_flag_append_eoln_crlf: buf[0] = '\r'; buf[1] = '\n'; return 2;

        case gkr_log_fo_flag_append_eoln_none:
        default:
            return 0;
    }
}
int calc_eoln_flags(int eoln)
{
    switch(eoln)
    {
        default:
        case gkr_log_tf_eoln_lf  : return gkr_log_fo_flag_append_eoln_lf;
        case gkr_log_tf_eoln_cr  : return gkr_log_fo_flag_append_eoln_cr;
        case gkr_log_tf_eoln_crlf: return gkr_log_fo_flag_append_eoln_crlf;
        case gkr_log_tf_eoln_none: return gkr_log_fo_flag_append_eoln_none;
    }
}
std::string eval_full_path(const char* filepath)
{
    std::string path;

    bool append_stamp = false;

    if(filepath == nullptr)
    {
        path = gkr::sys::get_current_process_path();

        gkr::sys::path_del_extension(path);

        append_stamp = true;
    }
    else
    {
        path = gkr::sys::expand_current_process_env_vars(filepath);

        if(gkr::sys::path_ends_with_separator(path) || gkr::sys::path_is_dir(path))
        {
            gkr::sys::path_add_filename(path, gkr::sys::get_current_process_name());

            append_stamp = true;
        }
    }
    if(append_stamp)
    {
        char ext[32];
        const unsigned len = gkr_log_apply_time_format(ext, sizeof(ext), "_%Y%m%d_%H%M%S.log", gkr::stamp_now(), 0);
        path.append(ext, len);
    }
    return path;
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

class c_text_file_consumer : public c_consumer<text_file_consumer>
{
    using base_t = c_consumer<text_file_consumer>;

    void (*m_on_file_opened     )(void*, void*);
    void (*m_on_file_closing    )(void*, void*);
    void (*m_on_enter_file_write)(void*, void*);
    void (*m_on_leave_file_write)(void*, void*);

public:
    c_text_file_consumer(
        void* param,
        const gkr_log_text_file_consumer_callbacks& callbacks,
        const char* filepath,
        int eoln
        )
        : base_t    (param,&callbacks.opt_callbacks , filepath, eoln)
        , m_on_file_opened (callbacks.on_file_opened)
        , m_on_file_closing(callbacks.on_file_closing)
        , m_on_enter_file_write(callbacks.on_enter_file_write)
        , m_on_leave_file_write(callbacks.on_leave_file_write)
    {
    }
    virtual ~c_text_file_consumer() override
    {
    }

protected:
    virtual void on_file_opened() override
    {
        if(m_on_file_opened != nullptr)
        {
            (*m_on_file_opened)(m_param, static_cast<c_text_file_consumer*>(this));
        }
    }
    virtual void on_file_closing() override
    {
        if(m_on_file_closing != nullptr)
        {
            (*m_on_file_closing)(m_param, static_cast<c_text_file_consumer*>(this));
        }
    }
    virtual void on_enter_file_write() override
    {
        if(m_on_enter_file_write != nullptr)
        {
            (*m_on_enter_file_write)(m_param, static_cast<c_text_file_consumer*>(this));
        }
    }
    virtual void on_leave_file_write() override
    {
        if(m_on_leave_file_write != nullptr)
        {
            (*m_on_leave_file_write)(m_param, static_cast<c_text_file_consumer*>(this));
        }
    }
};

}
}

extern "C"
{

int gkr_log_add_text_file_consumer(
    void* channel,
    void* param,
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln
    )
{
    Check_Arg_NotNull(callbacks, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_text_file_consumer(param, *callbacks, filepath, eoln));

    return gkr_log_add_consumer(channel, consumer);
}

gkr_log_tfs gkr_log_text_file_get_size(void* arg)
{
    return static_cast<gkr::log::c_text_file_consumer*>(arg)->get_size();
}

const char* gkr_log_text_file_get_path(void* arg)
{
    return static_cast<gkr::log::c_text_file_consumer*>(arg)->get_path();
}

void gkr_log_text_file_write_text(void* arg, const char* line, unsigned len, int add_eoln)
{
    return static_cast<gkr::log::c_text_file_consumer*>(arg)->write_text(line, len, !!add_eoln);
}

void gkr_log_text_file_roll(void* arg, unsigned max_files)
{
    return static_cast<gkr::log::c_text_file_consumer*>(arg)->roll(max_files);
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
    : m_path(eval_full_path(filepath))
    , m_flags(calc_eoln_flags(eoln))
{
}

text_file_consumer::~text_file_consumer()
{
    close();
}

bool text_file_consumer::init_logging()
{
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
    const char* output = compose_output(msg, &len, m_flags);

    if(output == nullptr)
    {
        output = gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, m_flags, nullptr, 0, 0, &len);
    }
    write_text(output, len, false);
}
const char* text_file_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
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

void text_file_consumer::write_text(const char* line, unsigned len, bool add_eoln)
{
    Check_Arg_NotNull(line, );

    if(m_file == nullptr) return;

    if(len == 0) return;

    std::FILE* file = static_cast<std::FILE*>(m_file);

    if((m_mask & text_file_flag_prevent_write_line_events) == 0)
    {
        m_mask |=  text_file_flag_prevent_write_line_events;
        on_enter_file_write();
        m_mask &= ~text_file_flag_prevent_write_line_events;
    }
    std::fwrite(line, sizeof(char), len, file);
    m_size += len;

    if(add_eoln)
    {
        char buf[2];
        len = fill_eoln(m_flags, buf);
        if(len > 0)
        {
            std::fwrite(buf, sizeof(char), len, file);
            m_size += len;
        }
    }
    std::fflush(file);

    if((m_mask & text_file_flag_prevent_write_line_events) == 0)
    {
        m_mask |=  text_file_flag_prevent_write_line_events;
        on_leave_file_write();
        m_mask &= ~text_file_flag_prevent_write_line_events;
    }
}

void text_file_consumer::roll(unsigned max_files)
{
    Check_Arg_IsValid(max_files > 0, );

    if((m_mask & text_file_flag_prevent_recursive_roll) != 0) return;

    m_mask |= text_file_flag_prevent_recursive_roll;

    close();

    if(max_files <= 1)
    {
        remove(m_path.c_str());
    }
    else
    {
        std::string num = std::to_string(--max_files);
        std::string ext = sys::path_get_extension(m_path);

        std::string next_path = m_path;
        sys::path_set_extension(next_path, num);
        sys::path_add_extension(next_path, ext);

        remove(next_path.c_str());

        while(max_files > 1)
        {
            num = std::to_string(--max_files);

            std::string prev_path = m_path;
            sys::path_set_extension(prev_path, num);
            sys::path_add_extension(prev_path, ext);

            if(std::rename(prev_path.c_str(), next_path.c_str())) std::remove(prev_path.c_str()); //TODO:LOG and/or CHECK - https://learn.microsoft.com/en-us/cpp/code-quality/c6031

            next_path = std::move(prev_path);
        }
        if(!std::rename(m_path.c_str(), next_path.c_str())) std::remove(m_path.c_str()); //TODO:LOG and/or CHECK - https://learn.microsoft.com/en-us/cpp/code-quality/c6031
    }
    open();

    m_mask &= ~text_file_flag_prevent_recursive_roll;
}

void text_file_consumer::open()
{
    gkr::sys::path_ensure_dir_exists(m_path);

    m_file = std::fopen(m_path.c_str(), "ab");

    if(m_file == nullptr)
    {
        return gkr::sys::file_report_error(errno);
    }
    std::FILE* file = static_cast<std::FILE*>(m_file);
    const long pos  = std::ftell(file);
    if(pos == -1)
    {
        std::fclose(file);
        return gkr::sys::file_report_error(errno);
    }
    m_size = gkr_log_tfs(pos);
    m_mask = 0;

    if((m_mask & text_file_flag_prevent_open_close_events) == 0)
    {
        m_mask |=  text_file_flag_prevent_open_close_events;
        on_file_opened();
        m_mask &= ~text_file_flag_prevent_open_close_events;
    }
}

void text_file_consumer::close()
{
    if(m_file == nullptr) return;

    if((m_mask & text_file_flag_prevent_open_close_events) == 0)
    {
        m_mask |=  text_file_flag_prevent_open_close_events;
        on_file_closing();
        m_mask &= ~text_file_flag_prevent_open_close_events;
    }
    std::FILE* file = static_cast<std::FILE*>(m_file);

    m_file = nullptr;
    m_size = 0;
    m_mask = 0;

    if(std::fclose(file))
    {
        return gkr::sys::file_report_error(errno);
    }
}

}
}
