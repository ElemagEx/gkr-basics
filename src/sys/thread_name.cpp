#include <gkr/sys/thread_name.h>

#include <gkr/common.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gkr
{
namespace sys
{
bool set_current_thread_name(const char name[max_thread_name_cch])
{
	Check_NotNullArg(name, false);

	wchar_t buff[max_thread_name_cch];

	MultiByteToWideChar(CP_UTF8, 0, name, -1, buff, max_thread_name_cch);

	buff[max_thread_name_cch - 1] = 0;

	HRESULT hr = SetThreadDescription(GetCurrentThread(), buff);

	return SUCCEEDED(hr);
}
}
}

#else

#include <sys/prctl.h>

namespace gkr
{
namespace sys
{
bool set_current_thread_name(const char* name)
{
	Check_ValidArg(name != nullptr, false);

	return (0 == prctl(PR_SET_NAME, (long)name, 0, 0, 0));
}
}
}

#endif
