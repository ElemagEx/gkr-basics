#include <gkr/concurency/worker_thread.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace gkr;

class MyWorkerThread : public worker_thread
{
    MyWorkerThread           (const MyWorkerThread&) noexcept = delete;
    MyWorkerThread& operator=(const MyWorkerThread&) noexcept = delete;

public:
    MyWorkerThread()
    {
        check_args_order();
    }
    virtual ~MyWorkerThread() override
    {
        join(true);
    }

private:
    enum : action_id_t { Action_ID_SumInThread };

protected:
    const char* get_name() noexcept override
    {
        return "test-thread-0";
    }
    virtual long long get_wait_timeout_ns() noexcept override
    {
        return WAIT_INFINITE;
    }
    virtual size_t get_waitable_objects_count() noexcept override
    {
        return 0;
    }
    virtual waitable_object& get_waitable_object(std::size_t) noexcept override
    {
        return waitable_object::null_ref();
    }
    virtual bool on_start() override
    {
        return true;
    }
    virtual void on_finish() override
    {
    }
    virtual void on_cross_action(action_id_t action, void* param, void* result) override
    {
        switch(action)
        {
            case Action_ID_SumInThread:
                call_action_method<int>(&MyWorkerThread::sum_in_thread, param, result);
                break;
        }
    }
    virtual void on_queue_action(action_id_t action, void* data) override
    {
    }
    virtual void on_wait_timeout() override
    {
    }
    virtual void on_wait_success(std::size_t) override
    {
    }
    virtual bool on_exception(except_method_t, const std::exception*) noexcept override
    {
        return true;
    }

public:
    int sum_in_thread(int x, int y, int z)
    {
        if(!in_worker_thread())
        {
            return execute_action_method<int>(Action_ID_SumInThread, x, y, z);
        }

        const int sum = (x + y + z);

        return sum;
    }
};

TEST_CASE("concurency.worker_thread. main")
{
    MyWorkerThread myWorkerThread;

    REQUIRE(myWorkerThread.run());

    int x = 1, y = 2, z = 3;

    int result = myWorkerThread.sum_in_thread(x, y, z);

    CHECK(result == (x + y + z));
}
