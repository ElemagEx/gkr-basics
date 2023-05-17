if(BUILD_TESTING)
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         3f0283de7a9c43200033da996ff9093be3ac84dc # v.3.3.2
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(Catch2)
endif()
