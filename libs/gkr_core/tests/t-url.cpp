#include <gkr/defs.hpp>

#include <gkr/url.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("core.url. decompose")
{
    gkr::url url(128);

    CHECK(url.decompose("https://drago:pass1234@127.0.0.1:3000/binary/v0?key1=val1#bottom"));

    CHECK(url.change_scheme(nullptr));
    CHECK(url.change_scheme("wss"));


//CHECK(size > 0);
}
