/**
 * @file   FsUtilsTest.cpp
 * @brief  FsUtils tester.
 * @author zer0
 * @date   2017-03-02
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsUtils.hpp>

#include <string>
#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsUtilsTest, Default)
{
    std::cout << "TempDir: " << getTempDir() << std::endl
              << "WorkDir: " << getWorkDir() << std::endl
              << "HomeDir: " << getHomeDir() << std::endl
              << "ExePath: " << getExePath() << std::endl;
}


