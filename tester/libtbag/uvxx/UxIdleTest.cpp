/**
 * @file   UxIdleTest.cpp
 * @brief  UxIdle class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxIdle.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxIdleTest, Default)
{
    int idle_counter = 0;
    int close_counter = 0;

    UxIdle idle;
    ASSERT_FALSE(idle.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    idle.setOnIdle([&](){
        ++idle_counter;
        idle.close();
    });
    idle.setOnClose([&](){
        ++close_counter;
    });

    ASSERT_EQ(E_SUCCESS, idle.start());
    ASSERT_EQ(E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(1, idle_counter);
    ASSERT_EQ(1, close_counter);
}

