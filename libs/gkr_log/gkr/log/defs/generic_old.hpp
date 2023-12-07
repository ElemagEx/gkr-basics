#pragma once

#include <gkr/log/defs/generic.hpp>

#ifdef  LOG_GENERIC_DEF_NEW
#error  You cannot use both "generic_old.hpp" and "generic_new.hpp" - choose only one
#endif
#define LOG_GENERIC_DEF_OLD

#ifndef GKR_LOG_USE_C_DEFS

namespace GKR_LOG_NS
{
template<int severity, bool>
inline int log_simple(int facility, const char* message)
{
    return gkr_log_simple_message(severity, facility, message);
}
template<> inline int log_simple<LOG_SEVERITY_FATAL  ,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_ERROR  ,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_WARN   ,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_INFO   ,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_VERBOSE,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_DEBUG  ,false>(int, const char*) {}
template<> inline int log_simple<LOG_SEVERITY_TRACE  ,false>(int, const char*) {}

template<int severity, bool>
inline int log_simple_if(bool condition, int facility, const char* message)
{
    if(condition) {
        return gkr_log_simple_message(severity, facility, message);
    } else {
        return 0;
    }
}
template<> inline int log_simple_if<LOG_SEVERITY_FATAL  ,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_ERROR  ,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_WARN   ,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_INFO   ,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_VERBOSE,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_DEBUG  ,false>(bool, int, const char*) {}
template<> inline int log_simple_if<LOG_SEVERITY_TRACE  ,false>(bool, int, const char*) {}
}

template<int severity>
inline int LOG        (int facility, const char* message) { return GKR_LOG_NS::log_simple<severity            , log_threshold(severity            )>(facility, message); }

inline int LOGF       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility, message); }
inline int LOGE       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility, message); }
inline int LOGW       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility, message); }
inline int LOGI       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility, message); }
inline int LOGV       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility, message); }
inline int LOGD       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility, message); }
inline int LOGT       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility, message); }

inline int LOG_FATAL  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility, message); }
inline int LOG_ERROR  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility, message); }
inline int LOG_WARN   (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility, message); }
inline int LOG_INFO   (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility, message); }
inline int LOG_VERBOSE(int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility, message); }
inline int LOG_DEBUG  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility, message); }
inline int LOG_TRACE  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility, message); }

template<int severity>
inline int LOG_IF         (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<severity            , log_threshold(severity            )>(condition, facility, message); }

inline int LOGF_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility, message); }
inline int LOGE_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility, message); }
inline int LOGW_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility, message); }
inline int LOGI_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility, message); }
inline int LOGV_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility, message); }
inline int LOGD_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility, message); }
inline int LOGT_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility, message); }

inline int LOG_FATAL_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility, message); }
inline int LOG_ERROR_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility, message); }
inline int LOG_WARN_IF    (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility, message); }
inline int LOG_INFO_IF    (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility, message); }
inline int LOG_VERBOSE_IF (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility, message); }
inline int LOG_DEBUG_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility, message); }
inline int LOG_TRACE_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility, message); }

#ifndef GKR_NO_PRINTF_LOGGING

namespace GKR_LOG_NS
{
template<int severity, bool>
inline int log_printf(int facility, const char* format, va_list args)
{
    return gkr_log_valist_message(severity, facility, format, args);
}
template<> int log_printf<LOG_SEVERITY_FATAL  ,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_ERROR  ,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_WARN   ,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_INFO   ,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_VERBOSE,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_DEBUG  ,false>(int, const char*, va_list) { return 0; }
template<> int log_printf<LOG_SEVERITY_TRACE  ,false>(int, const char*, va_list) { return 0; }

template<int severity, bool>
inline int log_printf_if(bool condition, int facility, const char* format, va_list args)
{
    if(condition) {
        return gkr_log_valist_message(severity, facility, format, args);
    } else {
        return 0;
    }
}
template<> inline int log_printf_if<LOG_SEVERITY_FATAL  ,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_ERROR  ,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_WARN   ,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_INFO   ,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_VERBOSE,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_DEBUG  ,false>(bool, int, const char*, va_list) { return 0; }
template<> inline int log_printf_if<LOG_SEVERITY_TRACE  ,false>(bool, int, const char*, va_list) { return 0; }
}

template<int severity>
inline int LOG_        (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<severity            , log_threshold(severity            )>(facility, format, args); va_end(args); return no; }

inline int LOGF_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility, format, args); va_end(args); return no; }
inline int LOGE_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility, format, args); va_end(args); return no; }
inline int LOGW_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility, format, args); va_end(args); return no; }
inline int LOGI_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility, format, args); va_end(args); return no; }
inline int LOGV_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility, format, args); va_end(args); return no; }
inline int LOGD_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility, format, args); va_end(args); return no; }
inline int LOGT_       (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility, format, args); va_end(args); return no; }

inline int LOG_FATAL_  (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility, format, args); va_end(args); return no; }
inline int LOG_ERROR_  (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility, format, args); va_end(args); return no; }
inline int LOG_WARN_   (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility, format, args); va_end(args); return no; }
inline int LOG_INFO_   (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility, format, args); va_end(args); return no; }
inline int LOG_VERBOSE_(int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility, format, args); va_end(args); return no; }
inline int LOG_DEBUG_  (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility, format, args); va_end(args); return no; }
inline int LOG_TRACE_  (int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility, format, args); va_end(args); return no; }

template<int severity>
inline int LOG_IF_        (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<severity            , log_threshold(severity            )>(condition, facility, format, args); va_end(args); return no; }

inline int LOGF_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOGE_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOGW_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility, format, args); va_end(args); return no; }
inline int LOGI_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility, format, args); va_end(args); return no; }
inline int LOGV_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility, format, args); va_end(args); return no; }
inline int LOGD_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOGT_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility, format, args); va_end(args); return no; }

inline int LOG_FATAL_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_ERROR_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_WARN_IF_   (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_INFO_IF_   (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_VERBOSE_IF_(bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_DEBUG_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility, format, args); va_end(args); return no; }
inline int LOG_TRACE_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); int no = GKR_LOG_NS::log_printf_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility, format, args); va_end(args); return no; }

#endif /*GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace GKR_LOG_NS
{
struct log_dummy_ostream
{
    template<typename T>
    log_dummy_ostream& operator<<(const T& data) { return *this; }
    log_dummy_ostream& operator<<(std::ostream& (*data)(std::ostream&)) { return *this; }
};
template<int severity, bool>
inline auto log_stream(int facility)
{
    return gkr::log::ostream(severity, facility);
}
template<> auto log_stream<LOG_SEVERITY_FATAL  ,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_ERROR  ,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_WARN   ,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_INFO   ,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_VERBOSE,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_DEBUG  ,false>(int) { return log_dummy_ostream(); }
template<> auto log_stream<LOG_SEVERITY_TRACE  ,false>(int) { return log_dummy_ostream(); }
template<int severity, bool>
inline auto log_stream_if(bool condition, int facility)
{
    if(condition) {
        return gkr::log::ostream(severity, facility);
    } else {
        return gkr::log::ostream(nullptr);
    }
}
template<> auto log_stream_if<LOG_SEVERITY_FATAL  ,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_ERROR  ,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_WARN   ,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_INFO   ,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_VERBOSE,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_DEBUG  ,false>(bool, int) { return log_dummy_ostream(); }
template<> auto log_stream_if<LOG_SEVERITY_TRACE  ,false>(bool, int) { return log_dummy_ostream(); }
}

template<int severity>
inline auto LOG        (int facility) { return GKR_LOG_NS::log_stream<severity            , log_threshold(severity            )>(facility); }

inline auto LOGF       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility); }
inline auto LOGE       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility); }
inline auto LOGW       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility); }
inline auto LOGI       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility); }
inline auto LOGV       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility); }
inline auto LOGD       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility); }
inline auto LOGT       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility); }

inline auto LOG_FATAL  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(facility); }
inline auto LOG_ERROR  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(facility); }
inline auto LOG_WARN   (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(facility); }
inline auto LOG_INFO   (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(facility); }
inline auto LOG_VERBOSE(int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(facility); }
inline auto LOG_DEBUG  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(facility); }
inline auto LOG_TRACE  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(facility); }

template<int severity>
inline auto LOG_IF         (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<severity            , log_threshold(severity            )>(condition, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_FATAL  , log_threshold(LOG_SEVERITY_FATAL  )>(condition, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_ERROR  , log_threshold(LOG_SEVERITY_ERROR  )>(condition, facility); }
inline auto LOG_WARN_IF    (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_WARN   , log_threshold(LOG_SEVERITY_WARN   )>(condition, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_INFO   , log_threshold(LOG_SEVERITY_INFO   )>(condition, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_VERBOSE, log_threshold(LOG_SEVERITY_VERBOSE)>(condition, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_DEBUG  , log_threshold(LOG_SEVERITY_DEBUG  )>(condition, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_TRACE  , log_threshold(LOG_SEVERITY_TRACE  )>(condition, facility); }

#endif /*GKR_NO_STREAM_LOGGING*/

#endif /*GKR_LOG_USE_C_DEFS*/
