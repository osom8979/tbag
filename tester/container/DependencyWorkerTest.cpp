/**
 * @file   DependencyWorkerTest.cpp
 * @brief  DependencyWorker class tester.
 * @author zer0
 * @date   2016-08-31
 */

#include <gtest/gtest.h>
#include <libtbag/container/DependencyWorker.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(DependencyWorkerTest, Constructor)
{
    DependencyWorker<int> w0;

    DependencyWorker<int> w1(1);
    ASSERT_EQ(1, w1.getMaxRecursive());

    DependencyWorker<int> w2 = w1;
    ASSERT_EQ(1, w2.getMaxRecursive());

    DependencyWorker<int> temp1(2);
    ASSERT_EQ(2, temp1.getMaxRecursive());

    DependencyWorker<int> temp2 = std::move(temp1);
    ASSERT_EQ(2, temp2.getMaxRecursive());

    w1 = w0;
    ASSERT_EQ(w0.getMaxRecursive(), w1.getMaxRecursive());

    w2 = std::move(temp2);
    ASSERT_EQ(2, w2.getMaxRecursive());
}

TEST(DependencyWorkerTest, OnlyCoverage)
{
    DependencyWorker<int> worker;
    worker.empty();
    worker.size();
    worker.getMaxRecursive();
    worker.setMaxRecursive(10U);
    worker.clear();
}

TEST(DependencyWorkerTest, EndlessLoopGuard)
{
    DependencyWorker<int> worker;

    bool result1 = false;
    bool result2 = false;

    ASSERT_TRUE(worker.insert(1, DependencyWorker<int>::Dependencies{2}, [&](){ result1 = true; }));
    ASSERT_TRUE(worker.insert(2, DependencyWorker<int>::Dependencies{1}, [&](){ result2 = true; }));

    ASSERT_FALSE(worker.run(1));
    ASSERT_FALSE(worker.run(2));
    ASSERT_FALSE(result1);
    ASSERT_FALSE(result2);
}

TEST(DependencyWorkerTest, Default)
{
    DependencyWorker<int> worker;

    int index   = 0;
    int result1 = 0;
    int result2 = 0;
    int result3 = 0;
    int result4 = 0;

    ASSERT_TRUE(worker.insert(1, DependencyWorker<int>::Dependencies{3, 4}, [&](){ result1 = index++; }));
    ASSERT_TRUE(worker.insert(2, DependencyWorker<int>::Dependencies{3},    [&](){ result2 = index++; }));
    ASSERT_TRUE(worker.insert(3, DependencyWorker<int>::Dependencies(),     [&](){ result3 = index++; }));
    ASSERT_TRUE(worker.insert(4, DependencyWorker<int>::Dependencies{2},    [&](){ result4 = index++; }));

    ASSERT_TRUE(worker.run(1));
    ASSERT_EQ(3, result1);
    ASSERT_EQ(1, result2);
    ASSERT_EQ(0, result3);
    ASSERT_EQ(2, result4);

    ASSERT_TRUE(worker.run(2));
    ASSERT_TRUE(worker.run(3));
    ASSERT_TRUE(worker.run(4));
}

