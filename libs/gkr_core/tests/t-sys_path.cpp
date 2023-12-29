#include <gkr/defs.hpp>
#include <gkr/sys/path.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("sys.path. insert_ext")
{
    using namespace gkr::sys;

    std::string path = "/dir/xyz";

#if (DIAG_MODE != 4)
    errno = 0; CHECK(path_insert_ext(nullptr, "txt"   , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_insert_ext("file" , nullptr , 0).empty()); CHECK(errno == EINVAL);
    errno = 0; CHECK(path_insert_ext("file" , ""      , 0).empty()); CHECK(errno == EINVAL);
#endif
    path = path_insert_ext(path.c_str(), "txt",  0); CHECK(path == "/dir/xyz.txt");
    path = path_insert_ext(path.c_str(), "exe",  0); CHECK(path == "/dir/xyz.txt.exe");
    path = path_insert_ext(path.c_str(), "doc", +1); CHECK(path == "/dir/xyz.doc.txt.exe");
    path = path_insert_ext(path.c_str(), "cpp", -1); CHECK(path == "/dir/xyz.doc.txt.cpp.exe");
}

TEST_CASE("sys.path. has_extension")
{
    using namespace gkr::sys;

    CHECK( path_has_extension("/home/user/file.txt", "txt"));
    CHECK(!path_has_extension("/home/user/file.txt", "json"));
}
