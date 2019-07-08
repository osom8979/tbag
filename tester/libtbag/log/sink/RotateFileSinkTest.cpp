/**
 * @file   RotateFileSinkTest.cpp
 * @brief  RotateFileSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/sink/RotateFileSink.hpp>

#include <algorithm>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(RotateFileSinkTest, Default)
{
    char const * const TEST_FILENAME = "test";
    tttDir_Automatic();
    auto const DIR = tttDir_Get();
    auto const PREFIX = DIR / TEST_FILENAME;
    auto const ARGUMENTS = std::string("size=4 archive=.zip counter=") + PREFIX.getGenericString();

    RotateFileSink sink(ARGUMENTS);
    ASSERT_TRUE(sink.write("123", 3));
    auto files = DIR.scanNameOnly();
    ASSERT_EQ(1, files.size());
    ASSERT_STREQ("test0.log", files[0].c_str());

    ASSERT_TRUE(sink.write("4", 1));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(2, files.size());
    ASSERT_STREQ("test0.log.zip", files[0].c_str());
    ASSERT_STREQ("test1.log", files[1].c_str());

    ASSERT_TRUE(sink.write("1234", 4));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(3, files.size());
    ASSERT_STREQ("test0.log.zip", files[0].c_str());
    ASSERT_STREQ("test1.log.zip", files[1].c_str());
    ASSERT_STREQ("test2.log", files[2].c_str());

    ASSERT_TRUE(sink.write("12", 2));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(3, files.size());

    ASSERT_TRUE(sink.write("3", 1));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(3, files.size());

    ASSERT_TRUE(sink.write("4", 1));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(4, files.size());
    ASSERT_STREQ("test0.log.zip", files[0].c_str());
    ASSERT_STREQ("test1.log.zip", files[1].c_str());
    ASSERT_STREQ("test2.log.zip", files[2].c_str());
    ASSERT_STREQ("test3.log", files[3].c_str());

    ASSERT_TRUE(sink.write("12341234", 8));
    files = DIR.scanNameOnly();
    std::sort(files.begin(), files.end());
    ASSERT_EQ(6, files.size());
    ASSERT_STREQ("test0.log.zip", files[0].c_str());
    ASSERT_STREQ("test1.log.zip", files[1].c_str());
    ASSERT_STREQ("test2.log.zip", files[2].c_str());
    ASSERT_STREQ("test3.log.zip", files[3].c_str());
    ASSERT_STREQ("test4.log.zip", files[4].c_str());
    ASSERT_STREQ("test5.log", files[5].c_str());
}

