/**
 * @file   FilesystemTest.cpp
 * @brief  Filesystem utilities tester.
 * @author zer0
 * @date   2016-04-08
 * @date   2016-12-13 (Rename: CommonTest -> FilesystemTest)
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <libtbag/filesystem/details/FsCanonical.hpp>
#include <libtbag/filesystem/details/FsNode.hpp>
#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>

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
    //printInfos();
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
#if 0
    // TODO: Refactoring this code:
    ASSERT_EQ(TEST_CONTENT.size(), createSimpleTextFile(TEST_FILE_PATH, &TEST_CONTENT[0], TEST_CONTENT.size()));
#endif

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

TEST(CommonTest, isProhibitedName)
{
    ASSERT_FALSE(windows::isProhibitedName(UTF8_SOURCE));
    ASSERT_TRUE( windows::isProhibitedName(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( windows::isProhibitedName(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( windows::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(unix::isProhibitedName(UTF8_SOURCE));
    ASSERT_FALSE(unix::isProhibitedName(PATH_SEPARATOR_OF_WINDOWS + UTF8_NA + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_WINDOWS + UTF8_DA));
    ASSERT_FALSE(unix::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_WINDOWS));
    ASSERT_TRUE( unix::isProhibitedName(PATH_SEPARATOR_OF_POSIX + UTF8_NA + UTF8_DA));
    ASSERT_TRUE( unix::isProhibitedName(UTF8_GA + PATH_SEPARATOR_OF_POSIX + UTF8_DA));
    ASSERT_TRUE( unix::isProhibitedName(UTF8_GA + UTF8_NA + PATH_SEPARATOR_OF_POSIX));

    ASSERT_FALSE(isProhibitedName(UTF8_SOURCE));
}

TEST(CommonTest, removeLastSeparator)
{
    std::string const UTF8_SOURCE_WINDOWS = UTF8_SOURCE + PATH_SEPARATOR_OF_WINDOWS + PATH_SEPARATOR_OF_WINDOWS;
    std::string const UTF8_SOURCE_POSIX   = UTF8_SOURCE + PATH_SEPARATOR_OF_POSIX   + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_WINDOWS_POSIX = UTF8_SOURCE_WINDOWS + PATH_SEPARATOR_OF_POSIX;
    std::string const UTF8_SOURCE_POSIX_WINDOWS = UTF8_SOURCE_POSIX + PATH_SEPARATOR_OF_WINDOWS;

    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparator(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparator(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparator(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE, windows::removeLastSeparator(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , unix::removeLastSeparator(UTF8_SOURCE_WINDOWS));
    ASSERT_EQ(UTF8_SOURCE              , unix::removeLastSeparator(UTF8_SOURCE_POSIX));
    ASSERT_EQ(UTF8_SOURCE_WINDOWS      , unix::removeLastSeparator(UTF8_SOURCE_WINDOWS_POSIX));
    ASSERT_EQ(UTF8_SOURCE_POSIX_WINDOWS, unix::removeLastSeparator(UTF8_SOURCE_POSIX_WINDOWS));

    ASSERT_EQ(UTF8_SOURCE, removeLastSeparator(UTF8_SOURCE_POSIX));
}

TEST(CommonTest, removeDuplicateSeparators)
{
    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE + "\\";
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\/";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE + "/";

    ASSERT_EQ(RESULT_WINDOWS, windows::removeDuplicateSeparators(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::removeDuplicateSeparators(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::removeDuplicateSeparatorsWithGeneric(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::removeDuplicateSeparatorsWithGeneric(TEMP));

    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparators(UTF8_SOURCE));
    ASSERT_EQ(UTF8_SOURCE, removeDuplicateSeparatorsWithGeneric(UTF8_SOURCE));
}

TEST(CommonTest, getNative)
{
    std::string const TEMP = "/\\////\\\\" + UTF8_SOURCE + "\\//\\/";
    std::string const RESULT_WINDOWS = "\\" + UTF8_SOURCE;
    std::string const RESULT_POSIX   = "/\\/\\\\" + UTF8_SOURCE + "\\/\\";
    std::string const RESULT_GENERIC = "/" + UTF8_SOURCE;

    ASSERT_EQ(RESULT_WINDOWS, windows::getNative(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::getNative(TEMP));

    ASSERT_EQ(RESULT_GENERIC, windows::getGeneric(TEMP));
    ASSERT_EQ(RESULT_POSIX, unix::getGeneric(TEMP));

    ASSERT_EQ(UTF8_SOURCE, getGeneric(UTF8_SOURCE));
}

TEST(CommonTest, getRootDir)
{
    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::getRootDir(temp));
    ASSERT_EQ(result_posix  , unix::getRootDir(temp));
    ASSERT_FALSE(windows::isAbsolute(temp));
    ASSERT_TRUE(unix::isAbsolute(temp));
    ASSERT_TRUE(windows::isRelative(temp));
    ASSERT_FALSE(unix::isRelative(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDir(temp));
    ASSERT_EQ(result_posix  , unix::getRootDir(temp));
    ASSERT_FALSE(windows::isAbsolute(temp));
    ASSERT_FALSE(unix::isAbsolute(temp));
    ASSERT_TRUE(windows::isRelative(temp));
    ASSERT_TRUE(unix::isRelative(temp));

    temp = "D:\\" + UTF8_SOURCE + "\\";
    result_windows = "D:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDir(temp));
    ASSERT_EQ(result_posix  , unix::getRootDir(temp));
    ASSERT_TRUE(windows::isAbsolute(temp));
    ASSERT_FALSE(unix::isAbsolute(temp));
    ASSERT_FALSE(windows::isRelative(temp));
    ASSERT_TRUE(unix::isRelative(temp));

    temp = "z:\\";
    result_windows = "z:";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::getRootDir(temp));
    ASSERT_EQ(result_posix  , unix::getRootDir(temp));
    ASSERT_TRUE(windows::isAbsolute(temp));
    ASSERT_FALSE(unix::isAbsolute(temp));
    ASSERT_FALSE(windows::isRelative(temp));
    ASSERT_TRUE(unix::isRelative(temp));

    temp = UTF8_SOURCE + "/";
    ASSERT_EQ(std::string(""), windows::getRootDir(temp));
    ASSERT_EQ(std::string(""), unix::getRootDir(temp));
    ASSERT_EQ(std::string(""), getRootDir(temp));
    ASSERT_FALSE(isAbsolute(temp));
    ASSERT_TRUE(isRelative(temp));
}

TEST(CommonTest, removeLastNode)
{
    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = UTF8_SOURCE;
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "\\" + UTF8_SOURCE + "\\";
    result_windows = "\\";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "\\";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "/" + UTF8_SOURCE + "/";
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));

    temp = "/";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,      unix::removeLastNode(temp));
}

TEST(CommonTest, removeLastNode_for_Windows)
{
    std::string const TEMP0 = "G:/" + UTF8_SOURCE + "\\//\\/.///..//TEST\\/";
    std::string const TEMP1 = "G:/" + UTF8_SOURCE + "\\//\\/.///..//";
    std::string const TEMP2 = "G:/" + UTF8_SOURCE + "\\//\\/.///";
    std::string const TEMP3 = "G:/" + UTF8_SOURCE + "\\//\\/";
    std::string const TEMP4 = "G:/";
    std::string const TEMP5 = "";

    ASSERT_EQ(TEMP1, windows::removeLastNode(TEMP0));
    ASSERT_EQ(TEMP2, windows::removeLastNode(TEMP1));
    ASSERT_EQ(TEMP3, windows::removeLastNode(TEMP2));
    ASSERT_EQ(TEMP4, windows::removeLastNode(TEMP3));
    ASSERT_EQ(TEMP5, windows::removeLastNode(TEMP4));
}

TEST(CommonTest, removeLastNode_for_Unix)
{
    std::string const TEMP0 = "/" + UTF8_SOURCE + "\\//\\/.///..//TEST\\/";
    std::string const TEMP1 = "/" + UTF8_SOURCE + "\\//\\/.///..//";
    std::string const TEMP2 = "/" + UTF8_SOURCE + "\\//\\/.///";
    std::string const TEMP3 = "/" + UTF8_SOURCE + "\\//\\/";
    std::string const TEMP4 = "/" + UTF8_SOURCE + "\\//";
    std::string const TEMP5 = "/";
    std::string const TEMP6 = "";

    ASSERT_EQ(TEMP1, unix::removeLastNode(TEMP0));
    ASSERT_EQ(TEMP2, unix::removeLastNode(TEMP1));
    ASSERT_EQ(TEMP3, unix::removeLastNode(TEMP2));
    ASSERT_EQ(TEMP4, unix::removeLastNode(TEMP3));
    ASSERT_EQ(TEMP5, unix::removeLastNode(TEMP4));
    ASSERT_EQ(TEMP6, unix::removeLastNode(TEMP5));
}

TEST(CommonTest, splitNodes)
{
    std::string const TEMP = "../" + UTF8_SOURCE + "//";
    std::string const RESULT0 = "..";
    std::string const RESULT1 = UTF8_SOURCE;

    auto nodes = splitNodes(TEMP);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
}

TEST(CommonTest, splitNodes_for_Windows)
{
    std::string const TEMP0   = "D:/" + UTF8_GA + "\\//\\/.///..//" + UTF8_NA + "\\/";
    std::string const RESULT0 = "D:";
    std::string const RESULT1 = UTF8_GA;
    std::string const RESULT2 = ".";
    std::string const RESULT3 = "..";
    std::string const RESULT4 = UTF8_NA;

    auto nodes = windows::splitNodes(TEMP0);
    ASSERT_EQ(5U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
    ASSERT_EQ(RESULT2, nodes[2]);
    ASSERT_EQ(RESULT3, nodes[3]);
    ASSERT_EQ(RESULT4, nodes[4]);
}

TEST(CommonTest, splitNodes_for_Unix)
{
    std::string const TEMP0 = "/" + UTF8_GA + "\\//\\/.///..//" + UTF8_NA + "\\/";
    std::string const RESULT0 = "/";
    std::string const RESULT1 = UTF8_GA + "\\";
    std::string const RESULT2 = "\\";
    std::string const RESULT3 = ".";
    std::string const RESULT4 = "..";
    std::string const RESULT5 = UTF8_NA + "\\";

    auto nodes = unix::splitNodes(TEMP0);
    ASSERT_EQ(6U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
    ASSERT_EQ(RESULT2, nodes[2]);
    ASSERT_EQ(RESULT3, nodes[3]);
    ASSERT_EQ(RESULT4, nodes[4]);
    ASSERT_EQ(RESULT5, nodes[5]);
}

