/**
 * @file   StFrameTest.cpp
 * @brief  StFrame class tester.
 * @author zer0
 * @date   2017-05-28
 */

#include <gtest/gtest.h>
#include <libtbag/debug/st/StFrame.hpp>
#include <sstream>

using namespace libtbag;
using namespace libtbag::debug;
using namespace libtbag::debug::st;

TEST(StFrameTest, Default)
{
    int test[2] = {0,};
    StFrame frame0;
    StFrame frame1(&test[0]);
    StFrame frame2(&test[1]);

    ASSERT_TRUE (frame1 <  frame2);
    ASSERT_FALSE(frame1 >  frame2);
    ASSERT_TRUE (frame1 <= frame2);
    ASSERT_FALSE(frame1 >= frame2);
    ASSERT_FALSE(frame1 == frame2);
    ASSERT_TRUE (frame1 != frame2);

    ASSERT_TRUE (frame0.empty());
    ASSERT_FALSE(frame1.empty());
    ASSERT_FALSE(frame2.empty());

    ASSERT_FALSE(frame0.address() != nullptr);
    ASSERT_TRUE (frame1.address() != nullptr);
    ASSERT_TRUE (frame2.address() != nullptr);

    ASSERT_FALSE(static_cast<bool>(frame0));
    ASSERT_TRUE (static_cast<bool>(frame1));
    ASSERT_TRUE (static_cast<bool>(frame2));

    ASSERT_TRUE (!frame0);
    ASSERT_FALSE(!frame1);
    ASSERT_FALSE(!frame2);

    std::stringstream ss;
    ss << frame0 << std::endl;
    ASSERT_FALSE(ss.str().empty());

    ss.clear();
    ss << frame1 << std::endl;
    ASSERT_FALSE(ss.str().empty());

    ss.clear();
    ss << frame2 << std::endl;
    ASSERT_FALSE(ss.str().empty());

    ASSERT_NE(frame1.toHash(), frame2.toHash());
    frame0 = frame1;
    ASSERT_TRUE(frame0 == frame1);

    frame0 = StFrame(&test[1]);
    ASSERT_TRUE(frame0 == frame2);
}

