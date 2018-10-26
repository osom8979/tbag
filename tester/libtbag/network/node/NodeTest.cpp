/**
 * @file   NodeTest.cpp
 * @brief  Node class tester.
 * @author zer0
 * @date   2018-10-26
 */

#include <gtest/gtest.h>
#include <libtbag/network/node/Node.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::node;

TEST(NodeTest, Default)
{
    Node object;
    ASSERT_TRUE(true);
}

