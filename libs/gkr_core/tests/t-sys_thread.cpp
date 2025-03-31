#include <gkr/defs.hpp>

#include <gkr/sys/thread.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/testing/text_exception.hpp>

#include <string>

#define ARG_NULLPTR "Argument is NULL"

extern GKR_DIAG_REPORT_FUNC prev_func;

static void ReportFunc(int id, const char* text, const char* func, const char* file, int line)
{
    if(prev_func != nullptr)
    {
        (*prev_func)(id, text, func, file, line);
    }
    switch(id)
    {
        case DIAG_ID_ARG_IS_NULL    : throw gkr::testing::text_exception(ARG_NULLPTR);
    }
}

GKR_DIAG_REPORT_FUNC prev_func = gkr_diag_hook_report_func(ReportFunc);

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

TEST_CASE("sys.current_thread. get_name")
{
    using namespace gkr::sys;

    CHECK_THROWS_WITH(get_current_thread_name(nullptr), ARG_NULLPTR);

    char name[MAX_THREAD_NAME_CCH];
    CHECK(get_current_thread_name(name));
}

TEST_CASE("sys.current_thread. set_name")
{
    using namespace gkr::sys;

    char name[MAX_THREAD_NAME_CCH];
    CHECK(get_current_thread_name(name));

    CHECK_THROWS_WITH(set_current_thread_name(nullptr), ARG_NULLPTR);

    constexpr const char THREAD_NAME_1[MAX_THREAD_NAME_CCH] = "Thread-Name-1";
    constexpr const char THREAD_NAME_2[MAX_THREAD_NAME_CCH] = "Thread-Name-2";

    char buff[MAX_THREAD_NAME_CCH];

    CHECK(set_current_thread_name(THREAD_NAME_1));
    CHECK(get_current_thread_name(buff));

    CHECK_FALSE(std::string(THREAD_NAME_1).compare(buff));

    CHECK(set_current_thread_name(THREAD_NAME_2));
    CHECK(get_current_thread_name(buff));

    CHECK_FALSE(std::string(THREAD_NAME_2).compare(buff));

    CHECK(set_current_thread_name(name));
}
