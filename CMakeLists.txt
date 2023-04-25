cmake_minimum_required(VERSION 3.14)

project(gkr-basics)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_WARNINGS}")

#option(GKR_INTRUSIVE_DIAGNOSITCS "Intrusive Diagnostics in DEBUG builds" ON)
#if(GKR_INTRUSIVE_DIAGNOSITCS)
#    if(MSVC)
#        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDIAG_MODE=4")
#    else()
#        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDIAG_MODE=4")
#    endif()
#endif()

#message(STATUS "CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}")
#message(STATUS "CMAKE_CXX_FLAGS_DEBUG   = ${CMAKE_CXX_FLAGS_DEBUG}")
#message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")

set(gkr_basics_include "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(gkr_basics_inc_path "${CMAKE_CURRENT_SOURCE_DIR}/include/gkr")
set(gkr_basics_src_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(
    gkr-basics
    STATIC
    "${gkr_basics_src_path}/libmain.cpp"
)
# Headers only
target_sources(
    gkr-basics
    PRIVATE
    "${gkr_basics_inc_path}/diag/diagnostics.h"
    "${gkr_basics_inc_path}/lockfree_grow_only_bag.h"
    "${gkr_basics_inc_path}/lockfree_queue.h"
    "${gkr_basics_inc_path}/waitable_event.h"
    "${gkr_basics_inc_path}/waitable_mutex.h"
    "${gkr_basics_inc_path}/waitable_semaphore.h"
    "${gkr_basics_inc_path}/waitable_lockfree_queue.h"
    "${gkr_basics_inc_path}/misc/union_cast.h"
    "${gkr_basics_inc_path}/misc/stack_args_order.h"
    "${gkr_basics_inc_path}/cpp/lib_exchange_function.h"
    "${gkr_basics_inc_path}/cpp/lib_raw_memory_algorithms.h"
    "${gkr_basics_inc_path}/sys/stack_alloc.h"
    "${gkr_basics_inc_path}/sys/thread_name.h"
    "${gkr_basics_inc_path}/log/consumer.h"
    "${gkr_basics_inc_path}/log/message.h"
    "${gkr_basics_inc_path}/log/stamp.h"
    )
# Headers + Implementations
target_sources(
    gkr-basics
    PRIVATE
    "${gkr_basics_inc_path}/thread_worker.h"
    "${gkr_basics_src_path}/thread_worker.cpp"
    "${gkr_basics_inc_path}/sys/thread_name.h"
    "${gkr_basics_src_path}/sys/thread_name.cpp"
    "${gkr_basics_inc_path}/log/logger.h"
#   "${gkr_basics_src_path}/log/logger.cpp"
    "${gkr_basics_inc_path}/log/logging.h"
#   "${gkr_basics_src_path}/log/logging.cpp"
    )

target_include_directories(gkr-basics PUBLIC ${gkr_basics_include})

include(CTest)
#if(BUILD_TESTING)
find_package(Catch2 3 REQUIRED)
include(Catch)
add_subdirectory(tests)
#endif()
