/**
 * @file   TimeTest.cpp
 * @brief  Time class tester.
 * @author zer0
 * @date   2016-04-18
 */

#include <gtest/gtest.h>
#include <libtbag/Time.hpp>

#include <sstream>
#include <iostream>

using namespace libtbag;

TEST(TimeTest, ctime)
{
    time_t time = Time::getCurrentTime();

    tm *   gmt = Time::getCurrentGmtTime(time);
    tm * local = Time::getCurrentLocalTime(time);

    std::string    gmt_long_format = Time::getFormatString(Time::TIMESTAMP_LONG_FORMAT,  gmt);
    std::string   gmt_short_format = Time::getFormatString(Time::TIMESTAMP_SHORT_FORMAT, gmt);
    std::string  local_long_format = Time::getFormatString(Time::TIMESTAMP_LONG_FORMAT,  local);
    std::string local_short_format = Time::getFormatString(Time::TIMESTAMP_SHORT_FORMAT, local);

    std::cout <<  "* Long format timestamp: " <<    gmt_long_format << std::endl;
    std::cout << "* Short format timestamp: " <<   gmt_short_format << std::endl;
    std::cout <<  "* Long format timestamp: " <<  local_long_format << std::endl;
    std::cout << "* Short format timestamp: " << local_short_format << std::endl;
}

TEST(TimeTest, chrono)
{
    int test_count = 200;
    for (int i = 0; i < test_count; ++i) {
        ASSERT_GE(Time::getMillisec(std::chrono::system_clock::now()), 0);
        ASSERT_LT(Time::getMillisec(std::chrono::system_clock::now()), 1000);
    }
}

TEST(TimeTest, Duration)
{
    std::stringstream ss;
    long millisec = getDuration([&ss](){
        Duration<std::stringstream, std::chrono::nanoseconds> duration(ss);
    });
    ASSERT_GE(millisec, 0);

    std::string result = ss.str();
    ASSERT_EQ(result.empty(), false);
    ASSERT_GE(atoi(ss.str().c_str()), 0);
}

