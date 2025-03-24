#include <gkr/defs.hpp>
#include <gkr/net/tcp_listener.hpp>

#include <gkr/net/tcp_connection.hpp>

#include <gkr/diagnostics.hpp>

namespace gkr
{
namespace net
{

bool tcp_listener::start_listening(unsigned short listeniningPort, bool useIPv6)
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

void tcp_listener::stop_listening()
{
    if(m_socket.is_open())
    {
        m_socket.close();
    }
}

bool tcp_listener::accept(net::address& addr, tcp_connection& connection, unsigned timeout)
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
