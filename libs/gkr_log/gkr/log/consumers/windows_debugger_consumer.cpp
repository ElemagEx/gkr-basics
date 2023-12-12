#include "windows_debugger_consumer.hpp"

#include <gkr/stamp.hpp>
#include <gkr/diagnostics.h>
#include <gkr/log/galery.hpp>
#include <gkr/log/logging.hpp>

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char*);

inline void outputToWindowsDebugger(const char* text)
{
    OutputDebugStringA(text);
    OutputDebugStringA("\n");
}
#else
inline void outputToWindowsDebugger(const char* text)
{
}
#endif

namespace gkr
{
namespace log
{
class c_windows_debugger_consumer : public windows_debugger_consumer
{
    gkr_log_windows_debugger_consumer_callbacks m_callbacks {};

public:
    c_windows_debugger_consumer(const gkr_log_windows_debugger_consumer_callbacks* callbacks)
        : windows_debugger_consumer()
    {
        if(callbacks != nullptr) m_callbacks = *callbacks;
    }
    virtual ~c_windows_debugger_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, len, colored);
        } else {
            return windows_debugger_consumer::compose_output(msg, len, colored);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_windows_debugger_consumer(
    void* instance,
    const gkr_log_windows_debugger_consumer_callbacks* callbacks
    )
{
    return gkr_log_add_consumer(instance, std::make_shared<gkr::log::c_windows_debugger_consumer>(callbacks));
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

    outputToWindowsDebugger(output);
}

const char* windows_debugger_consumer::compose_output(const message& msg, unsigned* len, bool colored)
{
    return gkr_log_format_output(GENERIC_FMT_MESSAGE, &msg, 0, nullptr, 0, 0, len);
}

}
}
