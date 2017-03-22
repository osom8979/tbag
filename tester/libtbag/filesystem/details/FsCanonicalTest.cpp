/**
 * @file   FsCanonicalTest.cpp
 * @brief  FsCanonical class tester.
 * @author zer0
 * @date   2017-03-22
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/details/FsCanonical.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;
using namespace libtbag::filesystem::details;
using namespace libtbag::test_data;

TEST(FsCanonicalTest, RemoveLastSeparator)
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

TEST(FsCanonicalTest, RemoveDuplicateSeparators)
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

TEST(FsCanonicalTest, RemoveLastNode)
{
    std::string temp;
    std::string result_windows;
    std::string result_posix;

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = UTF8_SOURCE;
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "\\" + UTF8_SOURCE + "\\";
    result_windows = "\\";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "\\";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "/" + UTF8_SOURCE;
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "/" + UTF8_SOURCE + "/";
    result_windows = "/";
    result_posix   = "/";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));

    temp = "/";
    result_windows = "";
    result_posix   = "";
    ASSERT_EQ(result_windows, windows::removeLastNode(temp));
    ASSERT_EQ(result_posix  ,    unix::removeLastNode(temp));
}

TEST(FsCanonicalTest, RemoveLastNode_Windows)
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

TEST(FsCanonicalTest, RemoveLastNode_Unix)
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

