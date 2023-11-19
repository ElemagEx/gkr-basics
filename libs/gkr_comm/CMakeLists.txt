add_library(gkr_comm INTERFACE)

target_include_directories(gkr_comm INTERFACE ".")

target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/udpMessagePacket.h"
    )
