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
    "gkr/stack_alloc.h"
    "gkr/diagnostics.h"
)

# C++ INTERFACE PUBLIC
target_sources(
    gkr_core
    INTERFACE
    "gkr/stamp.hpp"
    "gkr/container/raw_buffer.hpp"
    "gkr/container/lockfree_queue.hpp"
    "gkr/container/lockfree_grow_only_bag.hpp"
    "gkr/concurency/worker_thread.hpp"
    "gkr/concurency/events_waiting.hpp"
    "gkr/concurency/waitable_lockfree_queue.hpp"
    "gkr/cpp/lib_is_swappable.hpp"
    "gkr/cpp/lib_exchange_function.hpp"
    "gkr/cpp/lib_raw_memory_algorithms.hpp"
)

# C++ INTERFACE INTERNAL
target_sources(
    gkr_core
    INTERFACE
    "gkr/misc/union_cast.hpp"
    "gkr/misc/stack_args_order.hpp"
    "gkr/sys/file.hpp"
    "gkr/sys/thread.hpp"
    "gkr/sys/process.hpp"
    "gkr/net/lib.hpp"
    "gkr/net/address.hpp"
    "gkr/net/socket.hpp"
)

# C++ PRIVATE sources
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
