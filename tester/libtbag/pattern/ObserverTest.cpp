/**
 * @file   ObserverTest.cpp
 * @brief  Observer class tester.
 * @author zer0
 * @date   2016-04-11
 */

#include <gtest/gtest.h>
#include <libtbag/pattern/Observer.hpp>

#include <functional>

using namespace libtbag;
using namespace libtbag::pattern;

TEST(ObserverTest, UnorderedObservable)
{
    int const TEST_NUMBER = 100;
    int test0 = 0;
    int test1 = 0;

    using Functor = std::function<void(void)>;
    ObservableSet<Functor> observable;

    observable.add([&](){
            test0 = TEST_NUMBER;
        });
    observable.add([&](){
            test1 = TEST_NUMBER;
        });
    observable.notify();

    ASSERT_LE(TEST_NUMBER, test0);
    ASSERT_LE(TEST_NUMBER, test1);
}

