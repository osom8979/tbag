/**
 * @file   StorageNodeTest.cpp
 * @brief  StorageNode class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/node/StorageNode.hpp>

using namespace libtbag;
using namespace libtbag::res;
using namespace libtbag::res::node;

TEST(StorageNodeTest, Default)
{
    tttDir_Automatic();
    auto path = tttDir_Get() / "test";

    StorageNode object;
}

