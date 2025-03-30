add_library(gkr_core)

target_include_directories(gkr_core PUBLIC ".")

target_sources(gkr_core
    ${GKR_C_INTERFACE}
    "gkr/capi/api.h"
    "gkr/capi/url.h"
    "gkr/capi/defs.h"
    "gkr/capi/stamp.h"
    "gkr/capi/params.h"
    "gkr/capi/stack_alloc.h"
    "gkr/capi/net/lib.h"
)
target_sources(gkr_core
    ${GKR_CPP_INTERFACE}
    "gkr/api.hpp"
    "gkr/url.hpp"
    "gkr/defs.hpp"
    "gkr/stamp.hpp"
    "gkr/params.hpp"
    "gkr/concurency/waitable_lockfree_queue.hpp"
    "gkr/concurency/waitable_semaphore.hpp"
    "gkr/concurency/waitable_object.hpp"
    "gkr/concurency/waitable_event.hpp"
    "gkr/concurency/worker_thread.hpp"
)
target_sources(gkr_core
    ${GKR_CPP_INTERFACE}
    "gkr/stack_alloc.hpp"
    "gkr/misc/ptr_sentry.hpp"
    "gkr/misc/union_cast.hpp"
    "gkr/misc/stack_args_order.hpp"
    "gkr/sys/path.hpp"
    "gkr/sys/file.hpp"
    "gkr/sys/thread.hpp"
    "gkr/sys/process.hpp"
    "gkr/sys/rw_lock.hpp"
    "gkr/net/lib.hpp"
    "gkr/net/address.hpp"
    "gkr/net/socket.hpp"
    "gkr/net/udp_sender.hpp"
    "gkr/net/udp_receiver.hpp"
    "gkr/net/tcp_listener.hpp"
    "gkr/net/tcp_connection.hpp"
)
target_sources(gkr_core
    PRIVATE
    "gkr/url.cpp"
    "gkr/defs.cpp"
    "gkr/stamp.cpp"
    "gkr/params.cpp"
    "gkr/concurency/waitable_semaphore.cpp"
    "gkr/concurency/waitable_object.cpp"
    "gkr/concurency/waitable_event.cpp"
    "gkr/concurency/worker_thread.cpp"
    "gkr/sys/path.cpp"
    "gkr/sys/file.cpp"
    "gkr/sys/thread.cpp"
    "gkr/sys/process.cpp"
    "gkr/sys/rw_lock.cpp"
    "gkr/net/lib.cpp"
    "gkr/net/address.cpp"
    "gkr/net/socket.cpp"
    "gkr/net/udp_sender.cpp"
    "gkr/net/udp_receiver.cpp"
    "gkr/net/tcp_listener.cpp"
    "gkr/net/tcp_connection.cpp"
)
if(WIN32)
target_sources(gkr_core PUBLIC "gkr/gkr_core.natvis")
target_link_libraries(gkr_core PRIVATE ws2_32)
endif()

target_link_libraries(gkr_core PUBLIC gkr_inline gkr_data)

set_target_properties(gkr_core PROPERTIES DEFINE_SYMBOL BUILDING_GKR_CORE)

if(GKR_BASICS_TESTS)
add_executable        (t-address "tests/t-address.cpp")
target_link_libraries (t-address PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-address PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-address)

add_executable        (t-socket "tests/t-socket.cpp")
target_link_libraries (t-socket PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-socket PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-socket)

add_executable        (t-sys_path "tests/t-sys_path.cpp")
target_link_libraries (t-sys_path PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_path PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_path)

add_executable        (t-sys_thread "tests/t-sys_thread.cpp")
target_link_libraries (t-sys_thread PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-sys_thread PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-sys_thread)

add_executable        (t-worker_thread "tests/t-worker_thread.cpp")
target_link_libraries (t-worker_thread PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-worker_thread PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-worker_thread)

add_executable        (t-waitable_object "tests/t-waitable_object.cpp")
target_link_libraries (t-waitable_object PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_object PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_object)

add_executable        (t-waitable_event "tests/t-waitable_event.cpp")
target_link_libraries (t-waitable_event PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_event PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_event)

add_executable        (t-waitable_semaphore "tests/t-waitable_semaphore.cpp")
target_link_libraries (t-waitable_semaphore PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-waitable_semaphore PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-waitable_semaphore)
endif()
