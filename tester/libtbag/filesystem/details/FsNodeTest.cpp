/**
 * @file   FsNodeTest.cpp
 * @brief  FsNode class tester.
 * @author zer0
 * @date   2017-03-22
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsNode.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;
using namespace libtbag::test_data;

TEST(FsNodeTest, GetNative)
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

TEST(FsNodeTest, GetRootDir)
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

TEST(FsNodeTest, SplitNodes)
{
    std::string const TEMP = "../" + UTF8_SOURCE + "//";
    std::string const RESULT0 = "..";
    std::string const RESULT1 = UTF8_SOURCE;

    auto nodes = splitNodes(TEMP);
    ASSERT_EQ(2U, nodes.size());
    ASSERT_EQ(RESULT0, nodes[0]);
    ASSERT_EQ(RESULT1, nodes[1]);
}

TEST(FsNodeTest, SplitNodes_Windows)
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

TEST(FsNodeTest, SplitNodes_Unix)
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

