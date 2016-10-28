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

TEST(WindowsPathTest, IsProhibitedFilename_WithPosix)
{
    char const * const TEMP0 = "TEMP";
    char const * const TEMP1 = "/TEMP";
    char const * const TEMP2 = "TEMP/";
    char const * const TEMP3 = "TE/MP";

    ASSERT_FALSE(WindowsPath::isProhibitedFilename(TEMP0));
    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP2));
    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP3));
}

TEST(WindowsPathTest, IsProhibitedFilename_OnlyWindows)
{
    char const * const TEMP0 = "TE\\MP";
    char const * const TEMP1 = "?TEMP";
    char const * const TEMP2 = "TEMP\x1FTEMP";
    char const * const TEMP3 = "TE_MP";

    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP0));
    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(WindowsPath::isProhibitedFilename(TEMP2));
    ASSERT_FALSE(WindowsPath::isProhibitedFilename(TEMP3));
}

TEST(WindowsPathTest, RemoveLastSeparator)
{
    char const * const TEMP0 = "/\\/TEMP";
    char const * const RESULT0 = "/\\/TEMP";
    ASSERT_STREQ(RESULT0, WindowsPath::removeLastSeparator(TEMP0).c_str());

    char const * const TEMP1 = "/\\/TEMP/";
    char const * const RESULT1 = "/\\/TEMP";
    ASSERT_STREQ(RESULT1, WindowsPath::removeLastSeparator(TEMP1).c_str());

    char const * const TEMP2 = "/\\/TEMP\\/";
    char const * const RESULT2 = "/\\/TEMP";
    ASSERT_STREQ(RESULT2, WindowsPath::removeLastSeparator(TEMP2).c_str());

    char const * const TEMP3 = "/\\/TEMP\\/\\";
    char const * const RESULT3 = "/\\/TEMP";
    ASSERT_STREQ(RESULT3, WindowsPath::removeLastSeparator(TEMP3).c_str());
}

TEST(WindowsPathTest, MakePreferred)
{
    char const * const TEMP0 = "/\\/TEMP\\/\\";
    char const * const RESULT0 = "\\\\\\TEMP\\\\\\";
    ASSERT_STREQ(RESULT0, WindowsPath::makePreferred(TEMP0).c_str());
}

TEST(WindowsPathTest, RemoveDuplicateSeparators)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "\\TEMP\\";
    ASSERT_STREQ(RESULT0, WindowsPath::removeDuplicateSeparators(TEMP0).c_str());
}

TEST(WindowsPathTest, GetGeneric)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/TEMP";
    ASSERT_STREQ(RESULT0, WindowsPath::getGeneric(TEMP0).c_str());
}

TEST(WindowsPathTest, GetRootDir)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "";
    ASSERT_STREQ(RESULT0, WindowsPath::getRootDir(TEMP0).c_str());

    char const * const TEMP1 = "TEMP/";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, WindowsPath::getRootDir(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, WindowsPath::getRootDir(TEMP2).c_str());

    char const * const TEMP3 = "D:\\TEMP\\";
    char const * const RESULT3 = "D:";
    ASSERT_STREQ(RESULT3, WindowsPath::getRootDir(TEMP3).c_str());

    char const * const TEMP4 = "Z:\\";
    char const * const RESULT4 = "Z:";
    ASSERT_STREQ(RESULT4, WindowsPath::getRootDir(TEMP4).c_str());
}

TEST(WindowsPathTest, QueryOperators)
{
    char const * const TEMP0 = "D:/TEMP";
    ASSERT_TRUE(WindowsPath::isAbsolute(TEMP0));
    ASSERT_FALSE(WindowsPath::isRelative(TEMP0));

    char const * const TEMP1 = "TEMP/";
    ASSERT_FALSE(WindowsPath::isAbsolute(TEMP1));
    ASSERT_TRUE(WindowsPath::isRelative(TEMP1));

    char const * const TEMP2 = "";
    ASSERT_FALSE(WindowsPath::isAbsolute(TEMP2));
    ASSERT_TRUE(WindowsPath::isRelative(TEMP2));
}

TEST(WindowsPathTest, GetParent_1)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(RESULT0, WindowsPath::getParent(TEMP0).c_str());

    char const * const TEMP1 = "TEMP";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, WindowsPath::getParent(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, WindowsPath::getParent(TEMP2).c_str());

    char const * const TEMP3 = "\\TEMP\\";
    char const * const RESULT3 = "\\";
    ASSERT_STREQ(RESULT3, WindowsPath::getParent(TEMP3).c_str());

    char const * const TEMP4 = "\\";
    char const * const RESULT4 = "";
    ASSERT_STREQ(RESULT4, WindowsPath::getParent(TEMP4).c_str());
}

TEST(WindowsPathTest, GetParent_2)
{
    char const * const TEMP0 = "G:/TEMP\\//\\/.///..//TEST\\/";
    char const * const TEMP1 = "G:/TEMP\\//\\/.///..//";
    char const * const TEMP2 = "G:/TEMP\\//\\/.///";
    char const * const TEMP3 = "G:/TEMP\\//\\/";
    char const * const TEMP4 = "G:/";
    char const * const TEMP5 = "";

    ASSERT_STREQ(TEMP1, WindowsPath::getParent(TEMP0).c_str());
    ASSERT_STREQ(TEMP2, WindowsPath::getParent(TEMP1).c_str());
    ASSERT_STREQ(TEMP3, WindowsPath::getParent(TEMP2).c_str());
    ASSERT_STREQ(TEMP4, WindowsPath::getParent(TEMP3).c_str());
    ASSERT_STREQ(TEMP5, WindowsPath::getParent(TEMP4).c_str());
}

TEST(WindowsPathTest, SplitNodes_1)
{
    char const * const TEMP0 = "../TEMP\\//";
    char const * const RESULT0 = "..";
    char const * const RESULT1 = "TEMP";

    auto nodes = WindowsPath::splitNodes(TEMP0);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
}

TEST(WindowsPathTest, SplitNodes_2)
{
    char const * const TEMP0 = "D:/TEMP\\//\\/.///..//TEST\\/";
    char const * const RESULT0 = "D:";
    char const * const RESULT1 = "TEMP";
    char const * const RESULT2 = ".";
    char const * const RESULT3 = "..";
    char const * const RESULT4 = "TEST";

    auto nodes = WindowsPath::splitNodes(TEMP0);
    ASSERT_EQ(5U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
    ASSERT_STREQ(RESULT2, nodes[2].c_str());
    ASSERT_STREQ(RESULT3, nodes[3].c_str());
    ASSERT_STREQ(RESULT4, nodes[4].c_str());
}

