#include <gkr/lockfree_grow_only_bag.h>

#include <string>

#include <catch2/catch_test_macros.hpp>

struct Data
{
    int a;
    bool b;
};

TEST_CASE("container.lockfree_bag. Instantiation")
{
    gkr::lockfree_grow_only_bag<int>         bag1;
    gkr::lockfree_grow_only_bag<Data>        bag2;
    gkr::lockfree_grow_only_bag<std::string> bag3;

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());
}

TEST_CASE("container.lockfree_bag. Clear")
{
    gkr::lockfree_grow_only_bag<int>         bag1;
    gkr::lockfree_grow_only_bag<Data>        bag2;
    gkr::lockfree_grow_only_bag<std::string> bag3;

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());

    bag1.add();
    bag2.add();
    bag3.add();

    CHECK(!bag1.empty());
    CHECK(!bag2.empty());
    CHECK(!bag3.empty());

    bag1.clear();
    bag2.clear();
    bag3.clear();

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());
}
