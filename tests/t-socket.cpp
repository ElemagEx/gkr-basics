#include <gkr/net/socket.hpp>
#include <gkr/net/lib.hpp>

#include <catch2/catch_test_macros.hpp>

static gkr::net::lib networking;

TEST_CASE("network.socket. main")
{
    gkr::net::socket socket;

    REQUIRE(socket.open_as_udp());

    CHECK(socket.family() != 0);

    auto sndBufSize = socket.get_send_buffer_size();
    auto rcvBufSize = socket.get_receive_buffer_size();

    CHECK(sndBufSize > 0);
    CHECK(rcvBufSize > 0);

    socket.close();
}
