#include <gkr/defs.hpp>
#include <gkr/sys/process.hpp>

#include <gkr/diagnostics.hpp>

#include <memory>
#include <cstring>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

namespace gkr
{
namespace sys
{
unsigned get_current_process_id()
{
    return GetCurrentProcessId();
}
std::string get_current_process_name()
{
    char name[FILENAME_MAX];

    unsigned len = GetModuleBaseNameA(GetCurrentProcess(), nullptr, name, FILENAME_MAX);

    Check_ValidState((len > 0) && (len < FILENAME_MAX), {});

    char* ext = std::strrchr(name, '.');

    if((ext != nullptr) && (0 == std::strcmp(ext, ".exe")))
    {
        *ext = 0;
        len -= 4;
    }
    return std::string(name, len);
}
std::string get_current_process_path()
{
    for(unsigned cch = 512; ; cch *= 2)
    {
        std::unique_ptr<char[]> buf(new char[cch]);

        unsigned len = GetModuleFileNameA(nullptr, buf.get(), cch);

        if((len > 0) && (len < cch))
        {
            char* ext = std::strrchr(buf.get(), '.');

            if((ext != nullptr) && (0 == std::strcmp(ext, ".exe")))
            {
                *ext = 0;
                len -= 4;
            }
            return std::string(buf.get(), len);
        }
    }
}

std::string expand_process_env_vars(const char* str, bool nativeFormat)
{
    Assert_Check(str != nullptr);

    std::string res;

    if(nativeFormat)
    {
        for( ; ; )
        {
            const char* pos = std::strchr(str, '%');

            if(pos == nullptr)
            {
                res += str;
                break;
            }

            const char* end = std::strchr(++str, '%');

            if(end == nullptr)
            {
                break;
            }
            if(end == str)
            {
                res += '%';
                ++str;
                continue;
            }
            std::string var(str, std::size_t(end - str));

            const char* val = std::getenv(var.c_str());

            if(val != nullptr) res += val;
            str += end - str + 1;
        }
    }
    else
    {
        Assert_FailureMsg("Not implemented");
    }
    return res;
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
unsigned get_current_process_id()
{
    return unsigned(getpid());
}
std::string get_current_process_name()
{
    return std::string(__progname);
}
std::string get_current_process_path()
{
    for(std::size_t cch = 512; ; cch *= 2)
    { 
        std::unique_ptr<char[]> buf(new char[cch]);

        const ssize_t len = readlink("/proc/self/exe", buf.get(), cch - 1);

        if(len != -1)
        {
            return std::string(buf.get(), std::size_t(len));
        }
    }
}

std::string expand_process_env_vars(const char* str, bool nativeFormat)
{
    Assert_Check(str != nullptr);
    return "";
}

}
}

#endif
