#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <gkr/defs.hpp>
#include <gkr/concurency/events_waiting.hpp>
#include <gkr/concurency/os_events_waiting.hpp>

#include <thread>

static auto now() { return std::chrono::steady_clock::now(); }

using namespace gkr;
using namespace std::chrono_literals;

TEMPLATE_TEST_CASE("concurency.events_waiting. Binding (common)", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter;

    CHECK(waiter.events_count() == 0);

    event_controller events[WAIT_MAX_OBJECTS];
    event_controller event;
    //
    // Binding an events
    //
    CHECK(events[0].bind_with(waiter, false, false));
    CHECK(events[0].is_bound());
    CHECK(waiter.events_count() == 1);
    //
    // Binding events during waiting must fail
    //
    std::thread t([&waiter] () noexcept(DIAG_NOEXCEPT) { waiter.wait(); });
    waiter.wait_for(100ms);
#if (DIAG_MODE != DIAG_MODE_INTRUSIVE)
    CHECK(!event.bind_with(waiter, false, false));
    CHECK(!event.is_bound());
#endif
    CHECK(events[0].fire());
    t.join();
    //
    // Binding to maximum number of events
    //
    for(std::size_t index = 1; index < WAIT_MAX_OBJECTS; ++index)
    {
        CHECK(events[index].bind_with(waiter, false, false));
        CHECK(events[index].is_bound());
        CHECK(waiter.events_count() == (index+1));
    }
    //
    // Binding events beyond maximum number of events must fail
    //
#if (DIAG_MODE != DIAG_MODE_INTRUSIVE)
    CHECK(!event.bind_with(waiter, false, false));
    CHECK(!event.is_bound());
#endif
    CHECK(waiter.events_count() == WAIT_MAX_OBJECTS);
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    for(std::size_t index = 0; index < WAIT_MAX_OBJECTS; ++index)
    {
        events[index].unbind();
        CHECK(!events[index].is_bound());
    }
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Binding (Flag_ForbidMultipleEventsBind)", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter(events_waiter::Flag_ForbidMultipleEventsBind);

    CHECK(waiter.events_count() == 0);

    event_controller event1;
    event_controller event2;
    //
    // Binding an events
    //
    CHECK(event1.bind_with(waiter, false, false));
    CHECK(event1.is_bound());
    CHECK(waiter.events_count() == 1);
    //
    // Binding second event must fail
    //
#if (DIAG_MODE != DIAG_MODE_INTRUSIVE)
    CHECK(!event2.bind_with(waiter, false, false));
    CHECK(!event2.is_bound());
#endif
    CHECK(waiter.events_count() == 1);
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event1.unbind();
    event2.unbind();
    CHECK(!event1.is_bound());
    CHECK(!event2.is_bound());
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Cleanup", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter;

    CHECK(waiter.events_count() == 0);
    //
    // Adding an events
    //
    event_controller event;
    CHECK(event.bind_with(waiter, false, false));
    CHECK(event.is_bound());
    CHECK(waiter.events_count() == 1);
    //
    // Cleanup during waiting must fail
    //
    std::thread t([&waiter] () noexcept(DIAG_NOEXCEPT) { waiter.wait(); });
    waiter.wait_for(100ms);
#if (DIAG_MODE != DIAG_MODE_INTRUSIVE)
    CHECK(!waiter.remove_all_events());
#endif
    CHECK(event.fire());
    t.join();
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event.unbind();
    CHECK(!event.is_bound());
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Waiting (common)", "", stl_events_waiter, os_events_waiter)
{
    wait_result_t wait_result;

    TestType waiter;

    CHECK(waiter.events_count() == 0);
    //
    // Waiting without bound events must fail
    //
#if (DIAG_MODE != DIAG_MODE_INTRUSIVE)
    CHECK(WAIT_RESULT_ERROR == waiter.check());
    CHECK(WAIT_RESULT_ERROR == waiter.wait());
    CHECK(WAIT_RESULT_ERROR == waiter.wait_for(1h));
    CHECK(WAIT_RESULT_ERROR == waiter.wait_until(now() + 1h));
#endif
    //
    // Binding events
    //
    event_controller event1(waiter, false, true);
    event_controller event2(waiter, true , true);
    event_controller event3(waiter, false, true);
    CHECK(waiter.events_count() == 3);
    //
    // Check (without waiting)
    //
    wait_result = waiter.check();
    CHECK(wait_result != WAIT_RESULT_ERROR);
    CHECK(event1.is_signaled(wait_result));
    CHECK(event2.is_signaled(wait_result));
    CHECK(event3.is_signaled(wait_result));
    //
    // Wait (infinite time)
    //
    wait_result = waiter.wait();
    CHECK(wait_result != WAIT_RESULT_ERROR);
    CHECK(!event1.is_signaled(wait_result));
    CHECK( event2.is_signaled(wait_result));
    CHECK(!event3.is_signaled(wait_result));
    //
    // Wait (with timeout)
    //
    wait_result = waiter.wait_for(1h);
    CHECK(wait_result != WAIT_RESULT_ERROR);
    CHECK(!event1.is_signaled(wait_result));
    CHECK( event2.is_signaled(wait_result));
    CHECK(!event3.is_signaled(wait_result));
    //
    // Wait (till time)
    //
    wait_result = waiter.wait_until(now() + 1h);
    CHECK(wait_result != WAIT_RESULT_ERROR);
    CHECK(!event1.is_signaled(wait_result));
    CHECK( event2.is_signaled(wait_result));
    CHECK(!event3.is_signaled(wait_result));
    //
    // Timeout result
    //
    event2.reset();
    //
    // Check (without waiting)
    //
    wait_result = waiter.check();
    CHECK(wait_result == WAIT_RESULT_TIMEOUT);
    //
    // Wait (with timeout)
    //
    wait_result = waiter.wait_for(10ms);
    CHECK(wait_result == WAIT_RESULT_TIMEOUT);
    //
    // Wait (till time)
    //
    wait_result = waiter.wait_until(now() + 10ms);
    CHECK(wait_result == WAIT_RESULT_TIMEOUT);
    //
    // Waiting in multiple threads
    //
    std::thread threads[10];
    for(auto& thread : threads)
    {
        thread = std::thread([&waiter] () { waiter.wait(); });
    }
    waiter.wait_for(10ms);
    event2.fire();
    for(auto& thread : threads)
    {
        thread.join();
    }
    //
    // Partial check always suceeded (even without flag Flag_AllowPartialEventsWait)
    //
    CHECK(!event1.try_consume());
    CHECK( event2.try_consume());
    CHECK(!event3.try_consume());
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event1.unbind();
    event2.unbind();
    event3.unbind();
    CHECK(!event1.is_bound());
    CHECK(!event2.is_bound());
    CHECK(!event3.is_bound());
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Waiting (Flag_AllowPartialEventsWait)", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter(events_waiter::Flag_AllowPartialEventsWait);

    CHECK(waiter.events_count() == 0);
    //
    // Binding events
    //
    event_controller event1(waiter, false, true);
    event_controller event2(waiter, true , true);
    event_controller event3(waiter, false, true);
    CHECK(waiter.events_count() == 3);
    //
    // Waiting on single events
    //
    CHECK(event1.try_consume());
    CHECK(event2.try_consume());
    CHECK(event3.try_consume());
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event1.unbind();
    event2.unbind();
    event3.unbind();
    CHECK(!event1.is_bound());
    CHECK(!event2.is_bound());
    CHECK(!event3.is_bound());
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Event (auto reset)", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter;

    CHECK(waiter.events_count() == 0);
    //
    // Binding event
    //
    event_controller event(waiter, false, true);
    //
    // Successful consume
    //
    CHECK(event.try_consume());
    //
    // Failed consume
    //
    CHECK(!event.try_consume());
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event.unbind();
    CHECK(!event.is_bound());
}

TEMPLATE_TEST_CASE("concurency.events_waiting. Event (manual reset)", "", stl_events_waiter, os_events_waiter)
{
    TestType waiter;

    CHECK(waiter.events_count() == 0);
    //
    // Binding event
    //
    event_controller event(waiter, true, true);
    //
    // Successful consume
    //
    for(std::size_t index = 0; index < 5; ++index)
    {
        CHECK(event.try_consume());
    }
    //
    // Failed consume
    //
    CHECK(event.reset());
    CHECK(!event.try_consume());
    //
    // Cleanup
    //
    CHECK(waiter.remove_all_events());
    CHECK(waiter.events_count() == 0);
    event.unbind();
    CHECK(!event.is_bound());
}
