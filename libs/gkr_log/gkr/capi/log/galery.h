#pragma once

//
// Generic
//
#define GENERIC_FMT_ANDROID         "[<SNAM>][<FNAM>][<TNAM>] - <TEXT>"
#define GENERIC_FMT_MESSAGE         "<C090>%F %T:<R003><MSEC><C000> [<IS00><P 07><SNAM><C000>] (<C033><P 10><FNAM><C000>) [<R 06><T_ID>] - <TEXT>"

#define GENERIC_CONSOLE_ARGS_STRS   {"<C097><C101>", "<C091>",  "<C093>", "<C092>", "<C096>", "<C034>", "<C035>" }
#define GENERIC_CONSOLE_ARGS_ROWS   1
#define GENERIC_CONSOLE_ARGS_COLS   7

//
// PLOG
//
#ifndef GKR_GALERY_PLOG

#define PLOG_CSV_HEAD           "Date;Time;Severity;TID;This;Function;Message"
#define PLOG_CSV_FORMAT         "<IS00>%Y/%m/%d;%T:<R003><MSEC>;<SNAM>;<T_ID>;0;<FILE>@<LINE>;\"<TEXT>\"<IS01>"

#define PLOG_FULL_MSG_FORMAT    "<IS00>%F %T:<R003><MSEC> <P 05><SNAM> [<T_ID>] [<FUNC>@<LINE>] <TEXT><IS01>"
#define PLOG_FUNC_MSG_FORMAT    "<IS00><FUNC>@<LINE>: <TEXT><IS01>"
#define PLOG_ONLY_MSG_FORMAT    "<IS00><TEXT><IS01>"

#define PLOG_CONSOLE_ARGS_STRS  {"", "<C097><C101>", "<C091>", "<C093>", "", "<C096>", "<C096>", \
                                 "", "<C000>"      , "<C000>", "<C000>", "", "<C000>", "<C000>", }
#define PLOG_CONSOLE_ARGS_ROWS  2
#define PLOG_CONSOLE_ARGS_COLS  7

#endif
