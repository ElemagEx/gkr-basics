add_library(gkr_data INTERFACE)

target_include_directories(gkr_data INTERFACE ".")

target_sources(gkr_data
    ${GKR_CPP_INTERFACE}
    "gkr/data/split_packet.hpp"
    "gkr/data/log_message.hpp"
)
