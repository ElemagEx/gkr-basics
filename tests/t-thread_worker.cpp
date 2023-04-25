#include <gkr/thread_worker.h>

#include <catch2/catch_test_macros.hpp>

using namespace gkr;

class MyThreadWorker : public thread_worker
{
    MyThreadWorker           (const MyThreadWorker&) noexcept = delete;
    MyThreadWorker& operator=(const MyThreadWorker&) noexcept = delete;

public:
    MyThreadWorker()
    {
        check_args_order();
    }
    virtual ~MyThreadWorker() override
    {
        join(true);
    }

protected:
    const char* get_name() noexcept override
    {
        return "test-thread-0";
    }
    virtual std::chrono::nanoseconds get_wait_timeout() noexcept override
    {
        return std::chrono::nanoseconds::max();
    }
    virtual size_t get_wait_objects_count() noexcept override
    {
        return 0;
    }
    virtual waitable_object* get_wait_object(size_t) noexcept override
    {
        return nullptr;
    }
    virtual bool on_start() override
    {
        return true;
    }
    virtual void on_finish() override
    {
    }

    enum : action_id_t { Action_ID_SumInThread };

    virtual void on_action(action_id_t action, void* param, void* result) override
    {
        switch(action)
        {
            case Action_ID_SumInThread:
                call_action_method<int>(&MyThreadWorker::sum_in_thread, param, result);
                break;
        }
    }
    virtual void on_wait_timeout() override
    {
    }
    virtual void on_wait_success(size_t) override
    {
    }
    virtual bool on_exception(except_method_t, const std::exception*) noexcept override
    {
        return true;
    }

public:
    int sum_in_thread(int x, int y, int z)
    {
        if(in_worker_thread())
        {
            return (x + y + z);
        }
        else
        {
            return execute_action_method<int>(Action_ID_SumInThread, x, y, z);
        }
    }
};

TEST_CASE("thread.worker. main")
{
    MyThreadWorker myThread;

    REQUIRE(myThread.run());

    int x = 1, y = 2, z = 3;

    int result = myThread.sum_in_thread(x, y, z);

    CHECK(result == (x + y + z));
}
