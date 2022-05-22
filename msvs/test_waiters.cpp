#include "main.h"

#include <gkr/objects_waiter.h>
#include <gkr/sync_event.h>

gkr::sync_event<> e1;
gkr::sync_event<> e2;

gkr::objects_waiter waiter;

using namespace std::literals::chrono_literals;

void foo()
{
	waiter.wait(10ms, e1, e2);
}

int test_waiters()
{
	std::thread t1(foo);
//	std::thread t2(foo);

	std::this_thread::sleep_for(100ms);

	//e1.set();
	//e2.set();

	t1.join();

	return 0;
}
