#include <gkr/defs.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.hpp>

#include <gkr/log/c_consumer.hxx>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);

inline void output_to_windows_debugger(const char* output, const gkr_log_message& msg)
{
    if(output == nullptr)
    {
        output = gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, 0, nullptr, 0, 0, nullptr);
    }
    OutputDebugStringA(output);
    OutputDebugStringA("\n");
}
#else
inline void output_to_windows_debugger(const char* output, const gkr_log_message& msg)
{
}
#endif

namespace gkr
{
namespace log
{
class c_windows_debugger_consumer : public c_consumer
{
     const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);

public:
    c_windows_debugger_consumer(void* param, const gkr_log_windows_debugger_consumer_callbacks& callbacks) noexcept
        : c_consumer(param, callbacks.opt_callbacks)
        , m_compose_output(callbacks.compose_output)
    {
    }
    virtual ~c_windows_debugger_consumer() override
    {
    }

protected:
    void consume_log_message(const message& msg)
    {
        const char* output = (m_compose_output == nullptr)
            ? nullptr
            : (*m_compose_output)(m_param, &msg, nullptr, 0);

        output_to_windows_debugger(output, msg);
    }
};
}
}

extern "C" {

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

windows_debugger_consumer::windows_debugger_consumer() noexcept
{
}

windows_debugger_consumer::~windows_debugger_consumer()
{
}

bool windows_debugger_consumer::init_logging()
{
#ifdef _WIN32
    return true;
#else
    return false;
#endif
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
    const char* output = compose_output(msg);

    output_to_windows_debugger(output, msg);
}

const char* windows_debugger_consumer::compose_output(const message& msg, unsigned* len, bool colored)
{
    return nullptr;
}

}
}
