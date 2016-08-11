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

    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    auto & prepare = queue.prepareManual();
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 1U);

    prepare.at() = TEST_VALUE;
    queue.push(prepare);
    ASSERT_EQ(queue.size(),          1U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    auto & pop = queue.popManual();
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 1U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);
    ASSERT_EQ(pop.at(), TEST_VALUE);

    {
        queue.prepare();
        ASSERT_EQ(queue.size(),          1U);
        ASSERT_EQ(queue.sizeOfReading(), 1U);
        ASSERT_EQ(queue.sizeOfRemove(),  0U);
        ASSERT_EQ(queue.sizeOfPrepare(), 0U);
    }

    queue.readEnd(pop);
    ASSERT_EQ(queue.size(),          1U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  1U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

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
    ASSERT_EQ(queue.size(),          1U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    ASSERT_TRUE(queue.popAndReadEnd());
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  1U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);
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
        ASSERT_EQ(*pop.get()->at().get(), TEST_NUMBER_01);
    }
    ASSERT_THROW(queue.popUntil(1U), IllegalStateException);
    ASSERT_EQ(*queue.popUntil(0U).get()->at().get(), TEST_NUMBER_02);

    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    queue.push(queue.prepareManual());
    ASSERT_EQ(queue.size(),          5U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    queue.popUntil(0U);
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  5U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);
}

TEST(SafetyPrepareQueueTest, autoOperator)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.prepare();
        ASSERT_EQ(queue.size(),          0U);
        ASSERT_EQ(queue.sizeOfReading(), 0U);
        ASSERT_EQ(queue.sizeOfRemove(),  0U);
        ASSERT_EQ(queue.sizeOfPrepare(), 1U);
    }
    ASSERT_EQ(queue.size(),          1U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    {
        auto pop = queue.pop();
        ASSERT_EQ(queue.size(),          0U);
        ASSERT_EQ(queue.sizeOfReading(), 1U);
        ASSERT_EQ(queue.sizeOfRemove(),  0U);
        ASSERT_EQ(queue.sizeOfPrepare(), 0U);
    }
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  1U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);
}

TEST(SafetyPrepareQueueTest, cancelPrepare)
{
    SafetyPrepareQueue<int> queue;

    {
        auto prepare = queue.prepare();
        ASSERT_EQ(queue.size(),          0U);
        ASSERT_EQ(queue.sizeOfReading(), 0U);
        ASSERT_EQ(queue.sizeOfRemove(),  0U);
        ASSERT_EQ(queue.sizeOfPrepare(), 1U);
        prepare->cancel();
    }
    ASSERT_EQ(queue.size(),          0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  1U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);
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
    ASSERT_EQ(queue.size(),          3U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    {
        auto pop = queue.pop();
        ASSERT_EQ(pop->at(), TEST_VALUE_01);
        ASSERT_EQ(queue.size(),          2U);
        ASSERT_EQ(queue.sizeOfReading(), 1U);
        ASSERT_EQ(queue.sizeOfRemove(),  0U);
        ASSERT_EQ(queue.sizeOfPrepare(), 0U);
        pop->cancel();
    }
    ASSERT_EQ(queue.size(),          3U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);
    ASSERT_EQ(queue.sizeOfRemove(),  0U);
    ASSERT_EQ(queue.sizeOfPrepare(), 0U);

    auto pop = queue.pop();
    ASSERT_EQ(pop->at(), TEST_VALUE_01);
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
                ASSERT_EQ(queue.sizeOfAll(), all_count);
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
            ASSERT_EQ(*reading->at().get(), reading_index);
            ++reading_index;
        }
    });

    t1.join();
    t2.join();

    ASSERT_EQ(queue.size(), 0U);
    ASSERT_EQ(queue.sizeOfReading(), 0U);

    std::cout << "SafetyPrepareQueue.sizeOfRemove: " << queue.sizeOfRemove() << std::endl;
    ASSERT_GT(queue.sizeOfRemove(), 1U);
    ASSERT_LT(queue.sizeOfRemove(), static_cast<std::size_t>(MAX_TEST));
}

