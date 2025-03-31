#include <gkr/defs.hpp>
#include <gkr/sys/thread.hpp>

#include <gkr/diagnostics.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gkr
{
namespace sys
{
bool set_current_thread_name(const char name[MAX_THREAD_NAME_CCH])
{
    Check_Arg_NotNull(name, false);

    wchar_t buff[MAX_THREAD_NAME_CCH];

    DIAG_VAR(int, cch)
    MultiByteToWideChar(CP_UTF8, 0, name, -1, buff, MAX_THREAD_NAME_CCH);
    Check_Sys_Inspect(cch > 0, false);

    buff[MAX_THREAD_NAME_CCH - 1] = 0;

    DIAG_VAR(HRESULT, hr)
    SetThreadDescription(GetCurrentThread(), buff);
    Check_Sys_Error(hr, false);

    return true;
}
bool get_current_thread_name(char name[MAX_THREAD_NAME_CCH])
{
    Check_Arg_NotNull(name, false);

    wchar_t* buff;

    DIAG_VAR(HRESULT, hr)
    GetThreadDescription(GetCurrentThread(), &buff);
    Check_Sys_Error(hr, false);

    DIAG_VAR(int, cch)
    WideCharToMultiByte(CP_UTF8, 0, buff, -1, name, MAX_THREAD_NAME_CCH, nullptr, nullptr);
    Check_Sys_Inspect(cch > 0, false);

    name[MAX_THREAD_NAME_CCH - 1] = 0;

    LocalFree(buff);

    return true;
}
}
}

#else

#include <sys/prctl.h>

namespace gkr
{
namespace sys
{
bool set_current_thread_name(const char name[MAX_THREAD_NAME_CCH])
{
    Check_Arg_NotNull(name, false);

    DIAG_VAR(int, res)
    prctl(PR_SET_NAME, long(name), 0, 0, 0);
    Check_Sys_Inspect(res >= 0, false);

    return true;
}
bool get_current_thread_name(char name[MAX_THREAD_NAME_CCH])
{
    Check_Arg_NotNull(name, false);

    DIAG_VAR(int, res)
    prctl(PR_GET_NAME, long(name), 0, 0, 0);
    Check_Sys_Inspect(res >= 0, false);

    return true;
}
}
}

#endif
