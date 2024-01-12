cmake_minimum_required(VERSION 3.22)

project(gkr-basics)

option(BUILD_SHARED_LIBS    "Build using shared libraries" ON)
option(GKR_CAPI_ONLY        "Install only C API interface" OFF)
option(GKR_FEATURE_LOG      "Build and install log featyre" ON)
option(GKR_BASICS_DEV       "Specifies that this is development build" ON)
option(GKR_BASICS_TESTS     "Build unit tests for the library" ON)

option(GKR_BASICS_TESTS_PROSPONE "Prospone tests execution" ON) #Dependent option

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)
set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

if(NOT BUILD_SHARED_LIBS)
    add_compile_definitions(GKR_STATIC_LIBS)
endif()

if(WIN32 AND NOT BUILD_SHARED_LIBS)
    set(CMAKE_DEBUG_POSTFIX   "_sd")
    set(CMAKE_RELEASE_POSTFIX "_s")
else()
    set(CMAKE_DEBUG_POSTFIX   "_d")
    set(CMAKE_RELEASE_POSTFIX "")
endif()

if(CMAKE_VERSION VERSION_LESS "3.23")
    set(GKR_C_INTERFACE      INTERFACE)
    set(GKR_CPP_INTERFACE    INTERFACE)
    set(GKR_FILE_SET_HEADERS )
elseif(GKR_CAPI_ONLY)
    set(GKR_C_INTERFACE      INTERFACE FILE_SET HEADERS FILES)
    set(GKR_CPP_INTERFACE    INTERFACE)
    set(GKR_FILE_SET_HEADERS FILE_SET HEADERS)
else()
    set(GKR_C_INTERFACE      INTERFACE FILE_SET HEADERS FILES)
    set(GKR_CPP_INTERFACE    INTERFACE FILE_SET HEADERS FILES)
    set(GKR_FILE_SET_HEADERS FILE_SET HEADERS)
endif()

include(warnings)
set(CMAKE_CXX_FLAGS         ${GKR_BASIC_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG   ${GKR_BASIC_CXX_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${GKR_BASIC_CXX_FLAGS_RELEASE})
add_compile_options(${GKR_BASICS_FLAGS_WARNINGS})

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

add_library(gkr::headersOnly ALIAS gkr_headers_only)
add_library(gkr::core        ALIAS gkr_core)
add_library(gkr::data        ALIAS gkr_data)
add_library(gkr::comm        ALIAS gkr_comm)
if(GKR_FEATURE_LOG)
add_library(gkr::log         ALIAS gkr_log)
endif()

install(TARGETS gkr_headers_only ${GKR_FILE_SET_HEADERS})
install(TARGETS gkr_core         ${GKR_FILE_SET_HEADERS})
install(TARGETS gkr_data         ${GKR_FILE_SET_HEADERS})
install(TARGETS gkr_comm         ${GKR_FILE_SET_HEADERS})
if(GKR_FEATURE_LOG)
install(TARGETS gkr_log          ${GKR_FILE_SET_HEADERS})
endif()

if(CMAKE_VERSION VERSION_LESS "3.23")
if(GKR_CAPI_ONLY)
    set(GKR_HEADERS_PATTERN FILES_MATCHING PATTERN "*.h")
else()
    set(GKR_HEADERS_PATTERN FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
endif()
install(DIRECTORY "libs/gkr_core/gkr" TYPE INCLUDE ${GKR_HEADERS_PATTERN})
install(DIRECTORY "libs/gkr_data/gkr" TYPE INCLUDE ${GKR_HEADERS_PATTERN})
install(DIRECTORY "libs/gkr_comm/gkr" TYPE INCLUDE ${GKR_HEADERS_PATTERN})
if(GKR_FEATURE_LOG)
install(DIRECTORY "libs/gkr_log/gkr"  TYPE INCLUDE ${GKR_HEADERS_PATTERN})
endif()
endif()

#include(GNUInstallDirs)
#include(GenerateExportHeader)
#include(CMakePackageConfigHelpers)
