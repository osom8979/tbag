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
    ASSERT_EQ(1U, queue.sizeOfInsertQueue());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

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
    ASSERT_EQ(0U, queue.sizeOfInsertQueue());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    end_working.store(true);
    worker_thread.join();

    ASSERT_EQ(0U, queue.sizeOfInsertQueue());
    ASSERT_EQ(1U, queue.sizeOfRemoveQueue());

    int find_value = queue.findAndRemoveFromRemoveQueue([&](int value) -> bool {
        return value == TEST_VALUE;
    });
    ASSERT_EQ(TEST_VALUE, find_value);

    ASSERT_EQ(0U, queue.sizeOfInsertQueue());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());
}

TEST(SafetyWorkingQueueTest, Exception)
{
    SafetyWorkingQueue<int> queue;
    ASSERT_THROW(queue.popFromRemoveQueue(), ContainerEmptyException);
}

TEST(SafetyWorkingQueueTest, Remove)
{
    SafetyWorkingQueue<int> queue;
    queue.push(100);
    queue.push(20);
    queue.push(0);
    ASSERT_EQ(3U, queue.sizeOfInsertQueue());

    ASSERT_TRUE(queue.removeFromInsertQueue([](int const & value) -> bool {
        return value == 20;
    }));
    ASSERT_EQ(2U, queue.sizeOfInsertQueue());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());

    ASSERT_TRUE(queue.work());
    ASSERT_TRUE(queue.work());
    ASSERT_EQ(0U, queue.sizeOfInsertQueue());
    ASSERT_EQ(2U, queue.sizeOfRemoveQueue());

    ASSERT_EQ(100, queue.popFromRemoveQueue());
    ASSERT_EQ(  0, queue.popFromRemoveQueue());
}

TEST(SafetyWorkingQueueTest, clear)
{
    SafetyWorkingQueue<int> queue;
    queue.push(100);
    queue.push(20);
    queue.push(0);
    queue.work([](int&) -> bool { return true; });

    ASSERT_EQ(2U, queue.sizeOfInsertQueue());
    ASSERT_EQ(1U, queue.sizeOfRemoveQueue());

    queue.clear();
    ASSERT_EQ(0U, queue.sizeOfInsertQueue());
    ASSERT_EQ(0U, queue.sizeOfRemoveQueue());
}

