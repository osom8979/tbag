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

TEST(FindPathTest, Paths)
{
    auto const merged_paths = mergePaths({"A", "B", "A", "C", "B", "D"});
    ASSERT_FALSE(merged_paths.empty());
    auto const divided_path = splitPaths(merged_paths);
    ASSERT_EQ(4, divided_path.size());
    ASSERT_STREQ("A", divided_path[0].c_str());
    ASSERT_STREQ("B", divided_path[1].c_str());
    ASSERT_STREQ("C", divided_path[2].c_str());
    ASSERT_STREQ("D", divided_path[3].c_str());
}

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
#if defined(SKIP_FINDPATH_TESTER)
    std::cout << "Skip this test. (The SKIP_FINDPATH_TESTER macro has been defined)" << std::endl;
    return;
#endif
    std::string exe_file;
    if (isWindowsPlatform()) {
        exe_file = "^cmd\\.exe$";
    } else {
        exe_file = "^bash$";
    }
    auto result = findUtf8ExecuteFile(exe_file); // [WARNING] Not found in Travis-CI.
    ASSERT_FALSE(result.empty());
}

TEST(FindPathTest, FindDefaultShell)
{
    auto path = findDefaultShell();
    ASSERT_FALSE(path.empty());
    std::cout << "Default shell: " << path << std::endl;
}

