/**
 * @file   ThreadGroupTest.cpp
 * @brief  ThreadGroup class tester.
 * @author zer0
 * @date   2016-05-12
 */

#include <gtest/gtest.h>
#include <libtbag/thread/ThreadGroup.hpp>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::thread;

TEST(ThreadGroupTest, Default)
{
    int const TEST_NUMBER = 10000;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;

    ThreadGroup group;
    auto thread1 = group.createThread([&](){ while ((++t1) < TEST_NUMBER); });
    auto thread2 = group.createThread([&](){ while ((++t2) < TEST_NUMBER); });
    auto thread3 = group.createThread([&](){ while ((++t3) < TEST_NUMBER); });
    auto thread4 = group.createThread([&](){ while ((++t4) < TEST_NUMBER); });
    group.joinAll();

    auto ids = group.ids();
    ASSERT_EQ(4, ids.size());
    bool const THREAD1_FIND = std::find(ids.begin(), ids.end(), thread1) != ids.end();
    bool const THREAD2_FIND = std::find(ids.begin(), ids.end(), thread2) != ids.end();
    bool const THREAD3_FIND = std::find(ids.begin(), ids.end(), thread3) != ids.end();
    bool const THREAD4_FIND = std::find(ids.begin(), ids.end(), thread4) != ids.end();
    ASSERT_TRUE(THREAD1_FIND);
    ASSERT_TRUE(THREAD2_FIND);
    ASSERT_TRUE(THREAD3_FIND);
    ASSERT_TRUE(THREAD4_FIND);

    ASSERT_FALSE(group.existsCurrentThread());
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

    ThreadGroup group;
    ASSERT_TRUE(group.empty());
    ASSERT_EQ(0, group.size());

    auto thread1 = group.createThread([&](){ while ((++t1) < TEST_NUMBER); });
    ASSERT_FALSE(group.empty());
    ASSERT_EQ(1, group.size());

    ASSERT_TRUE(group.erase(thread1));
    ASSERT_TRUE(group.empty());
    ASSERT_EQ(0, group.size());

    group.joinAll();
}

