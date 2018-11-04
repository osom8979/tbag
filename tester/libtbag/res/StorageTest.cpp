/**
 * @file   StorageTest.cpp
 * @brief  Storage class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(StorageTest, Config)
{
    std::string const DIR1 = "dir1";
    std::string const DIR2 = "dir2";

    std::string const NAME1 = "grp1";
    std::string const NAME2 = "grp2";
    std::string const NAME3 = "grp3";

    std::string const KEY1 = "key1";
    std::string const KEY2 = "key2";
    std::string const KEY3 = "key3";

    std::string const VAL1 = "val1";
    std::string const VAL1_2 = "val1_2";
    std::string const VAL2 = "val2";
    std::string const VAL3 = "val3";

    tttDir_Automatic();
    auto const PATH = tttDir_Get();

    Storage storage;
    storage.setLayoutConfig(PATH);

    ASSERT_TRUE(storage.getConfigFilenames().empty());
    ASSERT_TRUE(storage.saveConfig(NAME1, KEY1, VAL1));
    ASSERT_EQ(1, storage.getConfigFilenames().size());
    ASSERT_EQ(1, storage.getConfigKeys(NAME1).size());

    std::string read;
    ASSERT_FALSE(storage.readConfig(NAME2, KEY1, read));
    ASSERT_FALSE(storage.readConfig(NAME1, KEY2, read));
    ASSERT_TRUE(storage.readConfig(NAME1, KEY1, read));
    ASSERT_EQ(1, storage.getConfigFilenames().size());
    ASSERT_EQ(1, storage.getConfigKeys(NAME1).size());
    ASSERT_EQ(VAL1, read);

    ASSERT_TRUE(storage.saveConfig(NAME1, KEY1, VAL1_2));
    ASSERT_EQ(1, storage.getConfigFilenames().size());
    ASSERT_EQ(1, storage.getConfigKeys(NAME1).size());

    ASSERT_TRUE(storage.readConfig(NAME1, KEY1, read));
    ASSERT_EQ(VAL1_2, read);

    ASSERT_TRUE(storage.saveConfig(NAME2, KEY2, VAL2));
    ASSERT_EQ(2, storage.getConfigFilenames().size());
    ASSERT_EQ(1, storage.getConfigKeys(NAME2).size());

    ASSERT_TRUE(storage.saveConfig(NAME2, KEY3, VAL3));
    ASSERT_EQ(2, storage.getConfigFilenames().size());
    ASSERT_EQ(2, storage.getConfigKeys(NAME2).size());

    ASSERT_TRUE(storage.readConfig(NAME2, KEY2, read));
    ASSERT_EQ(VAL2, read);

    ASSERT_TRUE(storage.readConfig(NAME2, KEY3, read));
    ASSERT_EQ(VAL3, read);

    ASSERT_TRUE(storage.saveConfig(NAME3, KEY3, VAL3));
    ASSERT_EQ(3, storage.getConfigFilenames().size());
    ASSERT_EQ(1, storage.getConfigKeys(NAME3).size());

    ASSERT_TRUE(storage.readConfig(NAME3, KEY3, read));
    ASSERT_EQ(VAL3, read);

    storage.removeConfig(NAME1);
    ASSERT_EQ(2, storage.getConfigFilenames().size());

    storage.removeAllConfig();
    ASSERT_TRUE(storage.getConfigFilenames().empty());
}

