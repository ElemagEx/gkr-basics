#include <gkr/net/address.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("network.address. main")
{
    constexpr char ip[] = "192.168.0.2";

    gkr::net::address addr(ip, 1234);

    CHECK(addr.is_valid());

    CHECK(addr.family() != 0);

    char buf[32];
    addr.host(buf);

    CHECK(!std::strcmp(ip, buf));

}
