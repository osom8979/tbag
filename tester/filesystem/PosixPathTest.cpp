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
    char const * const TEMP  = "TEMP";
    char const * const TEMP1 = "/TEMP";
    char const * const TEMP2 = "TEMP/";
    char const * const TEMP3 = "TE/MP";

    ASSERT_FALSE(PosixPath::isProhibitedFilename(TEMP));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP1));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP2));
    ASSERT_TRUE(PosixPath::isProhibitedFilename(TEMP3));
}

