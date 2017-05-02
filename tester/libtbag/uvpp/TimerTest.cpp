/**
 * @file   TimerTest.cpp
 * @brief  Timer libuv class tester.
 * @author zer0
 * @date   2017-02-24
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Timer.hpp>

#include <memory>
#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

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
    std::shared_ptr<TimerTest> timer;
    std::shared_ptr<Loop> loop;

    loop.reset(new Loop());
    timer = loop->newHandle<TimerTest>(*loop);

    timer->start(0, 1/*millisec*/);
    loop->run();
    ASSERT_EQ(1, timer->counter);
}

