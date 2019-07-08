/**
 * @file   StorageNodeTest.cpp
 * @brief  StorageNode class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/res/node/StorageNode.hpp>

using namespace libtbag;
using namespace libtbag::res;
using namespace libtbag::res::node;

TBAG_CONSTEXPR static char const * const STORAGE_NODE_XML = R"(
<storage root='${WORK_DIR}'>
  <user name='temp' absolute='false' raw='true'>temp.dir</user>
</storage>
)";

TEST(StorageNodeTest, LoadAndSave)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / "storage.xml";

    using namespace libtbag::filesystem;
    ASSERT_EQ(E_SUCCESS, writeFile(PATH.toString(), STORAGE_NODE_XML));
    ASSERT_TRUE(PATH.isRegularFile());

    StorageNode node;
    ASSERT_TRUE(node.loadFromXmlFile(PATH));
    ASSERT_EQ(1, node.property().users.size());
    ASSERT_EQ("temp", node.property().users[0].name);
    ASSERT_EQ("temp.dir", node.property().users[0].text);
    ASSERT_TRUE(node.property().users[0].exists);
    ASSERT_FALSE(node.property().users[0].abs);
    ASSERT_TRUE(node.property().users[0].raw);

    ASSERT_STREQ("${WORK_DIR}", node.root().c_str());

    ASSERT_TRUE(PATH.remove());
    ASSERT_FALSE(PATH.isRegularFile());

    ASSERT_TRUE(node.writeToXmlFile(PATH));
    ASSERT_TRUE(PATH.isRegularFile());
}

TEST(StorageNodeTest, SaveAndLoad)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get();
    std::string const FILE_NAME = "storage.xml";
    auto const FILE_PATH = PATH / FILE_NAME;

    std::string const NAME1 = "name1";
    std::string const NAME2 = "name2";

    std::string const KEY1 = "key1";
    std::string const KEY2 = "key2";

    std::string const VAL1 = "val1";
    std::string const VAL2 = "val2";

    StorageNode node1;
    node1.teardown();
    node1.setup();
    node1.root() = PATH;
    node1.update();

    ASSERT_TRUE(node1.writeToXmlFile(FILE_PATH));
    ASSERT_TRUE(FILE_PATH.isRegularFile());
    ASSERT_TRUE(node1.storage().saveConfig(NAME1, KEY1, VAL1));
    ASSERT_TRUE(node1.storage().saveConfig(NAME2, KEY2, VAL2));

    StorageNode node2;
    ASSERT_TRUE(node2.loadFromXmlFile(FILE_PATH));

    std::string value1;
    ASSERT_TRUE(node2.storage().readConfig(NAME1, KEY1, value1));
    ASSERT_EQ(VAL1, value1);

    std::string value2;
    ASSERT_TRUE(node2.storage().readConfig(NAME2, KEY2, value2));
    ASSERT_EQ(VAL2, value2);
}

