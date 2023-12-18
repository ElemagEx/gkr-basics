# gkr::logging

## GKR Defines
- `GKR_NO_PRINTF_LOGGING` - disables logging with `printf` style
- `GKR_NO_STREAM_LOGGING` - disables logging with C++ output streams
- `GKR_NO_FORMAT_LOGGING` - disables logging with C++20 format

## Generic Definitions Defines
- `LOG_INSTANCE` - log instance all LOG_xxx macros/inlines use to log (C/C++) - default is (`NULL`/`nullptr`)
- `LOG_THRESHOLD_LEVEL=<integer>` - define compile-time logging threshold - only severity levels below this threshold will be logged (C++ only)
- `LOG_USE_C_DEFS` - use C style logging macros instead C++ inline overloaded functions (C++ only)
- `LOG_NS` - namespace where all inline helper functions will be placed (C++ only)

## Uniques
- recursive logging - when logging is used inside logging thread from consumers

## Format output replace codes

| Replace Code | Description
| :--- | :---
| `<STMP>` | raw stamp
| `<S_MS>` | stamp milliseconds
| `<S_US>` | stamp microseconds
| `<S_NS>` | stamp nanoseconds
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

