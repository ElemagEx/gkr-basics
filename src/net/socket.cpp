#include <gkr/net/socket.h>

#include <gkr/net/address.h>

#include <gkr/diag/diagnostics.h>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
#else
#include <unistd.h>
#include <netinet/in.h>
inline int closesocket(int fd)
{
	return close(fd);
}
#endif

namespace gkr
{
namespace net
{

bool socket::open_as_udp(bool ipv6)
{
    Check_ValidState(!is_open(), false);

    const int s_family = ipv6 ? AF_INET6 : AF_INET;

    m_socket = ::socket(s_family, SOCK_DGRAM, IPPROTO_UDP);

    if(m_socket != INVALID_SOCKET_VALUE)
    {
        return true;
    }
    //errno
    return false;
}

bool socket::reopen()
{
    Check_ValidState(is_open(), false);

    const int s_family   = family();
    const int s_type     = type();
    const int s_protocol = protocol();

    socket_t new_socket = ::socket(s_family, s_type, s_protocol);

    if(new_socket != INVALID_SOCKET_VALUE)
    {
        close();
        m_socket = new_socket;
        return true;
    }
    //errno
    return false;
}

void socket::close()
{
    if(m_socket == INVALID_SOCKET_VALUE) return;

    if(0 == closesocket(m_socket))
    {
        m_socket = INVALID_SOCKET_VALUE;
        return;
    }
    //errno
    return;
}

bool socket::is_ipv4() const
{
    Check_ValidState(is_open(), false);

    return (family() == AF_INET);
}

bool socket::is_ipv6() const
{
    Check_ValidState(is_open(), false);

    return (family() == AF_INET6);
}

int socket::family() const
{
    Check_ValidState(is_open(), AF_UNSPEC);

#ifdef WIN32
    WSAPROTOCOL_INFOW info;
    int len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iAddressFamily;
    }
#else
    int family;
    int len = sizeof(family)
    if(0 == getsocket(m_socket, SOL_SOCKET, SO_DOMAIN, reinterpret_cast<char*>(&family), &len))
    {
        return family;
    }
#endif
    //errno
    return AF_UNSPEC;
}

int socket::type() const
{
    Check_ValidState(is_open(), 0);

#ifdef WIN32
    WSAPROTOCOL_INFOW info;
    int len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iSocketType;
    }
#else
    int type;
    int len = sizeof(type)
    if(0 == getsocket(m_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&type), &len))
    {
        return type;
    }
#endif
    //errno
    return 0;
}

int socket::protocol() const
{
    Check_ValidState(is_open(), 0);

#ifdef WIN32
    WSAPROTOCOL_INFOW info;
    int len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iProtocol;
    }
#else
    int protocol;
    int len = sizeof(protocol)
    if(0 == getsocket(m_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&protocol), &len))
    {
        return protocol;
    }
#endif
    //errno
    return 0;
}

std::size_t socket::get_send_buffer_size() const
{
    Check_ValidState(is_open(), 0);

    int val;
    int len = sizeof(val);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&val), &len))
    {
        return std::size_t(val);
    }
    //errno
    return 0;
}

bool socket::set_send_buffer_size(std::size_t size)
{
    Check_ValidState(is_open(), false);

    int val = int(size);

    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&val), sizeof(val)))
    {
        return true;
    }
    //errno
    return false;
}

std::size_t socket::get_receive_buffer_size() const
{
    Check_ValidState(is_open(), 0);

    int val;
    int len = sizeof(val);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&val), &len))
    {
        return std::size_t(val);
    }
    //errno
    return 0;
}

bool socket::set_receive_buffer_size(std::size_t size)
{
    Check_ValidState(is_open(), false);

    int val = int(size);

    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<char*>(&val), sizeof(val)))
    {
        return true;
    }
    //errno
    return false;
}

bool socket::set_send_timeout(unsigned timeout)
{
    Check_ValidState(is_open(), false);

#if _WIN32
    int len = sizeof(timeout);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&timeout), &len))
    {
        return true;
    }
#else
	timeval tv;
	to.tv_sec  = (timeout / 1000);
	to.tv_usec = (timeout % 1000) * 1000000;
    int len = sizeof(tv);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&tv), &len))
    {
        return true;
    }
#endif
    //errno
    return false;
}

bool socket::set_receive_timeout(unsigned timeout)
{
    Check_ValidState(is_open(), false);

#if _WIN32
    int len = sizeof(timeout);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout), &len))
    {
        return true;
    }
#else
	timeval tv;
	to.tv_sec  = (timeout / 1000);
	to.tv_usec = (timeout % 1000) * 1000000;
    int len = sizeof(tv);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&tv), &len))
    {
        return true;
    }
#endif
    //errno
    return false;
}

bool socket::bind(unsigned short port)
{
    Check_ValidState(is_open(), false);

    address addr(family(), port);

    const socklen_t tolen = socklen_t(addr.size());

    const sockaddr* to = static_cast<const sockaddr*>(addr.data());

    if(0 == ::bind(m_socket, to, tolen))
    {
        return true;
    }
    //errno
    return false;
}

std::size_t socket::send_to(const void* buff, std::size_t size, const address& addr)
{
    Check_Arg_NotNull(buff, 0);
    Check_Arg_IsValid(size > 0, 0);

    Check_ValidState(is_open(), 0);

    const socklen_t tolen = socklen_t(addr.size());

    const sockaddr* to = static_cast<const sockaddr*>(addr.data());

    const auto sent = sendto(m_socket, static_cast<const char*>(buff), socklen_t(size), 0, to, tolen);

    if(sent > 0)
    {
        return std::size_t(sent);
    }
    //errno
    return 0;
}

}
}
