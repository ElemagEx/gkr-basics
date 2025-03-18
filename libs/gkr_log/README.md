# gkr::logging library

## Generic logging

### Facilities
Facility are user-defined constants that are passed to log functions and later log messages can be filtered by facility value (*aka tags*)

### Macro/Inline log args
| | :x: Channel | :white_check_mark: Channel
| :--- | :--- | :--- 
| :x: Facility | `bLOGxxx` - basic log | `cLOGxxx` - channel log
| :white_check_mark: Facility | `fLOGxxx` - facility log | `gLOGxxx` - genenral log

### Macro/Inline name structure
Structure: `[prefix]LOG[severity][_IF][_]`
- `prefix` - can be one of `b`, `f`, `c`, or `g` (see table above) - if it is missing then alias is used (see below)
- `severity`- long or short ref of severity (for compile-time eval) or missing (for run-time eval)
- `_IF` - if it is used then the first argument of macro/inline is a run-time evaluated condition which if it is false then the log message is skipped
- `_` - if last character of macro/inline is underscope then it must be used for printf-like message otherwise it is used for all other types of messages

### Severity (*aka logging level*)
| Name    | Define               | Long ref   | Short ref | Value | Description
| :---    | :---                 | :---       | :---:     | :---: | :---
| Fatal   | LOG_SEVERITY_FATAL   | `_FATAL`   | `F`       | 0     | A fatal error occured and the program must be terminated
| Error   | LOG_SEVERITY_ERROR   | `_ERROR`   | `E`       | 1     | A severe error occured and the program can be recovered
| Warning | LOG_SEVERITY_WARNING | `_WARNING` | `W`       | 2     | A warning occured and the program will continue normally
| Info    | LOG_SEVERITY_INFO    | `_INFO`    | `I`       | 3     | Program general information
| Verbose | LOG_SEVERITY_VERBOSE | `_VERBOSE` | `V`       | 4     | Program verbose information
| Debug   | LOG_SEVERITY_DEBUG   | `_DEBUG`   | `D`       | 5     | Program debug information
| Trace   | LOG_SEVERITY_TRACE   | `_TRACE`   | `T`       | 6     | Program trace information

### Generic defines
- `LOG_FACILITY` - basic and channel log macros/inlines use this value for its facility argument - default is 0
- `LOG_CHANNEL` - basic and facility log macros/inlines use this value for its channel argument - default is (`NULL`/`nullptr`)
- `LOG_THRESHOLD_LEVEL=<integer>` - define compile-time log threshold - all macros/inlines that are grater or equal to this value are compiled out - default is 100
- `LOG_SOURCE_LOCATION` - add source location of macros - available only with `GENERIC_LOG_USE_C_DEFS` - default is undefined

- `GENERIC_LOG_USE_C_DEFS` - use C style logging macros instead C++ inline overloaded functions
- `GENERIC_LOG_USE_CPP_LEGACY` - use old C++ (pre C++ 17) techniques for compile-time severity filtering (for C++ 17 and later `if constexpr` is used)
- `GENERIC_LOG_SKIP_PRINTF` - disables logging with `printf` style
- `GENERIC_LOG_SKIP_STREAM` - disables logging with C++ output streams
- `GENERIC_LOG_SKIP_FORMAT` - disables logging with C++20 format

## Uniques
- recursive logging - when logging is used inside logging thread from consumers

## Format output replace codes

| Replace Code | Description
| :---     | :---
| `<STMP>` | raw stamp
| `<MSEC>` | stamp milliseconds
| `<USEC>` | stamp microseconds
| `<NSEC>` | stamp nanoseconds
| `<T_ID>` | thread ID
| `<S_ID>` | severity value
| `<F_ID>` | facility value
| `<TNAM>` | thread name
| `<CNAM>` | channel name
| `<SNAM>` | severity name
| `<FNAM>` | facility name
| `<FUNC>` | source function
| `<FILE>` | source file
| `<LINE>` | source line
| `<TEXT>` | message text

## Format output control codes

| Control Code | Description
| :---     | :---
| `<ISnn>` | insert text argument located in row indexed by `nn` and column indexed by `severity`
| `<IFnn>` | insert text argument located in row indexed by `nn` and column indexed by `facility`
| `<Pcnn>` | specifies left padding with `nn` width with `c` character for next replacement
| `<Rcnn>` | specifies right padding with `nn` width with `c` character for next replacement
| `<Cnnn>` | changes console text color/style

## Console control codes used with `<Cnnn>`

`nnn` | Description
| :--- | :---
| 000 | reset console colors/style to default
| | **Text style**
| 001 | normal
| 002 | bold
| 003 | italic
| | **Text foreground color**
| 030 | black
| 034 | dark blue
| 032 | dark green
| 036 | light blue
| 031 | dark red
| 035 | magenta
| 033 | orange
| 037 | light gray
| 090 | gray
| 094 | blue
| 092 | green
| 096 | cyan
| 091 | red
| 095 | pink
| 093 | yellow
| 097 | white
| | **Text background color**
| 040 | black
| 044 | dark blue
| 042 | dark green
| 046 | light blue
| 041 | dark red
| 045 | magenta
| 043 | orange
| 047 | light gray
| 100 | gray
| 104 | blue
| 102 | green
| 106 | cyan
| 101 | red
| 105 | pink
| 103 | yellow
| 107 | white

