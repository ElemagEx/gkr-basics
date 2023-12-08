cmake_minimum_required(VERSION 3.22)

project(gkr-basics)

option(GKR_BASICS_DEV "Specifies that this is development build" ON)
option(GKR_BASICS_TESTS "Build unit tests for the library" ON)

option(GKR_BASICS_TESTS_PROSPONE "Prospone tests execution" ON) #Dependent option

include(cmake/warnings.cmake) #sets variable ${GKR_BASICS_FLAGS_WARNINGS} and ${GKR_BASICS_TESTING_WARNINGS}

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
endif()
if(MSVC)
    set(CMAKE_CXX_FLAGS "/permissive- /EHsc /GS /sdl")
endif()
if(GKR_BASICS_DEV)
    add_compile_definitions(DIAG_MODE=4)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GKR_BASICS_FLAGS_WARNINGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(GKR_BASICS_TESTS)
enable_testing()
find_package(Catch2 3 REQUIRED)
if(${GKR_BASICS_TESTS_PROSPONE})
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
endif()
include(Catch)
endif()

add_subdirectory(libs)

if(GKR_BASICS_TESTS)
add_subdirectory(tests)
endif()
