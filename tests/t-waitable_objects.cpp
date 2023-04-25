#include <gkr/thread_waiting.h>
#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/waitable_semaphore.h>

#include <thread>

#include <catch2/catch_test_macros.hpp>

using namespace gkr;
using namespace std::chrono_literals;


static waitable_event<> e1;
static waitable_event<> e2;

//waitable_semaphore<> s1(50);

TEST_CASE("thread.waitable_objects. main")
{
    int n = 0;

    waitable_mutex<> m1;

    std::thread t1([&m1] () {
        std::lock_guard<waitable_mutex<>> lock(m1);
        std::this_thread::sleep_for(5s);
    });

    std::this_thread::sleep_for(1s);

    this_thread_objects_waiter().wait(m1);

    {
        std::lock_guard<waitable_mutex<>> lock(m1, std::adopt_lock_t{} );
        ++n;
    }

    std::this_thread::sleep_for(100ms);

    e1.fire();
    e2.fire();

    t1.join();

    waitable_mutex<> mutex;
    waitable_event<> event1, event2;
//  waitable_semaphore<> semaphore(1);

    event1.fire();
    event2.fire();

    auto wait_result = this_thread_objects_waiter().wait(mutex, event1, event2/*, semaphore*/);

    if(waitable_object_wait_is_completed(wait_result, 0))
    {
        std::lock_guard<waitable_mutex<>> lock(mutex, std::adopt_lock_t{} );
    }
    if(waitable_object_wait_is_completed(wait_result, 1))
    {
    }
    if(waitable_object_wait_is_completed(wait_result, 2))
    {
    }
//  if(auto guard = guard_waitable_object(wait_result, 3, semaphore))
//  {
//  }
}
