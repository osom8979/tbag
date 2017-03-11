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
    }
};

TEST(TimerTest, Default)
{
    Loop looper;
    std::shared_ptr<TimerTest> timer(new (std::nothrow) TimerTest(looper));
    timer->start(0, 1/*millisec*/);
    looper.run();

    ASSERT_EQ(1, timer->counter);
}

