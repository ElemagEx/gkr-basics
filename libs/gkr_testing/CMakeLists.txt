add_library(gkr_testing INTERFACE)

target_include_directories(gkr_testing INTERFACE ".")

target_sources(
    gkr_testing
    INTERFACE
    "gkr/testing/allocator.h"
    "gkr/testing/diag_names.h"
    "gkr/testing/diag_undefs.h"
    )
