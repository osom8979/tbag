/**
 * @file   NngSocketTest.cpp
 * @brief  NngSocket class tester.
 * @author zer0
 * @date   2019-05-29
 */

#include <gtest/gtest.h>
#include <libtbag/mq/NngSocket.hpp>

using namespace libtbag;
using namespace libtbag::mq;

TEST(NngSocketTest, Default)
{
    NngSocket object;
    ASSERT_TRUE(true);
}

