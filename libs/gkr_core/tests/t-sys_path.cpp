#include <gkr/defs.hpp>

#include <gkr/sys/path.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/testing/text_exception.hpp>

#define ARG_NULLPTR "Argument is NULL"
#define ARG_INVALID "Argument is invalid"

extern GKR_DIAG_REPORT_FUNC prev_func;

static void ReportFunc(int id, const char* text, const char* func, const char* file, unsigned line)
{
    if(prev_func != nullptr)
    {
        (*prev_func)(id, text, func, file, line);
    }
    switch(id)
    {
        case DIAG_ID_ARG_IS_NULL    : throw gkr::testing::text_exception(ARG_NULLPTR);
        case DIAG_ID_ARG_NOT_VALID  : throw gkr::testing::text_exception(ARG_INVALID);
        case DIAG_ID_ASSERT_FAIL_MSG: throw gkr::testing::text_exception(text);
    }
}

GKR_DIAG_REPORT_FUNC prev_func = gkr_diag_hook_report_func(ReportFunc);

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#ifdef _WIN32
#define ROOT_PATH "C:/"
#define NODE_PATH "C:"
#else
#define ROOT_PATH "/"
#define NODE_PATH ""
#endif

TEST_CASE("sys.path. is_valid")
{
    using namespace gkr::sys;

    CHECK_THROWS_WITH(path_is_valid(nullptr), ARG_NULLPTR);
#ifdef _WIN32
    CHECK_THROWS_WITH(path_is_valid(R"(\\?\C:\a.txt)"), "UNC path is not supported yet");
#endif

    CHECK(path_is_valid(""));
    CHECK(path_is_valid("/"));
    CHECK(path_is_valid("."));
    CHECK(path_is_valid(".."));
    CHECK(path_is_valid("dir1"));
    CHECK(path_is_valid("dir2/"));
    CHECK(path_is_valid("textfile.txt"));
    CHECK(path_is_valid("textfile.txt/"));
    CHECK(path_is_valid("/textfile.txt"));
    CHECK(path_is_valid("abc/textfile.txt"));
    CHECK(path_is_valid("abc/textfile.txt"));
    CHECK(path_is_valid(ROOT_PATH "abc/textfile.txt"));
    CHECK(path_is_valid(NODE_PATH "xyz/datafile.json"));

    CHECK_FALSE(path_is_valid("//"));
    CHECK_FALSE(path_is_valid("..."));
    CHECK_FALSE(path_is_valid("<file>"));
    CHECK_FALSE(path_is_valid("file..name"));
    CHECK_FALSE(path_is_valid("/dir1?name"));
    CHECK_FALSE(path_is_valid("/text*file"));
    CHECK_FALSE(path_is_valid("/data:file"));
    CHECK_FALSE(path_is_valid("file|name.txt"));
}

TEST_CASE("sys.path. has_extension")
{
    using namespace gkr::sys;

    CHECK_THROWS_WITH(path_has_extension(nullptr, nullptr), ARG_NULLPTR);
    CHECK_THROWS_WITH(path_has_extension(nullptr, "txt"  ), ARG_NULLPTR);

    CHECK_THROWS_WITH(path_has_extension("/dir/file.txt", ""     ), ARG_INVALID);
    CHECK_THROWS_WITH(path_has_extension("/dir/file.txt", ".."   ), ARG_INVALID);
    CHECK_THROWS_WITH(path_has_extension("/dir/file.txt", "a/b"  ), ARG_INVALID);
    CHECK_THROWS_WITH(path_has_extension("/dir/file.txt", "a..b" ), ARG_INVALID);
    CHECK_THROWS_WITH(path_has_extension("/dir/file.txt", ".a..b"), ARG_INVALID);

    CHECK(path_has_extension("/dir/file.txt" , nullptr));
    CHECK(path_has_extension("/dir/file.txt.", nullptr));

    CHECK(path_has_extension("/dir/file.txt" , "txt"  ));
    CHECK(path_has_extension("/dir/file.exe" , "exe"  ));
    CHECK(path_has_extension("/dir/file.json", "json" ));
    CHECK(path_has_extension("/file.data.exe", "data.exe"));

    CHECK(path_has_extension("/dir/file.txt" , ".txt"  ));
    CHECK(path_has_extension("/dir/file.exe" , ".exe"  ));
    CHECK(path_has_extension("/dir/file.json", ".json" ));
    CHECK(path_has_extension("/file.data.exe", ".data.exe"));

    CHECK(path_has_extension("/dir/file.txt." , "txt."  ));
    CHECK(path_has_extension("/dir/file.txt." , ".txt." ));

    CHECK(path_has_extension("/dir/filename." , "." ));

    CHECK_FALSE(path_has_extension("/dir/filename" , nullptr));
    CHECK_FALSE(path_has_extension("/dir/filename.", nullptr));

    CHECK_FALSE(path_has_extension("/dir/filename" , "." ));

    CHECK_FALSE(path_has_extension("/dir.jpg/file.txt", "jpg"));
    CHECK_FALSE(path_has_extension("/dir.png/file.txt", "png"));
    CHECK_FALSE(path_has_extension("/file.data.exe"   , "exe"));
    CHECK_FALSE(path_has_extension("/file.data.exe"   , ".exe"));
}

TEST_CASE("sys.path. add_extension")
{
    using namespace gkr::sys;

    std::string path;

    path = "/dir/filename"; CHECK(path_add_extension(path, "." )); CHECK(path == "/dir/filename");
}

TEST_CASE("sys.path. del_extension")
{
    using namespace gkr::sys;
}

TEST_CASE("sys.path. set_extension")
{
    using namespace gkr::sys;
}

TEST_CASE("sys.path. get_extension")
{
    using namespace gkr::sys;
}
