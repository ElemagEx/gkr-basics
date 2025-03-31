#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_event.hpp>

#include <atomic>
#include <thread>

#include <gkr/diagnostics.hpp>
#include <gkr/testing/text_exception.hpp>

#define BROKEN_WAIT "There is an active thread that wait for this object"

extern GKR_DIAG_REPORT_FUNC prev_func;

static void ReportFunc(int id, const char* text, const char* func, const char* file, int line)
{
    if(prev_func != nullptr)
    {
        (*prev_func)(id, text, func, file, line);
    }
    switch(id)
    {
        case DIAG_ID_ASSERT_COND_MSG:
            throw gkr::testing::text_exception(text);
    }
}

GKR_DIAG_REPORT_FUNC prev_func = gkr_diag_hook_report_func(ReportFunc);

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

TEST_CASE("concurency.waitable_event. lifecycle")
{
    using namespace gkr;

    SECTION("auto-reset, Non-Signaled")
    {
        waitable_event ev(false, false);

        CHECK_FALSE(ev.try_consume());
        ev.fire();
        CHECK(ev.try_consume());
        CHECK_FALSE(ev.try_consume());
    }
    SECTION("auto-reset, Signaled")
    {
        waitable_event ev(false, true);

        CHECK(ev.try_consume());
        CHECK_FALSE(ev.try_consume());
    }
    SECTION("manual-reset, Non-Signaled")
    {
        waitable_event ev(true, false);

        CHECK_FALSE(ev.try_consume());
        ev.fire();
        CHECK(ev.try_consume());
        CHECK(ev.try_consume());
    }
    SECTION("manual-reset, Signaled")
    {
        waitable_event ev(true, true);

        CHECK(ev.try_consume());
        CHECK(ev.try_consume());
    }
}

TEST_CASE("concurency.waitable_event. is_valid")
{
    using namespace gkr;

    waitable_event ev;

    CHECK(ev.is_valid());

    ev.close();

    CHECK_FALSE(ev.is_valid());
}

TEST_CASE("concurency.waitable_event. is_manual_reset")
{
    using namespace gkr;

    SECTION("auto-reset")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.is_manual_reset());
    }
    SECTION("manual-reset")
    {
        waitable_event ev(true, false);
        CHECK(ev.is_manual_reset());
    }
}

TEST_CASE("concurency.waitable_event. set_manual_reset")
{
    using namespace gkr;

    SECTION("auto-reset")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.is_manual_reset());

        ev.set_manual_reset(true);
        CHECK(ev.is_manual_reset());

        ev.set_manual_reset(true);
        CHECK(ev.is_manual_reset());
    }
    SECTION("manual-reset")
    {
        waitable_event ev(true, false);
        CHECK(ev.is_manual_reset());

        ev.set_manual_reset(false);
        CHECK_FALSE(ev.is_manual_reset());

        ev.set_manual_reset(false);
        CHECK_FALSE(ev.is_manual_reset());
    }
}

TEST_CASE("concurency.waitable_event. fire")
{
    using namespace gkr;

    SECTION("auto-reset - singlethreaded")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());
        
        ev.fire();
        
        CHECK(ev.try_consume());
        CHECK_FALSE(ev.try_consume());
    }
    SECTION("auto-reset - multithreaded (1 threads)")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        std::thread thread([&ev] () { ev.consume(); });

        ev.fire();

        thread.join();

        CHECK_FALSE(ev.try_consume());
    }
    SECTION("auto-reset - multithreaded (2 threads)")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        std::atomic_uint val {0};

        std::thread thread1([&ev, &val] () { ev.consume(); ++val; });
        std::thread thread2([&ev, &val] () { ev.consume(); ++val; });

        for(ev.fire(); val.load() == 0; std::this_thread::yield());

        CHECK_FALSE(ev.try_consume());

        for(ev.fire(); val.load() == 1; std::this_thread::yield());

        CHECK_FALSE(ev.try_consume());

        thread2.join();
        thread1.join();
    }
    SECTION("manual-reset - singlethreaded")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        ev.fire();

        CHECK(ev.try_consume());
        CHECK(ev.try_consume());
    }
    SECTION("manual-reset - multithreaded (1 threads)")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        std::thread thread([&ev] () { ev.consume(); });

        ev.fire();

        thread.join();

        CHECK(ev.try_consume());
    }
    SECTION("manual-reset - multithreaded (2 threads)")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        std::thread thread1([&ev]() { ev.consume(); });
        std::thread thread2([&ev]() { ev.consume(); });

        ev.fire();

        thread2.join();
        thread1.join();

        CHECK(ev.try_consume());
    }
}

TEST_CASE("concurency.waitable_event. reset")
{
    using namespace gkr;

    SECTION("auto-reset")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        ev.reset();
        CHECK_FALSE(ev.try_consume());

        ev.fire();
        ev.reset();
        CHECK_FALSE(ev.try_consume());
    }
    SECTION("manual-reset")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        ev.reset();
        CHECK_FALSE(ev.try_consume());

        ev.fire();
        CHECK(ev.try_consume());
        CHECK(ev.try_consume());
        ev.reset();
        CHECK_FALSE(ev.try_consume());
    }
}

TEST_CASE("concurency.waitable_event. wait-broken")
{
    #ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    using namespace gkr;
    using namespace std::literals::chrono_literals;

    waitable_event ev(false, false);
    CHECK_FALSE(ev.try_consume());

    std::thread thread([&ev] () { ev.consume(); });

    std::this_thread::sleep_for(1ms);

    CHECK_THROWS_WITH(ev.close(), BROKEN_WAIT);

    ev.fire();

    thread.join();
    #endif
}

TEST_CASE("concurency.waitable_event. wait-single")
{
    using namespace gkr;
    using namespace std::literals::chrono_literals;

    SECTION("auto-reset - singlethreaded")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        CHECK_FALSE(ev.wait_for(0ms));
        CHECK_FALSE(ev.wait_for(1ms));

        ev.fire();
        CHECK(ev.wait_for(0ms));

        ev.fire();
        CHECK(ev.wait_for(1min));
    }
    SECTION("auto-reset - multithreaded (1 threads)")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        bool res;

        std::thread thread1([&ev, &res] () { res = ev.wait_for(0ms); });
        thread1.join();
        CHECK_FALSE(res);

        std::thread thread2([&ev, &res] () { res = ev.wait_for(1ms); });
        thread2.join();
        CHECK_FALSE(res);

        std::thread thread3([&ev, &res] () { res = ev.wait_for(1min); });
        std::this_thread::sleep_for(1ms);
        ev.fire();
        thread3.join();
        CHECK(res);
        CHECK_FALSE(ev.try_consume());

        ev.fire();
        std::thread thread4([&ev, &res] () { res = ev.wait_for(2min); });
        thread4.join();
        CHECK(res);
    }
    SECTION("auto-reset - multithreaded (2 threads)")
    {
        waitable_event ev(false, false);
        CHECK_FALSE(ev.try_consume());

        bool res1, res2;
        std::thread thread1([&ev, &res1] () { res1 = ev.wait_for(0ms); });
        std::thread thread2([&ev, &res2] () { res2 = ev.wait_for(1ms); });
        std::this_thread::sleep_for(1ms);
        thread1.join();
        thread2.join();
        CHECK_FALSE(res1);
        CHECK_FALSE(res2);
        CHECK_FALSE(ev.try_consume());

        bool res3, res4;
        std::thread thread3([&ev, &res3] () { res3 = ev.wait_for(1min); });
        std::thread thread4([&ev, &res4] () { res4 = ev.wait_for(2min); });
        std::this_thread::sleep_for(1ms);
        ev.fire();
        std::this_thread::sleep_for(1ms);
        ev.fire();
        thread3.join();
        thread4.join();
        CHECK(res3);
        CHECK(res3);
        CHECK_FALSE(ev.try_consume());
    }
    SECTION("manual-reset - singlethreaded")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        CHECK_FALSE(ev.wait_for(0ms));
        CHECK_FALSE(ev.wait_for(1ms));

        ev.fire();

        CHECK(ev.wait_for(0min));
        CHECK(ev.wait_for(1min));
        CHECK(ev.try_consume());
    }
    SECTION("manual-reset - multithreaded (1 threads)")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        bool res;

        std::thread thread1([&ev, &res] () { res = ev.wait_for(0ms); });
        thread1.join();
        CHECK_FALSE(res);

        std::thread thread2([&ev, &res] () { res = ev.wait_for(1ms); });
        thread2.join();
        CHECK_FALSE(res);

        std::thread thread3([&ev, &res] () { res = ev.wait_for(1min); });
        std::this_thread::sleep_for(1ms);
        ev.fire();
        thread3.join();
        CHECK(res);

        std::thread thread4([&ev, &res] () { res = ev.wait_for(1min); });
        thread4.join();
        CHECK(res);
    }
    SECTION("manual-reset - multithreaded (2 threads)")
    {
        waitable_event ev(true, false);
        CHECK_FALSE(ev.try_consume());

        bool res1, res2;
        std::thread thread1([&ev, &res1] () { res1 = ev.wait_for(0ms); });
        std::thread thread2([&ev, &res2] () { res2 = ev.wait_for(1ms); });
        std::this_thread::sleep_for(1ms);
        thread1.join();
        thread2.join();
        CHECK_FALSE(res1);
        CHECK_FALSE(res2);
        CHECK_FALSE(ev.try_consume());

        bool res3, res4;
        std::thread thread3([&ev, &res3] () { res3 = ev.wait_for(1min); });
        std::thread thread4([&ev, &res4] () { res4 = ev.wait_for(2min); });
        std::this_thread::sleep_for(1ms);
        ev.fire();
        thread3.join();
        thread4.join();
        CHECK(res3);
        CHECK(res3);
        CHECK(ev.try_consume());
    }
}

TEST_CASE("concurency.waitable_event. wait-multiple")
{
    using namespace gkr;
    using namespace std::literals::chrono_literals;

    SECTION("singlethreaded")
    {
        waitable_event ev1(false, false);
        CHECK_FALSE(ev1.try_consume());

        waitable_event ev2(true, false);
        CHECK_FALSE(ev2.try_consume());

        waitable_event ev3(false, false);
        CHECK_FALSE(ev3.try_consume());

        wait_result_t wait_result;
        wait_result = waitable_object::wait_many_for(1ms, ev1, ev2, ev3);
        CHECK(wait_result == WAIT_RESULT_TIMEOUT);

        ev2.fire();
        wait_result = waitable_object::wait_many_for(1min, ev1, ev2, ev3);

        CHECK(!waitable_object::is_signaled(wait_result, 0));
        CHECK( waitable_object::is_signaled(wait_result, 1));
        CHECK(!waitable_object::is_signaled(wait_result, 2));
    }
    SECTION("multithreaded")
    {
        waitable_event ev1(false, false);
        CHECK_FALSE(ev1.try_consume());

        waitable_event ev2(true, false);
        CHECK_FALSE(ev2.try_consume());

        waitable_event ev3(false, false);
        CHECK_FALSE(ev3.try_consume());

        wait_result_t wait_result;
        std::thread thread1([&ev1, &ev2, &ev3, &wait_result] () { wait_result = waitable_object::wait_many_for(1ms, ev1, ev2, ev3); });
        thread1.join();
        CHECK(wait_result == WAIT_RESULT_TIMEOUT);

        std::thread thread2([&ev1, &ev2, &ev3, &wait_result] () { wait_result = waitable_object::wait_many_for(1min, ev1, ev2, ev3); });
        std::this_thread::sleep_for(1ms);
        ev2.fire();
        thread2.join();
        CHECK(!waitable_object::is_signaled(wait_result, 0));
        CHECK( waitable_object::is_signaled(wait_result, 1));
        CHECK(!waitable_object::is_signaled(wait_result, 2));
    }
}
