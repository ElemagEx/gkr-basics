#include <gkr/net/socket.h>

#include <gkr/net/address.h>

#include <gkr/diag/diagnostics.h>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
inline int net_error() { return WSAGetLastError(); }
enum
{
    NET_ERROR_TIMEDOUT = WSAETIMEDOUT,
};
#else
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

inline int closesocket(int fd)
{
    return close(fd);
}
inline int net_error() { return errno; }
enum
{
    NET_ERROR_TIMEDOUT = ETIMEDOUT,
};
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
    socklen_t len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iAddressFamily;
    }
#else
    int family;
    socklen_t len = sizeof(family);
    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_DOMAIN, reinterpret_cast<char*>(&family), &len))
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
    socklen_t len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iSocketType;
    }
#else
    int type;
    socklen_t len = sizeof(type);
    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&type), &len))
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
    socklen_t len = sizeof(info);

    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_PROTOCOL_INFO, reinterpret_cast<char*>(&info), &len))
    {
        return info.iProtocol;
    }
#else
    int protocol;
    socklen_t len = sizeof(protocol);
    if(0 == getsockopt(m_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<char*>(&protocol), &len))
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
    socklen_t len = sizeof(val);

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
    socklen_t len = sizeof(val);

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

#ifdef _WIN32
    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&timeout), sizeof(timeout)))
    {
        return true;
    }
#else
    timeval tv;
    tv.tv_sec  = (timeout / 1000);
    tv.tv_usec = (timeout % 1000) * 1000000;

    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&tv), sizeof(tv)))
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

#ifdef _WIN32
    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout), sizeof(timeout)))
    {
        return true;
    }
#else
    timeval tv;
    tv.tv_sec  = (timeout / 1000);
    tv.tv_usec = (timeout % 1000) * 1000000;

    if(0 == setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&tv), sizeof(tv)))
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

    address addr(is_ipv6(), port);

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

std::size_t socket::receive_from(void* buff, std::size_t size, address& addr, unsigned* errors)
{
    Check_Arg_NotNull(buff, 0);
    Check_Arg_IsValid(size > 0, 0);

    Check_ValidState(is_open(), 0);

    socklen_t fromlen = socklen_t(addr.capacity());

    sockaddr* from = static_cast<sockaddr*>(addr.data());

    const auto recv = recvfrom(m_socket, static_cast<char*>(buff), socklen_t(size), 0, from, &fromlen);

    if(recv >= 0)
    {
        if(errors != nullptr) *errors = error_none;
        Check_ValidState(std::size_t(recv) <= size, 0);
        return std::size_t(recv);
    }
    const int error = net_error();

    if(check_errors(error, errors)) return 0;

    //errno
    return 0;
}

bool socket::check_errors(int error, unsigned* errors)
{
    if(errors != nullptr)
    {
        switch(error)
        {
            case NET_ERROR_TIMEDOUT: if((*errors & error_timeout)  == 0) break; *errors = error_timeout; return true;
        }
        *errors = error_other;
    }
    return false;
}

}
}
