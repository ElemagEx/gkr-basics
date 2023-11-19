add_library(gkr_comm SHARED)

target_link_libraries(gkr_comm PRIVATE gkr_core gkr_data)
target_include_directories(gkr_comm PUBLIC ".")

set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_REMOTE)

target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/udpSocketReceiver.h"
    )
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/udpSocketReceiver.cpp"
    )
