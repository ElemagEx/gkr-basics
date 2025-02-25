#include <gkr/defs.hpp>
#include <gkr/comm/tcp_socket_listener.hpp>

#include <gkr/comm/tcp_socket_connection.hpp>

#include <gkr/diagnostics.hpp>

namespace gkr
{
namespace comm
{

bool tcp_socket_listener::start_listening(unsigned short listeniningPort, bool useIPv6)
{
    Check_ValidState(!is_listening(), false);

    if(!m_socket.open_as_tcp(useIPv6))
    {
        return false;
    }
    if(!m_socket.bind(listeniningPort))
    {
        stop_listening();
        return false;
    }
    if(!m_socket.listen())
    {
        stop_listening();
        return false;
    }
    return true;
}

void tcp_socket_listener::stop_listening()
{
    if(m_socket.is_open())
    {
        m_socket.close();
    }
}

bool tcp_socket_listener::accept(net::address& addr, tcp_socket_connection& connection, unsigned timeout)
{
    unsigned errors = net::socket::error_timeout;

    net::socket conn;
    if(m_socket.accept(addr, conn, timeout, &errors))
    {
        connection.attach(std::move(conn));
        return true;
    }
    return false;
}

}
}
