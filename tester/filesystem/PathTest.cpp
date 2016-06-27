/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PathTest, StyleChecker)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_TRUE(Path::isWindowsStyle());
    ASSERT_FALSE(Path::isPosixStyle());
#else
    ASSERT_FALSE(Path::isWindowsStyle());
    ASSERT_TRUE(Path::isPosixStyle());
#endif
}

TEST(PathTest, Constructors_1)
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

TEST(PathTest, Constructors_2)
{
    std::vector<std::string> nodes;
    nodes.push_back("1");
    nodes.push_back("2");

    Path p = nodes;
    ASSERT_STREQ(p.getString().c_str(), "1/2");
}

TEST(PathTest, initializer_list)
{
    Path p0 = {"/", "TE", "M", "P"};
    ASSERT_EQ(p0.getString(), std::string("/TE/M/P"));
}

TEST(PathTest, AssignOperators)
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

TEST(PathTest, getName)
{
    char const * const TEMP = "/1/2/3/4/5.test";

    Path const path1(TEMP);
    ASSERT_STREQ(path1.getName().c_str(), "5.test");

    Path const path2("");
    ASSERT_STREQ(path2.getName().c_str(), "");
}

TEST(PathTest, splitNodesWithCanonical_1)
{
    std::string home = Common::getHomeDir();
    std::string work = Common::getWorkDir();

    char const * const TEMP1 = "~/TEMP1/TEMP2/../TEMP3/./../../TEMP4";
    char const * const TEMP2 = "TEMP2/.";

    Path path1(TEMP1);
    Path path2(TEMP2);

    path1.updateCanonical();
    path2.updateCanonical();

    ASSERT_EQ(path1.getString(), home + "/TEMP4");
    ASSERT_EQ(path2.getString(), work + "/TEMP2");
}

TEST(PathTest, splitNodesWithCanonical_2)
{
#if defined(WIN32) || defined(_WIN32)
    char const * const TEMP = "/TEMP";
#else
    char const * const TEMP = "C:\\TEMP";
#endif

    Path const temp(TEMP);
    Path const path = Path(temp.splitNodesWithCanonical());
    ASSERT_STREQ(temp.getString().c_str(), TEMP);
}

