#include <gkr/defs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <gkr/log/c_consumer.hxx>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/sys/file.hpp>

#include <cstdio>
#include <iostream>

namespace 
{
bool is_output_atty(int method)
{
    switch(method)
    {
        case gkr_log_appConsoleWriteMethod_fputs2stderr:
        case gkr_log_appConsoleWriteMethod_stream2cerr:
        case gkr_log_appConsoleWriteMethod_stream2clog:
            return gkr::sys::file_is_atty(stderr);

        case gkr_log_appConsoleWriteMethod_fputs2stdout:
        case gkr_log_appConsoleWriteMethod_stream2cout:
        default:
            return gkr::sys::file_is_atty(stdout);
    }
}
inline int calc_flags(int method, int colorless)
{
    int flags = (method < gkr_log_appConsoleWriteMethod_stream2cout) ? 0 : gkr_log_fo_flag_append_eoln_lf;

    if(!colorless && is_output_atty(method)) flags |= gkr_log_fo_flag_use_coloring;

    return flags;
}
}

namespace gkr
{
namespace log
{
class c_app_console_consumer : public c_consumer<app_console_consumer>
{
    using base_t = c_consumer<app_console_consumer>;

    const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);

public:
    c_app_console_consumer(void* param, const gkr_log_app_console_consumer_callbacks& callbacks, int method, int colorless)
        : base_t   (param, callbacks.opt_callbacks , method, !!colorless)
        , m_compose_output(callbacks.compose_output)
    {
    }
    virtual ~c_app_console_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, int flags) override
    {
        if(m_compose_output != nullptr)
        {
            return (*m_compose_output)(m_param, &msg, len, flags);
        }
        else
        {
            return app_console_consumer::compose_output(msg, len, flags);
        }
    }
};
}
}

extern "C"
{

int gkr_log_add_app_console_consumer(
    void* channel,
    void* param,
    const gkr_log_app_console_consumer_callbacks* callbacks,
    int method,
    int colorless
    )
{
    Check_Arg_NotNull(callbacks, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_app_console_consumer(param, *callbacks, method, colorless));

    return gkr_log_add_consumer(channel, consumer);
}

}

namespace gkr
{
namespace log
{

app_console_consumer::app_console_consumer(int method, bool colorless)
    : m_method(method)
    , m_flags(calc_flags(method, colorless))
{
}

app_console_consumer::~app_console_consumer()
{
}

bool app_console_consumer::init_logging()
{
    return true;
}

void app_console_consumer::done_logging()
{
}

bool app_console_consumer::filter_log_message(const message& msg)
{
    return false;
}

void app_console_consumer::consume_log_message(const message& msg)
{
    int flags = m_flags | gkr_log_fo_flag_use_inserts | gkr_log_fo_flag_use_padding;

    const char* output = compose_output(msg, nullptr, flags);

    if(output == nullptr)
    {
        constexpr const char* args[] = GENERIC_CONSOLE_ARGS_STRS;

        output = gkr_log_format_output(
            GENERIC_FMT_MESSAGE,
            &msg,
            flags,
            args,
            GENERIC_CONSOLE_ARGS_ROWS,
            GENERIC_CONSOLE_ARGS_COLS,
            nullptr);
    }
    switch(m_method)
    {
        default:
            std::puts(output);
            break;

        case gkr_log_appConsoleWriteMethod_fputs2stdout:
            std::fputs(output, stdout);
            break;
        case gkr_log_appConsoleWriteMethod_fputs2stderr:
            std::fputs(output, stderr);
            break;

        case gkr_log_appConsoleWriteMethod_stream2cout: std::cout << output; break;
        case gkr_log_appConsoleWriteMethod_stream2cerr: std::cerr << output; break;
        case gkr_log_appConsoleWriteMethod_stream2clog: std::clog << output; break;
    }
}

const char* app_console_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

}
}
