#include <gkr/defs.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/log/c_consumer.hpp>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif

extern "C"
{

int gkr_log_add_windows_debugger_consumer(
    void* channel,
    void* param,
    const gkr_log_windows_debugger_consumer_callbacks* callbacks
    )
{
    const struct gkr_log_consumer_aid_callbacks* aid_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->aid_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::log::windows_debugger_consumer>(param, aid_callbacks));

    return gkr_log_add_consumer(channel, consumer);
}

}

namespace gkr
{
namespace log
{

windows_debugger_consumer::windows_debugger_consumer()
{
}

windows_debugger_consumer::~windows_debugger_consumer()
{
}

bool windows_debugger_consumer::init_logging()
{
    return true;
}

void windows_debugger_consumer::done_logging()
{
}

bool windows_debugger_consumer::filter_log_message(const message& msg)
{
    return false;
}

void windows_debugger_consumer::consume_log_message(const message& msg)
{
#ifdef _WIN32
    const int flags = gkr_log_fo_flag_append_eoln_lf;

    const char* output = compose_output(msg, nullptr, flags);

    if(output == nullptr)
    {
        output = gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, flags, nullptr, 0, 0, nullptr);
    }
    OutputDebugStringA(output);
#endif
}

const char* windows_debugger_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

}
}
