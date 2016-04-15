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

TEST(PosixPathStaticTest, isProhibitedFilename)
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

TEST(PosixPathStaticTest, removeLastSeparator)
{
    char const * const TEMP0 = "/\\/TEMP";
    char const * const RESULT0 = "/\\/TEMP";
    ASSERT_STREQ(PosixPath::removeLastSeparator(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "/\\/TEMP/";
    char const * const RESULT1 = "/\\/TEMP";
    ASSERT_STREQ(PosixPath::removeLastSeparator(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "/\\/TEMP\\/";
    char const * const RESULT2 = "/\\/TEMP\\";
    ASSERT_STREQ(PosixPath::removeLastSeparator(TEMP2).c_str(), RESULT2);

    char const * const TEMP3 = "/\\/TEMP\\/\\";
    char const * const RESULT3 = "/\\/TEMP\\/\\";
    ASSERT_STREQ(PosixPath::removeLastSeparator(TEMP3).c_str(), RESULT3);
}

TEST(PosixPathStaticTest, makePreferred)
{
    char const * const TEMP0 = "/\\/TEMP\\/\\";
    char const * const RESULT0 = "/\\/TEMP\\/\\";
    ASSERT_STREQ(PosixPath::makePreferred(TEMP0).c_str(), RESULT0);
}

TEST(PosixPathStaticTest, removeDuplicateSeparators)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/\\/\\\\TEMP\\/\\/";
    ASSERT_STREQ(PosixPath::removeDuplicateSeparators(TEMP0).c_str(), RESULT0);
}

TEST(PosixPathStaticTest, getGeneric)
{
    char const * const TEMP0 = "/\\////\\\\TEMP\\//\\/";
    char const * const RESULT0 = "/\\/\\\\TEMP\\/\\";
    ASSERT_STREQ(PosixPath::getGeneric(TEMP0).c_str(), RESULT0);
}

TEST(PosixPathStaticTest, getRootDir)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(PosixPath::getRootDir(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "TEMP/";
    char const * const RESULT1 = "";
    ASSERT_STREQ(PosixPath::getRootDir(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(PosixPath::getRootDir(TEMP2).c_str(), RESULT2);
}

TEST(PosixPathStaticTest, QueryOperators)
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

TEST(PosixPathStaticTest, getParent_1)
{
    char const * const TEMP0 = "/TEMP";
    char const * const RESULT0 = "/";
    ASSERT_STREQ(PosixPath::getParent(TEMP0).c_str(), RESULT0);

    char const * const TEMP1 = "TEMP";
    char const * const RESULT1 = "";
    ASSERT_STREQ(PosixPath::getParent(TEMP1).c_str(), RESULT1);

    char const * const TEMP2 = "";
    char const * const RESULT2 = "";
    ASSERT_STREQ(PosixPath::getParent(TEMP2).c_str(), RESULT2);

    char const * const TEMP3 = "/TEMP/";
    char const * const RESULT3 = "/";
    ASSERT_STREQ(PosixPath::getParent(TEMP3).c_str(), RESULT3);

    char const * const TEMP4 = "/";
    char const * const RESULT4 = "";
    ASSERT_STREQ(PosixPath::getParent(TEMP4).c_str(), RESULT4);
}

TEST(PosixPathStaticTest, getParent_2)
{
    char const * const TEMP0 = "/TEMP\\//\\/.///..//TEST\\/";
    char const * const TEMP1 = "/TEMP\\//\\/.///..//";
    char const * const TEMP2 = "/TEMP\\//\\/.///";
    char const * const TEMP3 = "/TEMP\\//\\/";
    char const * const TEMP4 = "/TEMP\\//";
    char const * const TEMP5 = "/";
    char const * const TEMP6 = "";

    ASSERT_STREQ(PosixPath::getParent(TEMP0).c_str(), TEMP1);
    ASSERT_STREQ(PosixPath::getParent(TEMP1).c_str(), TEMP2);
    ASSERT_STREQ(PosixPath::getParent(TEMP2).c_str(), TEMP3);
    ASSERT_STREQ(PosixPath::getParent(TEMP3).c_str(), TEMP4);
    ASSERT_STREQ(PosixPath::getParent(TEMP4).c_str(), TEMP5);
    ASSERT_STREQ(PosixPath::getParent(TEMP5).c_str(), TEMP6);
}

TEST(PosixPathStaticTest, splitNodes_1)
{
    char const * const TEMP0 = "../TEMP\\//";
    char const * const RESULT0 = "..";
    char const * const RESULT1 = "TEMP\\";

    auto nodes = PosixPath::splitNodes(TEMP0);
    ASSERT_EQ(nodes.size(), 2U);
    ASSERT_STREQ(nodes[0].c_str(), RESULT0);
    ASSERT_STREQ(nodes[1].c_str(), RESULT1);
}

TEST(PosixPathStaticTest, splitNodes_2)
{
    char const * const TEMP0 = "/TEMP\\//\\/.///..//TEST\\/";
    char const * const RESULT0 = "/";
    char const * const RESULT1 = "TEMP\\";
    char const * const RESULT2 = "\\";
    char const * const RESULT3 = ".";
    char const * const RESULT4 = "..";
    char const * const RESULT5 = "TEST\\";

    auto nodes = PosixPath::splitNodes(TEMP0);
    ASSERT_EQ(nodes.size(), 6U);
    ASSERT_STREQ(nodes[0].c_str(), RESULT0);
    ASSERT_STREQ(nodes[1].c_str(), RESULT1);
    ASSERT_STREQ(nodes[2].c_str(), RESULT2);
    ASSERT_STREQ(nodes[3].c_str(), RESULT3);
    ASSERT_STREQ(nodes[4].c_str(), RESULT4);
    ASSERT_STREQ(nodes[5].c_str(), RESULT5);
}

