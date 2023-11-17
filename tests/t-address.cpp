#include <gkr/net/address.h>

#include <catch2/catch_test_macros.hpp>

constexpr char ipv4_sample1[] = "192.168.0.2";
constexpr char ipv6_sample1[] = "2001:0db8:0000:0000:0000:ff00:0042:8329";

constexpr int family_ipv4 = 2;
#ifdef _WIN32
constexpr int family_ipv6 = 23;
#else
constexpr int family_ipv6 = 10;
#endif
constexpr std::size_t sizeof_ipv4 = 16;
constexpr std::size_t sizeof_ipv6 = 28;

TEST_CASE("network.address. Comparisions")
{
    gkr::net::address addr0;
    gkr::net::address addr1(ipv4_sample1, 0);
    gkr::net::address addr2(ipv4_sample1, 1);
    gkr::net::address addr3(ipv4_sample1, 1);
    gkr::net::address addr4(ipv4_sample1, 2);

    CHECK(addr0.compare(addr0) == 0);
    CHECK(addr0.compare(addr1) <  0);
    CHECK(addr0.compare(addr2) <  0);
    CHECK(addr0.compare(addr3) <  0);
    CHECK(addr0.compare(addr4) <  0);

    CHECK(addr1.compare(addr0) >  0);
    CHECK(addr1.compare(addr1) == 0);
    CHECK(addr1.compare(addr2) <  0);
    CHECK(addr1.compare(addr3) <  0);
    CHECK(addr1.compare(addr4) <  0);

    CHECK(addr2.compare(addr0) >  0);
    CHECK(addr2.compare(addr1) >  0);
    CHECK(addr2.compare(addr2) == 0);
    CHECK(addr2.compare(addr3) == 0);
    CHECK(addr2.compare(addr4) <  0);

    CHECK(addr3.compare(addr0) >  0);
    CHECK(addr3.compare(addr1) >  0);
    CHECK(addr3.compare(addr2) == 0);
    CHECK(addr3.compare(addr3) == 0);
    CHECK(addr3.compare(addr4) <  0);

    CHECK(addr4.compare(addr0) >  0);
    CHECK(addr4.compare(addr1) >  0);
    CHECK(addr4.compare(addr2) >  0);
    CHECK(addr4.compare(addr3) >  0);
    CHECK(addr4.compare(addr4) == 0);

    CHECK(addr0 == addr0);
    CHECK(addr0 != addr1);
    CHECK(addr0 != addr2);
    CHECK(addr0 != addr3);
    CHECK(addr0 != addr4);

    CHECK(addr1 != addr0);
    CHECK(addr1 == addr1);
    CHECK(addr1 != addr2);
    CHECK(addr1 != addr3);
    CHECK(addr1 != addr4);

    CHECK(addr2 != addr0);
    CHECK(addr2 != addr1);
    CHECK(addr2 == addr2);
    CHECK(addr2 == addr3);
    CHECK(addr2 != addr4);

    CHECK(addr3 != addr0);
    CHECK(addr3 != addr1);
    CHECK(addr3 == addr2);
    CHECK(addr3 == addr3);
    CHECK(addr3 != addr4);

    CHECK(addr4 != addr0);
    CHECK(addr4 != addr1);
    CHECK(addr4 != addr2);
    CHECK(addr4 != addr3);
    CHECK(addr4 == addr4);
}

TEST_CASE("network.address. Getters")
{
    gkr::net::address addr1;

    CHECK(addr1.data() != nullptr);
    CHECK(addr1.capacity() == sizeof(addr1));

    CHECK(addr1.family() == 0);
    CHECK(addr1.size  () == 0);
    CHECK(addr1.port  () == 0);

    CHECK(!addr1.is_valid());
    CHECK(!addr1.is_ipv4());
    CHECK(!addr1.is_ipv6());

    gkr::net::address addr2(ipv4_sample1, 1234);

    CHECK(addr2.data() != nullptr);
    CHECK(addr2.capacity() == sizeof(addr2));

    CHECK(addr2.family() == family_ipv4);
    CHECK(addr2.size  () == sizeof_ipv4);
    CHECK(addr2.port  () == 1234);

    CHECK( addr2.is_valid());
    CHECK( addr2.is_ipv4());
    CHECK(!addr2.is_ipv6());

    gkr::net::address addr3(ipv6_sample1, 4321);

    CHECK(addr3.data() != nullptr);
    CHECK(addr3.capacity() == sizeof(addr3));

    CHECK(addr3.family() == family_ipv6);
    CHECK(addr3.size  () == sizeof_ipv6);
    CHECK(addr3.port  () == 4321);

    CHECK( addr3.is_valid());
    CHECK(!addr3.is_ipv4());
    CHECK( addr3.is_ipv6());
}

TEST_CASE("network.address. main")
{
    gkr::net::address addr(ipv4_sample1, 54321);

    CHECK(addr.is_valid());

    CHECK(addr.family() == 2);

    char buf[32];
    addr.host(buf);

    CHECK(!std::strcmp(ipv4_sample1, buf));
}
