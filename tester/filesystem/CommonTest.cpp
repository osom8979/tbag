/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CommonStaticTest, GlobalPathOperators)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
# else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
# endif
}

TEST(CommonStaticTest, GetHomeEnvName)
{
# if defined(WIN32) || defined(_WIN32)
    EXPECT_STREQ(GetHomeEnvName(), "USERPROFILE");
# else
    EXPECT_STREQ(GetHomeEnvName(), "HOME");
# endif
}

TEST(CommonStaticTest, RepresentationDirectory)
{
    ASSERT_GT(Common::getWorkDir().size(), 0U);
    ASSERT_GT(Common::getHomeDir().size(), 0U);
    ASSERT_GT(Common::getExeDir().size(), 0U);
}

