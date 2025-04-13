#include <gkr/defs.hpp>

#include <gkr/params.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("core.params. compact")
{
    gkr::singlethreaded_params parameters;

    parameters.set_value("key1", "value1");
    parameters.set_value("key2", "value2");
    parameters.set_value("key3", "value3");
    parameters.set_value("key2", "value4");

    std::size_t size = parameters.compact();

    CHECK(size > 0);
}
