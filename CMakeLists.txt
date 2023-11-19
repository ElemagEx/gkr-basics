cmake_minimum_required(VERSION 3.22)

project(gkr-basics)

option(GKR_BASICS_TESTS "Build unit tests for the library" ON)

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

add_subdirectory(libs)

if(GKR_BASICS_TESTS)
enable_testing()
add_subdirectory(tests)
endif()
