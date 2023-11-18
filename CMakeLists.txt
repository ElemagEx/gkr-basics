cmake_minimum_required(VERSION 3.22)

project(gkr-basics)

option(GKR_BUILD_TESTS    "Build unit tests for the library" ON)
option(GKR_BUILD_EXAMPLES "Build examples" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(dependencies)
include(warnings) #includes ./cmake/warnings.cmake - sets variable ${GKR_BASICS_FLAGS_WARNINGS}

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
    add_compile_definitions(DIAG_MODE=4)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "/permissive- /EHsc /GS /sdl")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GKR_BASICS_FLAGS_WARNINGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(gkr_core_inc_path "${CMAKE_CURRENT_SOURCE_DIR}/include/gkr")
set(gkr_core_src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

#
# GKR Core Library
#
add_library(gkr_core SHARED "${gkr_core_src_path}/libmain.cpp")

target_include_directories(gkr_core PUBLIC  "include")
target_compile_definitions(gkr_core PRIVATE BUILDING_GKR_CORE)

if(MSVC)
    target_sources(gkr_core PUBLIC gkr.natvis)
endif()
if(WIN32)
    target_link_libraries(gkr_core PRIVATE ws2_32)
endif()

# Headers only
target_sources(
    gkr_core
    PRIVATE
    "${gkr_core_inc_path}/api.h"
    "${gkr_core_inc_path}/diagnostics.h"
    "${gkr_core_inc_path}/concurency/events_waiting.h"
    "${gkr_core_inc_path}/concurency/waitable_lockfree_queue.h"
    "${gkr_core_inc_path}/container/lockfree_grow_only_bag.h"
    "${gkr_core_inc_path}/container/lockfree_queue.h"
    "${gkr_core_inc_path}/misc/union_cast.h"
    "${gkr_core_inc_path}/misc/stack_args_order.h"
    "${gkr_core_inc_path}/cpp/lib_exchange_function.h"
    "${gkr_core_inc_path}/cpp/lib_raw_memory_algorithms.h"
    "${gkr_core_inc_path}/sys/stack_alloc.h"
    "${gkr_core_inc_path}/log/consumer.h"
    "${gkr_core_inc_path}/log/message.h"
    "${gkr_core_inc_path}/testing/allocator.h"
    "${gkr_core_inc_path}/testing/diag_names.h"
    "${gkr_core_inc_path}/testing/diag_undefs.h"
    )
# Headers + Implementations
target_sources(
    gkr_core
    PRIVATE
    "${gkr_core_inc_path}/stamp.h"
    "${gkr_core_src_path}/stamp.cpp"
    "${gkr_core_inc_path}/concurency/worker_thread.h"
    "${gkr_core_src_path}/concurency/worker_thread.cpp"
    "${gkr_core_inc_path}/sys/thread_name.h"
    "${gkr_core_src_path}/sys/thread_name.cpp"
    "${gkr_core_src_path}/sys/process_name.cpp"
    "${gkr_core_inc_path}/log/logger.h"
    "${gkr_core_src_path}/log/logger.cpp"
    "${gkr_core_inc_path}/log/logging.h"
    "${gkr_core_src_path}/log/logging.cpp"
    "${gkr_core_inc_path}/net/lib.h"
    "${gkr_core_src_path}/net/lib.cpp"
    "${gkr_core_inc_path}/net/address.h"
    "${gkr_core_src_path}/net/address.cpp"
    "${gkr_core_inc_path}/net/socket.h"
    "${gkr_core_src_path}/net/socket.cpp"
    )

if(GKR_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

if(GKR_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
endif()
