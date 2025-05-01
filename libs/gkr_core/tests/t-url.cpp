#include <gkr/defs.hpp>

#include <gkr/url.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("core.url. decompose")
{
    gkr::url url(128);

    CHECK(url.assign("https://drago:pass1234@127.0.0.1:3000/binary/v0?key1=val1#bottom"));
}

TEST_CASE("core.url. change_part")
{
    gkr::url url(128);

    CHECK(url.assign("https://drago:pass1234@127.0.0.1:3000/binary/v0?key1=val1#bottom"));

    CHECK(url.change_scheme("wss"));
    CHECK(std::string(url.c_str()) == "wss://drago:pass1234@127.0.0.1:3000/binary/v0?key1=val1#bottom");

    CHECK(url.change_password(""));
    CHECK(std::string(url.c_str()) == "wss://drago:@127.0.0.1:3000/binary/v0?key1=val1#bottom");

    CHECK(url.change_password(nullptr));
    CHECK(std::string(url.c_str()) == "wss://drago@127.0.0.1:3000/binary/v0?key1=val1#bottom");

    CHECK(url.change_username(nullptr));
    CHECK(std::string(url.c_str()) == "wss://127.0.0.1:3000/binary/v0?key1=val1#bottom");

    CHECK(url.change_query(nullptr));
    CHECK(std::string(url.c_str()) == "wss://127.0.0.1:3000/binary/v0#bottom");

    CHECK(url.change_fragment(nullptr));
    CHECK(std::string(url.c_str()) == "wss://127.0.0.1:3000/binary/v0");

    CHECK(url.change_port(nullptr));
    CHECK(std::string(url.c_str()) == "wss://127.0.0.1/binary/v0");

    CHECK(url.change_scheme(nullptr));
    CHECK(std::string(url.c_str()) == "//127.0.0.1/binary/v0");

    CHECK(url.change_host(nullptr));
    CHECK(std::string(url.c_str()) == "/binary/v0");

    CHECK(url.change_host("localhost"));
    CHECK(std::string(url.c_str()) == "//localhost/binary/v0");

    CHECK(url.change_path(nullptr));
    CHECK(std::string(url.c_str()) == "//localhost");

    CHECK(url.change_path("/json/v0"));
    CHECK(std::string(url.c_str()) == "//localhost/json/v0");

    CHECK(url.change_scheme("ws"));
    CHECK(std::string(url.c_str()) == "ws://localhost/json/v0");
}
