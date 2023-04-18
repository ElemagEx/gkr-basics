#include <gkr/lockfree_grow_only_bag.h>

#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

struct Data
{
    int a;
    bool b;
};


TEST_SUITE_BEGIN("Lock-free grow-only bag");

TEST_CASE("Instantiation")
{
    gkr::lockfree_grow_only_bag<int>         bag1;
    gkr::lockfree_grow_only_bag<Data>        bag2;
    gkr::lockfree_grow_only_bag<std::string> bag3;

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());
}

TEST_SUITE_END;
