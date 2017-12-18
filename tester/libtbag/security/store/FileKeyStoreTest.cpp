/**
 * @file   FileKeyStoreTest.cpp
 * @brief  FileKeyStore class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/security/store/FileKeyStore.hpp>

#include <vector>
#include <string>

using namespace libtbag;
using namespace libtbag::security;
using namespace libtbag::security::store;

TEST(FileKeyStoreTest, Default)
{
    tttDir(true, true);
    auto const FILE_PATH = tttDirGet() / "store";

    std::string const KEY1 = "key1";
    std::string const VAL1 = "value1";

    std::string value;
    std::vector<std::string> keys;

    FileKeyStore store(FILE_PATH.toString());
    ASSERT_TRUE(store.list().empty());
    ASSERT_FALSE(store.get(KEY1, value));

    ASSERT_TRUE(store.create(KEY1));
    keys = store.list();
    ASSERT_EQ(1, keys.size());
    ASSERT_EQ(KEY1, keys[0]);

    ASSERT_TRUE(store.set(KEY1, VAL1));
    ASSERT_TRUE(store.get(KEY1, value));
    ASSERT_FALSE(value.empty());

    ASSERT_EQ(VAL1, value);
    ASSERT_TRUE(store.cmp(KEY1, VAL1));

    ASSERT_TRUE(store.remove(KEY1));
    ASSERT_FALSE(store.get(KEY1, value));

    ASSERT_TRUE(store.remove(KEY1));
    ASSERT_FALSE(store.get(KEY1, value));
    ASSERT_TRUE(store.list().empty());
}

TEST(FileKeyStoreTest, Encrypt)
{
    tttDir(true, true);
    auto const FILE_PATH = tttDirGet() / "store";

    std::string const KEY1 = "key1";
    std::string const VAL1 = "value1";

    std::string value;
    std::vector<std::string> keys;

    FileKeyStore store(FILE_PATH.toString());
    ASSERT_TRUE(store.list().empty());
    ASSERT_FALSE(store.get(KEY1, value));

    ASSERT_TRUE(store.create(KEY1));
    keys = store.list();
    ASSERT_EQ(1, keys.size());
    ASSERT_EQ(KEY1, keys[0]);

    ASSERT_TRUE(store.set(KEY1, VAL1, true));
    ASSERT_TRUE(store.get(KEY1, value));
    ASSERT_FALSE(value.empty());

    ASSERT_NE(VAL1, value);
    ASSERT_TRUE(store.cmp(KEY1, VAL1, true));

    ASSERT_TRUE(store.remove(KEY1));
    ASSERT_FALSE(store.get(KEY1, value));

    ASSERT_TRUE(store.remove(KEY1));
    ASSERT_FALSE(store.get(KEY1, value));
    ASSERT_TRUE(store.list().empty());
}

