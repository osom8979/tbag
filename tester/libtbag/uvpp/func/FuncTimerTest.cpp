/**
 * @file   FuncTimerTest.cpp
 * @brief  FuncTimer class tester.
 * @author zer0
 * @date   2017-05-30
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/func/FuncTimer.hpp>
#include <libtbag/uvpp/func/FuncCheck.hpp>
#include <libtbag/uvpp/func/FuncPrepare.hpp>
#include <libtbag/uvpp/Loop.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

TEST(FuncTimerTest, Default)
{
    int prepare_counter = 0;
    int prepare_close_counter = 0;

    int timer_counter = 0;
    int timer_close_counter = 0;

    int check_counter = 0;
    int check_close_counter = 0;

    using DefaultFuncPrepare = FuncPrepare<Prepare>;
    using DefaultFuncTimer   = FuncTimer<Timer>;
    using DefaultFuncCheck   = FuncCheck<Check>;

    Loop loop;
    auto prepare = loop.newHandle<DefaultFuncPrepare>(loop);
    auto timer   = loop.newHandle<DefaultFuncTimer>(loop);
    auto check   = loop.newHandle<DefaultFuncCheck>(loop);

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

