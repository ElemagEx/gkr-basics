#include "main.h"

#include <gkr/misc/stack_args_order.h>

#include <gkr/sys/thread_name.h>

int main()
{
    gkr::sys::set_current_thread_name("gkr-main-thread");

    int n = 0;

    test_logging();
//  test_lockfree_queue();
//  test_waiters();
//  test_thread_worker();

    return n;
}
