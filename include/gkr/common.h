#pragma once

[[noreturn]]
inline void HALT() { *(int*)0 = 0; }
inline void STOP() { *(int*)0 = 0; }

#define Assert_Check(check) if(!(check)) HALT()
#define Assert_Fail()                    HALT()
#define Assert_NotImplemented()          HALT()

#define Check_ValidState(check, ...) if(!(check) && (STOP(), true)) return __VA_ARGS__
#define Check_Failure(          ...)                 STOP();        return __VA_ARGS__
#define Check_ValidArg(  check, ...) if(!(check) && (STOP(), true)) return __VA_ARGS__
#define Check_Verify(    check, ...) if(!(check) && (STOP(), true)) return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) for(auto ndx = 0; ndx < decltype(ndx)(cnt); ++ndx) if(!(check) && (STOP(), true)) return __VA_ARGS__
