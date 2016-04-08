/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PathStaticTest, GlobalFunction)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
# endif
}

TEST(PathStaticTest, CopyOperators)
{
    Path path1 = Path("TEMP");
    Path path2 = path1;
    Path path3;
    path3 = path1;

    ASSERT_EQ(path2.getString(), path1.getString());
    ASSERT_EQ(path3.getString(), path1.getString());
}

TEST(PathStaticTest, MoveOperators)
{
    auto rvalue_test = []() -> Path {
        return Path("TEMP");
    };

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif

    Path path1 = std::move(rvalue_test());
    Path path2;

#if defined(__COMP_LLVM__)
# pragma GCC diagnostic pop
#endif

    path2 = rvalue_test();
    ASSERT_EQ(path1.getString(), path2.getString());
}

// Fixture.

class PathTest : public ::testing::Test
{
public:
    std::string _native_separator;
    std::string _generic_separator;

    std::string _test_path;
    std::string _windows_path;
    std::string _unix_path;
    std::string _generic_path;
    std::string _canonical_path;
    Path _path;

public:
    PathTest() = default;
    virtual ~PathTest() = default;

public:
    virtual void SetUp() override {
        _native_separator  = std::string() + GetPathSeparator();
        _generic_separator = std::string() + GetGenericPathSeparator();

        _test_path      = std::string("O:\\\\Temp\\Directory/..\\.////\\\\/File.tmp");
        _windows_path   = std::string("O:\\Temp\\File.tmp");
        _unix_path      = std::string("O:/Temp/Directory/.././File.tmp");
        _generic_path   = std::string("O:/Temp/Directory/.././File.tmp");
        _canonical_path = std::string("O:/Temp/File.tmp");

        _path = Path(_test_path);
    }

    virtual void TearDown() override {
    }
};

TEST_F(PathTest, getGeneric)
{
    ASSERT_EQ(_path.getGeneric(), _generic_path);
}

TEST_F(PathTest, getNative)
{
# if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ(_path.getNative(), _windows_path);
# else
    ASSERT_EQ(_path.getNative(), _unix_path);
# endif
}


