# gkr::logging

## Defines
- `GKR_LOG_THRESHOLD_LEVEL=<integer>` - define compile-time logging threshold - only severity levels below this threshold will be logged (C++ only)
- `GKR_LOG_USE_C_DEFS` - use C style logging macros instead C++ inline overloaded functions (C++ only)
- `GKR_NO_PRINTF_LOGGING` - disables logging with `printf` style
- `GKR_NO_STREAM_LOGGING` - disables logging with C++ output streams
- `GKR_NO_FORMAT_LOGGING` - disables logging with C++20 format

## Uniques
- recursive logging - when logging is used inside logging thread from consumers
