#include "process.hpp"

#include <gkr/diagnostics.h>

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

    unsigned len = GetModuleBaseNameA(GetCurrentProcess(), nullptr, file, MAX_PATH);

    Check_ValidState((len > 0) && (len < MAX_PATH), 0);

    char* ext = std::strrchr(file, '.');

    if((ext != nullptr) && (0 == std::strcmp(ext, ".exe")))
    {
        *ext = 0;
        len -= 4;
    }
    if((name != nullptr) && (len < cch))
    {
        std::strncpy(name, file, cch);
        return int(len);
    }
    else
    {
        return int(len + 1);
    }
}
int get_current_process_path(char* path, unsigned cch)
{
    char file[MAX_PATH];

    unsigned len = GetModuleFileNameA(nullptr, file, MAX_PATH);

    Check_ValidState((len > 0) && (len < MAX_PATH), 0);

    char* ext = std::strrchr(file, '.');

    if((ext != nullptr) && (0 == std::strcmp(ext, ".exe")))
    {
        *ext = 0;
        len -= 4;
    }
    if((path != nullptr) && (len < cch))
    {
        std::strncpy(path, file, cch);
        return int(len);
    }
    else
    {
        return int(len + 1);
    }
}
}
}

#else

#include <unistd.h>
#include <linux/limits.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif
extern char *__progname;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

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
int get_current_process_path(char* name, unsigned cch)
{
    char path[PATH_MAX];

    ssize_t len = readlink("/proc/self/exe", path, PATH_MAX);

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
