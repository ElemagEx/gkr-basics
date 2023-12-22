add_library(gkr_comm)

target_include_directories(gkr_comm PUBLIC ".")

target_sources(gkr_comm
    ${GKR_CPP_INTERFACE}
    "gkr/comm/api.h"
    "gkr/comm/udp_socket_sender.hpp"
    "gkr/comm/udp_socket_receiver.hpp"
)
target_sources(gkr_comm
    PRIVATE
    "gkr/comm/udp_socket_sender.cpp"
    "gkr/comm/udp_socket_receiver.cpp"
)

target_link_libraries(gkr_comm PUBLIC gkr_headers_only gkr_core gkr_data)

set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_COMM)
