/**
 * @file   SignalHandlerTest.cpp
 * @brief  SignalHandler class tester.
 * @author zer0
 * @date   2016-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/signal/SignalHandler.hpp>
#include <mutex>

using namespace libtbag;
using namespace libtbag::signal;

static std::mutex g_test_signal_mutex;
volatile static std::sig_atomic_t g_test_signal;

struct SignalHandlerTest : public SignalHandler
{
    virtual void run(int signal) override
    {
        g_test_signal = signal;
    }
};

TEST(SignalHandlerTest, Default)
{
    registerHandler(SIGINT, -1, new SignalHandlerTest);
    std::raise(SIGINT);
    ASSERT_EQ(SIGINT, g_test_signal);
}

