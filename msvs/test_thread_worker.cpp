#include "main.h"

#include <gkr/thread_worker_base.h>

class MyThreadWorker : public gkr::thread_worker_base
{
    MyThreadWorker           (const MyThreadWorker&) noexcept = delete;
    MyThreadWorker& operator=(const MyThreadWorker&) noexcept = delete;

public:
    MyThreadWorker() = default;

    virtual ~MyThreadWorker() override
    {
        join(true);
    }

private:
    int sum(int x, int y, int z)
    {
        return (x + y + z);
    }

protected:
    const char* get_name() noexcept override
    {
        return "test-thread-0";
    }
    std::chrono::nanoseconds get_wait_timeout() noexcept override
    {
        return std::chrono::nanoseconds::max();
    }
    size_t get_wait_objects_count() noexcept override
    {
        return 0;
    }
    gkr::waitable_object* get_wait_object(size_t) override
    {
        return nullptr;
    }
    bool start() override
    {
        return true;
    }
    void finish() override
    {
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
    bool on_exception(bool, const std::exception*) noexcept override
    {
        return true;
    }

public:
    int sum_in_thread(int x, int y, int z)
    {
        return execute_action_method<int>(action_id_t(1), x, y, z);
    }
};

int test_thread_worker()
{
    MyThreadWorker myThread;

    int result = 0;

    if(myThread.run())
    {
        result = myThread.sum_in_thread(1, 2, 3);
    }

    return result;
}
