/**
 * @file   TimeTest.cpp
 * @brief  Time class tester.
 * @author zer0
 * @date   2016-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/time/Time.hpp>

#include <sstream>
#include <iostream>

using namespace libtbag;

TEST(TimeTest, ctime)
{
    /* TODO: Valgrind memory leak:
     * ==38325== 56 bytes in 1 blocks are definitely lost in loss record 49 of 107
     * ==38325==    at 0x100277EBB: malloc (in /usr/local/Cellar/valgrind/3.11.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
     * ==38325==    by 0x100696956: gmtime (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100157820: libtbag::time::getCurrentGmtTime(long const&) (in ./tester)
     * ==38325==    by 0x100154BC3: TimeTest_ctime_Test::TestBody() (in ./tester)
     * ==38325==    by 0x1001786C9: void testing::internal::HandleExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void (testing::Test::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x100165149: testing::Test::Run() (gtest.cc:2150)
     * ==38325==    by 0x100165E01: testing::TestInfo::Run() (gtest.cc:2326)
     * ==38325==    by 0x100166642: testing::TestCase::Run() (gtest.cc:2444)
     * ==38325==    by 0x10016D77A: testing::internal::UnitTestImpl::RunAllTests() (gtest.cc:4315)
     * ==38325==    by 0x100178EE2: bool testing::internal::HandleExceptionsInMethodIfSupported<testing::internal::UnitTestImpl, bool>(testing::internal::UnitTestImpl*, bool (testing::internal::UnitTestImpl::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x10016D3ED: testing::UnitTest::Run() (gtest.cc:3926)
     * ==38325==    by 0x10017E310: main (gtest.h:2288)
     *
     * ==38325== 56 bytes in 1 blocks are definitely lost in loss record 50 of 107
     * ==38325==    at 0x100277EBB: malloc (in /usr/local/Cellar/valgrind/3.11.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
     * ==38325==    by 0x10069680A: localtime (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100157860: libtbag::time::getCurrentLocalTime(long const&) (in ./tester)
     * ==38325==    by 0x100154BD6: TimeTest_ctime_Test::TestBody() (in ./tester)
     * ==38325==    by 0x1001786C9: void testing::internal::HandleExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void (testing::Test::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x100165149: testing::Test::Run() (gtest.cc:2150)
     * ==38325==    by 0x100165E01: testing::TestInfo::Run() (gtest.cc:2326)
     * ==38325==    by 0x100166642: testing::TestCase::Run() (gtest.cc:2444)
     * ==38325==    by 0x10016D77A: testing::internal::UnitTestImpl::RunAllTests() (gtest.cc:4315)
     * ==38325==    by 0x100178EE2: bool testing::internal::HandleExceptionsInMethodIfSupported<testing::internal::UnitTestImpl, bool>(testing::internal::UnitTestImpl*, bool (testing::internal::UnitTestImpl::*)(), char const*) (gtest.cc:2078)
     * ==38325==    by 0x10016D3ED: testing::UnitTest::Run() (gtest.cc:3926)
     * ==38325==    by 0x10017E310: main (gtest.h:2288)
     */

    std::chrono::system_clock::time_point tp = time::getNowSystemClock();
    time_t time = time::getTime(tp);
    int millisec = time::getMillisec(tp);

    tm   gmt = {0,};
    tm local = {0,};

    ASSERT_TRUE(time::getGmtTime(time, &gmt));
    ASSERT_TRUE(time::getLocalTime(time, &local));

    std::string    gmt_long_format = time::getFormatString(time::TIMESTAMP_LONG_FORMAT,  &gmt);
    std::string   gmt_short_format = time::getFormatString(time::TIMESTAMP_SHORT_FORMAT, &gmt);
    std::string  local_long_format = time::getFormatString(time::TIMESTAMP_LONG_FORMAT,  &local);
    std::string local_short_format = time::getFormatString(time::TIMESTAMP_SHORT_FORMAT, &local);

    std::cout <<  "* Long format timestamp: " <<    gmt_long_format << "," << millisec << std::endl;
    std::cout << "* Short format timestamp: " <<   gmt_short_format << "," << millisec << std::endl;
    std::cout <<  "* Long format timestamp: " <<  local_long_format << "," << millisec << std::endl;
    std::cout << "* Short format timestamp: " << local_short_format << "," << millisec << std::endl;
}

TEST(TimeTest, chrono)
{
    int test_count = 200;
    for (int i = 0; i < test_count; ++i) {
        ASSERT_LE(   0, time::getMillisec(std::chrono::system_clock::now()));
        ASSERT_GT(1000, time::getMillisec(std::chrono::system_clock::now()));
    }
}

TEST(TimeTest, getMillisec)
{
    std::chrono::system_clock::time_point tp;
    tp += std::chrono::hours(1);
    tp += std::chrono::minutes(2);
    tp += std::chrono::seconds(3);
    tp += std::chrono::milliseconds(4);

    ASSERT_EQ(4, time::getMillisec(tp));

    tp += std::chrono::milliseconds(1001);
    ASSERT_EQ(5, time::getMillisec(tp));
}

