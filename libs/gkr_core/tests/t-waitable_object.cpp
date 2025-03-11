#include <gkr/defs.hpp>

#include <gkr/concurency/waitable_object.hpp>
#include <gkr/concurency/waitable_event.hpp>
#include <gkr/concurency/waitable_semaphore.hpp>

//#include <gkr/diagnostics.hpp>
//#include <gkr/testing/text_exception.hpp>
//
//#include <string>
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
//#include <catch2/catch_template_test_macros.hpp>
//#include <catch2/matchers/catch_matchers.hpp>

TEST_CASE("concurency.waitable_object. wait_single")
{
}

TEST_CASE("concurency.waitable_object. wait_many")
{
}
