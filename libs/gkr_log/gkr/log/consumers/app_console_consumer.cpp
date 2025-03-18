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
void output_to_console(int method, int flags, const char* text, const gkr_log_message& msg)
{
    if(text == nullptr)
    {
        constexpr const char* args[] = GENERIC_CONSOLE_ARGS_STRS;

        flags |= gkr_log_fo_flag_use_inserts | gkr_log_fo_flag_use_padding;

        text = gkr_log_format_output(
            GENERIC_FMT_MESSAGE,
            &msg,
            flags,
            args,
            GENERIC_CONSOLE_ARGS_ROWS,
            GENERIC_CONSOLE_ARGS_COLS,
            nullptr
            );
    }
    switch(method)
    {
        default:
            printf("%s\n", text);
            break;

        case gkr_log_appConsoleWriteMethod_fputs2stderr:
            std::fputs(text, stderr);
            std::fputs("\n", stderr);
            break;
        case gkr_log_appConsoleWriteMethod_fputs2stdout:
            std::fputs(text, stdout);
            std::fputs("\n", stdout);
            break;

        case gkr_log_appConsoleWriteMethod_stream2cout: std::cout << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2cerr: std::cerr << text << std::endl; break;
        case gkr_log_appConsoleWriteMethod_stream2clog: std::clog << text << std::endl; break;
    }
}
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
inline int calc_flags(int method, int colorless) noexcept
{
    return (is_output_atty(method) && !colorless)
        ? gkr_log_fo_flag_use_coloring
        : 0;
}
}

namespace gkr
{
namespace log
{
class c_app_console_consumer : public c_consumer
{
    const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);

    int m_method;
    int m_flags;

public:
    c_app_console_consumer(void* param, const gkr_log_app_console_consumer_callbacks& callbacks, int method, int colorless) noexcept
        : c_consumer(param, callbacks.opt_callbacks)
        , m_compose_output(callbacks.compose_output)
        , m_method(method)
        , m_flags(calc_flags(method, colorless))
    {
    }
    virtual ~c_app_console_consumer() override
    {
    }

protected:
    virtual void consume_log_message(const message& msg) override
    {
        const char* output = (m_compose_output == nullptr)
            ? nullptr
            : (*m_compose_output)(m_param, &msg, nullptr, m_flags);

        output_to_console(m_method, m_flags, output, msg);
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

app_console_consumer::app_console_consumer(int method, bool colorless) noexcept
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
    const char* output = compose_output(msg, nullptr, m_flags);

    output_to_console(m_method, m_flags, output, msg);
}

const char* app_console_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

}
}
