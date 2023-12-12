#pragma once

#include <gkr/log/galery.h>
#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/android_log_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.hpp>

#ifdef __ANDROID__
#include <android/log.h>
#endif

template<typename T, unsigned SIZE>
constexpr T value_by_inidex(int index, T (&arr)[SIZE])
{
    return (unsigned(index) < SIZE)
        ?  arr[index]
        : *arr;
}

//
// PLOG
//
#ifndef GKR_GALERY_PLOG

enum
{
    plog_formatter_only_msg,
    plog_formatter_func_msg,
    plog_formatter_csv,
    plog_formatter_csv_utc,
    plog_formatter_txt,
    plog_formatter_txt_utc,

    plog_stream_std_out = gkr_log_appConsoleWriteMethod_stream2cout,
    plog_stream_std_err = gkr_log_appConsoleWriteMethod_stream2cerr,
};
constexpr int plog_format_flags[] = {0, 0, 0, gkr_log_fo_flag_use_utc_time, 0, gkr_log_fo_flag_use_utc_time};
constexpr const char* plog_format_strs[] = {PLOG_ONLY_MSG_FORMAT, PLOG_FUNC_MSG_FORMAT, PLOG_CSV_FORMAT, PLOG_CSV_FORMAT, PLOG_FULL_MSG_FORMAT, PLOG_FULL_MSG_FORMAT};
constexpr const char* plog_format_args[] = PLOG_CONSOLE_ARGS_STRS;

template<int formatter>
constexpr int plog_calc_flags(bool colored = false)
{
    return value_by_inidex(formatter, plog_format_flags) | (colored ? gkr_log_fo_flag_use_coloring : 0);
}
template<int formatter>
constexpr const char* plog_calc_fmt()
{
    return value_by_inidex(formatter, plog_format_strs);
}

template<int formatter>
inline int plog_add_appender_color_console(void* instance = nullptr, int method = plog_stream_std_out)
{
    return gkr::log::add_consumer<gkr::log::app_console_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool colored)
    {
        int                 flags = plog_calc_flags<formatter>(colored) | gkr_log_fo_flag_use_padding | gkr_log_fo_flag_use_inserts;
        constexpr const char* fmt = plog_calc_fmt  <formatter>();
        return gkr_log_format_output(fmt, &msg, flags, plog_format_args, PLOG_CONSOLE_ARGS_COLS, PLOG_CONSOLE_ARGS_ROWS, len);
    },
    method);
}
template<int formatter>
inline int plog_add_appender_console(void* instance = nullptr, int method = plog_stream_std_out)
{
    return gkr::log::add_consumer<gkr::log::app_console_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
        constexpr int       flags = plog_calc_flags<formatter>() | gkr_log_fo_flag_use_padding;
        constexpr const char* fmt = plog_calc_fmt  <formatter>();
        return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
    },
    method);
}
template<int formatter>
inline int plog_add_appender_android(void* instance = nullptr, const char* tag = "")
{
#ifdef __ANDROID__
    constexpr int levels[] = {ANDROID_LOG_UNKNOWN, ANDROID_LOG_FATAL, ANDROID_LOG_ERROR, ANDROID_LOG_WARN, ANDROID_LOG_INFO, ANDROID_LOG_DEBUG, ANDROID_LOG_VERBOSE};
#else
    constexpr int levels[] = {0, 7, 6, 5, 4, 3, 2};
#endif
    return gkr::log::add_consumer<gkr::log::android_log_consumer_ex>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
        constexpr int       flags = plog_calc_flags<formatter>();
        constexpr const char* fmt = plog_calc_fmt  <formatter>();
        return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
    },
    tag, levels, sizeof(levels)/sizeof(*levels));
}
template<int formatter>
inline int plog_add_appender_debug_output(void* instance = nullptr)
{
    return gkr::log::add_consumer<gkr::log::windows_debugger_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
        constexpr int       flags = plog_calc_flags<formatter>() | gkr_log_fo_flag_use_padding;
        constexpr const char* fmt = plog_calc_fmt  <formatter>();
        return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
    });
}

#endif
