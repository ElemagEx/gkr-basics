#include <plog/Wrapper.h>

#include <gkr/log/consumers/android_log_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>
#include <gkr/log/consumers/text_file_consumer.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.h>

#include <gkr/log/galery.hpp>

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

static const char* format_output(bool colored, formatter_t& formatter, const message& msg)
{
    int flags = gkr_log_fo_flag_use_inserts | gkr_log_fo_flag_use_padding;
    if(colored) flags |= gkr_log_fo_flag_use_coloring;
    const char* fmt = "";

    switch(formatter.type)
    {
        case formatter_CsvFormatter        : fmt = PLOG_CSV_FORMAT ; break;
        case formatter_CsvFormatterUtcTime : fmt = PLOG_CSV_FORMAT ; flags |= gkr_log_fo_flag_use_utc_time; break;
        case formatter_TxtFormatter        : fmt = PLOG_TEXT_FORMAT; break;
        case formatter_TxtFormatterUtcTime : fmt = PLOG_TEXT_FORMAT; flags |= gkr_log_fo_flag_use_utc_time; break;

        case formatter_FuncMessageFormatter: fmt = PLOG_FUNC_MSG_FORMAT; break;
        case formatter_MessageOnlyFormatter: fmt = PLOG_ONLY_MSG_FORMAT; break;
    }
    constexpr const char* args[] = PLOG_TEXT_ARGS_STRS;
    return gkr_log_format_output(fmt, &msg, flags, args, PLOG_TEXT_ARGS_COLS, PLOG_TEXT_ARGS_ROWS, nullptr);
}

class ColorConsoleAppenderWrapper : public app_console_consumer
{
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
    virtual const char* compose_output(const message& msg, bool colored) override
    {
        return format_output(colored, m_formatter, msg);
    }
};
class ConsoleAppenderWrapper : public app_console_consumer
{
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
    virtual const char* compose_output(const message& msg, bool colored) override
    {
        return format_output(false, m_formatter, msg);
    }
};
class AndroidAppenderWrapper : public android_log_consumer
{
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
    virtual const char* compose_output(const message& msg) override
    {
        return format_output(false, m_formatter, msg);
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

consumer_t makeArduinoAppenderWrapper(formatter_t& formatter, const Stream& stream)
{
    return nullptr;
}

consumer_t makeEventLogAppenderWrapper(formatter_t& formatter, const char* sourceName)
{
    return nullptr;
}

consumer_t makeRollingFileAppenderWrapper(formatter_t& formatter1, formatter_t& formatter2, const plog::RollingFileData& rollingData)
{
    return nullptr;
}

}
}
}
