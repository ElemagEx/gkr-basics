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
        case DIAG_ID_ARG_IS_NULL: throw std::exception(ARG_NULLPTR);
    }
}

GKR_DIAG_REPORT_FUNC prev_func = gkr_diag_hook_report_func(ReportFunc);

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#ifdef _WIN32
#define ROOT_PATH "C:/"
#else
#define ROOT_PATH "/"
#endif

TEST_CASE("sys.path. is_valid")
{
    using namespace gkr::sys;

    CHECK_THROWS_WITH(path_is_valid(nullptr), ARG_NULLPTR);

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

    CHECK_FALSE(path_is_valid("//"));
    CHECK_FALSE(path_is_valid("..."));
}

TEST_CASE("sys.path. insert_ext")
{
    using namespace gkr::sys;

    std::string path = ROOT_PATH "dir/xyz";

#if (DIAG_MODE != 4)
    errno = 0; CHECK(path_insert_extension(nullptr, "txt"   , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_insert_extension("file" , nullptr , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_insert_extension("file" , ""      , 0).empty()); CHECK(errno == EINVAL);
#endif
    path = path_insert_extension(path.c_str(), "txt",  0); CHECK(path == ROOT_PATH "dir/xyz.txt");
    path = path_insert_extension(path.c_str(), "exe",  0); CHECK(path == ROOT_PATH "dir/xyz.txt.exe");
    path = path_insert_extension(path.c_str(), "doc", +1); CHECK(path == ROOT_PATH "dir/xyz.doc.txt.exe");
    path = path_insert_extension(path.c_str(), "cpp", -1); CHECK(path == ROOT_PATH "dir/xyz.doc.txt.cpp.exe");
}

TEST_CASE("sys.path. has_extension")
{
    using namespace gkr::sys;

    CHECK( path_has_extension("/home/user/file.txt", "txt" ));
    CHECK(!path_has_extension("/home/user/file.txt", "json"));
}

TEST_CASE("sys.path. add_filename")
{
    using namespace gkr::sys;


}
