#include <gkr/defs.hpp>
#include <gkr/log/consumers/android_log_consumer.hpp>

#include <gkr/log/c_consumer.hxx>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>

#ifdef __ANDROID__
#include <android/log.h>
constexpr int DEFAULT_LOG_PRIORITY = ANDROID_LOG_VERBOSE;
inline void write_to_android_log(int priority, const char* tag, const char* output, const gkr_log_message& msg)
{
    if(tag    == nullptr) tag = "";
    if(output == nullptr) output = gkr_log_format_output(GENERIC_FMT_ANDROID, &msg, 0, nullptr, 0, 0, nullptr);

    __android_log_write(priority, tag, text);
}
#else
constexpr int DEFAULT_LOG_PRIORITY = 0;
inline void write_to_android_log(int priority, const char* tag, const char* output, const gkr_log_message& msg)
{
}
#endif

namespace gkr
{
namespace log
{
class c_android_log_consumer : public c_consumer
{
     const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
     const char* (*m_get_tag       )(void*, const struct gkr_log_message*);
     int         (*m_get_priority  )(void*, const struct gkr_log_message*);

public:
    c_android_log_consumer(void* param, const gkr_log_android_log_consumer_callbacks& callbacks) noexcept
        : c_consumer(param, callbacks.opt_callbacks)
        , m_compose_output(callbacks.compose_output)
        , m_get_tag       (callbacks.get_tag       )
        , m_get_priority  (callbacks.get_priority  )
    {
    }
    virtual ~c_android_log_consumer() override
    {
    }

protected:
    void consume_log_message(const message& msg)
    {
        const char* tag      = (m_get_tag        == nullptr) ? nullptr              : (*m_get_tag       )(m_param, &msg);
        const int   priority = (m_get_priority   == nullptr) ? DEFAULT_LOG_PRIORITY : (*m_get_priority  )(m_param, &msg);
        const char* output   = (m_compose_output == nullptr) ? nullptr              : (*m_compose_output)(m_param, &msg, nullptr, 0);

        write_to_android_log(priority, tag, output, msg);
    }
};
}
}

extern "C"
{

int gkr_log_add_android_log_consumer(
    void* channel,
    void* param,
    const gkr_log_android_log_consumer_callbacks* callbacks
    )
{
    Check_Arg_NotNull(callbacks, -1);

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_android_log_consumer(param, *callbacks));

    return gkr_log_add_consumer(channel, consumer);
}

}

namespace gkr
{
namespace log
{

android_log_consumer::android_log_consumer() noexcept
{
}

android_log_consumer::~android_log_consumer()
{
}

bool android_log_consumer::init_logging()
{
    return true;
}

void android_log_consumer::done_logging()
{
}

bool android_log_consumer::filter_log_message(const message& msg)
{
    return false;
}

void android_log_consumer::consume_log_message(const message& msg)
{
    const int   priority = get_priority(msg);
    const char* tag      = get_tag(msg);
    const char* output   = compose_output(msg);

    write_to_android_log(priority, tag, output, msg);
}

const char* android_log_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

const char* android_log_consumer::get_tag(const message& msg)
{
    return "";
}

int android_log_consumer::get_priority(const message& msg)
{
    return DEFAULT_LOG_PRIORITY;
}

}
}
