add_library(gkr_inline INTERFACE)

target_include_directories(gkr_inline INTERFACE ".")

target_sources(gkr_inline
    ${GKR_C_INTERFACE}
    "gkr/capi/diagnostics.h"
)
target_sources(gkr_inline
    ${GKR_CPP_INTERFACE}
    "gkr/diagnostics.hpp"
    "gkr/container/raw_buffer.hpp"
    "gkr/container/lockfree_queue.hpp"
    "gkr/container/lockfree_grow_only_bag.hpp"
    "gkr/cpp/lib_is_swappable.hpp"
    "gkr/cpp/lib_exchange_function.hpp"
)

if(WIN32)
target_sources(gkr_inline PUBLIC "gkr/gkr_inline.natvis")
endif()

if(GKR_BASICS_TESTS)
#add_executable        (t-diagnostic "tests/t-diagnostic.cpp")
#target_link_libraries (t-diagnostic PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
#target_compile_options(t-diagnostic PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
#catch_discover_tests  (t-diagnostic)

add_executable        (t-raw_buffer "tests/t-raw_buffer.cpp")
target_link_libraries (t-raw_buffer PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-raw_buffer PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-raw_buffer)

add_executable        (t-lockfree_grow_only_bag "tests/t-lockfree_grow_only_bag.cpp")
target_link_libraries (t-lockfree_grow_only_bag PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-lockfree_grow_only_bag PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-lockfree_grow_only_bag)

add_executable        (t-lockfree_queue "tests/t-lockfree_queue.cpp")
target_link_libraries (t-lockfree_queue PRIVATE Catch2::Catch2WithMain gkr_testing gkr_core)
target_compile_options(t-lockfree_queue PRIVATE ${GKR_BASICS_TESTING_WARNINGS})
catch_discover_tests  (t-lockfree_queue)
endif()
