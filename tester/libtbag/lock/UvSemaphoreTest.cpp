/**
 * @file   UvSemaphoreTest.cpp
 * @brief  UvSemaphore class tester.
 * @author zer0
 * @date   2017-05-03
 */

#include <gtest/gtest.h>
#include <libtbag/lock/UvSemaphore.hpp>

using namespace libtbag;
using namespace libtbag::lock;

TEST(UvSemaphoreTest, Default)
{
    UvSemaphore object(1);
}

