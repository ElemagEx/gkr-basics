if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(GKR_BASICS_EXCLUDE_CLANG_WARNINGS "-Wno-unsafe-buffer-usage -Wno-c++98-compat-pedantic -Wno-c++98-compat -Wno-format-nonliteral -Wno-global-constructors -Wno-exit-time-destructors -Wno-c++17-extensions -Wno-unknown-attributes")
endif()

if(MSVC)
    # C4100 - 'identifier' : unreferenced formal parameter
    # C4514 - 'function' : unreferenced inline function has been removed
    # C4820 - 'bytes' bytes padding added after construct 'member_name'
    # C5039 - 'function': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
    # C5045 - Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    # C5051 - attribute 'attribute-name' requires at least 'standard-level'; ignored
    # REMOVE ASAP
    # _CRT_SECURE_NO_WARNINGS
    set(GKR_BASICS_FLAGS_WARNINGS "/Wall /wd4100 /wd4514 /wd4710 /wd4711 /wd4820 /wd5039 /wd5045 /wd5051 /D_CRT_SECURE_NO_WARNINGS")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(GKR_BASICS_FLAGS_WARNINGS "${GKR_BASICS_FLAGS_WARNINGS} ${GKR_BASICS_EXCLUDE_CLANG_WARNINGS}")
    endif()

    if(GKR_BASICS_TESTS)
        # C4388 - '==': signed/unsigned mismatch
        # C4623 - default constructor was implicitly defined as deleted
        # C4625 - copy constructor was implicitly defined as deleted
        # C4626 - assignment operator was implicitly defined as deleted
        # C5026 - move constructor was implicitly defined as deleted
        # C5027 - move assignment operator was implicitly defined as deleted
        set(GKR_BASICS_TESTING_WARNINGS /wd4388 /wd4623 /wd4625 /wd4626 /wd5026 /wd5027)
    endif()
else()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(GKR_BASICS_FLAGS_WARNINGS "-Weverything -Wno-unused-parameter -Wno-padded -Wno-alloca -Wno-weak-vtables ${GKR_BASICS_EXCLUDE_CLANG_WARNINGS}")
    else()
        set(GKR_BASICS_FLAGS_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wunused -Wno-unused-parameter")
    endif()
endif()
