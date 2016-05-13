/**
 * @file   ThreadGroupTest.cpp
 * @brief  ThreadGroup class tester.
 * @author zer0
 * @date   2016-05-12
 */

#include <gtest/gtest.h>
#include <libtbag/thread/ThreadGroup.hpp>

using namespace libtbag;
using namespace libtbag::thread;

TEST(ThreadGroupTest, Default)
{
    int const TEST_NUMBER = 10000;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;

    thread::ThreadGroup group;
    group.createThread([&](){ while ((++t1) < TEST_NUMBER); });
    group.createThread([&](){ while ((++t2) < TEST_NUMBER); });
    group.createThread([&](){ while ((++t3) < TEST_NUMBER); });
    group.createThread([&](){ while ((++t4) < TEST_NUMBER); });
    group.joinAll();

    ASSERT_EQ(t1, TEST_NUMBER);
    ASSERT_EQ(t2, TEST_NUMBER);
    ASSERT_EQ(t3, TEST_NUMBER);
    ASSERT_EQ(t4, TEST_NUMBER);
}

