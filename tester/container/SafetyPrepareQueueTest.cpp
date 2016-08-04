/**
 * @file   SafetyPrepareQueueTest.cpp
 * @brief  SafetyPrepareQueue class tester.
 * @author zer0
 * @date   2016-08-04
 */

#include <gtest/gtest.h>
#include <libtbag/container/SafetyPrepareQueue.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(SafetyPrepareQueueTest, Default)
{
    SafetyPrepareQueue<int> queue;
}

