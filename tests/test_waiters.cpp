#include "main.h"

#include <gkr/objects_waiter.h>
#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/waitable_semaphore.h>

#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/waitable_semaphore.h>

#include <gkr/objects_waiter.h>

#include <thread>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

static gkr::waitable_event<> e1;
static gkr::waitable_event<> e2;

static gkr::waitable_mutex<> m1;

static gkr::objects_waiter g_waiter;

//gkr::waitable_semaphore<> s1(50);

void foo();
void foo()
{
//  g_waiter.wait(10ms, e1, e2);

    std::lock_guard<gkr::waitable_mutex<>> lock(m1);

    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int test_waiters()
{
    int n = 0;


//  gkr::waitable_event<> e3 = std::move(e1);

    std::thread t1(foo);
//  std::thread t2(foo);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_waiter.wait(gkr::timeout_infinite, m1);

    {
        std::lock_guard<gkr::waitable_mutex<>> lock(m1, std::adopt_lock_t{} );

        ++n;
    }

//  std::this_thread::sleep_for(100ms);

//  e1.set();
//  e2.set();

    t1.join();

    gkr::waitable_mutex<> mutex;
    gkr::waitable_event<> event1, event2;
//  gkr::waitable_semaphore<> semaphore(1);

    event1.fire();
    event2.fire();

    gkr::objects_waiter waiter;

    auto wait_result = waiter.wait(mutex, event1, event2/*, semaphore*/);

    if(auto guard = gkr::guard_waitable_object(wait_result, 0, mutex))
    {
    }
    if(auto guard = gkr::guard_waitable_object(wait_result, 1, event1))
    {
    }
    if(auto guard = gkr::guard_waitable_object(wait_result, 2, event2))
    {
    }
//  if(auto guard = gkr::guard_waitable_object(wait_result, 3, semaphore); guard.wait_is_completed())
//  {
//  }

    return n;
}
