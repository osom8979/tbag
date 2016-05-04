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

TEST(CommonTest, getPathSeparator)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ(GetPathSeparator(), '\\');
    ASSERT_EQ(GetPathSplitter(),  ';');
#else
    ASSERT_EQ(getPathSeparator(), '/');
    ASSERT_EQ(getPathSplitter(),  ':');
#endif
}

TEST(CommonTest, RepresentationDirectory)
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

TEST(CommonTest, scanDir)
{
    std::set<std::string> files = Common::scanDir(Common::getExeDir());
    for (auto cursor : files) {
        std::cout << "* Scan file: " << cursor << std::endl;
    }
    ASSERT_GE(files.size(), 1U);
}

TEST(CommonTest, create_and_remove)
{
    std::string dir_name = "__filesystem_common_dir_test.dir";
    ASSERT_TRUE(Common::createDir(dir_name));
    ASSERT_TRUE(Common::removeDir(dir_name));
}

