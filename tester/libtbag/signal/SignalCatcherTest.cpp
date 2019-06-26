/**
 * @file   SignalCatcherTest.cpp
 * @brief  SignalCatcher class tester.
 * @author zer0
 * @date   2019-06-26
 */

#include <gtest/gtest.h>
#include <libtbag/signal/SignalCatcher.hpp>

#include <thread>
#include <atomic>

using namespace libtbag;
using namespace libtbag::signal;

TEST(SignalCatcherTest, NotWorking)
{
    SignalCatcher catcher;
    ASSERT_FALSE(catcher.exists());
    catcher.done();
}

TEST(SignalCatcherTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Skip this test in Windows Platform.\n";
    return;
#endif

    SignalCatcher catcher;
    ASSERT_FALSE(catcher.exists());

    std::atomic_bool on_signal(false);
    auto const CODE = catcher.startInterrupt([&](){
        on_signal = true;
    });
    ASSERT_EQ(E_SUCCESS, CODE);
    ASSERT_TRUE(catcher.exists());
    ASSERT_EQ(E_SUCCESS, catcher.asyncRaise());

    int const TIMEOUT_COUNT = 100;
    int count = 0;

    while (!on_signal) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ++count;
        if (count >= TIMEOUT_COUNT) {
            std::cout << "Timeout count is max." << std::endl;
            break;
        }
    }
    catcher.done();

    ASSERT_TRUE(on_signal);
    ASSERT_FALSE(catcher.exists());
}

