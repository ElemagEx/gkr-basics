#include "main.h"

#include <gkr/objects_waiter.h>
#include <gkr/sync_event.h>
#include <gkr/sync_mutex.h>
#include <gkr/sync_semaphore.h>

#include <thread>

gkr::sync_event<> e1;
gkr::sync_event<> e2;

gkr::sync_mutex<> m1;

gkr::objects_waiter g_waiter;

//gkr::sync_semaphore<> s1(50);

void foo()
{
//  g_waiter.wait(10ms, e1, e2);

    std::lock_guard<gkr::sync_mutex<>> lock(m1);

    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int test_waiters()
{
    int n = 0;


//  gkr::sync_event<> e3 = std::move(e1);

    std::thread t1(foo);
//  std::thread t2(foo);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    g_waiter.wait(gkr::timeout_infinite, m1);

    {
        std::lock_guard<gkr::sync_mutex<>> lock(m1, std::adopt_lock_t{} );

        ++n;
    }

//  std::this_thread::sleep_for(100ms);

//  e1.set();
//  e2.set();

    t1.join();

    return n;
}
