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

TEST(WindowsPathTest, isProhibitedFilename_WithPosix)
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

TEST(WindowsPathTest, isProhibitedFilename_OnlyWindows)
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

TEST(WindowsPathTest, removeLastSeparator)
{
    char const * const TEMP0 = "/\\/TEMP";
    char const * const RESULT0 = "/\\/TEMP";
    ASSERT_STREQ(RESULT0, TestPath::removeLastSeparator(TEMP0).c_str());

    char const * const TEMP1 = "/\\/TEMP/";
    char const * const RESULT1 = "/\\/TEMP";
    ASSERT_STREQ(RESULT1, TestPath::removeLastSeparator(TEMP1).c_str());

    char const * const TEMP2 = "/\\/TEMP\\/";
    char const * const RESULT2 = "/\\/TEMP";
    ASSERT_STREQ(RESULT2, TestPath::removeLastSeparator(TEMP2).c_str());

    char const * const TEMP3 = "/\\/TEMP\\/\\";
    char const * const RESULT3 = "/\\/TEMP";
    ASSERT_STREQ(RESULT3, TestPath::removeLastSeparator(TEMP3).c_str());
}

TEST(WindowsPathTest, makePreferred)
{
    char const * const TEMP0 = "/\\/TEMP\\/\\";
    char const * const RESULT0 = "\\\\\\TEMP\\\\\\";
    ASSERT_STREQ(RESULT0, TestPath::makePreferred(TEMP0).c_str());
}

TEST(WindowsPathTest, removeDuplicateSeparators)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "\\TEMP\\";
    ASSERT_STREQ(RESULT0, TestPath::removeDuplicateSeparators(TEMP0).c_str());
}

TEST(WindowsPathTest, getGeneric)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/TEMP";
    ASSERT_STREQ(RESULT0, TestPath::getGeneric(TEMP0).c_str());
}

TEST(WindowsPathTest, getRootDir)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "";
    ASSERT_STREQ(RESULT0, TestPath::getRootDir(TEMP0).c_str());

    char const * const TEMP1 = "TEMP/";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, TestPath::getRootDir(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, TestPath::getRootDir(TEMP2).c_str());

    char const * const TEMP3 = "D:\\TEMP\\";
    char const * const RESULT3 = "D:";
    ASSERT_STREQ(RESULT3, TestPath::getRootDir(TEMP3).c_str());

    char const * const TEMP4 = "Z:\\";
    char const * const RESULT4 = "Z:";
    ASSERT_STREQ(RESULT4, TestPath::getRootDir(TEMP4).c_str());
}

TEST(WindowsPathTest, QueryOperators)
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

TEST(WindowsPathTest, getParent_1)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(RESULT0, TestPath::getParent(TEMP0).c_str());

    char const * const TEMP1 = "TEMP";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, TestPath::getParent(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, TestPath::getParent(TEMP2).c_str());

    char const * const TEMP3 = "\\TEMP\\";
    char const * const RESULT3 = "\\";
    ASSERT_STREQ(RESULT3, TestPath::getParent(TEMP3).c_str());

    char const * const TEMP4 = "\\";
    char const * const RESULT4 = "";
    ASSERT_STREQ(RESULT4, TestPath::getParent(TEMP4).c_str());
}

TEST(WindowsPathTest, getParent_2)
{
    char const * const TEMP0 = "G:/TEMP\\//\\/.///..//TEST\\/";
    char const * const TEMP1 = "G:/TEMP\\//\\/.///..//";
    char const * const TEMP2 = "G:/TEMP\\//\\/.///";
    char const * const TEMP3 = "G:/TEMP\\//\\/";
    char const * const TEMP4 = "G:/";
    char const * const TEMP5 = "";

    ASSERT_STREQ(TEMP1, TestPath::getParent(TEMP0).c_str());
    ASSERT_STREQ(TEMP2, TestPath::getParent(TEMP1).c_str());
    ASSERT_STREQ(TEMP3, TestPath::getParent(TEMP2).c_str());
    ASSERT_STREQ(TEMP4, TestPath::getParent(TEMP3).c_str());
    ASSERT_STREQ(TEMP5, TestPath::getParent(TEMP4).c_str());
}

TEST(WindowsPathTest, splitNodes_1)
{
    char const * const TEMP0 = "../TEMP\\//";
    char const * const RESULT0 = "..";
    char const * const RESULT1 = "TEMP";

    auto nodes = TestPath::splitNodes(TEMP0);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
}

TEST(WindowsPathTest, splitNodes_2)
{
    char const * const TEMP0 = "D:/TEMP\\//\\/.///..//TEST\\/";
    char const * const RESULT0 = "D:";
    char const * const RESULT1 = "TEMP";
    char const * const RESULT2 = ".";
    char const * const RESULT3 = "..";
    char const * const RESULT4 = "TEST";

    auto nodes = TestPath::splitNodes(TEMP0);
    ASSERT_EQ(5U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
    ASSERT_STREQ(RESULT2, nodes[2].c_str());
    ASSERT_STREQ(RESULT3, nodes[3].c_str());
    ASSERT_STREQ(RESULT4, nodes[4].c_str());
}

