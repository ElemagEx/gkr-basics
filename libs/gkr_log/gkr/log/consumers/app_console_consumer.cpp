#include <gkr/defs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/sys/file.hpp>

#include <cstdio>
#include <iostream>

namespace gkr
{
namespace log
{
class c_app_console_consumer : public app_console_consumer
{
    gkr_log_app_console_consumer_callbacks m_callbacks {};

public:
    c_app_console_consumer(const gkr_log_app_console_consumer_callbacks* callbacks, int method)
        : app_console_consumer(method)
    {
        if(callbacks != nullptr) m_callbacks = *callbacks;
    }
    virtual ~c_app_console_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, len, colored);
        } else {
            return app_console_consumer::compose_output(msg, len, colored);
        }
    }
};
}
}

namespace 
{
void outputToConsole(int method, const char* text)
{
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
bool isOutputAtty(int method)
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
}

extern "C" {

int gkr_log_add_app_console_consumer(
    void* channel,
    const gkr_log_app_console_consumer_callbacks* callbacks,
    int method
    )
{
    return gkr_log_add_consumer(channel, std::make_shared<gkr::log::c_app_console_consumer>(callbacks, method));
}

}

namespace gkr
{
namespace log
{

app_console_consumer::app_console_consumer(int method)
    : m_method(method)
    , m_isAtty(isOutputAtty(method))
{
}

app_console_consumer::~app_console_consumer()
{
}

bool app_console_consumer::init_logging()
{
#ifdef _WIN32
    return true;
#else
    return false;
#endif
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
    const char* output = compose_output(msg, nullptr, m_isAtty);

    outputToConsole(m_method, output);
}

const char* app_console_consumer::compose_output(const message& msg, unsigned* len, bool colored)
{
    constexpr const char* args[] = GENERIC_CONSOLE_ARGS_STRS;

    const int flags =
        gkr_log_fo_flag_use_inserts |
        gkr_log_fo_flag_use_padding |
        (colored ? gkr_log_fo_flag_use_coloring : 0)
        ;
    const char* output = gkr_log_format_output(
        GENERIC_FMT_MESSAGE,
        &msg,
        flags,
        args,
        GENERIC_CONSOLE_ARGS_ROWS,
        GENERIC_CONSOLE_ARGS_COLS,
        len
        );
    return output;
}

}
}
