/**
 * @file   StorageNodeTest.cpp
 * @brief  StorageNode class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <libtbag/res/node/StorageNode.hpp>

using namespace libtbag;
using namespace libtbag::res;
using namespace libtbag::res::node;

TEST(StorageNodeTest, Default)
{
    StorageNode object;
    ASSERT_TRUE(true);
}

