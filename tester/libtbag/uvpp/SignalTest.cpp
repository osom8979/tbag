/**
 * @file   SignalTest.cpp
 * @brief  Signal class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Signal.hpp>
#include <libtbag/uvpp/Loop.hpp>
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
#else
    Loop loop;
    auto signal = loop.newHandle<SignalTest>(loop);
    signal->start(signal::SIGNAL_INTERRUPT);

    std::thread thread = std::thread([&loop](){
        loop.run();
    });

    signal::raise(signal::SIGNAL_INTERRUPT);
    thread.join();

    ASSERT_EQ(1, signal->counter);
    ASSERT_EQ(signal::SIGNAL_INTERRUPT, signal->last_signum);
#endif
}

