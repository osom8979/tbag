/**
 * @file   SignalHandlerTest.cpp
 * @brief  SignalHandler class tester.
 * @author zer0
 * @date   2016-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/signal/SignalHandler.hpp>

#include <csignal>
#include <mutex>

using namespace libtbag;
using namespace libtbag::signal;

volatile static std::sig_atomic_t g_test_signal;
volatile static std::sig_atomic_t g_test_signal2;
volatile static std::sig_atomic_t g_test_signal3;

struct SignalHandlerTest : public SignalHandler
{
    virtual void run(int signal) override
    { g_test_signal = signal; }
};

struct SignalHandlerTest2 : public SignalHandler
{
    virtual void run(int signal) override
    { g_test_signal2 = signal; }
};

struct SignalHandlerTest3 : public SignalHandler
{
    virtual void run(int signal) override
    { g_test_signal3 = signal; }
};

TEST(SignalHandlerTest, Default)
{
    g_test_signal  = 0;
    g_test_signal2 = 0;
    g_test_signal3 = 0;

    registerHandler(SIGINT,  0, new SignalHandlerTest);
    registerHandler(SIGTERM, 0, new SignalHandlerTest2);
    registerHandler(SIGINT,  0, new SignalHandlerTest3);

    signal::raise(SIGINT);
    ASSERT_EQ(SIGINT, g_test_signal);
    ASSERT_EQ(     0, g_test_signal2);
    ASSERT_EQ(SIGINT, g_test_signal3);
}

