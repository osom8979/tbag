/**
 * @file   FsUtilsTest.cpp
 * @brief  FsUtils tester.
 * @author zer0
 * @date   2017-03-02
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsUtils.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;

TEST(FsUtilsTest, Default)
{
    std::string back_end;
    std::string temp;
    std::string work;
    std::string home;
    std::string exe;

    if (::libtbag::isWindowsPlatform()) {
        back_end = "Windows back-end";
        temp = getWindowsTempDir();
        work = getWindowsWorkDir();
        home = getWindowsHomeDir();
        exe  = getWindowsExePath();
    } else {
        back_end = "Uv back-end";
        temp = getUvTempDir();
        work = getUvWorkDir();
        home = getUvHomeDir();
        exe  = getUvExePath();
    }                

    std::cout << "FsUtils: " << back_end << " back-end.\n"
              << "TempDir: " << temp << std::endl
              << "WorkDir: " << work << std::endl
              << "HomeDir: " << home << std::endl
              << "ExePath: " << exe  << std::endl;
}


