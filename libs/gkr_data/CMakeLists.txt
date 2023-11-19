add_library(gkr_data INTERFACE)

target_include_directories(gkr_data INTERFACE ".")

target_sources(
    gkr_data
    INTERFACE
    "gkr/data/split_packet.h"
    "gkr/data/log_message.h"
    )
