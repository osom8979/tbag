/**
 * @file   FsProhibitedTest.cpp
 * @brief  FsProhibited tester.
 * @author zer0
 * @date   2017-03-05
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsProhibited.hpp>

#include <limits>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsProhibitedTest, Default)
{
    ASSERT_TRUE(isProhibitedChar('/'));
    ASSERT_TRUE(isPathSeparatorChar('/'));
}

TEST(FsProhibitedTest, UnixProhibited)
{
    char min = std::numeric_limits<char>::min();
    char max = std::numeric_limits<char>::max();

    ASSERT_FALSE(unix::isProhibitedChar(min));
    ASSERT_FALSE(unix::isProhibitedChar(max));

    for (char i = min + 1; i < max; ++i) {
        if (i == '\0' || i == '/') {
            ASSERT_TRUE(unix::isProhibitedChar(i));
        } else {
            ASSERT_FALSE(unix::isProhibitedChar(i));
        }
    }
}

TEST(FsProhibitedTest, UnixPathSeparator)
{
    char min = std::numeric_limits<char>::min();
    char max = std::numeric_limits<char>::max();

    ASSERT_FALSE(unix::isPathSeparatorChar(min));
    ASSERT_FALSE(unix::isPathSeparatorChar(max));

    for (char i = min + 1; i < max; ++i) {
        if (i == '/') {
            ASSERT_TRUE(unix::isPathSeparatorChar(i));
        } else {
            ASSERT_FALSE(unix::isPathSeparatorChar(i));
        }
    }
}

