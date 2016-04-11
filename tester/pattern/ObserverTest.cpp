/**
 * @file   ObserverTest.cpp
 * @brief  Observer class tester.
 * @author zer0
 * @date   2016-04-11
 */

#include <gtest/gtest.h>
#include <libtbag/pattern/Observer.hpp>

using namespace libtbag;
using namespace libtbag::pattern;

TEST(ObserverStaticTest, UnorderedObservable)
{
    int const TEST_NUMBER = 100;
    int test0 = 0;
    int test1 = 0;

    UnorderedObservable observable;
    observable.add([&](){
            test0 = TEST_NUMBER;
        });
    observable.add([&](){
            test1 = TEST_NUMBER;
        });
    observable.notify();

    ASSERT_GE(test0, TEST_NUMBER);
    ASSERT_GE(test1, TEST_NUMBER);
}

