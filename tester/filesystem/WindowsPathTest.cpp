/**
 * @file   WindowsPathTest.cpp
 * @brief  WindowsPath class tester.
 * @author zer0
 * @date   2016-04-15
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/WindowsPath.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

using TestPath = WindowsPath<char>;

TEST(WindowsPathStaticTest, isProhibitedFilename_WithPosix)
{
    char const * const TEMP0 = "TEMP";
    char const * const TEMP1 = "/TEMP";
    char const * const TEMP2 = "TEMP/";
    char const * const TEMP3 = "TE/MP";

    ASSERT_FALSE(TestPath::isProhibitedFilename(TEMP0));
    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP2));
    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP3));
}

TEST(WindowsPathStaticTest, isProhibitedFilename_OnlyWindows)
{
    char const * const TEMP0 = "TE\\MP";
    char const * const TEMP1 = "?TEMP";
    char const * const TEMP2 = "TEMP\x1FTEMP";
    char const * const TEMP3 = "TE_MP";

    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP0));
    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(TestPath::isProhibitedFilename(TEMP2));
    ASSERT_FALSE(TestPath::isProhibitedFilename(TEMP3));
}

TEST(WindowsPathStaticTest, removeLastSeparator)
{
    char const * const TEMP0 = "/\\/TEMP";
    char const * const RESULT0 = "/\\/TEMP";
    ASSERT_STREQ(TestPath::removeLastSeparator(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "/\\/TEMP/";
    char const * const RESULT1 = "/\\/TEMP";
    ASSERT_STREQ(TestPath::removeLastSeparator(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "/\\/TEMP\\/";
    char const * const RESULT2 = "/\\/TEMP";
    ASSERT_STREQ(TestPath::removeLastSeparator(TEMP2).c_str(), RESULT2);

    char const * const TEMP3 = "/\\/TEMP\\/\\";
    char const * const RESULT3 = "/\\/TEMP";
    ASSERT_STREQ(TestPath::removeLastSeparator(TEMP3).c_str(), RESULT3);
}

TEST(WindowsPathStaticTest, makePreferred)
{
    char const * const TEMP0 = "/\\/TEMP\\/\\";
    char const * const RESULT0 = "\\\\\\TEMP\\\\\\";
    ASSERT_STREQ(TestPath::makePreferred(TEMP0).c_str(), RESULT0);
}

TEST(WindowsPathStaticTest, removeDuplicateSeparators)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "\\TEMP\\";
    ASSERT_STREQ(TestPath::removeDuplicateSeparators(TEMP0).c_str(), RESULT0);
}

TEST(WindowsPathStaticTest, getGeneric)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/TEMP";
    ASSERT_STREQ(TestPath::getGeneric(TEMP0).c_str(), RESULT0);
}

TEST(WindowsPathStaticTest, getRootDir)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "";
    ASSERT_STREQ(TestPath::getRootDir(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "TEMP/";
    char const * const RESULT1 = "";
    ASSERT_STREQ(TestPath::getRootDir(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(TestPath::getRootDir(TEMP2).c_str(), RESULT2);

    char const * const TEMP3 = "D:\\TEMP\\";
    char const * const RESULT3 = "D:";
    ASSERT_STREQ(TestPath::getRootDir(TEMP3).c_str(), RESULT3);

    char const * const TEMP4 = "Z:\\";
    char const * const RESULT4 = "Z:";
    ASSERT_STREQ(TestPath::getRootDir(TEMP4).c_str(), RESULT4);
}

TEST(WindowsPathStaticTest, QueryOperators)
{
    char const * const TEMP0 = "D:/TEMP";
    ASSERT_TRUE(TestPath::isAbsolute(TEMP0));
    ASSERT_FALSE(TestPath::isRelative(TEMP0));

    char const * const TEMP1 = "TEMP/";
    ASSERT_FALSE(TestPath::isAbsolute(TEMP1));
    ASSERT_TRUE(TestPath::isRelative(TEMP1));

    char const * const TEMP2 = "";
    ASSERT_FALSE(TestPath::isAbsolute(TEMP2));
    ASSERT_TRUE(TestPath::isRelative(TEMP2));
}

TEST(WindowsPathStaticTest, getParent_1)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(TestPath::getParent(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "TEMP";
    char const * const RESULT1 = "";
    ASSERT_STREQ(TestPath::getParent(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(TestPath::getParent(TEMP2).c_str(), RESULT2);

    char const * const TEMP3 = "\\TEMP\\";
    char const * const RESULT3 = "\\";
    ASSERT_STREQ(TestPath::getParent(TEMP3).c_str(), RESULT3);

    char const * const TEMP4 = "\\";
    char const * const RESULT4 = "";
    ASSERT_STREQ(TestPath::getParent(TEMP4).c_str(), RESULT4);
}

TEST(WindowsPathStaticTest, getParent_2)
{
    char const * const TEMP0 = "G:/TEMP\\//\\/.///..//TEST\\/";
    char const * const TEMP1 = "G:/TEMP\\//\\/.///..//";
    char const * const TEMP2 = "G:/TEMP\\//\\/.///";
    char const * const TEMP3 = "G:/TEMP\\//\\/";
    char const * const TEMP4 = "G:/";
    char const * const TEMP5 = "";

    ASSERT_STREQ(TestPath::getParent(TEMP0).c_str(), TEMP1);
    ASSERT_STREQ(TestPath::getParent(TEMP1).c_str(), TEMP2);
    ASSERT_STREQ(TestPath::getParent(TEMP2).c_str(), TEMP3);
    ASSERT_STREQ(TestPath::getParent(TEMP3).c_str(), TEMP4);
    ASSERT_STREQ(TestPath::getParent(TEMP4).c_str(), TEMP5);
}

TEST(WindowsPathStaticTest, splitNodes_1)
{
    char const * const TEMP0 = "../TEMP\\//";
    char const * const RESULT0 = "..";
    char const * const RESULT1 = "TEMP";

    auto nodes = TestPath::splitNodes(TEMP0);
    ASSERT_EQ(nodes.size(), 2U);
    ASSERT_STREQ(nodes[0].c_str(), RESULT0);
    ASSERT_STREQ(nodes[1].c_str(), RESULT1);
}

TEST(WindowsPathStaticTest, splitNodes_2)
{
    char const * const TEMP0 = "D:/TEMP\\//\\/.///..//TEST\\/";
    char const * const RESULT0 = "D:";
    char const * const RESULT1 = "TEMP";
    char const * const RESULT2 = ".";
    char const * const RESULT3 = "..";
    char const * const RESULT4 = "TEST";

    auto nodes = TestPath::splitNodes(TEMP0);
    ASSERT_EQ(nodes.size(), 5U);
    ASSERT_STREQ(nodes[0].c_str(), RESULT0);
    ASSERT_STREQ(nodes[1].c_str(), RESULT1);
    ASSERT_STREQ(nodes[2].c_str(), RESULT2);
    ASSERT_STREQ(nodes[3].c_str(), RESULT3);
    ASSERT_STREQ(nodes[4].c_str(), RESULT4);
}

