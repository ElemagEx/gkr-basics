add_library(gkr_headers_only INTERFACE)

target_include_directories(gkr_headers_only INTERFACE ".")

target_sources(gkr_headers_only
    ${GKR_C_INTERFACE}
    "gkr/diagnostics.h"
    "gkr/stack_alloc.h"
)
target_sources(gkr_headers_only
    ${GKR_CPP_INTERFACE}
    "gkr/diagnostics.hpp"
    "gkr/container/raw_buffer.hpp"
    "gkr/container/lockfree_queue.hpp"
    "gkr/container/lockfree_grow_only_bag.hpp"
    "gkr/concurency/events_waiting.hpp"
    "gkr/concurency/waitable_lockfree_queue.hpp"
    "gkr/cpp/lib_is_swappable.hpp"
    "gkr/cpp/lib_exchange_function.hpp"
)

add_library(gkr_core)

target_include_directories(gkr_core PUBLIC ".")

target_sources(gkr_core
    ${GKR_C_INTERFACE}
    "gkr/api.h"
    "gkr/stamp.h"
)
target_sources(gkr_core
    ${GKR_CPP_INTERFACE}
    "gkr/stamp.hpp"
    "gkr/concurency/worker_thread.hpp"
)
target_sources(gkr_core
    ${GKR_CPP_INTERFACE}
    "gkr/misc/union_cast.hpp"
    "gkr/misc/stack_args_order.hpp"
    "gkr/sys/path.hpp"
    "gkr/sys/file.hpp"
    "gkr/sys/thread.hpp"
    "gkr/sys/process.hpp"
    "gkr/net/lib.hpp"
    "gkr/net/address.hpp"
    "gkr/net/socket.hpp"
    )
target_sources(gkr_core
    PRIVATE
    "gkr/stamp.cpp"
    "gkr/concurency/worker_thread.cpp"
    "gkr/sys/path.cpp"
    "gkr/sys/file.cpp"
    "gkr/sys/thread.cpp"
    "gkr/sys/process.cpp"
    "gkr/net/lib.cpp"
    "gkr/net/address.cpp"
    "gkr/net/socket.cpp"
)
if(WIN32)
target_sources(gkr_core PUBLIC "gkr/gkr.natvis")
target_link_libraries(gkr_core PRIVATE ws2_32)
endif()

target_link_libraries(gkr_core PUBLIC gkr_headers_only)

set_target_properties(gkr_core PROPERTIES DEFINE_SYMBOL BUILDING_GKR_CORE)

if(GKR_BASICS_TESTS)
add_subdirectory(tests)
endif()
