#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Wrapper.h>
#include <plog/Util.h>
#include <vector>

#include <gkr/diagnostics.h>

#ifdef  PLOG_DEFAULT_INSTANCE
#define PLOG_DEFAULT_INSTANCE_ID PLOG_DEFAULT_INSTANCE
#endif

#ifndef PLOG_DEFAULT_INSTANCE_ID
#define PLOG_DEFAULT_INSTANCE_ID 0
#endif

namespace plog
{

template<int instanceId>
class Logger : public IAppender
{
    Severity m_maxSeverity;

public:
    Logger(Severity maxSeverity = none) noexcept : m_maxSeverity(maxSeverity)
    {
        constexpr gkr_log_name_id_pair SEVERITIES[] = {
            {severityToString(Severity::none   ), Severity::none   },
            {severityToString(Severity::fatal  ), Severity::fatal  },
            {severityToString(Severity::error  ), Severity::error  },
            {severityToString(Severity::warning), Severity::warning},
            {severityToString(Severity::info   ), Severity::info   },
            {severityToString(Severity::debug  ), Severity::debug  },
            {severityToString(Severity::verbose), Severity::verbose},
            {nullptr                            , 0                }
        };
        gkr_log_init(SEVERITIES, nullptr, 1023, 64);
    }
    virtual ~Logger() override
    {
        gkr_log_done();
    }

    template<template<class> class Appender, class Formatter>
    Logger& addAppender(Appender<Formatter>* appender)
    {
        Check_Arg_NotNull(appender, *this);
        auto consumer = gkr::log::impl::makePlogConsumerWrapper<Appender, Formatter>(instanceId, appender);
        gkr_log_add_consumer(consumer);
        return *this;
    }
//  Logger& addAppender(IAppender* appender)
//  {
//      Check_Arg_NotNull(appender, *this);
//      gkr_log_add_consumer();
//      return *this;
//  }
    Severity getMaxSeverity() const noexcept
    {
        return m_maxSeverity;
    }
    void setMaxSeverity(Severity severity) noexcept
    {
        m_maxSeverity = severity;
    }
    bool checkSeverity(Severity severity) const noexcept
    {
        return (severity <= m_maxSeverity);
    }
    void operator+=(const Record& record)
    {
    }
    virtual void write(const Record& record) override
    {
    }
};

namespace impl
{
template<int instanceId>
inline Logger<instanceId>* getLoggerInstance(Severity maxSeverity = none)
{
    static Logger<instanceId> s_instance(maxSeverity);
    return &s_instance;
}
}

template<int instanceId>
inline Logger<instanceId>* get()
{
    return impl::getLoggerInstance<instanceId>();
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>* get()
{
    return impl::getLoggerInstance<PLOG_DEFAULT_INSTANCE_ID>();
}

}
