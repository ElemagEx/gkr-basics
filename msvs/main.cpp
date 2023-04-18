#include "main.h"

#include <gkr/sys/thread_name.h>

int main()
{
    gkr::sys::set_current_thread_name("gkr-main-thread");

    int n = 0;

    test_lockfree_grow_only_bag();
//  test_logging();
//  test_lockfree_queue();
//  test_waiters();
//  test_thread_worker();

    return n;
}
