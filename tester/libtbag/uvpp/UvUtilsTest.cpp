/**
 * @file   UvUtilsTest.cpp
 * @brief  Test uvpp miscellaneous utilities.
 * @author zer0
 * @date   2017-05-27
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/UvUtils.hpp>

#include <iostream>
#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(UvUtilsTest, GetEnv)
{
    std::string path;
    ASSERT_EQ(E_SUCCESS, getEnv("PATH", path));
    ASSERT_FALSE(path.empty());
}

TEST(UvUtilsTest, GetEnv_For_Windows)
{
    if (!isWindowsPlatform()) {
        std::cout << "Skip this test. (windows only)" << std::endl;
        return;
    }

    std::string path;
    ASSERT_EQ(E_SUCCESS, getEnv("SystemDrive", path));
    ASSERT_FALSE(path.empty());

    path.clear();
    ASSERT_EQ(E_SUCCESS, getEnv("SystemRoot", path));
    ASSERT_FALSE(path.empty());

    path.clear();
    ASSERT_EQ(E_SUCCESS, getEnv("TEMP", path));
    ASSERT_FALSE(path.empty());

    path.clear();
    ASSERT_EQ(E_SUCCESS, getEnv("TMP", path));
    ASSERT_FALSE(path.empty());
}

