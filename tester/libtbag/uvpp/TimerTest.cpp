/**
 * @file   TimerTest.cpp
 * @brief  Timer libuv class tester.
 * @author zer0
 * @date   2017-02-24
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Check.hpp>
#include <libtbag/uvpp/Prepare.hpp>

#include <memory>
#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

struct CheckTest : public Check
{
    int counter;

    CheckTest(Loop & loop) : Check(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onCheck() override
    {
        ++counter;
        stop();
        close();
    }
};

struct PrepareTest : public Prepare
{
    int counter;

    PrepareTest(Loop & loop) : Prepare(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onPrepare() override
    {
        ++counter;
        stop();
        close();
    }
};

struct TimerTest : public Timer
{
    int counter;

    TimerTest(Loop & loop) : Timer(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onTimer() override
    {
        ++counter;
        stop();
        close();
    }
};

TEST(TimerTest, Default)
{
    Loop loop;
    auto prepare = loop.newHandle<PrepareTest>(loop);
    auto timer   = loop.newHandle<TimerTest>(loop);
    auto check   = loop.newHandle<CheckTest>(loop);

    prepare->start();
    timer->start(0, 1/*millisec*/);
    check->start();

    loop.run();

    ASSERT_EQ(1, prepare->counter);
    ASSERT_EQ(1, timer->counter);
    ASSERT_EQ(1, check->counter);
}

