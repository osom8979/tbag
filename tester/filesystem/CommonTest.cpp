/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>
#include <fstream>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CommonTest, GetPathSeparator)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ('\\', PATH_SEPARATOR);
    ASSERT_EQ(';', PATH_SPLITTER);
#else
    ASSERT_EQ('/', PATH_SEPARATOR);
    ASSERT_EQ(':', PATH_SPLITTER);
#endif
}

TEST(CommonTest, printInfos)
{
    printInfos();
}

TEST(CommonTest, Default)
{
    auto createTestFile = [](std::string const & path) -> bool {
        try {
            std::ofstream f(path);
            f << "TEST";
            f.close();
        } catch (...) {
            return false;
        }
        return true;
    };

    std::string const TEST_DIR_NAME  = "__filesystem_common_test_default";
    std::string const TEST_FILE_NAME = "test";
    std::string const MOVE_FILE_NAME = "test_move";

    std::string const TEST_DIR = getWorkDir() + PATH_SEPARATOR + TEST_DIR_NAME;
    std::string const TEST_FILE_PATH = TEST_DIR + PATH_SEPARATOR + TEST_FILE_NAME;
    std::string const MOVE_FILE_PATH = TEST_DIR + PATH_SEPARATOR + MOVE_FILE_NAME;

    {
        if (isRegularFile(TEST_FILE_PATH)) {
            removeFile(TEST_FILE_PATH);
        }
        if (isRegularFile(MOVE_FILE_PATH)) {
            removeFile(MOVE_FILE_PATH);
        }
        if (isDirectory(TEST_DIR)) {
            removeDirectory(TEST_DIR);
        }
    }

    ASSERT_FALSE(exists(TEST_DIR));
    ASSERT_FALSE(exists(TEST_FILE_PATH));
    ASSERT_FALSE(exists(MOVE_FILE_PATH));

    ASSERT_TRUE(createDirectory(TEST_DIR));
    ASSERT_TRUE(createTestFile(TEST_FILE_PATH));

    ASSERT_TRUE(exists(TEST_DIR));
    ASSERT_TRUE(exists(TEST_FILE_PATH));

    ASSERT_TRUE(isDirectory(TEST_DIR));
    ASSERT_TRUE(isRegularFile(TEST_FILE_PATH));

    ASSERT_TRUE(isExecutable(getExePath()));
    ASSERT_TRUE(isReadable(TEST_DIR));
    ASSERT_TRUE(isWritable(TEST_DIR));

    auto const SCAN_FILES = scanDir(TEST_DIR);
    ASSERT_EQ(1U, SCAN_FILES.size());
    ASSERT_EQ(TEST_FILE_NAME, SCAN_FILES[0]);

    ASSERT_TRUE(rename(TEST_FILE_PATH, MOVE_FILE_PATH));
    auto const SCAN_FILES2 = scanDir(TEST_DIR);
    ASSERT_EQ(1U, SCAN_FILES2.size());
    ASSERT_EQ(MOVE_FILE_NAME, SCAN_FILES2[0]);

    ASSERT_FALSE(isRegularFile(TEST_FILE_PATH));
    ASSERT_TRUE(isRegularFile(MOVE_FILE_PATH));

    ASSERT_TRUE(removeFile(MOVE_FILE_PATH));
    ASSERT_TRUE(removeDirectory(TEST_DIR));
}

