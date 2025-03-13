# gkr::headers_only and gkr::core

## GKR Diagnostics mode
- `DIAG_MODE_DISABLED` - disables all diagnostics
- `DIAG_MODE_SILENT` - asserts are disabled and checks are silient - default for Release builds
- `DIAG_MODE_STEADY` - asserts are disabled and checks warns on fail
- `DIAG_MODE_NOISY` - asserts halts execution and checks warns on fail - default for Debug builds
- `DIAG_MODE_INTRUSIVE` - all diagnostics halts execution

## GKR Diagnostics External APIs
If you want to provide your own version of `gkr_warn` and/or `gkr_halt` apis

### GKR Diagnostics Defines
- `DIAG_ID_xxx` - id of diagnostic

### GKD Diagnostics source information for external APIs
- `DIAG_SRC_INFO_NONE` - no source information is provided (C/C++)
- `DIAG_SRC_INFO_PREPROCESSOR` - source information from preprocessor - file and line (C/C++ only)
- `DIAG_SRC_INFO_PREPROCESSOR_EX` - source information from preprocessor - function, file, and line (C/C++ only)
- `DIAG_SRC_INFO_SOURCE_LOCATION` - `std::source_location` information for source location - C++ 20 and later
- `DIAG_SRC_INFO_STACKTRACE` - `std::stacktrace` information for source location - C++ 23 and later
