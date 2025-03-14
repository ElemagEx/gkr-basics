if(GKR_BASICS_TESTS)
add_library(gkr_testing INTERFACE)

target_include_directories(gkr_testing INTERFACE ".")

target_sources(
    gkr_testing
    INTERFACE
    "gkr/testing/log_defs.hpp"
    "gkr/testing/allocator.hpp"
    "gkr/testing/diag_names.hpp"
    "gkr/testing/diag_undefs.hpp"
)

#add_executable        (t-tester_c "tests/main.c")
#target_link_libraries (t-tester_c PRIVATE gkr_core)
#
#add_executable        (t-tester_cpp "tests/main.cpp")
#target_link_libraries (t-tester_cpp PRIVATE gkr_core)
endif()
