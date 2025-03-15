add_library(gkr_log)

target_include_directories(gkr_log PUBLIC ".")
target_include_directories(gkr_log PUBLIC "gkr/log/imposters")

target_sources(gkr_log
    ${GKR_C_INTERFACE}
    "gkr/capi/log/galery.h"
    "gkr/capi/log/log.h"
    "gkr/capi/log/logging.h"
    "gkr/capi/log/consumer.h"
    "gkr/capi/log/defs/generic.h"
    "gkr/capi/log/defs/generic_cdefs.h"
    "gkr/capi/log/consumers/dummy_consumer.h"
    "gkr/capi/log/consumers/text_file_consumer.h"
    "gkr/capi/log/consumers/udp_socket_consumer.h"
    "gkr/capi/log/consumers/android_log_consumer.h"
    "gkr/capi/log/consumers/app_console_consumer.h"
    "gkr/capi/log/consumers/windows_debugger_consumer.h"
    "gkr/capi/log/formatters/win32_format_message.h"
)
target_sources(gkr_log
    ${GKR_CPP_INTERFACE}
    "gkr/log/log.hpp"
    "gkr/log/logging.hpp"
    "gkr/log/galery.hpp"
    "gkr/log/consumer.hpp"
    "gkr/log/defs/generic.hpp"
    "gkr/log/defs/generic_new.hpp"
    "gkr/log/defs/generic_old.hpp"
    "gkr/log/defs/generic_old_cdefs.hpp"
    "gkr/log/consumers/dummy_consumer.hpp"
    "gkr/log/consumers/text_file_consumer.hpp"
    "gkr/log/consumers/udp_socket_consumer.hpp"
    "gkr/log/consumers/android_log_consumer.hpp"
    "gkr/log/consumers/app_console_consumer.hpp"
    "gkr/log/consumers/windows_debugger_consumer.hpp"
    "gkr/log/formatters/win32_format_message.hpp"
)
target_sources(gkr_log
    PRIVATE
    "gkr/log/logger.hxx"
)
target_sources(gkr_log
    PRIVATE
    "gkr/log/galery.cpp"
    "gkr/log/logger.cpp"
    "gkr/log/logging.cpp"
    "gkr/log/consumer.cpp"
    "gkr/log/consumers/dummy_consumer.cpp"
    "gkr/log/consumers/text_file_consumer.cpp"
    "gkr/log/consumers/udp_socket_consumer.cpp"
    "gkr/log/consumers/android_log_consumer.cpp"
    "gkr/log/consumers/app_console_consumer.cpp"
    "gkr/log/consumers/windows_debugger_consumer.cpp"
)
target_sources(gkr_log
    ${GKR_CPP_INTERFACE}
    "gkr/log/imposters/plog/Init.h"
    "gkr/log/imposters/plog/Log.h"
    "gkr/log/imposters/plog/Logger.h"
    "gkr/log/imposters/plog/Record.h"
    "gkr/log/imposters/plog/Severity.h"
    "gkr/log/imposters/plog/Util.h"
    "gkr/log/imposters/plog/WinApi.h"
    "gkr/log/imposters/plog/Initializers/ConsoleInitializer.h"
    "gkr/log/imposters/plog/Initializers/RollingFileInitializer.h"
    "gkr/log/imposters/plog/Appenders/IAppender.h"
    "gkr/log/imposters/plog/Appenders/AndroidAppender.h"
    "gkr/log/imposters/plog/Appenders/ArduinoAppender.h"
    "gkr/log/imposters/plog/Appenders/ColorConsoleAppender.h"
    "gkr/log/imposters/plog/Appenders/ConsoleAppender.h"
    "gkr/log/imposters/plog/Appenders/DebugOutputAppender.h"
    "gkr/log/imposters/plog/Appenders/DynamicAppender.h"
    "gkr/log/imposters/plog/Appenders/EventLogAppender.h"
    "gkr/log/imposters/plog/Appenders/RollingFileAppender.h"
    "gkr/log/imposters/plog/Formatters/CsvFormatter.h"
    "gkr/log/imposters/plog/Formatters/FuncMessageFormatter.h"
    "gkr/log/imposters/plog/Formatters/MessageOnlyFormatter.h"
    "gkr/log/imposters/plog/Formatters/TxtFormatter.h"
    "gkr/log/imposters/plog/Converters/NativeEOLConverter.h"
    "gkr/log/imposters/plog/Converters/UTF8Converter.h"
    "gkr/log/imposters/plog/Helpers/AscDump.h"
    "gkr/log/imposters/plog/Helpers/HexDump.h"
    "gkr/log/imposters/plog/Helpers/PrintVar.h"
)
target_sources(gkr_log
    ${GKR_CPP_INTERFACE}
    "gkr/log/imposters/plog/Wrapper.h"
)

target_link_libraries(gkr_log PUBLIC  gkr_inine gkr_core)
target_link_libraries(gkr_log PRIVATE gkr_data  gkr_comm)

set_target_properties(gkr_log PROPERTIES DEFINE_SYMBOL BUILDING_GKR_LOG)

if(GKR_BASICS_TESTS)
add_executable        (t-logging "tests/t-logging.cpp")
target_link_libraries (t-logging PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
target_compile_options(t-logging PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-logging)

add_executable        (t-udp_socket_consumer_receiver "tests/t-udp_socket_consumer_receiver.cpp")
target_link_libraries (t-udp_socket_consumer_receiver PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_data gkr_log gkr_comm)
target_compile_options(t-udp_socket_consumer_receiver PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-udp_socket_consumer_receiver)

#add_executable        (t-log_fmt_output "tests/t-log_fmt_output.cpp")
#target_link_libraries (t-log_fmt_output PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
#target_compile_options(t-log_fmt_output PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-log_fmt_output)
#
#add_executable        (t-log_consumer_text_file "tests/t-log_consumer_text_file.cpp")
#target_link_libraries (t-log_consumer_text_file PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
#target_compile_options(t-log_consumer_text_file PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-log_consumer_text_file)
#
#add_executable        (t-log_defs_generic "tests/t-log_defs_generic.cpp")
#target_link_libraries (t-log_defs_generic PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
#target_compile_options(t-log_defs_generic PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-log_defs_generic)
#
#add_executable        (t-log_defs_plog "tests/t-log_defs_plog.cpp")
#target_link_libraries (t-log_defs_plog PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core gkr_log)
#target_compile_options(t-log_defs_plog PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-log_defs_plog)
endif()
