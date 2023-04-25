#include <gkr/thread_waiting.h>
#include <gkr/waitable_event.h>

#include <catch2/catch_test_macros.hpp>

#include <thread>

TEST_CASE("thread.waiting. main")
{
    using namespace gkr;
    using namespace std::chrono_literals;

    waitable_event<> e1;

    auto t1 = std::thread([&e1] () {
        objects_waiter& waiter = this_thread_objects_waiter();

        auto result = waiter.wait(e1);

        CHECK(waitable_object_wait_is_completed(result, 0));
    });

    std::this_thread::sleep_for(1s);

    e1.fire();
    t1.join();
}
