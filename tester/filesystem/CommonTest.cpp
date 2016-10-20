/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>
#include <libtbag/debug/UvError.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::common;

TEST(CommonTest, getPathSeparator)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ('\\', getPathSeparator());
    ASSERT_EQ(';', getPathSplitter());
#else
    ASSERT_EQ('/', getPathSeparator());
    ASSERT_EQ(':', getPathSplitter());
#endif
}

TEST(CommonTest, RepresentationDirectory)
{
    using namespace std;

    std::string work_dir = getWorkDir();
    std::string home_dir = getHomeDir();
    std::string exe_dir  = getExeDir();

    cout << "* Working directory: " << work_dir << endl
         << "* Home directory: "    << home_dir << endl
         << "* Execute directory: " << exe_dir  << endl;

    ASSERT_LT(0U, work_dir.size());
    ASSERT_LT(0U, home_dir.size());
    ASSERT_LT(0U,  exe_dir.size());
}

TEST(CommonTest, scanDir)
{
    std::set<std::string> files = scanDir(getExeDir());
    for (auto cursor : files) {
        std::cout << "* Scan file: " << cursor << std::endl;
    }
    ASSERT_LE(1U, files.size());
}

TEST(CommonTest, create_and_remove)
{
    std::string dir_name = "__filesystem_common_dir_test.dir";
    ASSERT_TRUE(createDir(dir_name));
    ASSERT_TRUE(removeDir(dir_name));
}

TEST(CommonTest, file_rw)
{
    std::string file_name = "__filesystem_common_file_write_test";

    std::string const TEST = "TEST";
    int const PERMISSION = 0644;
    std::vector<char> read_buffer;
    read_buffer.resize(TEST.size(), 0x00);

    int fd = common::open(file_name, (FILE_OPEN_FLAG_READ_WRITE | FILE_OPEN_CREATE), PERMISSION);
    ASSERT_LT(0, fd);
    ASSERT_TRUE(common::isExistsMode(file_name));
    ASSERT_EQ(getFixedPermission(PERMISSION), common::getPermission(file_name));

    ASSERT_EQ(TEST.size(), common::write(fd, TEST.data(), TEST.size()));
    ASSERT_EQ(read_buffer.size(), common::read(fd, read_buffer.data(), read_buffer.size(), 0));

    std::string buffer_to_string;
    buffer_to_string.assign(read_buffer.begin(), read_buffer.end());
    ASSERT_EQ(TEST, buffer_to_string);
    ASSERT_TRUE(common::remove(file_name));
    ASSERT_FALSE(common::isExistsMode(file_name));
}

