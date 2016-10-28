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
    auto thread1 = group.createThread([&](){ while ((++t1) < TEST_NUMBER); });
    auto thread2 = group.createThread([&](){ while ((++t2) < TEST_NUMBER); });
    auto thread3 = group.createThread([&](){ while ((++t3) < TEST_NUMBER); });
    auto thread4 = group.createThread([&](){ while ((++t4) < TEST_NUMBER); });
    group.joinAll();

    ASSERT_TRUE(group.exists(thread1));
    ASSERT_TRUE(group.exists(thread2));
    ASSERT_TRUE(group.exists(thread3));
    ASSERT_TRUE(group.exists(thread4));

    ASSERT_EQ(TEST_NUMBER, t1);
    ASSERT_EQ(TEST_NUMBER, t2);
    ASSERT_EQ(TEST_NUMBER, t3);
    ASSERT_EQ(TEST_NUMBER, t4);
}

TEST(ThreadGroupTest, Add_And_Remove)
{
    int const TEST_NUMBER = 10000;
    int t1 = 0;

    thread::ThreadGroup group;
    std::thread * thread1 = new std::thread([&](){ while ((++t1) < TEST_NUMBER); });
    group.addThread(thread1);
    ASSERT_EQ(1U, group.size());

    group.removeThread(thread1);
    ASSERT_EQ(0U, group.size());

    thread1->join();
    delete thread1;
}

