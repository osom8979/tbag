/**
 * @file   PosixPathTest.cpp
 * @brief  PosixPath class tester.
 * @author zer0
 * @date   2016-04-15
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/PosixPath.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PosixPathTest, IsProhibitedFilename)
{
    char const * const TEMP0 = "TEMP";
    char const * const TEMP1 = "/TEMP";
    char const * const TEMP2 = "TEMP/";
    char const * const TEMP3 = "TE/MP";

    ASSERT_FALSE(PosixPath::isProhibitedFilename(TEMP0));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP2));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP3));
}

TEST(PosixPathTest, RemoveLastSeparator)
{
    char const * const TEMP0 = "/\\/TEMP";
    char const * const RESULT0 = "/\\/TEMP";
    ASSERT_STREQ(RESULT0, PosixPath::removeLastSeparator(TEMP0).c_str());

    char const * const TEMP1 = "/\\/TEMP/";
    char const * const RESULT1 = "/\\/TEMP";
    ASSERT_STREQ(RESULT1, PosixPath::removeLastSeparator(TEMP1).c_str());

    char const * const TEMP2 = "/\\/TEMP\\/";
    char const * const RESULT2 = "/\\/TEMP\\";
    ASSERT_STREQ(RESULT2, PosixPath::removeLastSeparator(TEMP2).c_str());

    char const * const TEMP3 = "/\\/TEMP\\/\\";
    char const * const RESULT3 = "/\\/TEMP\\/\\";
    ASSERT_STREQ(RESULT3, PosixPath::removeLastSeparator(TEMP3).c_str());
}

TEST(PosixPathTest, MakePreferred)
{
    char const * const TEMP0 = "/\\/TEMP\\/\\";
    char const * const RESULT0 = "/\\/TEMP\\/\\";
    ASSERT_STREQ(RESULT0, PosixPath::makePreferred(TEMP0).c_str());
}

TEST(PosixPathTest, RemoveDuplicateSeparators)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/\\/\\\\TEMP\\/\\/";
    ASSERT_STREQ(RESULT0, PosixPath::removeDuplicateSeparators(TEMP0).c_str());
}

TEST(PosixPathTest, GetGeneric)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/\\/\\\\TEMP\\/\\";
    ASSERT_STREQ(RESULT0, PosixPath::getGeneric(TEMP0).c_str());
}

TEST(PosixPathTest, GetRootDir)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(RESULT0, PosixPath::getRootDir(TEMP0).c_str());

    char const * const TEMP1 = "TEMP/";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, PosixPath::getRootDir(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, PosixPath::getRootDir(TEMP2).c_str());
}

TEST(PosixPathTest, QueryOperators)
{
    char const * const TEMP0 = "/TEMP";
    ASSERT_TRUE(PosixPath::isAbsolute(TEMP0));
    ASSERT_FALSE(PosixPath::isRelative(TEMP0));

    char const * const TEMP1 = "TEMP/";
    ASSERT_FALSE(PosixPath::isAbsolute(TEMP1));
    ASSERT_TRUE(PosixPath::isRelative(TEMP1));

    char const * const TEMP2 = "";
    ASSERT_FALSE(PosixPath::isAbsolute(TEMP2));
    ASSERT_TRUE(PosixPath::isRelative(TEMP2));
}

TEST(PosixPathTest, GetParent_1)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(RESULT0, PosixPath::getParent(TEMP0).c_str());

    char const * const TEMP1 = "TEMP";
    char const * const RESULT1 = "";
    ASSERT_STREQ(RESULT1, PosixPath::getParent(TEMP1).c_str());

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(RESULT2, PosixPath::getParent(TEMP2).c_str());

    char const * const TEMP3 = "/TEMP/";
    char const * const RESULT3 = "/";
    ASSERT_STREQ(RESULT3, PosixPath::getParent(TEMP3).c_str());

    char const * const TEMP4 = "/";
    char const * const RESULT4 = "";
    ASSERT_STREQ(RESULT4, PosixPath::getParent(TEMP4).c_str());
}

TEST(PosixPathTest, GetParent_2)
{
    char const * const TEMP0 = "/TEMP\\//\\/.///..//TEST\\/";
    char const * const TEMP1 = "/TEMP\\//\\/.///..//";
    char const * const TEMP2 = "/TEMP\\//\\/.///";
    char const * const TEMP3 = "/TEMP\\//\\/";
    char const * const TEMP4 = "/TEMP\\//";
    char const * const TEMP5 = "/";
    char const * const TEMP6 = "";

    ASSERT_STREQ(TEMP1, PosixPath::getParent(TEMP0).c_str());
    ASSERT_STREQ(TEMP2, PosixPath::getParent(TEMP1).c_str());
    ASSERT_STREQ(TEMP3, PosixPath::getParent(TEMP2).c_str());
    ASSERT_STREQ(TEMP4, PosixPath::getParent(TEMP3).c_str());
    ASSERT_STREQ(TEMP5, PosixPath::getParent(TEMP4).c_str());
    ASSERT_STREQ(TEMP6, PosixPath::getParent(TEMP5).c_str());
}

TEST(PosixPathTest, SplitNodes_1)
{
    char const * const TEMP0 = "../TEMP\\//";
    char const * const RESULT0 = "..";
    char const * const RESULT1 = "TEMP\\";

    auto nodes = PosixPath::splitNodes(TEMP0);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
}

TEST(PosixPathTest, SplitNodes_2)
{
    char const * const TEMP0 = "/TEMP\\//\\/.///..//TEST\\/";
    char const * const RESULT0 = "/";
    char const * const RESULT1 = "TEMP\\";
    char const * const RESULT2 = "\\";
    char const * const RESULT3 = ".";
    char const * const RESULT4 = "..";
    char const * const RESULT5 = "TEST\\";

    auto nodes = PosixPath::splitNodes(TEMP0);
    ASSERT_EQ(6U, nodes.size());
    ASSERT_STREQ(RESULT0, nodes[0].c_str());
    ASSERT_STREQ(RESULT1, nodes[1].c_str());
    ASSERT_STREQ(RESULT2, nodes[2].c_str());
    ASSERT_STREQ(RESULT3, nodes[3].c_str());
    ASSERT_STREQ(RESULT4, nodes[4].c_str());
    ASSERT_STREQ(RESULT5, nodes[5].c_str());
}

