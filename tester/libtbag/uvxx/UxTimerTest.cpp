/**
 * @file   UxTimerTest.cpp
 * @brief  UxTimer class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxTimer.hpp>
#include <libtbag/uvxx/UxCheck.hpp>
#include <libtbag/uvxx/UxPrepare.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxTimerTest, Default)
{
    int prepare_counter = 0;
    int prepare_close_counter = 0;

    int timer_counter = 0;
    int timer_close_counter = 0;

    int check_counter = 0;
    int check_close_counter = 0;

    UxPrepare prepare;
    ASSERT_FALSE(prepare.isInit());

    UxCheck check;
    ASSERT_FALSE(check.isInit());

    UxTimer timer;
    ASSERT_FALSE(timer.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(E_SUCCESS, prepare.init(loop));
    ASSERT_TRUE(prepare.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, timer.init(loop));
    ASSERT_TRUE(timer.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    ASSERT_EQ(E_SUCCESS, check.init(loop));
    ASSERT_TRUE(check.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(3, loop.size());

    prepare.setOnPrepare([&](){
        ++prepare_counter;
        prepare.close();
    });
    prepare.setOnClose([&](){
        ++prepare_close_counter;
    });

    timer.setOnTimer([&](){
        ++timer_counter;
        timer.close();
    });
    timer.setOnClose([&](){
        ++timer_close_counter;
    });

    check.setOnCheck([&](){
        ++check_counter;
        check.close();
    });
    check.setOnClose([&](){
        ++check_close_counter;
    });

    ASSERT_EQ(E_SUCCESS, timer.start(0));
    ASSERT_EQ(E_SUCCESS, check.start());
    ASSERT_EQ(E_SUCCESS, prepare.start());
    ASSERT_EQ(E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(1, timer_counter);
    ASSERT_EQ(1, timer_close_counter);
    ASSERT_EQ(1, check_counter);
    ASSERT_EQ(1, check_close_counter);
    ASSERT_EQ(1, prepare_counter);
    ASSERT_EQ(1, prepare_close_counter);
}

