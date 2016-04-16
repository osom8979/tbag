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

TEST(PathStaticTest, StyleChecker)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_TRUE(Path::isWindowsStyle());
    ASSERT_FALSE(Path::isPosixStyle());
#else
    ASSERT_FALSE(Path::isWindowsStyle());
    ASSERT_TRUE(Path::isPosixStyle());
#endif
}

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

TEST(PathStaticTest, initializer_list)
{
    Path p0 = {"/", "TE", "M", "P"};
    ASSERT_EQ(p0.getString(), std::string("/TE/M/P"));
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

    p0 = TEMP;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = TEMP_STRING;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = std::string(TEMP);
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = TEMP_PATH;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = rvalue_test();
    ASSERT_STREQ(p0.getString().c_str(), TEMP);
}

TEST(PathStaticTest, replaceHomeDirectoryShortcut)
{
    std::string const HOME = Common::getHomeDir();
    std::string const TEMP = "~";
    ASSERT_EQ(Path::replaceHomeDirectoryShortcut(TEMP), HOME);
}

