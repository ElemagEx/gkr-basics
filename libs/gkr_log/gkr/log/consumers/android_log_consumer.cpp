#include <gkr/defs.hpp>
#include <gkr/log/consumers/android_log_consumer.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>

#ifdef __ANDROID__
#include <android/log.h>
inline void writeToAndroidLog(int priority, const char* tag, const char* text)
{
    __android_log_write(priority, tag, text);
}
#else
inline void writeToAndroidLog(int priority, const char* tag, const char* text)
{
}
#endif

namespace gkr
{
namespace log
{
class c_android_log_consumer : public android_log_consumer
{
    gkr_log_android_log_consumer_callbacks m_callbacks {};

public:
    c_android_log_consumer(const gkr_log_android_log_consumer_callbacks* callbacks)
    {
        if(callbacks != nullptr) m_callbacks = *callbacks;
    }
    virtual ~c_android_log_consumer() override
    {
    }

protected:
    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, len, colored);
        } else {
            return android_log_consumer::compose_output(msg, len, colored);
        }
    }
    virtual const char* get_tag(const message& msg) override
    {
        if(m_callbacks.get_tag != nullptr) {
            return (*m_callbacks.get_tag)(m_callbacks.param, &msg);
        } else {
            return android_log_consumer::get_tag(msg);
        }
    }
    virtual int get_priority(const message& msg) override
    {
        if(m_callbacks.get_priority != nullptr) {
            return (*m_callbacks.get_priority)(m_callbacks.param, &msg);
        } else {
            return android_log_consumer::get_priority(msg);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_android_log_consumer(
    void* instance,
    const gkr_log_android_log_consumer_callbacks* callbacks
    )
{
    return gkr_log_add_consumer(instance, std::make_shared<gkr::log::c_android_log_consumer>(callbacks));
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
#ifdef __ANDROID__
    return true;
#else
    return false;
#endif
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

    writeToAndroidLog(priority, tag, output);
}

const char* android_log_consumer::compose_output(const message& msg, unsigned* len, bool colored)
{
    return gkr_log_format_output(GENERIC_FMT_ANDROID, &msg, 0, nullptr, 0, 0, len);
}

const char* android_log_consumer::get_tag(const message& msg)
{
    return "";
}

int android_log_consumer::get_priority(const message& msg)
{
#ifdef __ANDROID__
    return ANDROID_LOG_VERBOSE;
#else
    return 0;
#endif
}

}
}
