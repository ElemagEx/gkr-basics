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
