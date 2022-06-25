if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_EXCLUDE_CLANG_WARNINGS "-Wno-c++98-compat-pedantic -Wno-c++98-compat -Wno-format-nonliteral -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++17-extensions -Wno-unknown-attributes")
endif()

if(MSVC)
    # C4100 - 'identifier' : unreferenced formal parameter
    # C4514 - 'function' : unreferenced inline function has been removed
    # C4820 - 'bytes' bytes padding added after construct 'member_name'
    # C5039 - 'function': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
    # C5045 - Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    # C5051 - attribute 'attribute-name' requires at least 'standard-level'; ignored
    # REMOVE ASAP
    # generated from <vector> and <atomic>
    # C4365 - 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
    set(CMAKE_CXX_FLAGS_WARNINGS "/Wall /wd4100 /wd4514 /wd4710 /wd4711 /wd4820 /wd4848 /wd5039 /wd5045 /wd5051 /D_CRT_SECURE_NO_WARNINGS /wd4365")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS_WARNINGS "${CMAKE_CXX_FLAGS_WARNINGS} ${CMAKE_CXX_FLAGS_EXCLUDE_CLANG_WARNINGS}")
    endif()
else()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS_WARNINGS "-Weverything -Wno-unused-parameter -Wno-padded -Wno-alloca -Wno-weak-vtables ${CMAKE_CXX_FLAGS_EXCLUDE_CLANG_WARNINGS}")
    else()
        set(CMAKE_CXX_FLAGS_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wunused -Wno-unused-parameter")
    endif()
endif()
