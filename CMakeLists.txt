cmake_minimum_required(VERSION 3.14)

project(gkr-basics)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

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

message(STATUS "CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG   = ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")

option(GKR_BASIC_DEVELOPMENT "gkr-basics: Current Development" ON)

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
    "${gkr_basics_inc_path}/lockfree_forward_list.h"
    "${gkr_basics_inc_path}/lockfree_queue.h"
    "${gkr_basics_inc_path}/objects_waiter.h"
    "${gkr_basics_inc_path}/waitable_object.h"
    "${gkr_basics_inc_path}/waitable_event.h"
    "${gkr_basics_inc_path}/waitable_mutex.h"
    "${gkr_basics_inc_path}/waitable_semaphore.h"
    "${gkr_basics_inc_path}/cpp_feature/lib_exchange_function.h"
    "${gkr_basics_inc_path}/cpp_feature/lib_raw_memory_algorithms.h"
    "${gkr_basics_inc_path}/misc/stamp.h"
    "${gkr_basics_inc_path}/misc/union_cast.h"
    "${gkr_basics_inc_path}/sys/stack_alloc.h"
    "${gkr_basics_inc_path}/sys/thread_name.h"
    "${gkr_basics_inc_path}/log/consumer.h"
    "${gkr_basics_inc_path}/log/entry.h"
    )
# Headers + Implementations
target_sources(
    gkr-basics
    PRIVATE
    "${gkr_basics_inc_path}/basic_thread_worker.h"
    "${gkr_basics_src_path}/basic_thread_worker.cpp"
    "${gkr_basics_inc_path}/sys/thread_name.h"
    "${gkr_basics_src_path}/sys/thread_name.cpp"
    "${gkr_basics_inc_path}/log/logger.h"
    "${gkr_basics_src_path}/log/logger.cpp"
    "${gkr_basics_inc_path}/log/logging.h"
    "${gkr_basics_src_path}/log/logging.cpp"
    )

target_include_directories(gkr-basics PUBLIC ${gkr_basics_include})

if(GKR_BASIC_DEVELOPMENT)
    set(gkr_main_src_path "${CMAKE_CURRENT_SOURCE_DIR}/msvs")

    add_executable(
        main
        "${gkr_main_src_path}/main.cpp"
    )
    target_sources(
        main
        PRIVATE
        "${gkr_main_src_path}/test_lockfree_queue.cpp"
        "${gkr_main_src_path}/test_thread_worker.cpp"
        "${gkr_main_src_path}/test_waiters.cpp"
        )
    target_link_libraries(main gkr-basics)
endif()
