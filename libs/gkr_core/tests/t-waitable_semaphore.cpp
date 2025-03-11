#include <gkr/defs.hpp>

#include <gkr/concurency/waitable_semaphore.hpp>

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

TEST_CASE("concurency.waitable_semaphore. lifecycle")
{
    SECTION("empty")
    {
        gkr::waitable_semaphore sem(2, 2);
        CHECK_FALSE(sem.release());
    }
    SECTION("half")
    {
        gkr::waitable_semaphore sem(2, 1);
        CHECK(sem.release());

        CHECK_FALSE(sem.release());
    }
    SECTION("full")
    {
        gkr::waitable_semaphore sem(2, 0);
        CHECK(sem.release());
        CHECK(sem.release());

        CHECK_FALSE(sem.release());
    }
    SECTION("invalid")
    {
        //CHECK_THROWS_WITH(gkr::waitable_semaphore(0U        , 2), ARG_INVALID);
        //CHECK_THROWS_WITH(gkr::waitable_semaphore(0x7FFFFFFF, 2), ARG_INVALID);
        //CHECK_THROWS_WITH(gkr::waitable_semaphore(1         , 2), ARG_INVALID);
    }
}
