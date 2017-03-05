/**
 * @file   FilesystemTest.cpp
 * @brief  Filesystem utilities tester.
 * @author zer0
 * @date   2016-04-08
 * @date   2016-12-13 (Rename: CommonTest -> FilesystemTest)
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <libtbag/filesystem/details/FsUtils.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsNode.hpp>
#include <libtbag/filesystem/Filesystem.hpp>

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
    std::string const TEST_DIR_NAME  = "__filesystem_common_test_default";
    std::string const TEST_FILE_NAME = "test";
    std::string const MOVE_FILE_NAME = "test_move";
    std::string const TEST_CONTENT   = "TEST";

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
    ASSERT_EQ(TEST_CONTENT.size(), createSimpleTextFile(TEST_FILE_PATH, &TEST_CONTENT[0], TEST_CONTENT.size()));

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

// Don't use u8 literal.
static std::string const UTF8_GA = "\xea\xb0\x80"; // "가"
static std::string const UTF8_NA = "\xeb\x82\x98"; // "나"
static std::string const UTF8_DA = "\xeb\x8b\xa4"; // "다"
static std::string const UTF8_SOURCE = UTF8_GA + UTF8_NA + UTF8_DA;

TEST(CommonTest, isProhibitedNameWithUtf8)
{
    ASSERT_FALSE(windows::isProhibitedNameWithUtf8(UTF8_SOURCE));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(uv::isProhibitedNameWithUtf8(UTF8_SOURCE));
    ASSERT_FALSE(uv::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_FALSE(uv::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_FALSE(uv::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( uv::isProhibitedNameWithUtf8(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( uv::isProhibitedNameWithUtf8(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( uv::isProhibitedNameWithUtf8(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(isProhibitedName(UTF8_SOURCE));
}

TEST(CommonTest, removeLastSeparatorWithUtf8)
{
    std::string const UTF8_SOURCE_WINDOWS = UTF8_SOURCE + PATH_SEPARATOR_OF_WINDOWS + PATH_SEPARATOR_OF_WINDOWS;
    std::string const UTF8_SOURCE_POSIX   = UTF8_SOURCE + PATH_SEPARATOR_OF_POSIX   + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_WINDOWS_POSIX = UTF8_SOURCE_WINDOWS + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_POSIX_WINDOWS = UTF8_SOURCE_POSIX + PATH_SEPARATOR_OF_WINDOWS;

    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , uv::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE              , uv::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , uv::removeLastSeparatorWithUtf8(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE_POSIX_WINDOWS, uv::removeLastSeparatorWithUtf8(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE, removeLastSeparator(UTF8_SOURCE_POSIX));
}

TEST(CommonTest, removeDuplicateSeparatorsWithUtf8)
{
    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE + "\\";
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\/";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE + "/";

    ASSERT_EQ(RESULT_WINDOWS, windows::removeDuplicateSeparatorsWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, uv::removeDuplicateSeparatorsWithUtf8(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::removeDuplicateSeparatorsWithGenericUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, uv::removeDuplicateSeparatorsWithGenericUtf8(TEMP));

    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparators(UTF8_SOURCE));
    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparatorsWithGeneric(UTF8_SOURCE));
}

TEST(CommonTest, getNativeWithUtf8)
{
    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE;
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE;

    ASSERT_EQ(RESULT_WINDOWS, windows::getNativeWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, uv::getNativeWithUtf8(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::getGenericWithUtf8(TEMP));
    ASSERT_EQ(RESULT_POSIX, uv::getGenericWithUtf8(TEMP));

    ASSERT_EQ(UTF8_SOURCE, getGeneric(UTF8_SOURCE));
}

TEST(CommonTest, getRootDirWithUtf8)
{
    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , uv::getRootDirWithUtf8(temp));
    ASSERT_FALSE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(uv::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(windows::isRelativeWithUtf8(temp));
    ASSERT_FALSE(uv::isRelativeWithUtf8(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , uv::getRootDirWithUtf8(temp));
    ASSERT_FALSE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(uv::isAbsoluteWithUtf8(temp));
    ASSERT_TRUE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(uv::isRelativeWithUtf8(temp));

    temp = "D:\\" + UTF8_SOURCE + "\\";
    result_windows = "D:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , uv::getRootDirWithUtf8(temp));
    ASSERT_TRUE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(uv::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(uv::isRelativeWithUtf8(temp));

    temp = "z:\\";
    result_windows = "z:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(result_posix  , uv::getRootDirWithUtf8(temp));
    ASSERT_TRUE(windows::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(uv::isAbsoluteWithUtf8(temp));
    ASSERT_FALSE(windows::isRelativeWithUtf8(temp));
    ASSERT_TRUE(uv::isRelativeWithUtf8(temp));

    temp = UTF8_SOURCE + "/";
    ASSERT_EQ(std::string(""), windows::getRootDirWithUtf8(temp));
    ASSERT_EQ(std::string(""), uv::getRootDirWithUtf8(temp));
    ASSERT_EQ(std::string(""), getRootDir(temp));
    ASSERT_FALSE(isAbsolute(temp));
    ASSERT_TRUE(isRelative(temp));
}

TEST(CommonTest, removeLastNodeWithUtf8)
{
    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = UTF8_SOURCE;
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "\\" + UTF8_SOURCE + "\\";
    result_windows = "\\";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "\\";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "/" + UTF8_SOURCE + "/";
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));

    temp = "/";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNodeWithUtf8(temp));
    ASSERT_EQ(result_posix  ,      uv::removeLastNodeWithUtf8(temp));
}

TEST(CommonTest, removeLastNodeWithUtf8_for_Windows)
{
    std::string const TEMP0 = "G:/" + UTF8_SOURCE + "\\//\\/.///..//TEST\\/";
    std::string const TEMP1 = "G:/" + UTF8_SOURCE + "\\//\\/.///..//";
    std::string const TEMP2 = "G:/" + UTF8_SOURCE + "\\//\\/.///";
    std::string const TEMP3 = "G:/" + UTF8_SOURCE + "\\//\\/";
    std::string const TEMP4 = "G:/";
    std::string const TEMP5 = "";

    ASSERT_EQ(TEMP1, windows::removeLastNodeWithUtf8(TEMP0));
    ASSERT_EQ(TEMP2, windows::removeLastNodeWithUtf8(TEMP1));
    ASSERT_EQ(TEMP3, windows::removeLastNodeWithUtf8(TEMP2));
    ASSERT_EQ(TEMP4, windows::removeLastNodeWithUtf8(TEMP3));
    ASSERT_EQ(TEMP5, windows::removeLastNodeWithUtf8(TEMP4));
}

TEST(CommonTest, removeLastNodeWithUtf8_for_Unix)
{
    std::string const TEMP0 = "/" + UTF8_SOURCE + "\\//\\/.///..//TEST\\/";
    std::string const TEMP1 = "/" + UTF8_SOURCE + "\\//\\/.///..//";
    std::string const TEMP2 = "/" + UTF8_SOURCE + "\\//\\/.///";
    std::string const TEMP3 = "/" + UTF8_SOURCE + "\\//\\/";
    std::string const TEMP4 = "/" + UTF8_SOURCE + "\\//";
    std::string const TEMP5 = "/";
    std::string const TEMP6 = "";

    ASSERT_EQ(TEMP1, uv::removeLastNodeWithUtf8(TEMP0));
    ASSERT_EQ(TEMP2, uv::removeLastNodeWithUtf8(TEMP1));
    ASSERT_EQ(TEMP3, uv::removeLastNodeWithUtf8(TEMP2));
    ASSERT_EQ(TEMP4, uv::removeLastNodeWithUtf8(TEMP3));
    ASSERT_EQ(TEMP5, uv::removeLastNodeWithUtf8(TEMP4));
    ASSERT_EQ(TEMP6, uv::removeLastNodeWithUtf8(TEMP5));
}

TEST(CommonTest, splitNodesWithUtf8)
{
    std::string const TEMP = "../" + UTF8_SOURCE + "//";
    std::string const RESULT0 = "..";
    std::string const RESULT1 = UTF8_SOURCE;

    auto nodes = splitNodes(TEMP);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
}

TEST(CommonTest, splitNodesWithUtf8_for_Windows)
{
    std::string const TEMP0   = "D:/" + UTF8_GA + "\\//\\/.///..//" + UTF8_NA + "\\/";
    std::string const RESULT0 = "D:";
    std::string const RESULT1 = UTF8_GA;
    std::string const RESULT2 = ".";
    std::string const RESULT3 = "..";
    std::string const RESULT4 = UTF8_NA;

    auto nodes = windows::splitNodesWithUtf8(TEMP0);
    ASSERT_EQ(5U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
    ASSERT_EQ(RESULT2, nodes[2]);
    ASSERT_EQ(RESULT3, nodes[3]);
    ASSERT_EQ(RESULT4, nodes[4]);
}

TEST(CommonTest, splitNodesWithUtf8_for_Unix)
{
    std::string const TEMP0 = "/" + UTF8_GA + "\\//\\/.///..//" + UTF8_NA + "\\/";
    std::string const RESULT0 = "/";
    std::string const RESULT1 = UTF8_GA + "\\";
    std::string const RESULT2 = "\\";
    std::string const RESULT3 = ".";
    std::string const RESULT4 = "..";
    std::string const RESULT5 = UTF8_NA + "\\";

    auto nodes = uv::splitNodesWithUtf8(TEMP0);
    ASSERT_EQ(6U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
    ASSERT_EQ(RESULT2, nodes[2]);
    ASSERT_EQ(RESULT3, nodes[3]);
    ASSERT_EQ(RESULT4, nodes[4]);
    ASSERT_EQ(RESULT5, nodes[5]);
}

