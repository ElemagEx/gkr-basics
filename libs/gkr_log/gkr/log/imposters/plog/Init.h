#pragma once
#include <plog/Logger.h>

namespace plog
{
template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity = none)
{
    return *impl::getLoggerInstance<instanceId>(maxSeverity);
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity = none)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity);
}

template<int instanceId, int instanceIdAppender>
inline Logger<instanceId>& init(Severity maxSeverity, Logger<instanceIdAppender>* appender)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    if(appender != nullptr) logger.addAppender(appender);
    return logger;
}
template<int instanceIdAppender>
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, Logger<instanceIdAppender>* appender)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}

template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity, DynamicAppender* appender)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    if(appender != nullptr) logger.addAppender(appender);
    return logger;
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, DynamicAppender* appender)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}

template<int instanceId, template<class> class Appender, class Formatter>
inline Logger<instanceId>& init(Severity maxSeverity, Appender<Formatter>* appender)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    if(appender != nullptr) logger.addAppender(appender);
    return logger;
}
template<template<class> class Appender, class Formatter>
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, Appender<Formatter>* appender)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}

template<int instanceId, class Formatter, class Converter>
inline Logger<instanceId>& init(Severity maxSeverity, RollingFileAppender<Formatter, Converter>* appender)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    if(appender != nullptr) logger.addAppender(appender);
    return logger;
}
template<class Formatter, class Converter>
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, RollingFileAppender<Formatter, Converter>* appender)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}

template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity, IAppender* appender)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    if(appender) logger.addAppender(appender);
    return logger;
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, IAppender* appender)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}
}
