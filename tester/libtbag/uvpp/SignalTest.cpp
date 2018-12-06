/**
 * @file   SignalTest.cpp
 * @brief  Signal class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Signal.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::uvpp;

struct SignalTest : public Signal
{
    int counter;
    int last_signum;

    SignalTest(Loop & loop) : Signal(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onSignal(int signum) override
    {
        ++counter;
        last_signum = signum;

        stop();
        close();
    }
};

TEST(SignalTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Skip this test in Windows Platform.\n";
    return;
#endif

    Loop loop;
    auto signal = loop.newHandle<SignalTest>(loop);
    ASSERT_EQ(Err::E_SUCCESS, signal->start(libtbag::signal::TBAG_SIGNAL_INTERRUPT));

    auto idle = loop.newHandle<libtbag::uvpp::func::FuncIdle>(loop);
    idle->idle_cb = [&](){
        libtbag::signal::raise(libtbag::signal::TBAG_SIGNAL_INTERRUPT);
        idle->close();
    };
    ASSERT_EQ(Err::E_SUCCESS, idle->start());

    Err loop_result = Err::E_UNKNOWN;
    auto thread = std::thread([&](){
        loop_result = loop.run();
    });

    thread.join();
    ASSERT_EQ(Err::E_SUCCESS, loop_result);
    ASSERT_EQ(1, signal->counter);
    ASSERT_EQ(libtbag::signal::TBAG_SIGNAL_INTERRUPT, signal->last_signum);
}

