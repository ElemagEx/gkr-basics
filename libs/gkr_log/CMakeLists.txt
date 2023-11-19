add_library(gkr_log SHARED)

target_link_libraries(gkr_log PRIVATE gkr_core gkr_data)
target_include_directories(gkr_log PUBLIC ".")

set_target_properties(gkr_log PROPERTIES DEFINE_SYMBOL BUILDING_GKR_LOG)

target_sources(
    gkr_log
    INTERFACE
    "gkr/log/api.h"
    "gkr/log/log.h"
    "gkr/log/logging.h"
    "gkr/log/message.h"
    "gkr/log/consumer.h"
    "gkr/log/consumers/udpSocketConsumer.h"
    "gkr/log/consumers/androidLogConsumer.h"
    "gkr/log/consumers/appConsoleConsumer.h"
    "gkr/log/consumers/windowsDebuggerConsumer.h"
    )
target_sources(
    gkr_log
    PRIVATE
    "gkr/log/logger.h"
    "gkr/log/logger.cpp"
    "gkr/log/logging.cpp"
    "gkr/log/consumers/udpSocketConsumer.cpp"
    "gkr/log/consumers/androidLogConsumer.cpp"
    "gkr/log/consumers/appConsoleConsumer.cpp"
    "gkr/log/consumers/windowsDebuggerConsumer.cpp"
    )
