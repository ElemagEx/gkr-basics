#include "consumer.hpp"
#include "logging.hpp"

#include <gkr/diagnostics.h>

namespace gkr
{
namespace log
{

class c_consumer : public consumer
{
    gkr_log_consumer_callbacks m_callbacks;

public:
    c_consumer(const struct gkr_log_consumer_callbacks& callbacks) : m_callbacks(callbacks)
    {
    }
    virtual ~c_consumer() override
    {
        Assert_NotNullPtr(m_callbacks.destruct);
        (*m_callbacks.destruct)(m_callbacks.param);
    }

protected:
    virtual bool init_logging() override
    {
        Assert_NotNullPtr(m_callbacks.init_logging);
        return (0 != (*m_callbacks.init_logging)(m_callbacks.param));
    }
    virtual void done_logging() override
    {
        Assert_NotNullPtr(m_callbacks.done_logging);
        (*m_callbacks.done_logging)(m_callbacks.param);
    }
    virtual bool filter_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.filter_log_message);
        return (0 != (*m_callbacks.filter_log_message)(m_callbacks.param, &msg));
    }
    virtual void consume_log_message(const message& msg) override
    {
        Assert_NotNullPtr(m_callbacks.consume_log_message);
        (*m_callbacks.consume_log_message)(m_callbacks.param, &msg);
    }
};

}
}

#ifdef __cplusplus
extern "C" {
#endif

int gkr_log_add_c_consumer(const struct gkr_log_consumer_callbacks* callbacks)
{
    Check_Arg_NotNull(callbacks, 0);
    Check_Arg_NotNull(callbacks->destruct, 0);
    Check_Arg_NotNull(callbacks->init_logging, 0);
    Check_Arg_NotNull(callbacks->done_logging, 0);
    Check_Arg_NotNull(callbacks-> filter_log_message, 0);
    Check_Arg_NotNull(callbacks->consume_log_message, 0);
    return gkr_log_add_consumer(std::make_shared<gkr::log::c_consumer>(*callbacks));
}


#ifdef __cplusplus
}
#endif
#if 0
namespace gkr
{
namespace log
{


constexpr std::size_t CCH_MAX_BUFFER = 64 * 1024;     // 64K
constexpr std::size_t CCH_FMT_BUFFER =      1024 / 4; // 256 bytes
constexpr std::size_t CCH_TXT_BUFFER =  1 * 1024;     // 1K


struct format_buffers
{
    raw_buffer_t m_fmt;
    raw_buffer_t m_txt;
};

consumer::consumer()
{
}
consumer::~consumer()
{
    if(m_data) delete static_cast<format_buffers*>(m_data);
}

const char* consumer::format_output(const message& msg, bool local, bool colored, const char* format, unsigned* len)
{
    Check_Arg_NotNull(format, nullptr);

    if(*format == 0)
    {
        if(len != nullptr) *len = 0;
        return format;
    }
    if(m_data == nullptr)
    {
        m_data = new format_buffers;
    }
    struct tm tm;
    int ns = stamp_decompose(local, msg.stamp, tm);

    format_buffers& buffers = *static_cast<format_buffers*>(m_data);

    const std::size_t fmt_len = format_time(buffers.m_fmt, format, tm);
    if(fmt_len == 0) return nullptr;

    const std::size_t txt_len = format_text(buffers.m_txt, buffers.m_fmt.data<char>(), fmt_len, msg, ns, colored);
    if(txt_len == 0) return nullptr;

    if(len != nullptr) *len = txt_len;
    return buffers.m_txt.data<char>();
}

static std::size_t format_time(raw_buffer_t& buf, const char* fmt, struct tm& tm)
{
    Assert_Check(*fmt != 0);

    if(buf.capacity() == 0) buf.reserve(CCH_FMT_BUFFER);

    for( ; buf.capacity() <= CCH_MAX_BUFFER; buf.reserve(buf.capacity() * 2))
    {
        const std::size_t fmt_len = std::strftime(buf.data<char>(), buf.capacity(), fmt, &tm);

        if(fmt_len == 0) continue;

        return fmt_len;
    }
    //TODO:LOG - too large format buffer
    return 0;
}

static std::size_t format_text(raw_buffer_t& buf, const char* fmt, std::size_t len, const message& msg, int ns, bool colored)
{
    Assert_Check(*fmt != 0);

    if(buf.capacity() == 0) buf.reserve(CCH_TXT_BUFFER);

    for( ; buf.capacity() <= CCH_MAX_BUFFER; buf.reserve(buf.capacity() * 2))
    {
        const std::size_t txt_len = msg_construct(buf.data<char>(), buf.capacity(), fmt, len, msg, ns, colored);

        if(txt_len == 0) continue;

        return txt_len;
    }
    //TODO:LOG - too large format buffer
    return 0;
}

}
}
#endif
