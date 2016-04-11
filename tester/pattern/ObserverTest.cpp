/**
 * @file   ObserverTest.cpp
 * @brief  Observer class tester.
 * @author zer0
 * @date   2016-04-11
 */

#include <gtest/gtest.h>
#include <libtbag/pattern/Observer.hpp>

#include <thread>
#include <chrono>

using namespace libtbag;
using namespace libtbag::pattern;

struct ObserverMock : public Observer
{
public:
    ObserverMock() noexcept = default;
    virtual ~ObserverMock() = default;

public:
    int value = 0;

    virtual void notify(void * data) override
    {
        value = *(int*)(data);
        *(int*)(data) += 1;
    }
};

TEST(ObserverStaticTest, UnorderedObservable)
{
    int const TEST_NUMBER = 100;

    auto test1 = std::make_shared<ObserverMock>();
    auto test2 = std::make_shared<ObserverMock>();

    UnorderedObservable observable;
    observable.registerObserver(test1);
    observable.registerObserver(test2);

    observable.notifyObserver(std::make_shared<int>(TEST_NUMBER));

    ASSERT_GE(test1->value, TEST_NUMBER);
    ASSERT_GE(test2->value, TEST_NUMBER);
}

TEST(ObserverStaticTest, OrderedObservable)
{
    int const TEST_NUMBER = 100;

    auto test1 = std::make_shared<ObserverMock>();
    auto test2 = std::make_shared<ObserverMock>();
    auto test3 = std::make_shared<ObserverMock>();
    auto test4 = std::make_shared<ObserverMock>();

    OrderedObservable observable;
    observable.registerObserver(1, test1);
    observable.registerObserver(2, test2);
    observable.registerObserver(100, test3);
    observable.registerObserver(10, test4);

    observable.notifyObserver(std::make_shared<int>(TEST_NUMBER));

    ASSERT_EQ(test1->value, TEST_NUMBER);
    ASSERT_EQ(test2->value, TEST_NUMBER + 1);
    ASSERT_EQ(test4->value, TEST_NUMBER + 2);
    ASSERT_EQ(test3->value, TEST_NUMBER + 3);
}

TEST(ObserverStaticTest, OrderedObservable_unregister)
{
    int const TEST_NUMBER = 100;

    auto test1 = std::make_shared<ObserverMock>();
    auto test2 = std::make_shared<ObserverMock>();
    auto test3 = std::make_shared<ObserverMock>();
    auto test4 = std::make_shared<ObserverMock>();

    OrderedObservable observable;
    observable.registerObserver(1, test1);
    observable.registerObserver(2, test2);
    observable.registerObserver(100, test3);
    observable.registerObserver(10, test4);
    observable.unregisterObserver(test2);

    observable.notifyObserver(std::make_shared<int>(TEST_NUMBER));

    ASSERT_EQ(test1->value, TEST_NUMBER);
    ASSERT_EQ(test2->value, 0);
    ASSERT_EQ(test4->value, TEST_NUMBER + 1);
    ASSERT_EQ(test3->value, TEST_NUMBER + 2);
}

