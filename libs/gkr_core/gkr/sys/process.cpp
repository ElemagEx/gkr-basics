#include <gkr/defs.hpp>
#include <gkr/sys/process.hpp>

#include <gkr/diagnostics.hpp>

#include <memory>
#include <vector>
#include <string>
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
    char name[MAX_PATH];

    unsigned len = GetModuleBaseNameA(GetCurrentProcess(), nullptr, name, MAX_PATH);

    Check_Sys_Inspect((len > 0) && (len < MAX_PATH), {});

    char* ext = std::strrchr(name, '.');

    if(ext != nullptr)
    {
        len -= unsigned(std::strlen(ext));
        *ext = 0;
    }
    return std::string(name, len);
}
std::string get_current_process_path()
{
    std::vector<char> buff;

    for(buff.resize(256); ; buff.resize(2*buff.size()))
    { 
        const unsigned len = GetModuleFileNameA(nullptr, buff.data(), DWORD(buff.size()));

        Check_Sys_Inspect((len > 0) && (len <= buff.size()), {});

        if(len == buff.size()) continue;

        return std::string(buff.data(), len);
    }
}

std::string expand_current_process_env_vars(const char* str)
{
    Assert_NotNullPtr(str);

    std::string res;
    std::string var;

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
        var.assign(str, std::size_t(end - str));

        const char* val = std::getenv(var.c_str());

        if(val != nullptr) res += val;

        str = end + 1;
    }
    return res;
}

int get_current_process_command_line(struct command_line& cmd_line)
{
    const char* osCommandLine = GetCommandLineA();

    const std::size_t len = std::strlen(osCommandLine);

    cmd_line.buff.resize(len + 1);

    char* str = cmd_line.buff.data();

    char* arg = str;
    char* end = str + len;

    std::memcpy(str, osCommandLine, len + 1);

    cmd_line.args.push_back(arg);

    for(bool inSingleQuotes = false, inDoubleQuotes = false ; str < end; )
    {
        switch(*str)
        {
            case ' ':
                if(inSingleQuotes || inDoubleQuotes) break;
                ++str;
                if(*str == ' ') continue;
                if(*str ==  0 ) continue;
                *arg++ = 0;
                cmd_line.args.push_back(arg);
                continue;
            case '\"':
                if(inSingleQuotes) break;
                inDoubleQuotes = !inDoubleQuotes;
                ++str;
                continue;
            case '\'':
                if(inDoubleQuotes) break;
                inSingleQuotes = !inSingleQuotes;
                ++str;
                continue;

            case '\\':
                if(inSingleQuotes) { if(str[1] == '\'') ++str; }
                if(inDoubleQuotes) { if(str[1] == '\"') ++str; }
                break;
        }
        if(arg < str) *arg++ = *str++; else { ++arg; ++str; }
    }
    *arg = 0;

    return int(cmd_line.args.size());
}

}
}

#else

#include <gkr/misc/ptr_sentry.hpp>

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
    std::vector<char> buff;

    for(buff.resize(256); ; buff.resize(2*buff.size()))
    { 
        const ssize_t len = readlink("/proc/self/exe", buff.data(), buff.size());

        Check_Sys_Inspect((len > 0) && (len <= ssize_t(buff.size())), {});

        if(len == ssize_t(buff.size())) continue;

        return std::string(buff.data(), std::size_t(len));
    }
}

std::string expand_current_process_env_vars(const char* str)
{
    Assert_NotNullPtr(str);

    std::string res;
    std::string var;

    for(const char* start = str; ; )
    {
        const char* pos = std::strchr(str, '$');

        if(pos == nullptr)
        {
            res.append(str);
            break;
        }
        if(std::isdigit(pos[1]) || ((pos > start) && (*(pos - 1) == '\\')))
        {
            res.append(str, std::size_t(pos - str + 1));
            str = pos + 1;
            continue;
        }
        const char* end = ++str;

        while((*end == '_') || std::isalnum(*end)) ++end;

        var.assign(str, std::size_t(end - str));

        const char* val = std::getenv(var.c_str());

        if(val != nullptr) res += val;

        str = end + 1;
    }
    return res;
}

int get_current_process_command_line(struct command_line& cmd_line)
{
    misc::ptr_sentry<FILE> file(
        std::fopen("/proc/self/cmdline", "r"),
        [](FILE* f) noexcept { std::fclose(f); }
        );
    if(file.get() == nullptr)
    {
        Check_std_Fail(0);
    }
    cmd_line.buff.resize(256);

    for(std::size_t read = 0; ; )
    {
        const std::size_t cb = (cmd_line.buff.size() - read);

        if(cb == 0)
        {
            cmd_line.buff.resize(cmd_line.buff.size() * 2);
            continue;
        }
        const std::size_t len = std::fread(cmd_line.buff.data() + read, 1, cb, file.get());

        if(len == 0)
        {
            if(read == 0)
            {
                Check_std_Fail(0);
            }
            break;
        }
        read += len;
    }
    file.reset();

    char* arg = cmd_line.buff.data();
    char* end = arg + cmd_line.buff.size() - 1;

    cmd_line.args.push_back(arg);

    for( ; arg < end; ++arg)
    {
        if(*arg == 0)
        {
            cmd_line.args.push_back(arg + 1);
        }
    }
    return int(cmd_line.args.size());
}

}
}

#endif
