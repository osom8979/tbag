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

TEST(PathStaticTest, Constructors)
{
    char const * const TEMP = "TEMP";

    Path p0;
    Path p1 = Path(TEMP);
    Path p2 = Path(std::string(TEMP));
    Path p3 = Path(TEMP, Path::update_generic());
    Path p4 = Path(std::string(TEMP), Path::update_generic());
    Path p5 = p1;
    Path p6_move = Path(TEMP);
    Path p6 = std::move(p6_move);

    ASSERT_STREQ(p0.getString().c_str(), "");
    ASSERT_STREQ(p1.getString().c_str(), TEMP);
    ASSERT_STREQ(p2.getString().c_str(), TEMP);
    ASSERT_STREQ(p3.getString().c_str(), TEMP);
    ASSERT_STREQ(p4.getString().c_str(), TEMP);
    ASSERT_STREQ(p5.getString().c_str(), TEMP);
    ASSERT_STREQ(p6.getString().c_str(), TEMP);
}

TEST(PathStaticTest, AssignOperators)
{
    char const * const TEMP = "TEMP";

    auto rvalue_test = [&]() -> Path {
        return Path(TEMP);
    };

    Path const TEMP_PATH = Path(TEMP);
    std::string const TEMP_STRING = TEMP;
    ASSERT_STREQ(TEMP_PATH.getString().c_str(), TEMP);

    Path p0;

    p0.setString("");
    p0 = TEMP;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0.setString("");
    p0 = TEMP_STRING;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0.setString("");
    p0 = std::string(TEMP);
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0.setString("");
    p0 = TEMP_PATH;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0.setString("");
    p0 = rvalue_test();
    ASSERT_STREQ(p0.getString().c_str(), TEMP);
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

