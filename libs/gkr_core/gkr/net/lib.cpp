#include "lib.hpp"

#include <gkr/diagnostics.h>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
using sockaddr_inet = SOCKADDR_INET;
#else
#include <unistd.h>
#endif

#include <cstring>

namespace gkr
{
namespace net
{

bool startup()
{
#ifdef _WIN32
    WSADATA wsaData;
    const WORD versionRequested = MAKEWORD(2, 2);

    const int error = WSAStartup(versionRequested, &wsaData);

    return (error == 0);
#elif defined(__ANDROID__)
    //TODO:By calling some networking function (ex: socket) check
    //whether networking support permision is enabled for this app
    return true;
#else
    return true;
#endif
}

void shutdown()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

int get_hostname(char* buffer, unsigned cch)
{
    char hostname[256];

    if(0 != gethostname(hostname, 256))
    {
        return 0;
    }

    const std::size_t len = std::strlen(hostname);

    if(len >= cch) return int(cch + 1);

    Check_Arg_NotNull(buffer, 0);

    std::strncpy(buffer, hostname, cch);

    return int(len);
}

}
}
