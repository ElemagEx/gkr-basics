#include "main.h"

#include <gkr/thread_worker_base.h>

class MyThreadWorker : public gkr::thread_worker_base
{
    MyThreadWorker           (const MyThreadWorker&) noexcept = delete;
    MyThreadWorker& operator=(const MyThreadWorker&) noexcept = delete;

public:
    MyThreadWorker() = default;

    virtual ~MyThreadWorker()
    {
        join(true);
    }

protected:
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
    gkr::waitable_object& get_wait_object(size_t) override
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
    void on_wait_success(size_t) override
    {
    }

private:
    int sum(int x, int y)
    {
        return (x + y);
    }

public:
    int sum_in_thread(int x, int y)
    {
        return execute_action_ex<int>(1, &MyThreadWorker::sum, x, y);
    }
};

int test_thread_worker()
{
    MyThreadWorker myThread;

    if(myThread.run())
    {
        myThread.sum_in_thread(2, 3);
    }

    return 0;
}
