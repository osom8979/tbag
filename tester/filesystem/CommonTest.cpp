/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CommonStaticTest, GetPathSeparator)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ(GetPathSeparator(), '\\');
    ASSERT_EQ(GetPathSplitter(),  ';');
#else
    ASSERT_EQ(GetPathSeparator(), '/');
    ASSERT_EQ(GetPathSplitter(),  ':');
#endif
}

TEST(CommonStaticTest, RepresentationDirectory)
{
    using namespace std;

    std::string work_dir = Common::getWorkDir();
    std::string home_dir = Common::getHomeDir();
    std::string exe_dir  = Common::getExeDir();

    cout << "* Working directory: " << work_dir << endl
         << "* Home directory: "    << home_dir << endl
         << "* Execute directory: " << exe_dir  << endl;

    ASSERT_GT(work_dir.size(), 0U);
    ASSERT_GT(home_dir.size(), 0U);
    ASSERT_GT(exe_dir.size(), 0U);
}

TEST(CommonStaticTest, scanDir)
{
    using namespace std;

    std::set<std::string> files = Common::scanDir(".");

    for (auto cursor : files) {
        cout << "* File: " << cursor << endl;
    }

    ASSERT_GT(files.size(), 0U);
}
