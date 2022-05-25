#include "main.h"

#include <gkr/thread_worker_base.h>

class MyThreadWorker : public gkr::thread_worker_base
{
    const char* get_name() override
    {
        return "test-thread-0";
    }
    std::chrono::nanoseconds get_wait_timeout() override
    {
        return std::chrono::nanoseconds::max();
    }
    bool start() override
    {
        return true;
    }
    void finish() override
    {
    }
    size_t get_wait_objects_count() override
    {
        return 0;
    }
    gkr::waitable_object& get_wait_object(size_t index) override
    {
        return *(gkr::waitable_object*)nullptr;
    }
    void on_action(action_id_t action, void* param, void* result) override
    {
        switch(action)
        {
            case 1:
                call_action_method<int>(&MyThreadWorker::sum, param, result);
                break;
        }
    }
    void on_wait_timeout() override
    {
    }
    void on_wait_success(size_t index) override
    {
    }

public:
    int sum(int x, int y)
    {
        if(in_worker_thread())
        {
            return (x + y);
        }
        else
        {
            return execute_action_ex<int>(1, &MyThreadWorker::sum, x, y);
        }
    }
};

int test_thread_worker()
{
    return 0;
}
