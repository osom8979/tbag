/**
 * @file   SafetyWorkingQueueTest.cpp
 * @brief  SafetyWorkingQueue class tester.
 * @author zer0
 * @date   2016-07-26
 */

#include <gtest/gtest.h>
#include <libtbag/container/SafetyWorkingQueue.hpp>

#include <thread>
#include <atomic>

using namespace libtbag;
using namespace libtbag::container;

TEST(SafetyWorkingQueueTest, Default)
{
    int const TEST_VALUE = 100;
    SafetyWorkingQueue<int> queue;

    queue.push(TEST_VALUE);
    ASSERT_EQ(queue.sizeOfInsertQueue(), 1U);
    ASSERT_EQ(queue.getWorkingCount(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    std::atomic_bool end_working(false);
    std::atomic_bool is_waiting(false);

    std::thread worker_thread([&](){
        queue.work([&](int&) -> bool {
            is_waiting.store(true);
            while (end_working.load() == false) {
                // WAITING...
            }
            is_waiting.store(false);
            return true;
        });
    });

    while (is_waiting.load() == false) {
        // WAITING...
    }
    ASSERT_EQ(queue.sizeOfInsertQueue(), 0U);
    ASSERT_EQ(queue.getWorkingCount(), 1U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);

    end_working.store(true);
    worker_thread.join();

    ASSERT_EQ(queue.sizeOfInsertQueue(), 0U);
    ASSERT_EQ(queue.getWorkingCount(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 1U);

    int find_value = queue.findAndRemoveFromRemoveQueue([&](int value) -> bool {
        return value == TEST_VALUE;
    });
    ASSERT_EQ(find_value, TEST_VALUE);

    ASSERT_EQ(queue.sizeOfInsertQueue(), 0U);
    ASSERT_EQ(queue.getWorkingCount(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);
}

TEST(SafetyWorkingQueueTest, Exception)
{
    SafetyWorkingQueue<int> queue;
    ASSERT_THROW(queue.popFromRemoveQueue(), ContainerEmptyException);
}

TEST(SafetyWorkingQueueTest, Clear)
{
    SafetyWorkingQueue<int> queue;
    queue.push(100);
    queue.push(20);
    queue.push(0);
    queue.work([](int&) -> bool { return true; });

    ASSERT_EQ(queue.sizeOfInsertQueue(), 2U);
    ASSERT_EQ(queue.getWorkingCount(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 1U);

    queue.clear();
    ASSERT_EQ(queue.sizeOfInsertQueue(), 0U);
    ASSERT_EQ(queue.getWorkingCount(), 0U);
    ASSERT_EQ(queue.sizeOfRemoveQueue(), 0U);
}

