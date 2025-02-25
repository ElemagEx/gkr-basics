#include <gkr/defs.hpp>
#include <gkr/comm/tcp_socket_connection.hpp>

#include <gkr/diagnostics.hpp>

namespace gkr
{
namespace comm
{

void tcp_socket_connection::close()
{
    if(m_socket.is_open())
    {
        m_socket.close();
    }
}

bool tcp_socket_connection::attach(net::socket&& socket)
{
    Check_ValidState(!is_connected(), false);

    m_socket = std::move(socket);

    return false;
}

bool tcp_socket_connection::connect(const net::address& addr)
{
    Check_Arg_IsValid(addr.is_valid(), false);

    Check_ValidState(!is_connected(), false);

    unsigned errors = net::socket::error_timeout | net::socket::error_conn_refused;

    return m_socket.connect(addr, &errors);
}

std::size_t tcp_socket_connection::read_data(void* buffer, std::size_t size)
{
    unsigned errors = net::socket::error_timeout | net::socket::error_conn_aborted;

    const std::size_t read = m_socket.read(buffer, size, &errors);

    return read;
}

std::size_t tcp_socket_connection::write_data(const void* buffer, std::size_t size)
{
    unsigned errors = net::socket::error_timeout | net::socket::error_conn_aborted;

    const std::size_t sent = m_socket.write(buffer, size, &errors);

    return sent;
}

}
}
