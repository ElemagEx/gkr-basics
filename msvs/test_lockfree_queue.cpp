#include "main.h"

#include <gkr/lockfree_queue.h>

struct V
{
    float x, y, z;
};

int test_lockfree_queue()
{
    gkr::lockfree_queue<V, false> queue2(10);

    gkr::lockfree_queue<V, false> queue;
    
    queue = std::move(queue2);

    queue.threading.set_this_thread_as_exclusive_producer();
    queue.threading.any_thread_can_be_producer();
    queue.capacity();
    queue.count();

    {
        auto element(queue.try_start_push({1.f, 2.f, 3.f}));

        if(element.push_in_progress())
        {
            element->x += 1.f;
            element->y += 2.f;
            element->z += 3.f;
        }
    }

    gkr::lockfree_queue<void, true> q1(10, 16);
    q1.try_push();
    
    gkr::lockfree_queue<void, true> q2(10, 16);
    q2.try_push();

    q1 = std::move(q2);

    float s = 0.f;

    while(!queue.empty())
    {
        auto element(queue.try_start_pop());

        if(element.pop_in_progress())
        {
            s += element->x;
            s += element->y;
            s += element->z;
        }
    }

    gkr::lockfree_queue<void> q3;

    q3.try_push();
    q3.try_pop();

    return int(s);
}
