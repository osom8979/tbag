/**
 * @file   FsProhibitedTest.cpp
 * @brief  FsProhibited tester.
 * @author zer0
 * @date   2017-03-05
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>

#include <limits>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;
using namespace libtbag::test_data;

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

TEST(FsProhibitedTest, IsProhibitedName)
{
    ASSERT_FALSE(windows::isProhibitedName(UTF8_SOURCE));
    ASSERT_FALSE(unix::isProhibitedName(UTF8_SOURCE));
    ASSERT_FALSE(isProhibitedName(UTF8_SOURCE));
}

TEST(FsProhibitedTest, IsProhibitedName_Windows)
{
    ASSERT_TRUE(windows::isProhibitedName(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_TRUE(windows::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_TRUE(windows::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));

    ASSERT_TRUE(windows::isProhibitedName(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE(windows::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE(windows::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));
}

TEST(FsProhibitedTest, IsProhibitedName_Unix)
{
    ASSERT_FALSE(unix::isProhibitedName(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));

    ASSERT_TRUE(unix::isProhibitedName(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE(unix::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE(unix::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));
}

