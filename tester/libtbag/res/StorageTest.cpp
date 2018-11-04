/**
 * @file   StorageTest.cpp
 * @brief  Storage class tester.
 * @author zer0
 * @date   2018-11-03
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/Storage.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(StorageTest, Config)
{
    std::string const DIR1 = "dir1";
    std::string const DIR2 = "dir2";

    std::string const GRP1 = "grp1";
    std::string const GRP2 = "grp2";
    std::string const GRP3 = "grp3";

    std::string const KEY1 = "key1";
    std::string const KEY2 = "key2";
    std::string const KEY3 = "key3";

    std::string const VAL1 = "val1";
    std::string const VAL2 = "val2";
    std::string const VAL3 = "val3";

    std::string const VAL_EXT = "val_ext";

    tttDir_Automatic();
    auto const PATH = tttDir_Get();

    Storage storage;
    storage.setConfig(PATH);

    ASSERT_TRUE(storage.getConfigGroups().empty());
    ASSERT_TRUE(storage.saveConfig(GRP1, KEY1, VAL1));
    ASSERT_EQ(1, storage.getConfigGroups().size());
    ASSERT_EQ(1, storage.getConfigKeys(GRP1).size());

    std::string read;
    ASSERT_FALSE(storage.readConfig(GRP2, KEY1, read));
    ASSERT_FALSE(storage.readConfig(GRP1, KEY2, read));
    ASSERT_TRUE(storage.readConfig(GRP1, KEY1, read));
    ASSERT_EQ(1, storage.getConfigGroups().size());
    ASSERT_EQ(1, storage.getConfigKeys(GRP1).size());
    ASSERT_EQ(VAL1, read);

    ASSERT_TRUE(storage.saveConfig(GRP1, KEY1, VAL_EXT));
    ASSERT_EQ(1, storage.getConfigGroups().size());
    ASSERT_EQ(1, storage.getConfigKeys(GRP1).size());

    ASSERT_TRUE(storage.readConfig(GRP1, KEY1, read));
    ASSERT_EQ(VAL_EXT, read);

    ASSERT_TRUE(storage.saveConfig(GRP2, KEY2, VAL2));
    ASSERT_EQ(2, storage.getConfigGroups().size());
    ASSERT_EQ(1, storage.getConfigKeys(GRP2).size());

    ASSERT_TRUE(storage.saveConfig(GRP2, KEY3, VAL3));
    ASSERT_EQ(2, storage.getConfigGroups().size());
    ASSERT_EQ(2, storage.getConfigKeys(GRP2).size());

    ASSERT_TRUE(storage.readConfig(GRP2, KEY2, read));
    ASSERT_EQ(VAL2, read);

    ASSERT_TRUE(storage.readConfig(GRP2, KEY3, read));
    ASSERT_EQ(VAL3, read);

    ASSERT_TRUE(storage.saveConfig(GRP3, KEY3, VAL3));
    ASSERT_EQ(3, storage.getConfigGroups().size());
    ASSERT_EQ(1, storage.getConfigKeys(GRP3).size());

    ASSERT_TRUE(storage.readConfig(GRP3, KEY3, read));
    ASSERT_EQ(VAL3, read);

    storage.removeConfig(GRP1);
    ASSERT_EQ(2, storage.getConfigGroups().size());

    storage.removeAllConfig();
    ASSERT_TRUE(storage.getConfigGroups().empty());
}

