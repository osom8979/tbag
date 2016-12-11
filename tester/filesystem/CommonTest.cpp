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
using namespace libtbag::filesystem::details;

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

TEST(CommonTest, createDefaultTempDir)
{
    std::string const TEMP_DIR = createDefaultTempDir();
    std::cout << "Create default temp directory: " << TEMP_DIR << std::endl;
    ASSERT_TRUE(isDirectory(TEMP_DIR));
    ASSERT_TRUE(removeDirectory(TEMP_DIR));
    ASSERT_FALSE(isDirectory(TEMP_DIR));
}

TEST(CommonTest, isProhibitedNameWithUtf8)
{
    // Don't use u8 literal.
    std::string const UTF8_GA = "\xea\xb0\x80"; // 가
    std::string const UTF8_NA = "\xeb\x82\x98"; // 나
    std::string const UTF8_DA = "\xeb\x8b\xa4"; // 다
    std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

    ASSERT_FALSE(windows::isProhibitedNameWithUtf8(UTF8_SOURCE));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(unix::isProhibitedNameWithUtf8(UTF8_SOURCE));
    ASSERT_FALSE(unix::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( unix::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( unix::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( unix::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(isProhibitedNameWithUtf8(UTF8_SOURCE));
}

TEST(CommonTest, removeLastSeparatorWithUtf8)
{
    // Don't use u8 literal.
    std::string const UTF8_GA = "\xea\xb0\x80"; // 가
    std::string const UTF8_NA = "\xeb\x82\x98"; // 나
    std::string const UTF8_DA = "\xeb\x8b\xa4"; // 다
    std::string const UTF8_SOURCE         = UTF8_GA + UTF8_NA + UTF8_DA;
    std::string const UTF8_SOURCE_WINDOWS = UTF8_SOURCE + PATH_SEPARATOR_OF_WINDOWS + PATH_SEPARATOR_OF_WINDOWS;
    std::string const UTF8_SOURCE_POSIX   = UTF8_SOURCE + PATH_SEPARATOR_OF_POSIX   + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_WINDOWS_POSIX = UTF8_SOURCE_WINDOWS + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_POSIX_WINDOWS = UTF8_SOURCE_POSIX + PATH_SEPARATOR_OF_WINDOWS;

    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , unix::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE              , unix::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , unix::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE_POSIX_WINDOWS, unix::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE, removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX));
}

TEST(CommonTest, removeDuplicateSeparatorsWithUtf8)
{
    std::string const UTF8_GA = "\xea\xb0\x80"; // 가
    std::string const UTF8_NA = "\xeb\x82\x98"; // 나
    std::string const UTF8_DA = "\xeb\x8b\xa4"; // 다
    std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE + "\\";
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\/";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE + "/";

    ASSERT_EQ(RESULT_WINDOWS, windows::removeDuplicateSeparatorsWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::removeDuplicateSeparatorsWithUtf8(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::removeDuplicateSeparatorsWithGenericUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::removeDuplicateSeparatorsWithGenericUtf8(TEMP));

    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparatorsWithUtf8(UTF8_SOURCE));
    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparatorsWithGenericUtf8(UTF8_SOURCE));
}

TEST(CommonTest, getNativeWithUtf8)
{
    std::string const UTF8_GA = "\xea\xb0\x80"; // 가
    std::string const UTF8_NA = "\xeb\x82\x98"; // 나
    std::string const UTF8_DA = "\xeb\x8b\xa4"; // 다
    std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE;
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE;

    ASSERT_EQ(RESULT_WINDOWS, windows::getNativeWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::getNativeWithUtf8(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::getGenericWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::getGenericWithUtf8(TEMP));

    ASSERT_EQ(UTF8_SOURCE, getGenericWithUtf8(UTF8_SOURCE));
}

TEST(CommonTest, getRootDirWithUtf8)
{
    std::string const UTF8_GA = "\xea\xb0\x80"; // 가
    std::string const UTF8_NA = "\xeb\x82\x98"; // 나
    std::string const UTF8_DA = "\xeb\x8b\xa4"; // 다
    std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , unix::getRootDirWithUtf8(temp));
    ASSERT_FALSE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(unix::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(windows::isRelativeWithUtf8(temp));
    ASSERT_FALSE(unix::isRelativeWithUtf8(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , unix::getRootDirWithUtf8(temp));
    ASSERT_FALSE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(unix::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(unix::isRelativeWithUtf8(temp));

    temp = "D:\\" + UTF8_SOURCE + "\\";
    result_windows = "D:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , unix::getRootDirWithUtf8(temp));
    ASSERT_TRUE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(unix::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(unix::isRelativeWithUtf8(temp));

    temp = "z:\\";
    result_windows = "z:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , unix::getRootDirWithUtf8(temp));
    ASSERT_TRUE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(unix::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(unix::isRelativeWithUtf8(temp));

    temp = UTF8_SOURCE + "/";
    ASSERT_EQ(std::string(""), windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(std::string(""), unix::getRootDirWithUtf8(temp));
    ASSERT_EQ(std::string(""), getRootDirWithUtf8(temp));
    ASSERT_FALSE(isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(isRelativeWithUtf8(temp));
}

