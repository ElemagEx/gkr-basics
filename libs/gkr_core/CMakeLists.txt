add_library(gkr_core SHARED)

if(MSVC)
target_sources(gkr_core PUBLIC "gkr/gkr.natvis")
endif()
if(WIN32)
target_link_libraries(gkr_core PRIVATE ws2_32)
endif()

target_include_directories(gkr_core PUBLIC ".")
set_target_properties(gkr_core PROPERTIES DEFINE_SYMBOL BUILDING_GKR_CORE)

# C INTERFACE
target_sources(
    gkr_core
    INTERFACE
    "gkr/api.h"
    "gkr/stamp.h"
    "gkr/diagnostics.h"
)

# C++ INTERFACE
target_sources(
    gkr_core
    INTERFACE
    "gkr/stamp.hpp"
    "gkr/container/raw_buffer.h"
    "gkr/container/lockfree_queue.h"
    "gkr/container/lockfree_grow_only_bag.h"
    "gkr/concurency/worker_thread.h"
    "gkr/concurency/events_waiting.h"
    "gkr/concurency/waitable_lockfree_queue.h"
    "gkr/misc/union_cast.h"
    "gkr/misc/stack_args_order.h"
    "gkr/cpp/lib_is_swappable.h"
    "gkr/cpp/lib_exchange_function.h"
    "gkr/cpp/lib_raw_memory_algorithms.h"
    "gkr/sys/stack_alloc.h"
    "gkr/sys/file.hpp"
    "gkr/sys/thread.hpp"
    "gkr/sys/process.hpp"
    "gkr/net/lib.h"
    "gkr/net/address.h"
    "gkr/net/socket.h"
    )
target_sources(
    gkr_core
    PRIVATE
    "gkr/stamp.cpp"
    "gkr/concurency/worker_thread.cpp"
    "gkr/sys/file.cpp"
    "gkr/sys/thread.cpp"
    "gkr/sys/process.cpp"
    "gkr/net/lib.cpp"
    "gkr/net/address.cpp"
    "gkr/net/socket.cpp"
    )
