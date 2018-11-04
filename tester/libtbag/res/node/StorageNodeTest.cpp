/**
 * @file   StorageNodeTest.cpp
 * @brief  StorageNode class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/res/node/StorageNode.hpp>

using namespace libtbag;
using namespace libtbag::res;
using namespace libtbag::res::node;

TBAG_CONSTEXPR static char const * const STORAGE_NODE_XML = R"(
<storage default_root='dir' archive='dir'>
  <env name='env1.xml' default='true' system='true' />
  <config />
</storage>
)";

TEST(StorageNodeTest, Default)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / "filename.xml";
    ASSERT_EQ(Err::E_SUCCESS, libtbag::filesystem::writeFile(PATH.toString(), STORAGE_NODE_XML));

    StorageNode node;
    ASSERT_TRUE(node.loadFile(PATH));
}

