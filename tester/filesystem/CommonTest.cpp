/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>

// ---------------------
NAMESPACE_ANONYMOUS_OPEN
// ---------------------

constexpr bool IsPrintRepresentationDirectory() noexcept
{
    return true;
}

// ----------------------
NAMESPACE_ANONYMOUS_CLOSE
// ----------------------

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CommonStaticTest, GlobalPathOperators)
{
#if defined(WIN32) || defined(_WIN32)
    EXPECT_EQ(GetPathSeparator(), '\\');
    EXPECT_EQ(GetPathSplitter(),  ';');
#else
    EXPECT_EQ(GetPathSeparator(), '/');
    EXPECT_EQ(GetPathSplitter(),  ':');
#endif
}

TEST(CommonStaticTest, GetHomeEnvName)
{
#if defined(WIN32) || defined(_WIN32)
    EXPECT_STREQ(GetHomeEnvName(), "USERPROFILE");
#else
    EXPECT_STREQ(GetHomeEnvName(), "HOME");
#endif
}

TEST(CommonStaticTest, RepresentationDirectory)
{
    std::string work_dir = Common::getWorkDir();
    std::string home_dir = Common::getHomeDir();
    std::string exe_dir  = Common::getExeDir();

    if (::IsPrintRepresentationDirectory()) {
        using std::cout;
        using std::endl;
        cout << "Working directory: " << work_dir << endl
             << "Home directory: "    << home_dir << endl
             << "Execute directory: " << exe_dir  << endl;
    }

    ASSERT_GT(work_dir.size(), 0U);
    ASSERT_GT(home_dir.size(), 0U);
    ASSERT_GT(exe_dir.size(), 0U);
}

