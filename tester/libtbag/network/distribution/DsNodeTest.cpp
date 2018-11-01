/**
 * @file   DsNodeTest.cpp
 * @brief  DsNode class tester.
 * @author zer0
 * @date   2018-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/network/distribution/DsNode.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::distribution;

TEST(DsNodeTest, Default)
{
    DsNode object;
    ASSERT_TRUE(true);
}

