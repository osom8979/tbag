/**
 * @file   FindPathTest.cpp
 * @brief  FindPath class tester.
 * @author zer0
 * @date   2017-08-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/FindPath.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(FindPathTest, FindUtf8File)
{
    auto image_dir = DemoAsset::get_tester_dir_image();
    auto result1 = findUtf8File({image_dir.toString()}, ".*\\.png");
    ASSERT_EQ(1, result1.size());
    ASSERT_EQ(std::string("lena.png"), result1[0].getName());

    auto result2 = findUtf8File({image_dir.toString()}, ".*\\.jpg");
    ASSERT_TRUE(result2.empty());
}

TEST(FindPathTest, FindUtf8ExecuteFile)
{
    std::string exe_file;
    if (isWindowsPlatform()) {
        exe_file = "^cmd\\.exe$";
    } else {
        exe_file = "^sh$";
    }
    auto result = findUtf8ExecuteFile(exe_file);
    ASSERT_FALSE(result.empty());
}

TEST(FindPathTest, FindDefaultShell)
{
    auto path = findDefaultShell();
    ASSERT_FALSE(path.empty());
    std::cout << "Default shell: " << path << std::endl;
}

