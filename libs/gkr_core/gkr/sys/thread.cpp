#include "thread.hpp"

#include <gkr/diagnostics.h>

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

	MultiByteToWideChar(CP_UTF8, 0, name, -1, buff, MAX_THREAD_NAME_CCH);

	buff[MAX_THREAD_NAME_CCH - 1] = 0;

	HRESULT hr = SetThreadDescription(GetCurrentThread(), buff);

	return SUCCEEDED(hr);
}
bool get_current_thread_name(char name[MAX_THREAD_NAME_CCH])
{
	Check_Arg_NotNull(name, false);

	wchar_t* buff;

	HRESULT hr = GetThreadDescription(GetCurrentThread(), &buff);

	if(!SUCCEEDED(hr)) return false;

	WideCharToMultiByte(CP_UTF8, 0, buff, -1, name, MAX_THREAD_NAME_CCH, nullptr, nullptr);

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
	if(name == nullptr) return false;

	return (0 == prctl(PR_SET_NAME, long(name), 0, 0, 0));
}
bool get_current_thread_name(char name[MAX_THREAD_NAME_CCH])
{
	if(name == nullptr) return false;

	return (0 == prctl(PR_GET_NAME, long(name), 0, 0, 0));
}
}
}

#endif
