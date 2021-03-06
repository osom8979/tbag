/**
 * @file   FileTest.cpp
 * @brief  File class tester.
 * @author zer0
 * @date   2017-07-30
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(FileTest, Default)
{
    tttDir_Automatic();
    auto path = tttDir_Get() / "test_file";
    std::string const WRITE_CONTENT = "TEST_CONTENT";

    ASSERT_FALSE(path.exists());
    ASSERT_EQ(E_SUCCESS, writeFile(path, WRITE_CONTENT));
    ASSERT_TRUE(path.exists());

    std::string read_buffer;
    ASSERT_EQ(E_SUCCESS, readFile(path, read_buffer));
    ASSERT_EQ(WRITE_CONTENT, read_buffer);
}

