#include <gkr/net/address.h>

#include <catch2/catch_test_macros.hpp>

constexpr char ipv4[] = "192.168.0.2";

TEST_CASE("network.address. Lifecycle")
{
    gkr::net::address addr(ipv4, 54321);



}

TEST_CASE("network.address. main")
{
    gkr::net::address addr(ipv4, 54321);

    CHECK(addr.is_valid());

    CHECK(addr.family() == 2);

    char buf[32];
    addr.host(buf);

    CHECK(!std::strcmp(ipv4, buf));
}
