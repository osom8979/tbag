/**
 * @file   TimePointTest.cpp
 * @brief  TimePoint class tester.
 * @author zer0
 * @date   2017-04-12
 */

#include <gtest/gtest.h>
#include <libtbag/time/TimePoint.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::time;

TEST(TimePointTest, Default)
{
    TimePoint tp;
    ASSERT_EQ(1970, tp.year());
    ASSERT_EQ(   1, tp.month());
    ASSERT_EQ(   1, tp.day());
    ASSERT_EQ(   0, tp.hours());
    ASSERT_EQ(   0, tp.minutes());
    ASSERT_EQ(   0, tp.seconds());
    ASSERT_EQ(   0, tp.millisec());
    ASSERT_EQ(   0, tp.microsec());
}

TEST(TimePointTest, Constructor)
{
    auto now  = std::chrono::system_clock::now();
    auto diff = std::chrono::microseconds(1);

    TimePoint::Rep  now_rep =  now.time_since_epoch().count();
    TimePoint::Rep diff_rep = diff.count();

    TimePoint tp1;
    TimePoint tp2(now);
    TimePoint tp3(now, diff);
    TimePoint tp4(now_rep);
    TimePoint tp5(now_rep, diff_rep);
    TimePoint tp6(1970, 1, 1, 0, 0, 0, 0, 0);

    TimePoint tp7 = tp1;
    TimePoint tp8 = std::move(TimePoint(1970, 1, 1, 0, 0, 0, 0, 0, 0));

    ASSERT_EQ(0, tp8.getMicrosecTimeSinceEpoch());
    ASSERT_EQ(tp1, tp7);
    ASSERT_EQ(tp2, tp3);
    ASSERT_EQ(tp4, tp5);
}

TEST(TimePointTest, Swap)
{
    TimePoint tp1 = TimePoint::now();
    auto g1 = tp1.getTimeSinceEpoch();
    auto l1 = tp1.getLocalTimeSinceEpoch();

    std::this_thread::sleep_for(std::chrono::microseconds(1));

    TimePoint tp2 = TimePoint::now();
    auto g2 = tp2.getTimeSinceEpoch();
    auto l2 = tp2.getLocalTimeSinceEpoch();

    ASSERT_NE(g1, g2);
    ASSERT_NE(l1, l2);

    std::swap(tp1, tp2);
    ASSERT_EQ(g1, tp2.getTimeSinceEpoch());
    ASSERT_EQ(l1, tp2.getLocalTimeSinceEpoch());
    ASSERT_EQ(g2, tp1.getTimeSinceEpoch());
    ASSERT_EQ(l2, tp1.getLocalTimeSinceEpoch());
}

TEST(TimePointTest, Operators)
{
    using namespace std::chrono;

    TimePoint tp1;
    TimePoint tp2(1970, 1, 1, 0, 0, 0, 0, 0);
    ASSERT_EQ(tp1, tp2);

    ASSERT_EQ(TimePoint::SystemDuration(0), tp1.getTimeSinceEpoch());
    ASSERT_EQ(TimePoint::SystemDuration(0), tp2.getTimeSinceEpoch());

    tp1 += TimePoint::SystemDuration(1);
    tp2 += TimePoint::SystemDuration(1);
    ASSERT_EQ(tp1, tp2);

    ASSERT_EQ(TimePoint::SystemDuration(1), tp1.getTimeSinceEpoch());
    ASSERT_EQ(TimePoint::SystemDuration(1), tp2.getTimeSinceEpoch());

    tp1 = tp1 - TimePoint::SystemDuration(1);
    tp2 = tp2 - TimePoint::SystemDuration(1);

    ASSERT_EQ(TimePoint::SystemDuration(0), tp1.getTimeSinceEpoch());
    ASSERT_EQ(TimePoint::SystemDuration(0), tp2.getTimeSinceEpoch());

    tp1 = TimePoint::SystemDuration(0);
    tp2 = TimePoint::SystemDuration(1);

    ASSERT_LT(tp1, tp2);
    ASSERT_LE(tp1, tp2);
    ASSERT_GT(tp2, tp1);
    ASSERT_GE(tp2, tp1);
    ASSERT_NE(tp1, tp2);
}

TEST(TimePointTest, DateTime)
{
    TimePoint tp1(2017, 4, 5, 3, 28, 27, 100, 99);
    ASSERT_EQ(2017, tp1.year());
    ASSERT_EQ(   4, tp1.month());
    ASSERT_EQ(   5, tp1.day());
    ASSERT_EQ(   3, tp1.hours());
    ASSERT_EQ(  28, tp1.minutes());
    ASSERT_EQ(  27, tp1.seconds());
    ASSERT_EQ( 100, tp1.millisec());
    ASSERT_EQ(  99, tp1.microsec());

    TimePoint tp3 = TimePoint::now();
    TimePoint tp4(tp3.year(), tp3.month(), tp3.day(), tp3.hours(), tp3.minutes(), tp3.seconds(), tp3.millisec(), tp3.microsec(), tp3.nanosec());

    std::cout << "TP3 Long String: " << tp3.toLongString() << std::endl;
    std::cout << "TP4 Long String: " << tp4.toLongString() << std::endl;
    ASSERT_EQ(tp3, tp4);
}

TEST(TimePointTest, Format)
{
    TimePoint tp(2017, 4, 5, 3, 28, 27, 100, 99);
    ASSERT_STREQ("2017-04-05T03:28:27.100,099,000", tp.fformat("$pY-$pM-$pDT$pH:$pI:$pS.$pL,$pC,$pN").c_str());
    ASSERT_STREQ("2017-4-5T3:28:27.100,99,0", tp.fformat("$Y-$M-$DT$H:$I:$S.$L,$C,$N").c_str());
}

TEST(TimePointTest, TimeBeforeEpoch)
{
    //TimePoint tp2(100, 1, 1, 0, 0, 1, 0, 1);
    //ASSERT_EQ(100, tp2.year());
    //ASSERT_EQ(  1, tp2.month());
    //ASSERT_EQ(  1, tp2.day());
    //ASSERT_EQ(  0, tp2.hours());
    //ASSERT_EQ(  0, tp2.minutes());
    //ASSERT_EQ(  1, tp2.seconds());
    //ASSERT_EQ(  0, tp2.millisec());
    //ASSERT_EQ(  1, tp2.microsec());
}

