#pragma once

#include <gkr/capi/log/galery.h>

#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/text_file_consumer.hpp>
#include <gkr/log/consumers/android_log_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>
#include <gkr/log/consumers/windows_debugger_consumer.hpp>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
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
    plog_formatter_txt,
    plog_formatter_txt_utc,
    plog_formatter_csv,
    plog_formatter_csv_utc,

    plog_stream_std_out = gkr_log_appConsoleWriteMethod_stream2cout,
    plog_stream_std_err = gkr_log_appConsoleWriteMethod_stream2cerr,
};
constexpr int plog_format_flags[] = {0, 0, 0, gkr_log_fo_flag_use_utc_time, 0, gkr_log_fo_flag_use_utc_time};
constexpr const char* plog_format_strs[] = {PLOG_ONLY_MSG_FORMAT, PLOG_FUNC_MSG_FORMAT, PLOG_FULL_MSG_FORMAT, PLOG_FULL_MSG_FORMAT, PLOG_CSV_FORMAT, PLOG_CSV_FORMAT};
constexpr const char* plog_format_args[] = PLOG_CONSOLE_ARGS_STRS;

template<int formatter>
inline int plog_add_appender_color_console(void* instance = nullptr, int method = plog_stream_std_out)
{
    constexpr int       flags = value_by_inidex(formatter, plog_format_flags) | gkr_log_fo_flag_use_padding | gkr_log_fo_flag_use_inserts;
    constexpr const char* fmt = value_by_inidex(formatter, plog_format_strs );

    return gkr::log::add_composer_consumer<gkr::log::app_console_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool colored)
    {
        const int colored_flags = flags | (colored ? gkr_log_fo_flag_use_coloring : 0);
        return gkr_log_format_output(fmt, &msg, colored_flags, plog_format_args, PLOG_CONSOLE_ARGS_ROWS, PLOG_CONSOLE_ARGS_COLS, len);
    },
    method);
}
template<int formatter>
inline int plog_add_appender_console(void* instance = nullptr, int method = plog_stream_std_out)
{
    constexpr int       flags = value_by_inidex(formatter, plog_format_flags) | gkr_log_fo_flag_use_padding;
    constexpr const char* fmt = value_by_inidex(formatter, plog_format_strs );

    return gkr::log::add_composer_consumer<gkr::log::app_console_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
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
    constexpr int       flags = value_by_inidex(formatter, plog_format_flags);
    constexpr const char* fmt = value_by_inidex(formatter, plog_format_strs );

    return gkr::log::add_composer_consumer<gkr::log::android_log_consumer_ex>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
        return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
    },
    tag, levels, sizeof(levels)/sizeof(*levels));
}
template<int formatter>
inline int plog_add_appender_debug_output(void* instance = nullptr)
{
    constexpr int       flags = value_by_inidex(formatter, plog_format_flags);
    constexpr const char* fmt = value_by_inidex(formatter, plog_format_strs );

    return gkr::log::add_composer_consumer<gkr::log::windows_debugger_consumer>(instance, [] (const gkr::log::message& msg, unsigned* len, bool)
    {
        return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
    });
}

template<int formatter>
inline int plog_add_appender_rolling_file(void* instance, const char* file_name, unsigned max_file_size = 0, int max_files = 0)
{
    constexpr bool is_csv_fmt = ((formatter == plog_formatter_csv) || (formatter == plog_formatter_csv_utc));

    constexpr int       flags = value_by_inidex(formatter, plog_format_flags) | (is_csv_fmt ? gkr_log_fo_flag_escape_text_dquote : 0);
    constexpr const char* fmt = value_by_inidex(formatter, plog_format_strs );

    return gkr::log::add_text_file_consumer<gkr::log::text_file_consumer>(
        instance,
        [] (const gkr::log::message& msg, unsigned* len, bool)
        {
            return gkr_log_format_output(fmt, &msg, flags, nullptr, 0, 0, len);
        },
        [] (gkr::log::text_file_consumer& parent) //on_opened
        {
            if_constexpr(is_csv_fmt)
            {
                parent.write_line(PLOG_CSV_HEAD, sizeof(PLOG_CSV_HEAD)-1);
            }
        },
        [] (gkr::log::text_file_consumer&) //on_closing
        {
        },
        [] (gkr::log::text_file_consumer&) //on_enter_write_line
        {
        },
        [max_file_size, max_files] (gkr::log::text_file_consumer& parent) //on_leave_write_line
        {
            if((max_files > 0) && (max_file_size > 0) && (parent.get_size() > max_file_size))
            {
                parent.roll(unsigned(max_files));
            }
        },
        file_name
        );
}

#endif
