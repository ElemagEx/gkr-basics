cmake_minimum_required(VERSION 3.14)

project(gkr-basics)

option(GKR_BUILD_TESTS    "Build unit tests for the library" ON)
option(GKR_BUILD_EXAMPLES "Build examples" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(dependencies)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "/permissive- /EHsc /GS /sdl")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG")
else()
    set(CMAKE_CXX_FLAGS "-pthread")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
endif()

include(warnings) #includes ./cmake/warnings.cmake - sets variable ${CMAKE_CXX_FLAGS_WARNINGS}

add_compile_definitions(DIAG_MODE=4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_WARNINGS}")

set(gkr_basics_inc_path "${CMAKE_CURRENT_SOURCE_DIR}/include/gkr")
set(gkr_basics_src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(
    gkr-basics
    STATIC
    "${gkr_basics_src_path}/libmain.cpp"
)

if(MSVC)
target_sources(gkr-basics PUBLIC gkr.natvis)
endif()

# Headers only
target_sources(
    gkr-basics
    PRIVATE
    "${gkr_basics_inc_path}/diag/diagnostics.h"
    "${gkr_basics_inc_path}/concurency/events_waiting.h"
    "${gkr_basics_inc_path}/concurency/waitable_lockfree_queue.h"
    "${gkr_basics_inc_path}/container/lockfree_grow_only_bag.h"
    "${gkr_basics_inc_path}/container/lockfree_queue.h"
    "${gkr_basics_inc_path}/misc/union_cast.h"
    "${gkr_basics_inc_path}/misc/stack_args_order.h"
    "${gkr_basics_inc_path}/cpp/lib_exchange_function.h"
    "${gkr_basics_inc_path}/cpp/lib_raw_memory_algorithms.h"
    "${gkr_basics_inc_path}/sys/stack_alloc.h"
    "${gkr_basics_inc_path}/log/consumer.h"
    "${gkr_basics_inc_path}/log/message.h"
    "${gkr_basics_inc_path}/log/stamp.h"
    "${gkr_basics_inc_path}/testing/allocator.h"
    )
# Headers + Implementations
target_sources(
    gkr-basics
    PRIVATE
    "${gkr_basics_inc_path}/concurency/worker_thread.h"
    "${gkr_basics_src_path}/concurency/worker_thread.cpp"
    "${gkr_basics_inc_path}/sys/thread_name.h"
    "${gkr_basics_src_path}/sys/thread_name.cpp"
    "${gkr_basics_src_path}/sys/process_name.cpp"
    "${gkr_basics_inc_path}/log/logger.h"
    "${gkr_basics_src_path}/log/logger.cpp"
    "${gkr_basics_inc_path}/log/logging.h"
    "${gkr_basics_src_path}/log/logging.cpp"
    )

target_include_directories(gkr-basics PUBLIC "include")

if(GKR_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

if(GKR_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
endif()
