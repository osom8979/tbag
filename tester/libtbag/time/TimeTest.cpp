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
using namespace libtbag::time;

TEST(TimeTest, Ctime)
{
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

TEST(TimeTest, Chrono)
{
    int test_count = 200;
    for (int i = 0; i < test_count; ++i) {
        ASSERT_LE(   0, time::getMillisec(std::chrono::system_clock::now()));
        ASSERT_GT(1000, time::getMillisec(std::chrono::system_clock::now()));
    }
}

TEST(TimeTest, GetMillisec)
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

TEST(TimeTest, GetTime)
{
    long sec = 0;
    auto const CODE = time::getTimeOfDay(&sec);
    ASSERT_TRUE(isSuccess(CODE));
    ASSERT_NE(sec, 0);
}

TEST(TimeTest, SyncedWait)
{
    using namespace std::chrono;
    auto const result1 = syncedWait(system_clock::now(), seconds(1), milliseconds(1), []() -> bool {
        return true;
    });
    ASSERT_TRUE(result1);

    auto const result2 = syncedWait(system_clock::now(), seconds(1), milliseconds(1), []() -> bool {
        return false;
    });
    ASSERT_FALSE(result2);
}

TEST(TimeTest, SyncedWait2)
{
    auto const code1 = syncedWait(1, []() -> bool {
        return true;
    });
    ASSERT_EQ(E_SUCCESS, code1);
    auto const code2 = syncedWait(1, []() -> bool {
        return false;
    });
    ASSERT_EQ(E_TIMEOUT, code2);
}

