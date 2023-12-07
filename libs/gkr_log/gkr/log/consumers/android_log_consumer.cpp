#include "android_log_consumer.hpp"

#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>

#include <cstdio>
#include <cstdlib>
#include <utility>

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
    c_android_log_consumer           (const c_android_log_consumer&) noexcept = delete;
    c_android_log_consumer& operator=(const c_android_log_consumer&) noexcept = delete;

    gkr_log_android_log_consumer_callbacks m_callbacks {};

public:
    c_android_log_consumer(c_android_log_consumer&& other) noexcept
        : android_log_consumer(std::move(other))
        , m_callbacks(other.m_callbacks)
    {
        other.m_callbacks = gkr_log_android_log_consumer_callbacks();
    }
    c_android_log_consumer& operator=(c_android_log_consumer&& other) noexcept
    {
        android_log_consumer::operator=(std::move(other));
        m_callbacks = other.m_callbacks;
        other.m_callbacks = gkr_log_android_log_consumer_callbacks();
        return *this;
    }
    c_android_log_consumer(
        const gkr_log_android_log_consumer_callbacks* callbacks,
        unsigned bufferCapacity
        )
        : android_log_consumer(bufferCapacity)
    {
        if(callbacks != nullptr) {
            m_callbacks = *callbacks;
        }
    }
    virtual ~c_android_log_consumer() override
    {
    }

protected:
    virtual int get_priority(const message& msg) override
    {
        if(m_callbacks.get_priority != nullptr) {
            return (*m_callbacks.get_priority)(m_callbacks.param, &msg);
        } else {
            return android_log_consumer::get_priority(msg);
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
    virtual unsigned compose_output(const message& msg, char* buf, unsigned cch) override
    {
        if(m_callbacks.compose_output != nullptr) {
            return (*m_callbacks.compose_output)(m_callbacks.param, &msg, buf, cch);
        } else {
            return android_log_consumer::compose_output(msg, buf, cch);
        }
    }
};
}
}

extern "C" {

int gkr_log_add_android_log_consumer(
    const gkr_log_android_log_consumer_callbacks* callbacks,
    unsigned bufferCapacity
    )
{
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_android_log_consumer>(callbacks, bufferCapacity));
}

}

namespace gkr
{
namespace log
{

android_log_consumer::android_log_consumer(unsigned bufferCapacity)
    : m_buf(nullptr)
    , m_cch(bufferCapacity)
{
    Check_Arg_IsValid(bufferCapacity > 0, );

    m_buf = new char[bufferCapacity];
}

android_log_consumer::~android_log_consumer()
{
    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool android_log_consumer::init_logging()
{
#ifdef __ANDROID__
    return (param != nullptr);
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
    compose_output(msg, m_buf, m_cch);

    m_buf[m_cch - 1] = 0;

    const char* tag      = get_tag(msg);
    const int   priority = get_priority(msg);

    writeToAndroidLog(priority, tag, m_buf);
}

int android_log_consumer::get_priority(const message& msg)
{
#ifdef __ANDROID__
    return ANDROID_LOG_VERBOSE;
#else
    return 0;
#endif
}

const char* android_log_consumer::get_tag(const message& msg)
{
    return "";
}

unsigned android_log_consumer::compose_output(const message& msg, char* buf, unsigned cch)
{
    const int len = std::snprintf(
        buf,
        cch,
        "[%s][%s][%s] - %s",
        msg.severityName,
        msg.facilityName,
        msg.threadName,
        msg.messageText
        );
    return unsigned(len);
}

}
}
