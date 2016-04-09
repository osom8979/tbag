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
    std::string ws;
    std::string ps;
    std::string gs;
    std::string s;

    std::string depth0;
    std::string depth1;
    std::string depth2;
    std::string depth3;
    std::string depth4;
    std::string depth5;

    std::string test_path;
    std::string windows_path;
    std::string posix_path;
    std::string generic_path;
    std::string canonical_path;

    Path path;

public:
    PathTest() {
        ws = std::string() + PATH_SEPARATOR_OF_WINDOWS;
        ps = std::string() + PATH_SEPARATOR_OF_POSIX;
        gs = std::string() + GetGenericPathSeparator();
        s  = std::string() + GetPathSeparator();
    }

    virtual ~PathTest() {
        __EMPTY_BLOCK__
    }

public:
    virtual void SetUp() override {
        setupOfWindows();
    }

    virtual void TearDown() override {
        __EMPTY_BLOCK__
    }

public:
    void setupOfWindows() {
        depth0 = "O:";
        depth1 = "Temp";
        depth2 = "Directory";
        depth3 = "..";
        depth4 = ".";
        depth5 = "File.tmp";

        test_path      = depth0 + ws + ws + depth1 + ws + depth2 + ps
                       + depth3 + ws + depth4 + ps + ps + ws + ws + ps
                       + depth5 + ws + ps;
        windows_path   = depth0 + ws + depth1 + ws + depth5;
        posix_path     = depth0 + ps + depth1 + ps + depth2 + ps
                       + depth3 + ps + depth4 + ps + depth5;
        generic_path   = posix_path;
        canonical_path = depth0 + ps + depth1 + ps  + depth5;

        path = Path(test_path);
    }

    void setupOfPosix() {
        depth0 = "/";
        depth1 = "Temp";
        depth2 = "\\Directory";
        depth3 = "..";
        depth4 = ".";
        depth5 = "File.tmp";

        test_path      = depth0 + depth1 + ps + depth2 + ps + depth3 + ps
                       + depth4 + ps + ps + ps + ps + ps + depth5 + ps + ps;
        windows_path   = depth0 + depth1 + ps + depth5;
        posix_path     = depth0 + depth1 + ps + depth2 + ps
                       + depth3 + ps + depth4 + ps  + depth5;
        generic_path   = posix_path;
        canonical_path = depth0 + depth1 + ps + depth5;

        path = Path(test_path);
    }
};

TEST_F(PathTest, getGeneric)
{
    ASSERT_EQ(path.getGeneric(), generic_path);

    Path p1(test_path.c_str(), Path::update_generic());
    Path p2(test_path, Path::update_generic());
    ASSERT_EQ(p1.getString(), generic_path);
    ASSERT_EQ(p2.getString(), generic_path);
}

TEST_F(PathTest, getNative)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ(path.getNative(), windows_path);
#else
    ASSERT_EQ(path.getNative(), posix_path);
#endif
}

TEST_F(PathTest, hasAbsoluteOfWindows)
{
    setupOfWindows();
    ASSERT_TRUE(Path::isAbsoluteOfWindows(test_path));
    ASSERT_FALSE(Path::isAbsoluteOfPosix(test_path));

    setupOfPosix();
    ASSERT_TRUE(Path::isAbsoluteOfPosix(test_path));
    ASSERT_FALSE(Path::isAbsoluteOfWindows(test_path));
}

TEST_F(PathTest, splitNodes)
{
    std::vector<Path::BaseString> nodes = path.splitNodes();
    ASSERT_EQ(nodes.size(), 6U);
    ASSERT_EQ(nodes[0], depth0);
    ASSERT_EQ(nodes[1], depth1);
    ASSERT_EQ(nodes[2], depth2);
    ASSERT_EQ(nodes[3], depth3);
    ASSERT_EQ(nodes[4], depth4);
    ASSERT_EQ(nodes[5], depth5);
}

