#pragma once

#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/AndroidAppender.h>
#include <plog/Appenders/ArduinoAppender.h>
#include <plog/Appenders/EventLogAppender.h>
#include <plog/Appenders/DynamicAppender.h>

#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>

#include <type_traits>
#include <string>

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
#endif

namespace gkr
{
namespace log
{
namespace impl
{
enum
{
//  appender_DynamicAppender     ,
    appender_ColorConsoleAppender,
    appender_ConsoleAppender     ,
    appender_AndroidAppender     ,
    appender_ArduinoAppender     ,
    appender_EventLogAppender    ,
    appender_Other               ,

    formatter_CsvFormatter        ,
    formatter_CsvFormatterUtcTime ,
    formatter_TxtFormatter        ,
    formatter_TxtFormatterUtcTime ,
    formatter_FuncMessageFormatter,
    formatter_MessageOnlyFormatter,
    formatter_Other               ,
};
struct formatter_t
{
    std::string (*header)();
    std::string (*format)(const plog::Record&);
    int type;
    int instanceId;
};
using consumer_t = std::shared_ptr<gkr::log::consumer>;

GKR_LOG_API consumer_t makeColorConsoleAppenderWrapper(formatter_t& formatter, int outStream);
GKR_LOG_API consumer_t makeConsoleAppenderWrapper     (formatter_t& formatter, int outStream);
GKR_LOG_API consumer_t makeAndroidAppenderWrapper     (formatter_t& formatter, const char* tag);
GKR_LOG_API consumer_t makeArduinoAppenderWrapper     (formatter_t& formatter);
GKR_LOG_API consumer_t makeEventLogAppenderWrapper    (formatter_t& formatter);

template<template<class> class Appender, class Formatter>
consumer_t makePlogConsumerWrapper(int instanceId, Appender<Formatter>* appender)
{
    Check_Arg_NotNull(appender, nullptr);

    formatter_t formatter {nullptr, nullptr, formatter_Other, instanceId};
    if_constexpr(std::is_same<Formatter, plog::CsvFormatter        >::value) formatter.type = formatter_CsvFormatter        ; else
    if_constexpr(std::is_same<Formatter, plog::CsvFormatterUtcTime >::value) formatter.type = formatter_CsvFormatterUtcTime ; else
    if_constexpr(std::is_same<Formatter, plog::TxtFormatter        >::value) formatter.type = formatter_TxtFormatter        ; else
    if_constexpr(std::is_same<Formatter, plog::TxtFormatterUtcTime >::value) formatter.type = formatter_TxtFormatterUtcTime ; else
    if_constexpr(std::is_same<Formatter, plog::FuncMessageFormatter>::value) formatter.type = formatter_FuncMessageFormatter; else
    if_constexpr(std::is_same<Formatter, plog::MessageOnlyFormatter>::value) formatter.type = formatter_MessageOnlyFormatter; else
    {
        formatter.header = &Formatter::header;
        formatter.format = &Formatter::format;
    }
    if_constexpr(std::is_same<Appender<Formatter>, plog::ColorConsoleAppender<Formatter>>::value) return makeColorConsoleAppenderWrapper(formatter, *static_cast<const int *>(appender->data()));
    if_constexpr(std::is_same<Appender<Formatter>, plog::ConsoleAppender     <Formatter>>::value) return makeConsoleAppenderWrapper     (formatter, *static_cast<const int *>(appender->data()));
    if_constexpr(std::is_same<Appender<Formatter>, plog::AndroidAppender     <Formatter>>::value) return makeAndroidAppenderWrapper     (formatter,  static_cast<const char*>(appender->data()));
    if_constexpr(std::is_same<Appender<Formatter>, plog::EventLogAppender    <Formatter>>::value) return makeArduinoAppenderWrapper     (formatter);
    if_constexpr(std::is_same<Appender<Formatter>, plog::ArduinoAppender     <Formatter>>::value) return makeEventLogAppenderWrapper    (formatter);

    return nullptr;
}

}
}
}
