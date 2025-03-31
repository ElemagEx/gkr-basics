#include <gkr/defs.hpp>
#include <gkr/net/lib.hpp>

#include <gkr/diagnostics.hpp>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma warning(default:4668)
using sockaddr_inet = SOCKADDR_INET;
#else
#include <unistd.h>
#endif

#include <cstring>

extern "C"
{

int gkr_net_lib_startup()
{
#ifdef _WIN32
    WSADATA wsaData;
    const WORD versionRequested = MAKEWORD(2, 2);

    const int error = WSAStartup(versionRequested, &wsaData);

    return error;
#else
    return 0;
#endif
}

int gkr_net_lib_shutdown()
{
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

int gkr_net_lib_get_hostname(char* buf, unsigned cb)
{
    Check_Arg_NotNull(buf   , -1);
    Check_Arg_IsValid(cb > 0, -1);

    return gethostname(buf, int(cb));
}

}
