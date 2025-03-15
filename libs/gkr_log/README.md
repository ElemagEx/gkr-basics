# gkr::logging library

## Logging filter defines
- `GKR_NO_PRINTF_LOGGING` - disables logging with `printf` style
- `GKR_NO_STREAM_LOGGING` - disables logging with C++ output streams
- `GKR_NO_FORMAT_LOGGING` - disables logging with C++20 format

## Generic logging

### Facilities
Facility are user-defined constants that are passed to log functions and later log messages can be filtered by facility value (*aka tags*)

### Facility aware logging
All generic logging macros/inlines have two NOT compatible definitions - with and without facility argument. Usually smaller projects does not use facilities/tags.
If you define `LOG_FACILITY=<facility>` then all logging macros/inlines will not have facility argument.

### Classes
| | :x: Instance | :white_check_mark: Instance
| :--- | :--- | :--- 
| :x: Facility | `bLOGxxx` | `mLOGxxx`
| :white_check_mark: Facility | `fLOGxxx` | `gLOGxxx`

### Severity (*aka logging level*)
| Severity Name | Define               | Letter | Value | Description
| :---          | :---                 | :---:  | :---: | :---
| FATAL         | LOG_SEVERITY_FATAL   | `F`    | 0     | A fatal error occured and the program must be terminated
| ERROR         | LOG_SEVERITY_ERROR   | `E`    | 1     | A severe error occured and the program can be recovered
| WARNING       | LOG_SEVERITY_WARNING | `W`    | 2     | A warning occured and the program will continue normally
| INFO          | LOG_SEVERITY_INFO    | `I`    | 3     | Program general information
| VERBOSE       | LOG_SEVERITY_VERBOSE | `V`    | 4     | Program verbose information
| DEBUG         | LOG_SEVERITY_DEBUG   | `D`    | 5     | Program debug information
| TRACE         | LOG_SEVERITY_TRACE   | `T`    | 6     | Program trace information

### Generic defines
- `LOG_VARIANT`  - default 0
- `LOG_FACILITY` - all log macros/inlines do not have facility argument and their facility is the value of the define - not defined by default
- `LOG_INSTANCE` - log instance of all macros/inlines not starting with `C` macros/inlines use to log - default is (`NULL`/`nullptr`)
- `LOG_THRESHOLD_LEVEL=<integer>` - define compile-time logging threshold for all log macros/inlines - only severity levels below this threshold will be logged - default is 100
- `LOG_USE_C_DEFS` - use C style logging macros instead C++ inline overloaded functions

### Log macros/inlines groups

#### Macros/inlines with specific logging instance

| Feature | Description
| :---    | :---
| starting with `C`| all such macros/inlines have an argument `instance` and all other macros/inlines uses `LOG_INSTANCE` as instance
| containing `_IF` | all such macros/inlines have first argument run-time condition - if it is true only then the logging is performed
- `LOGx`, `LOG_xxx` - log with compile-time severity
- `LOG_IF` - conditional log with run-time severity
- `LOGx_IF`, `LOG_xxx_IF` - log with compile-time severity
- `CLOG` - log with run-time severity and instance
- `CLOGx`, `CLOG_xxx` - log with compile-time severity and instance
- `CLOG_IF` - conditional log with run-time severity and instance
- `CLOGx_IF`, `CLOG_xxx_IF` - conditional log with compile-time severity and instance

:information_source:***NOTE*** `x` is severity letter and `xxx` is severity name from the table above

## Uniques
- recursive logging - when logging is used inside logging thread from consumers

## Format output replace codes

| Replace Code | Description
| :--- | :---
| `<STMP>` | raw stamp
| `<MSEC>` | stamp milliseconds
| `<USEC>` | stamp microseconds
| `<NSEC>` | stamp nanoseconds
| `<T_ID>` | thread ID
| `<S_ID>` | severity value
| `<F_ID>` | facility value
| `<TNAM>` | thread name
| `<MNAM>` | module name
| `<SNAM>` | severity name
| `<FNAM>` | facility name
| `<FUNC>` | source function
| `<FILE>` | source file
| `<LINE>` | source line
| `<TEXT>` | message text

## Format output control codes

| Control Code | Description
| :--- | :---
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

