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
        objects_waiter& waiter = get_this_thread_objects_waiter();

        auto result = waiter.wait(e1);

        if(auto check = check_wait_result(result, 0, e1))
        {
        }
    });

    std::this_thread::sleep_for(1s);

    e1.fire();
    t1.join();
}
