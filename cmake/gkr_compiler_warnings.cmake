if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(GKR_BASICS_EXCLUDE_CLANG_WARNINGS
        -Wno-c++98-compat-pedantic
        -Wno-c++98-compat
        -Wno-format-nonliteral
        -Wno-global-constructors
        -Wno-exit-time-destructors
        -Wno-c++17-extensions
        -Wno-unknown-attributes
        -Wno-switch-default
    )
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
        list(APPEND GKR_BASICS_EXCLUDE_CLANG_WARNINGS -Wno-unsafe-buffer-usage)
    endif()
endif()

if(MSVC)
    set(GKR_BASICS_FLAGS_WARNINGS
        /Wall
        /wd4100 # 'identifier' : unreferenced formal parameter
        /wd4514 # 'function' : unreferenced inline function has been removed
        /wd4710 # 'function' : function not inlined
        /wd4711 # function 'function' selected for inline expansion
        /wd4820 # 'bytes' bytes padding added after construct 'member_name'
        /wd5039 # 'function': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.
        /wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5051 # attribute 'attribute-name' requires at least 'standard-level'; ignored
        /D_CRT_SECURE_NO_WARNINGS # TODO:Remove
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        list(APPEND GKR_BASICS_FLAGS_WARNINGS ${GKR_BASICS_EXCLUDE_CLANG_WARNINGS})
    endif()

    set(GKR_BASICS_TESTING_WARNINGS
        /wd4388 # '==': signed/unsigned mismatch
        /wd4623 # default constructor was implicitly defined as deleted
        /wd4625 # copy constructor was implicitly defined as deleted
        /wd4626 # assignment operator was implicitly defined as deleted
        /wd4868 # compiler may not enforce left-to-right evaluation order in braced initializer list
        /wd5026 # move constructor was implicitly defined as deleted
        /wd5027 # move assignment operator was implicitly defined as deleted
    )
else()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(GKR_BASICS_FLAGS_WARNINGS
            -Weverything
            -Wno-unused-parameter
            -Wno-padded
            -Wno-alloca
            -Wno-weak-vtables
            ${GKR_BASICS_EXCLUDE_CLANG_WARNINGS}
        )
    else()
        set(GKR_BASICS_FLAGS_WARNINGS
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wunused
            -Wno-unused-parameter
        )
    endif()
endif()

unset(GKR_BASICS_EXCLUDE_CLANG_WARNINGS)
