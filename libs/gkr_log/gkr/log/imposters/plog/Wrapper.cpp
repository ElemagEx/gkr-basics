#include <plog/Wrapper.h>

#include <gkr/log/consumers/android_log_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>
#include <gkr/log/consumers/text_file_consumer.hpp>
#include <gkr/log/consumers/windows_debugger_callbacks.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace gkr
{
namespace log
{
namespace impl
{

static constexpr int calcMethod(int outStream)
{
    return (outStream == plog::OutputStream::streamStdOut)
        ? gkr_log_appConsoleWriteMethod_stream2cout
        : gkr_log_appConsoleWriteMethod_stream2cerr
        ;
}

unsigned format_output(formatter_t& formatter, char* buf, unsigned cch, const gkr::log::message& msg)
{
    const plog::Record& record = *reinterpret_cast<const plog::Record*>(&msg); //HACK

    switch(formatter.type)
    {
        case formatter_CsvFormatter        : break;
        case formatter_CsvFormatterUtcTime : break;
        case formatter_TxtFormatter        : break;
        case formatter_TxtFormatterUtcTime : break;
        case formatter_FuncMessageFormatter: break;
        case formatter_MessageOnlyFormatter: break;

        default:
        case formatter_Other:
            std::strncpy(buf, (*formatter.format)(record).c_str(), cch);
            buf[cch - 1] = 0;
            return unsigned(std::strlen(buf));
    }
    return 0;
}

class ColorConsoleAppenderWrapper : public app_console_consumer
{
    ColorConsoleAppenderWrapper           (ColorConsoleAppenderWrapper&&) noexcept = delete;
    ColorConsoleAppenderWrapper& operator=(ColorConsoleAppenderWrapper&&) noexcept = delete;

    ColorConsoleAppenderWrapper           (const ColorConsoleAppenderWrapper&) noexcept = delete;
    ColorConsoleAppenderWrapper& operator=(const ColorConsoleAppenderWrapper&) noexcept = delete;

    formatter_t m_formatter;
public:
    ColorConsoleAppenderWrapper(formatter_t& formatter, int method) : app_console_consumer(method), m_formatter(formatter)
    {
    }
protected:
    virtual bool filter_log_message(const message& msg) override
    {
        return (msg.facility != m_formatter.instanceId);
    }
    virtual unsigned compose_output(char* buf, unsigned cch, const gkr::log::message& msg) override
    {
        return format_output(m_formatter, buf, cch, msg);
    }
};
class ConsoleAppenderWrapper : public app_console_consumer
{
    ConsoleAppenderWrapper           (ConsoleAppenderWrapper&&) noexcept = delete;
    ConsoleAppenderWrapper& operator=(ConsoleAppenderWrapper&&) noexcept = delete;

    ConsoleAppenderWrapper           (const ConsoleAppenderWrapper&) noexcept = delete;
    ConsoleAppenderWrapper& operator=(const ConsoleAppenderWrapper&) noexcept = delete;

    formatter_t m_formatter;
public:
    ConsoleAppenderWrapper(formatter_t& formatter, int method) : app_console_consumer(method), m_formatter(formatter)
    {
    }
protected:
    virtual bool filter_log_message(const message& msg) override
    {
        return (msg.facility != m_formatter.instanceId);
    }
    virtual unsigned compose_output(char* buf, unsigned cch, const gkr::log::message& msg) override
    {
        return format_output(m_formatter, buf, cch, msg);
    }
};
class AndroidAppenderWrapper : public android_log_consumer
{
    AndroidAppenderWrapper           (AndroidAppenderWrapper&&) noexcept = delete;
    AndroidAppenderWrapper& operator=(AndroidAppenderWrapper&&) noexcept = delete;

    AndroidAppenderWrapper           (const AndroidAppenderWrapper&) noexcept = delete;
    AndroidAppenderWrapper& operator=(const AndroidAppenderWrapper&) noexcept = delete;

    formatter_t m_formatter;
    const char* m_tag;
public:
    AndroidAppenderWrapper(formatter_t& formatter, const char* tag) : android_log_consumer(), m_formatter(formatter), m_tag(tag)
    {
    }
protected:
    virtual bool filter_log_message(const message& msg) override
    {
        return (msg.facility != m_formatter.instanceId);
    }
    virtual int get_priority(const message& msg) override
    {
#ifndef __ANDROID__
        return 0;
#else
        switch(severity)
        {
            case fatal  : return ANDROID_LOG_FATAL;
            case error  : return ANDROID_LOG_ERROR;
            case warning: return ANDROID_LOG_WARN;
            case info   : return ANDROID_LOG_INFO;
            case debug  : return ANDROID_LOG_DEBUG;
            case verbose: return ANDROID_LOG_VERBOSE;
            default     : return ANDROID_LOG_UNKNOWN;
        }
#endif
    }
    virtual const char* get_tag(const message& msg) override
    {
        return m_tag;
    }
    virtual unsigned compose_output(char* buf, unsigned cch, const gkr::log::message& msg) override
    {
        return format_output(m_formatter, buf, cch, msg);
    }
};

consumer_t makeColorConsoleAppenderWrapper(formatter_t& formatter, int outStream)
{
    return std::make_shared<ColorConsoleAppenderWrapper>(formatter, calcMethod(outStream));
}

consumer_t makeConsoleAppenderWrapper(formatter_t& formatter, int outStream)
{
    return std::make_shared<ConsoleAppenderWrapper>(formatter, calcMethod(outStream));
}

consumer_t makeAndroidAppenderWrapper(formatter_t& formatter, const char* tag)
{
    return std::make_shared<AndroidAppenderWrapper>(formatter, tag);
}

consumer_t makeArduinoAppenderWrapper(formatter_t& formatter)
{
    return nullptr;
}

consumer_t makeEventLogAppenderWrapper(formatter_t& formatter)
{
    return nullptr;
}

}
}
}
