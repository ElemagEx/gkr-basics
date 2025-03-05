#include <gkr/defs.hpp>
#include <gkr/diagnostics.hpp>

#include <gkr/sys/path.hpp>

#include <exception>

extern GKR_DIAG_REPORT_FUNC prev_func;

#define ARG_NULLPTR "Argument is NULL"

void ReportFunc(int id, const char* text, const char* func, const char* file, int line)
{
    if(prev_func != nullptr)
    {
        (*prev_func)(id, text, func, file, line);
    }
    switch(id)
    {
        case DIAG_ID_ARG_IS_NULL    : throw std::exception(ARG_NULLPTR);
        case DIAG_ID_ASSERT_FAIL_MSG: throw std::exception(text);
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
    CHECK_THROWS_WITH(path_is_valid(R"(\\?\C:\a.txt)"), "UNC path is not supported yet");

    CHECK(path_is_valid(""));
    CHECK(path_is_valid("/"));
    CHECK(path_is_valid("."));
    CHECK(path_is_valid(".."));
    CHECK(path_is_valid("dir1"));
    CHECK(path_is_valid("dir2/"));
    CHECK(path_is_valid("textfile.txt"));
    CHECK(path_is_valid("/textfile.txt"));
    CHECK(path_is_valid("abc/textfile.txt"));
    CHECK(path_is_valid("abc/textfile.txt"));
    CHECK(path_is_valid(ROOT_PATH "abc/textfile.txt"));
    CHECK(path_is_valid(NODE_PATH "xyz/datafile.json"));

    CHECK_FALSE(path_is_valid("//"));
    CHECK_FALSE(path_is_valid("..."));
}

TEST_CASE("sys.path. has_extension")
{
    using namespace gkr::sys;

    CHECK( path_has_extension("/home/user/file.txt", nullptr));
    CHECK( path_has_extension("/home/user/file.txt", "txt" ));
    CHECK(!path_has_extension("/home/user/file.txt", "json"));
}

TEST_CASE("sys.path. add_extension")
{
    using namespace gkr::sys;

    std::string path = ROOT_PATH "dir/xyz";

#if (DIAG_MODE != 4)
    errno = 0; CHECK(path_add_extension(nullptr, "txt"   , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_add_extension("file" , nullptr , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_add_extension("file" , ""      , 0).empty()); CHECK(errno == EINVAL);
#endif
    path = path_add_extension(path.c_str(), "txt"); CHECK(path == ROOT_PATH "dir/xyz.txt");
    path = path_add_extension(path.c_str(), "exe"); CHECK(path == ROOT_PATH "dir/xyz.txt.exe");
    path = path_add_extension(path.c_str(), "doc"); CHECK(path == ROOT_PATH "dir/xyz.doc.txt.exe");
    path = path_add_extension(path.c_str(), "cpp"); CHECK(path == ROOT_PATH "dir/xyz.doc.txt.cpp.exe");
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
