/**
 * @file   KeyStoreTest.cpp
 * @brief  KeyStore class tester.
 * @author zer0
 * @date   2017-12-06
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/security/KeyStore.hpp>

using namespace libtbag;
using namespace libtbag::security;

TEST(KeyStoreTest, File)
{
    tttDir(true, true);
    auto const file_path = tttDir_Get() / "store";

    KeyStore store;
    ASSERT_TRUE(store.open(KeyStore::Type::T_FILE, file_path.toString()));
    ASSERT_TRUE(store.isOpen());
    ASSERT_TRUE(file_path.isRegularFile());

    std::string const key1 = "key1";
    std::string const val1 = "value1";

    auto const get1 = store.get(key1);
    ASSERT_NE(E_SUCCESS, get1.code);
    ASSERT_TRUE(store.list().empty());

    ASSERT_EQ(E_SUCCESS, store.setSafe(key1, val1));
    ASSERT_EQ(1, store.list().size());

    auto const get2 = store.get(key1);
    ASSERT_EQ(E_SUCCESS, get2.code);
    ASSERT_EQ(val1, get2.value);
    ASSERT_EQ(1, store.list().size());
}

