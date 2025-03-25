#include <gkr/defs.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/log/c_consumer.hpp>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);
#endif

namespace gkr
{
namespace log
{
class c_windows_debugger_consumer : public c_consumer<windows_debugger_consumer>
{
    using base_t = c_consumer<windows_debugger_consumer>;

    const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);

public:
    c_windows_debugger_consumer(void* param, const gkr_log_windows_debugger_consumer_callbacks& callbacks)
        : base_t   (param, callbacks.opt_callbacks )
        , m_compose_output(callbacks.compose_output)
    {
    }
    virtual ~c_windows_debugger_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, int flags) override
    {
        if(m_compose_output != nullptr)
        {
            return (*m_compose_output)(m_param, &msg, nullptr, flags);
        }
        else
        {
            return windows_debugger_consumer::compose_output(msg, len, flags);
        }
    }
};
}
}

extern "C"
{

int gkr_log_add_windows_debugger_consumer(
    void* channel,
    void* param,
    const gkr_log_windows_debugger_consumer_callbacks* callbacks
    )
{
    Check_Arg_NotNull(callbacks, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_windows_debugger_consumer(param, *callbacks));

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
