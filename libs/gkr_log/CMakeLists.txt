add_library(gkr_log SHARED)

target_link_libraries(gkr_log PRIVATE gkr_core gkr_data gkr_comm)
target_include_directories(gkr_log PUBLIC ".")
target_include_directories(gkr_log PUBLIC "gkr/log/imposters")

set_target_properties(gkr_log PROPERTIES DEFINE_SYMBOL BUILDING_GKR_LOG)

# C INTERFACE
target_sources(
    gkr_log
    INTERFACE
    "gkr/log/log.h"
    "gkr/log/galery.h"
    "gkr/log/logging.h"
    "gkr/log/consumer.h"
    "gkr/log/defs/generic.h"
    "gkr/log/consumers/dummy_consumer.h"
    "gkr/log/consumers/text_file_consumer.h"
    "gkr/log/consumers/udp_socket_consumer.h"
    "gkr/log/consumers/android_log_consumer.h"
    "gkr/log/consumers/app_console_consumer.h"
    "gkr/log/consumers/windows_debugger_consumer.h"
)
# C++ INTERFACE
target_sources(
    gkr_log
    INTERFACE
    "gkr/log/log.hpp"
    "gkr/log/galery.hpp"
    "gkr/log/logging.hpp"
    "gkr/log/consumer.hpp"
    "gkr/log/defs/generic.hpp"
    "gkr/log/defs/generic_new.hpp"
    "gkr/log/defs/generic_old.hpp"
    "gkr/log/consumers/dummy_consumer.hpp"
    "gkr/log/consumers/text_file_consumer.hpp"
    "gkr/log/consumers/udp_socket_consumer.hpp"
    "gkr/log/consumers/android_log_consumer.hpp"
    "gkr/log/consumers/app_console_consumer.hpp"
    "gkr/log/consumers/windows_debugger_consumer.hpp"
)
# PRIVATE Headers
target_sources(
    gkr_log
    PRIVATE
    "gkr/log/logger.hpp"
)
# PRIVATE Sources
target_sources(
    gkr_log
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
# plog original headers
target_sources(
    gkr_log
    INTERFACE
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
# plog imposter header
target_sources(
    gkr_log
    PUBLIC
    "gkr/log/imposters/plog/Wrapper.h"
)

if(GKR_BASICS_TESTS)
add_subdirectory(tests)
endif()
