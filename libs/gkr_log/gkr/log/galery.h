#pragma once

//
// Generic
//
#define GENERIC_FMT_ANDROID         "[$SNAM$][$FNAM$][$TNAM$] - $TEXT$"
#define GENERIC_FMT_MESSAGE         "$C090$%T:$R003$$S_MS$$C000$ [$IS00$$P 07$$SNAM$$C000$] <$C033$$P 07$$FNAM$$C000$> [$R 06$$T_ID$] - $TEXT$"

#define GENERIC_CONSOLE_ARGS_STRS   {"$C097$$C101$", "$C091$",  "$C093$", "$C092$", "$C096$", "$C041$", "$C042$" }
#define GENERIC_CONSOLE_ARGS_COLS   1
#define GENERIC_CONSOLE_ARGS_ROWS   7

//
// PLOG
//

#define PLOG_CSV_HEAD           "Date;Time;Severity;TID;This;Function;Message"
#define PLOG_CSV_FORMAT         "%Y/%m/%d;%T:$R003$$S_MS$;$SNAM$;$T_ID$;0;$FILE$@$LINE$;\"$TEXT%\""

#define PLOG_FUNC_MSG_FORMAT    "$FUNC$@$LINE$: $TEXT$"
#define PLOG_ONLY_MSG_FORMAT    "$TEXT$"

#define PLOG_TEXT_FORMAT        "$IS00$%F %T:$R003$$S_MS$ $P 05$$SNAM$ [$T_ID$] [$FUNC$@$LINE$] $TEXT$$IS01$"
#define PLOG_TEXT_ARGS_STRS     {"", "", "$C097$$C101$", "$C000$", "$C091$", "$C000$", "$C093$", "$C000$", "", "", "$C096$", "$C000$", "$C096$", "$C000$"}
#define PLOG_TEXT_ARGS_COLS     2
#define PLOG_TEXT_ARGS_ROWS     7
