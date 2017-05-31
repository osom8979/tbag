/**
 * @file   TimerTest.cpp
 * @brief  Timer libuv class tester.
 * @author zer0
 * @date   2017-02-24
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalCheck.hpp>
#include <libtbag/uvpp/func/FunctionalPrepare.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

TEST(TimerTest, Default)
{
    int prepare_counter = 0;
    int prepare_close_counter = 0;

    int timer_counter = 0;
    int timer_close_counter = 0;

    int check_counter = 0;
    int check_close_counter = 0;

    Loop loop;
    auto prepare = loop.newHandle<FuncPrepare>(loop);
    auto timer   = loop.newHandle<FuncTimer>(loop);
    auto check   = loop.newHandle<FuncCheck>(loop);

    prepare->setOnPrepare([&](){
        ++prepare_counter;
        prepare->close();
    });
    prepare->setOnClose([&](){
        ++prepare_close_counter;
    });

    timer->setOnTimer([&](){
        ++timer_counter;
        timer->close();
    });
    timer->setOnClose([&](){
        ++timer_close_counter;
    });

    check->setOnCheck([&](){
        ++check_counter;
        check->close();
    });
    check->setOnClose([&](){
        ++check_close_counter;
    });

    ASSERT_EQ(Err::E_SUCCESS, timer->start(0));
    ASSERT_EQ(Err::E_SUCCESS, check->start());
    ASSERT_EQ(Err::E_SUCCESS, prepare->start());
    ASSERT_EQ(Err::E_SUCCESS, loop.run());

    ASSERT_EQ(1, timer_counter);
    ASSERT_EQ(1, timer_close_counter);
    ASSERT_EQ(1, check_counter);
    ASSERT_EQ(1, check_close_counter);
    ASSERT_EQ(1, prepare_counter);
    ASSERT_EQ(1, prepare_close_counter);
}

