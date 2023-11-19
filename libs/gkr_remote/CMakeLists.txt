add_library(gkr_remote SHARED)

target_link_libraries(gkr_remote PRIVATE gkr_core gkr_data gkr_log)#TODO:remote log dependency
target_include_directories(gkr_remote PUBLIC ".")

set_target_properties(gkr_remote PROPERTIES DEFINE_SYMBOL BUILDING_GKR_REMOTE)

target_sources(
    gkr_remote
    INTERFACE
    "gkr/remote/udpSocketReceiver.h"
    )
target_sources(
    gkr_remote
    PRIVATE
    "gkr/remote/udpSocketReceiver.cpp"
    )
