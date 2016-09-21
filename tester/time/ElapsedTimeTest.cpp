/**
 * @file   ElapsedTimeTest.cpp
 * @brief  ElapsedTime class tester.
 * @author zer0
 * @date   2016-09-21
 */

#include <gtest/gtest.h>
#include <libtbag/time/ElapsedTime.hpp>

#include <thread>
#include <sstream>

using namespace libtbag;
using namespace libtbag::time;

TEST(ElapsedTimeTest, Default)
{
    std::chrono::milliseconds const WAIT_TIME(1);
    std::stringstream ss;
    {
        ElapsedTime<std::stringstream> elapsed(ss);
        std::this_thread::sleep_for(WAIT_TIME);
    }

    ASSERT_FALSE(ss.str().empty());
    ASSERT_LE(WAIT_TIME.count(), atoi(ss.str().c_str()));
}

TEST(ElapsedTimeTest, getDuration)
{
    auto start = std::chrono::system_clock::now();
    std::chrono::milliseconds const WAIT_TIME(1);
    std::this_thread::sleep_for(WAIT_TIME);
    auto result = time::getDuration<std::chrono::milliseconds>(start);

    ASSERT_LE(WAIT_TIME.count(), result.count());
}

TEST(ElapsedTimeTest, getDurationWithPredicated)
{
    std::chrono::milliseconds const WAIT_TIME(1);
    bool check_point = false;

    auto result = getDurationWithPredicated<std::chrono::milliseconds>([&](){
        std::this_thread::sleep_for(WAIT_TIME);
        check_point = true;
    });

    ASSERT_TRUE(check_point);
    ASSERT_LE(WAIT_TIME.count(), result.count());
}

