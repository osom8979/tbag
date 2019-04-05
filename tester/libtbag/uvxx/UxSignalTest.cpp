/**
 * @file   UxSignalTest.cpp
 * @brief  UxSignal class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxSignal.hpp>
#include <libtbag/uvxx/UxIdle.hpp>

#include <libtbag/signal/SignalHandler.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxSignalTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Skip this test in Windows Platform.\n";
    return;
#endif

    UxSignal signal;
    ASSERT_FALSE(signal.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(E_SUCCESS, signal.init(loop));
    ASSERT_TRUE(signal.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, signal.init(loop));
    ASSERT_TRUE(signal.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    UxIdle idle;
    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(3, loop.size());

    idle.setOnIdle([&](){
        libtbag::signal::raise(libtbag::signal::TBAG_SIGNAL_INTERRUPT);
        idle.close();
    });
    ASSERT_EQ(E_SUCCESS, idle.start());

    int signal_counter = 0;
    int close_counter = 0;
    int signal_number = 0;

    signal.setOnSignal([&](int signum){
        ++signal_counter;
        signal_number = signum;
        signal.close();
    });
    signal.setOnClose([&](){
        ++close_counter;
    });

    ASSERT_EQ(E_SUCCESS, signal.start(libtbag::signal::TBAG_SIGNAL_INTERRUPT));

    Err loop_result = E_UNKNOWN;
    auto thread = std::thread([&](){
        loop_result = loop.run();
    });

    thread.join();
    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(E_SUCCESS, loop_result);
    ASSERT_EQ(1, signal_counter);
    ASSERT_EQ(1, close_counter);
    ASSERT_EQ(libtbag::signal::TBAG_SIGNAL_INTERRUPT, signal_number);
}

