add_library(gkr_log SHARED)

target_link_libraries(gkr_log PRIVATE gkr_core gkr_data)
target_include_directories(gkr_log PUBLIC ".")

set_target_properties(gkr_log PROPERTIES DEFINE_SYMBOL BUILDING_GKR_LOG)

# C INTERFACE
target_sources(
    gkr_log
    INTERFACE
    "gkr/log/api.h"
    "gkr/log/log.h"
    "gkr/log/logging.h"
    "gkr/log/consumer.h"
    "gkr/log/consumers/text_file_callbacks.h"
    "gkr/log/consumers/udp_socket_callbacks.h"
    "gkr/log/consumers/android_log_callbacks.h"
    "gkr/log/consumers/app_console_callbacks.h"
    "gkr/log/consumers/windows_debugger_callbacks.h"
)
# C++ INTERFACE
target_sources(
    gkr_log
    INTERFACE
    "gkr/log/log.hpp"
    "gkr/log/logging.hpp"
    "gkr/log/consumer.hpp"
    "gkr/log/consumers/text_file_consumer.hpp"
    "gkr/log/consumers/udp_socket_consumer.hpp"
    "gkr/log/consumers/android_log_consumer.hpp"
    "gkr/log/consumers/app_console_consumer.hpp"
    "gkr/log/consumers/windows_debugger_consumer.hpp"
)
# Headers
target_sources(
    gkr_log
    PRIVATE
    "gkr/log/logger.hpp"
)
# Sources
target_sources(
    gkr_log
    PRIVATE
    "gkr/log/logger.cpp"
    "gkr/log/logging.cpp"
    "gkr/log/consumers/text_file_consumer.cpp"
    "gkr/log/consumers/udp_socket_consumer.cpp"
    "gkr/log/consumers/android_log_consumer.cpp"
    "gkr/log/consumers/app_console_consumer.cpp"
    "gkr/log/consumers/windows_debugger_consumer.cpp"
)

if(GKR_BASICS_TESTS)
add_subdirectory(tests)
endif()
