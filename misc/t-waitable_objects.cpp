#include <gkr/thread_waiting.h>
#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/waitable_semaphore.h>

#include <thread>

#include <catch2/catch_test_macros.hpp>

using namespace gkr;
using namespace std::chrono_literals;

TEST_CASE("thread.waitable_objects. main")
{
//  int n = 0;

    waitable_mutex<> m1;

    std::thread t1([&m1] () {
        std::lock_guard<waitable_mutex<>> lock(m1);
        std::this_thread::sleep_for(5s);
    });

    std::this_thread::sleep_for(1s);

    this_thread_objects_waiter().wait(m1);

    {
        std::lock_guard<waitable_mutex<>> lock(m1, std::adopt_lock);
    }

    std::this_thread::sleep_for(100ms);

    static waitable_event<> e1;
    static waitable_event<> e2;
    e1.fire();
    e2.fire();

    t1.join();

    waitable_mutex<> mutex;
    waitable_event<> event1, event2;

    event1.fire();
    event2.fire();

#ifdef __cpp_lib_semaphore
    waitable_semaphore<> semaphore(1);
    auto wait_result = this_thread_objects_waiter().wait(mutex, event1, event2, semaphore);
#else
    auto wait_result = this_thread_objects_waiter().wait(mutex, event1, event2);
#endif

    if(waitable_object_wait_is_completed(wait_result, 0))
    {
        std::lock_guard<waitable_mutex<>> lock(mutex, std::adopt_lock);
    }
    if(waitable_object_wait_is_completed(wait_result, 1))
    {
    }
    if(waitable_object_wait_is_completed(wait_result, 2))
    {
    }
    if(waitable_object_wait_is_completed(wait_result, 3))
    {
    }
}
