/**
 * @file   LmdbxxTest.cpp
 * @brief  Lmdbxx library tester.
 * @author zer0
 * @date   2017-11-19
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/3rd/lmdbxx/lmdb++.h>
#include <libtbag/Unit.hpp>

#include <cstdio>

using namespace libtbag;

TEST(LmdbxxTest, Default)
{
    tttDir(true, true);
    auto const PATH = tttDirGet();

    auto env = lmdb::env::create();
    env.set_mapsize(1UL * GIGA_BYTE_TO_BYTE);
    env.open(PATH.toString().c_str());

    std::string const KEY1 = "username";
    std::string const KEY2 = "email";
    std::string const KEY3 = "github";

    std::string const VAL1 = "osom8979";
    std::string const VAL2 = "osom8979@gmail.com";
    std::string const VAL3 = "https://github.com/osom8979/tbag";

    // Insert some key/value pairs in a write transaction:
    auto wtxn = lmdb::txn::begin(env);
    auto dbi = lmdb::dbi::open(wtxn, nullptr);
    ASSERT_TRUE(dbi.put(wtxn, KEY1.c_str(), VAL1.c_str()));
    ASSERT_TRUE(dbi.put(wtxn, KEY2.c_str(), VAL2.c_str()));
    ASSERT_TRUE(dbi.put(wtxn, KEY3.c_str(), VAL3.c_str()));
    wtxn.commit();

    // Fetch key/value pairs in a read-only transaction:
    auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    auto cursor = lmdb::cursor::open(rtxn, dbi);
    std::string key, value;

    while (cursor.get(key, value, MDB_NEXT)) {
        if (key == KEY1) {
            ASSERT_EQ(value, VAL1);
        } else if (key == KEY2) {
            ASSERT_EQ(value, VAL2);
        } else if (key == KEY3) {
            ASSERT_EQ(value, VAL3);
        } else {
            ASSERT_TRUE(false);
        }
        std::cout << "KEY: " << key << ", VAL: " << value << std::endl;
    }

    cursor.close();
    rtxn.abort();
}

