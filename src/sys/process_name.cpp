#include <gkr/sys/process_name.h>

#include <gkr/diag/diagnostics.h>

#include <cstring>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

namespace gkr
{
namespace sys
{
int get_current_process_id()
{
	return (int)GetCurrentProcessId();
}
int get_current_process_name(char* name, unsigned cch)
{
	char file[MAX_PATH];

	const unsigned len = GetModuleBaseNameA(GetCurrentProcess(), nullptr, file, MAX_PATH);

	Check_ValidState((len > 0) && (len < MAX_PATH), 0);

	if((name != nullptr) && (len < cch))
	{
		std::strncpy(name, file, cch);
		return len;
	}
	else
	{
		return (len + 1);
	}
}
}
}

#else

#include <unistd.h>

extern char *__progname;

namespace gkr
{
namespace sys
{
int get_current_process_id()
{
	return int(getpid());
}
int get_current_process_name(char* name, unsigned cch)
{
	Check_ValidState(__progname != nullptr, 0);

	const std::size_t len = std::strlen(__progname);

	Check_ValidState(len > 0, 0);

	if((name != nullptr) && (len < cch))
	{
		std::strncpy(name, __progname, cch);
		return int(len);
	}
	else
	{
		return int(len + 1);
	}
}
}
}

#endif
