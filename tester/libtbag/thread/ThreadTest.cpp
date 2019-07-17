/**
 * @file   ThreadTest.cpp
 * @brief  Thread class tester.
 * @author zer0
 * @date   2019-07-17
 */

#include <gtest/gtest.h>
#include <libtbag/thread/Thread.hpp>
#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::thread;

TEST(ThreadTest, Default)
{
    Thread t;
    ASSERT_EQ(E_SUCCESS, t.run());
    ASSERT_EQ(E_SUCCESS, t.waitForRunningOrDone(1000));
    ASSERT_EQ(E_SUCCESS, t.join());
}

