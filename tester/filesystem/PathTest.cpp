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

TEST(PathStaticTest, GlobalPathOperators)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
# endif
}

TEST(PathStaticTest, GetHomeEnvName)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_STREQ(GetHomeEnvName(), "USERPROFILE");
# else
    EXPECT_STREQ(GetHomeEnvName(), "HOME");
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

    std::string _depth0_name;
    std::string _depth1_name;
    std::string _depth2_name;
    std::string _depth3_name;
    std::string _depth4_name;
    std::string _depth5_name;

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

        _depth0_name    = std::string("O:");
        _depth1_name    = std::string("Temp");
        _depth2_name    = std::string("Directory");
        _depth3_name    = std::string("..");
        _depth4_name    = std::string(".");
        _depth5_name    = std::string("File.tmp");

        _test_path      = std::string("O:\\\\Temp\\Directory/..\\.////\\\\/File.tmp\\/");
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

    Path p1(_test_path.c_str(), Path::update_generic());
    Path p2(_test_path, Path::update_generic());
    ASSERT_EQ(p1.getString(), _generic_path);
    ASSERT_EQ(p2.getString(), _generic_path);
}

TEST_F(PathTest, getNative)
{
# if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ(_path.getNative(), _windows_path);
# else
    ASSERT_EQ(_path.getNative(), _unix_path);
# endif
}

TEST_F(PathTest, splitNodes)
{
    std::vector<Path::BaseString> nodes = _path.splitNodes();
    ASSERT_EQ(nodes.size(), 6U);
    ASSERT_EQ(nodes[0], _depth0_name);
    ASSERT_EQ(nodes[1], _depth1_name);
    ASSERT_EQ(nodes[2], _depth2_name);
    ASSERT_EQ(nodes[3], _depth3_name);
    ASSERT_EQ(nodes[4], _depth4_name);
    ASSERT_EQ(nodes[5], _depth5_name);
}

