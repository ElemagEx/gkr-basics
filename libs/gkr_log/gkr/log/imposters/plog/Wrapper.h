#pragma once

#include <plog/Converters/NativeEOLConverter.h>
#include <plog/Converters/UTF8Converter.h>
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
#include <plog/Appenders/RollingFileAppender.h>

#include <gkr/diagnostics.h>
#include <gkr/log/logging.hpp>
#include <gkr/log/galery.hpp>

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

    converter_Utf8Converter                   ,
    converter_NativeEOLConverter_UTF8Converter,
    converter_Other               ,
};
struct formatter_t
{
    std::string (*header)();
    std::string (*format)(const plog::Record&);
    int type;
    int instanceId;
};

inline consumer_ptr_t makePlogConsumerWrapper0(int instanceId, int instanceIdAppender)
{
    return nullptr;
}

inline consumer_ptr_t makePlogConsumerWrapper1(int instanceId, plog::DynamicAppender* appender)
{
    return nullptr;
}

template<class Formatter>
constexpr int calc_formatter() { return -1; }

template<> constexpr int calc_formatter<plog::CsvFormatter        >() { return plog_formatter_csv     ; }
template<> constexpr int calc_formatter<plog::CsvFormatterUtcTime >() { return plog_formatter_csv_utc ; }
template<> constexpr int calc_formatter<plog::TxtFormatter        >() { return plog_formatter_txt     ; }
template<> constexpr int calc_formatter<plog::TxtFormatterUtcTime >() { return plog_formatter_txt_utc ; }
template<> constexpr int calc_formatter<plog::FuncMessageFormatter>() { return plog_formatter_only_msg; }
template<> constexpr int calc_formatter<plog::MessageOnlyFormatter>() { return plog_formatter_func_msg; }

template<template<class> class Appender, class Formatter>
int addPlogConsumerWrapper2(int instanceId, Appender<Formatter>* appender)
{
    Check_Arg_NotNull(appender, nullptr);

    constexpr int formatter = calc_formatter<Formatter>();

    if_constexpr(formatter != -1)
    {
        if_constexpr(std::is_same<Appender<Formatter>, plog::ColorConsoleAppender<Formatter> >::value) { return plog_add_appender_color_console<formatter>(nullptr, *static_cast<const int *>(appender->data())); } else
        if_constexpr(std::is_same<Appender<Formatter>, plog::ConsoleAppender     <Formatter> >::value) { return plog_add_appender_console      <formatter>(nullptr, *static_cast<const int *>(appender->data())); } else
        if_constexpr(std::is_same<Appender<Formatter>, plog::ConsoleAppender     <Formatter> >::value) { return plog_add_appender_android      <formatter>(nullptr, *static_cast<const char*>(appender->data())); } else
        if_constexpr(std::is_same<Appender<Formatter>, plog::ConsoleAppender     <Formatter> >::value) { return plog_add_appender_debug_output <formatter>(nullptr                                             ); } else
        {
            return 0;
        }
    //  if_constexpr(std::is_same<Appender<Formatter>, plog::AndroidAppender     <Formatter> >::value) 
    //  if_constexpr(std::is_same<Appender<Formatter>, plog::ArduinoAppender     <Formatter> >::value) 
    //  if_constexpr(std::is_same<Appender<Formatter>, plog::EventLogAppender    <Formatter> >::value) 
    }
    else 
    {
        return 0;
    }
}
template<class Formatter, class Converter>
consumer_ptr_t makePlogConsumerWrapper3(int instanceId, plog::RollingFileAppender<Formatter, Converter>* appender)
{
    Check_Arg_NotNull(appender, nullptr);

    formatter_t formatter1 {nullptr, nullptr, formatter_Other, instanceId};
    if_constexpr(std::is_same<Formatter, plog::CsvFormatter        >::value) formatter1.type = formatter_CsvFormatter        ; else
    if_constexpr(std::is_same<Formatter, plog::CsvFormatterUtcTime >::value) formatter1.type = formatter_CsvFormatterUtcTime ; else
    if_constexpr(std::is_same<Formatter, plog::TxtFormatter        >::value) formatter1.type = formatter_TxtFormatter        ; else
    if_constexpr(std::is_same<Formatter, plog::TxtFormatterUtcTime >::value) formatter1.type = formatter_TxtFormatterUtcTime ; else
    if_constexpr(std::is_same<Formatter, plog::FuncMessageFormatter>::value) formatter1.type = formatter_FuncMessageFormatter; else
    if_constexpr(std::is_same<Formatter, plog::MessageOnlyFormatter>::value) formatter1.type = formatter_MessageOnlyFormatter; else
    {
        formatter1.header = &Formatter::header;
        formatter1.format = &Formatter::format;
    }
    formatter_t formatter2 {nullptr, nullptr, converter_Other, instanceId};
    if(std::is_same<Converter, plog::UTF8Converter                           >::value) formatter2.type = converter_Utf8Converter; else
    if(std::is_same<Converter, plog::NativeEOLConverter<plog::UTF8Converter> >::value) formatter2.type = converter_NativeEOLConverter_UTF8Converter; else
    {
        formatter2.header = &Converter::header;
        formatter2.format = &Converter::format;
    }
    return makeRollingFileAppenderWrapper(formatter1, formatter2, *static_cast<const plog::RollingFileData*>(appender->data()));
}

inline consumer_ptr_t makePlogConsumerWrapper4(int instanceId, plog::IAppender* appender)
{
    return nullptr;
}

}
}
}
