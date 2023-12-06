#pragma once
#include <plog/Logger.h>

namespace plog
{
template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity = none)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    return logger;
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity = none)
{
    auto& logger = *impl::getLoggerInstance<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity);
    return logger;
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
    auto& logger = *impl::getLoggerInstance<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity);
    if(appender != nullptr) logger.addAppender(appender);
    return logger;
}

//template<int instanceId>
//inline Logger<instanceId>& init(Severity maxSeverity = none, IAppender* appender = nullptr)
//{
//    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
//    if(appender) logger.addAppender(appender);
//    return logger;
//}

//inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity = none, IAppender* appender = nullptr)
//{
//    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
//}
}
