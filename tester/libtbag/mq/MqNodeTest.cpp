/**
 * @file   MqNodeTest.cpp
 * @brief  MqNode class tester.
 * @author zer0
 * @date   2018-11-11
 */

#include <gtest/gtest.h>
#include <libtbag/mq/MqNode.hpp>

using namespace libtbag;
using namespace libtbag::mq;

TEST(MqNodeTest, Default)
{
    MqNode object;
    ASSERT_TRUE(true);
}

