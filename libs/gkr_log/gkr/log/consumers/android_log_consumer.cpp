#include <gkr/defs.hpp>
#include <gkr/log/consumers/android_log_consumer.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>
#include <gkr/log/c_consumer.hpp>

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace gkr
{
namespace log
{
class c_android_log_consumer : public c_consumer<android_log_consumer>
{
    using base_t = c_consumer<android_log_consumer>;

    const char* (*m_compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
    const char* (*m_get_tag       )(void*, const struct gkr_log_message*);
    int         (*m_get_priority  )(void*, const struct gkr_log_message*);

public:
    c_android_log_consumer(void* param, const gkr_log_android_log_consumer_callbacks& callbacks)
        : base_t   (param, callbacks.opt_callbacks )
        , m_compose_output(callbacks.compose_output)
        , m_get_tag       (callbacks.get_tag       )
        , m_get_priority  (callbacks.get_priority  )
    {
    }
    virtual ~c_android_log_consumer() override
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
            return android_log_consumer::compose_output(msg, len, flags);
        }
    }
    virtual const char* get_tag(const message& msg) override
    {
        if(m_get_tag != nullptr)
        {
            return (*m_get_tag)(m_param, &msg);
        }
        else
        {
            return android_log_consumer::get_tag(msg);
        }
    }
    virtual int get_priority(const message& msg) override
    {
        if(m_get_priority != nullptr)
        {
            return (*m_get_priority)(m_param, &msg);
        }
        else
        {
            return android_log_consumer::get_priority(msg);
        }
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

android_log_consumer::android_log_consumer()
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
#ifdef __ANDROID__
    const int   priority = get_priority(msg);
    const char* tag      = get_tag(msg);
    const char* output   = compose_output(msg);

    if(tag    == nullptr) tag = "";
    if(output == nullptr) output = gkr_log_format_output(GENERIC_FMT_ANDROID, &msg, 0, nullptr, 0, 0, nullptr);

    __android_log_write(priority, tag, text);
#endif
}

const char* android_log_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

const char* android_log_consumer::get_tag(const message& msg)
{
    return nullptr;
}

int android_log_consumer::get_priority(const message& msg)
{
#ifdef __ANDROID__
    return ANDROID_LOG_VERBOSE;
#else
    return 1000;
#endif
}

}
}
