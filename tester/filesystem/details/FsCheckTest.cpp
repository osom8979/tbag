/**
 * @file   FsCheckTest.cpp
 * @brief  FsCheck tester.
 * @author zer0
 * @date   2017-03-05
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsCheck.hpp>

#include <limits>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsCheckTest, Default)
{
    ASSERT_TRUE(isProhibitedChar('/'));
    ASSERT_TRUE(isPathSeparatorChar('/'));
}

TEST(FsCheckTest, UvProhibited)
{
    char min = std::numeric_limits<char>::min();
    char max = std::numeric_limits<char>::max();

    ASSERT_FALSE(uv::isProhibitedChar(min));
    ASSERT_FALSE(uv::isProhibitedChar(max));

    for (char i = min + 1; i < max; ++i) {
        if (i == '\0' || i == '/') {
            ASSERT_TRUE(uv::isProhibitedChar(i));
        } else {
            ASSERT_FALSE(uv::isProhibitedChar(i));
        }
    }
}

TEST(FsCheckTest, UvPathSeparator)
{
    char min = std::numeric_limits<char>::min();
    char max = std::numeric_limits<char>::max();

    ASSERT_FALSE(uv::isPathSeparatorChar(min));
    ASSERT_FALSE(uv::isPathSeparatorChar(max));

    for (char i = min + 1; i < max; ++i) {
        if (i == '/') {
            ASSERT_TRUE(uv::isPathSeparatorChar(i));
        } else {
            ASSERT_FALSE(uv::isPathSeparatorChar(i));
        }
    }
}

