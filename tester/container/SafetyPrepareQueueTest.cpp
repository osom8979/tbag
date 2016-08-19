/**
 * @file   SafetyPrepareQueueTest.cpp
 * @brief  SafetyPrepareQueue class tester.
 * @author zer0
 * @date   2016-08-04
 */

#include <gtest/gtest.h>
#include <libtbag/container/SafetyPrepareQueue.hpp>

#include <iostream>
#include <memory>
#include <thread>

using namespace libtbag;
using namespace libtbag::container;

TEST(SafetyPrepareQueueTest, Default)
{
    int const TEST_VALUE = 100;

    SafetyPrepareQueue<int> queue;

    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    auto & prepare = queue.prepareManual();
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(1U, queue.sizeOfPrepare());

    prepare.at() = TEST_VALUE;
    queue.push(prepare);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    auto & pop = queue.popManual();
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(1U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());
    ASSERT_EQ(TEST_VALUE, pop.at());

    {
        queue.prepare();
        ASSERT_EQ(1U, queue.size());
        ASSERT_EQ(1U, queue.sizeOfReading());
        ASSERT_EQ(0U, queue.sizeOfRemove());
        ASSERT_EQ(0U, queue.sizeOfPrepare());
    }

    queue.readEnd(pop);
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(1U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    queue.clear();
    ASSERT_TRUE(queue.empty());
    ASSERT_TRUE(queue.emptyOfReading());
    ASSERT_TRUE(queue.emptyOfRemove());
    ASSERT_TRUE(queue.emptyOfPrepare());
}

TEST(SafetyPrepareQueueTest, popAndReadEnd)
{
    SafetyPrepareQueue<int> queue;

    queue.push(queue.prepareManual());
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    ASSERT_TRUE(queue.popAndReadEnd());
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(1U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());
}

TEST(SafetyPrepareQueueTest, popUntil)
{
    using SharedInt = std::shared_ptr<int>;
    SafetyPrepareQueue<SharedInt> queue;

    int const TEST_NUMBER_01 = 100;
    int const TEST_NUMBER_02 = 200;
    {
        auto prepare = queue.prepare();
        prepare.get()->at().reset(new int (TEST_NUMBER_01));
    }
    ASSERT_THROW(queue.popUntil(1U), IllegalStateException);
    {
        auto prepare = queue.prepare();
        prepare.get()->at().reset(new int (TEST_NUMBER_02));
    }
    {
        auto pop = queue.popUntil(1U);
        ASSERT_EQ(TEST_NUMBER_01, *pop.get()->at().get());
    }
    ASSERT_THROW(queue.popUntil(1U), IllegalStateException);
    ASSERT_EQ(TEST_NUMBER_02, *queue.popUntil(0U).get()->at().get());

    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    ASSERT_EQ(5U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    queue.popUntil(0U);
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(5U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());
}

TEST(SafetyPrepareQueueTest, autoOperator)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.prepare();
        ASSERT_EQ(0U, queue.size());
        ASSERT_EQ(0U, queue.sizeOfReading());
        ASSERT_EQ(0U, queue.sizeOfRemove());
        ASSERT_EQ(1U, queue.sizeOfPrepare());
    }
    ASSERT_EQ(1U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    {
        auto pop = queue.pop();
        ASSERT_EQ(0U, queue.size());
        ASSERT_EQ(1U, queue.sizeOfReading());
        ASSERT_EQ(0U, queue.sizeOfRemove());
        ASSERT_EQ(0U, queue.sizeOfPrepare());
    }
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(1U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());
}

TEST(SafetyPrepareQueueTest, cancelPrepare)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.prepare();
        ASSERT_EQ(0U, queue.size());
        ASSERT_EQ(0U, queue.sizeOfReading());
        ASSERT_EQ(0U, queue.sizeOfRemove());
        ASSERT_EQ(1U, queue.sizeOfPrepare());
        prepare->cancel();
    }
    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(1U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());
}

TEST(SafetyPrepareQueueTest, cancelPop)
{
    SafetyPrepareQueue<int> queue;
    int const TEST_VALUE_01 = 100;
    int const TEST_VALUE_02 = 200;

    {
        auto prepare = queue.prepare();
        prepare->at() = TEST_VALUE_01;
    }
    {
        auto prepare = queue.prepare();
        prepare->at() = TEST_VALUE_02;
    }
    queue.push(queue.prepareManual());
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    {
        auto pop = queue.pop();
        ASSERT_EQ(TEST_VALUE_01, pop->at());
        ASSERT_EQ(2U, queue.size());
        ASSERT_EQ(1U, queue.sizeOfReading());
        ASSERT_EQ(0U, queue.sizeOfRemove());
        ASSERT_EQ(0U, queue.sizeOfPrepare());
        pop->cancel();
    }
    ASSERT_EQ(3U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());
    ASSERT_EQ(0U, queue.sizeOfRemove());
    ASSERT_EQ(0U, queue.sizeOfPrepare());

    auto pop = queue.pop();
    ASSERT_EQ(TEST_VALUE_01, pop->at());
}

TEST(SafetyPrepareQueueTest, Thread)
{
    using SharedInt = std::shared_ptr<int>;
    SafetyPrepareQueue<SharedInt> queue;

    int const MAX_TEST = 10000;
    int prepare_index = 0;
    int reading_index = 0;

    std::thread t1([&](){
        while (prepare_index < MAX_TEST) {
            std::size_t all_count = queue.sizeOfAll();
            std::size_t remove_count = queue.sizeOfRemove();

            auto prepare = queue.prepare();

            if (remove_count >= 1) {
                ASSERT_EQ(all_count, queue.sizeOfAll());
            }

            if (static_cast<bool>(prepare->at()) == false) {
                prepare->at().reset(new int(prepare_index++));
            } else {
                *prepare->at().get() = prepare_index++;
            }
        }
    });

    std::thread t2([&](){
        while (reading_index < MAX_TEST) {
            while (queue.empty()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            }

            auto reading = queue.pop();
            ASSERT_EQ(reading_index, *reading->at().get());
            ++reading_index;
        }
    });

    t1.join();
    t2.join();

    ASSERT_EQ(0U, queue.size());
    ASSERT_EQ(0U, queue.sizeOfReading());

    std::cout << "SafetyPrepareQueue.sizeOfRemove: " << queue.sizeOfRemove() << std::endl;
    ASSERT_LT(1U, queue.sizeOfRemove());
    ASSERT_GT(static_cast<std::size_t>(MAX_TEST), queue.sizeOfRemove());
}

