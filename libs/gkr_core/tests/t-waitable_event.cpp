#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_event.hpp>

#include <atomic>
#include <thread>

//#include <gkr/diagnostics.hpp>
//#include <gkr/testing/text_exception.hpp>
//
//#define ARG_INVALID "Argument is invalid"
//
//extern GKR_DIAG_REPORT_FUNC prev_func;
//
//void ReportFunc(int id, const char* text, const char* func, const char* file, int line)
//{
//    if(prev_func != nullptr)
//    {
//        (*prev_func)(id, text, func, file, line);
//    }
//    switch(id)
//    {
//        case DIAG_ID_ARG_NOT_VALID:
//            throw gkr::testing::text_exception(ARG_INVALID);
//    }
//}
//
//GKR_DIAG_REPORT_FUNC prev_func = gkr_diag_hook_report_func(ReportFunc);

#include <catch2/catch_test_macros.hpp>
//#include <catch2/matchers/catch_matchers.hpp>

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

    waitable_event ev1;

    CHECK(ev1.is_valid());

    waitable_event ev2 = std::move(ev1);

    CHECK(ev2.is_valid());

    CHECK_FALSE(ev1.is_valid());
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
