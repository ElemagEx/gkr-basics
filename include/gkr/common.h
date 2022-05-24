#pragma once

[[noreturn]]
inline void HALT() { *(int*)0 = 0; }
inline bool STOP() { HALT(); }

#define Assert_Check(check) if(!(check)) HALT()
#define Assert_Fail()                    HALT()

#define Check_ValidState(check, ...) if(!(check) && STOP()) return __VA_ARGS__
#define Check_Failure(          ...)                STOP(); return __VA_ARGS__
#define Check_ValidArg(  check, ...) if(!(check) && STOP()) return __VA_ARGS__
#define Check_Verify(    check, ...) if(!(check) && STOP()) return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) for(auto ndx = 0; ndx < decltype(ndx)(cnt); ++ndx) if(!(check) && STOP()) return __VA_ARGS__
