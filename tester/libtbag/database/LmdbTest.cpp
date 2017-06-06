/**
 * @file   LmdbTest.cpp
 * @brief  Lmdb class tester.
 * @author zer0
 * @date   2017-06-04
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/database/Lmdb.hpp>

using namespace libtbag;
using namespace libtbag::database;

TEST(LmdbTest, Default)
{
    tttDir(true, true);
    auto const PATH = tttDirGet();

    Lmdb db;
    ASSERT_TRUE(db.isCreate());
    ASSERT_EQ(Err::E_SUCCESS, db.setMaxIndividualDatabase(1));
    ASSERT_EQ(Err::E_SUCCESS, db.open(PATH.getString(), 0));

    std::string const TEST_DB_NAME1 = "TEST_DB_1";
    std::string const TEST_DB_NAME2 = "TEST_DB_2";

    std::string const TEST_KEY1 = "KEY1";
    std::string const TEST_VAL1 = "VAL1";

    std::string const TEST_KEY2 = "KEY2";
    std::string const TEST_VAL2 = "VAL2";

    {   // PUT
        LmdbTxn txn(db);
        ASSERT_EQ(Err::E_SUCCESS, txn.begin());

        LmdbDbi dbi(txn);
        ASSERT_EQ(Err::E_SUCCESS, dbi.open(TEST_DB_NAME1, TBAG_DBI_OF_CREATE));
        ASSERT_EQ(Err::E_SUCCESS, dbi.put(TEST_KEY1, TEST_VAL1));
        ASSERT_EQ(Err::E_SUCCESS, dbi.put(TEST_KEY2, TEST_VAL2));
        ASSERT_EQ(Err::E_SUCCESS, txn.commit());
    }

    {   // GET
        std::string key1;
        std::string val1;

        std::string key2;
        std::string val2;

        std::string key3;
        std::string val3;

        LmdbTxn txn(db);
        ASSERT_EQ(Err::E_SUCCESS, txn.begin(TBAG_LMDB_OF_RDONLY));

        LmdbDbi dbi(txn);
        ASSERT_EQ(Err::E_SUCCESS, dbi.open(TEST_DB_NAME1, TBAG_DBI_OF_CREATE));

        LmdbCursor cursor(txn, dbi);
        ASSERT_EQ(Err::E_SUCCESS , cursor.get(key1, val1, LmdbCursorOperations::CO_NEXT));
        ASSERT_EQ(Err::E_SUCCESS , cursor.get(key2, val2, LmdbCursorOperations::CO_NEXT));
        ASSERT_EQ(Err::E_NOTFOUND, cursor.get(key3, val3, LmdbCursorOperations::CO_NEXT));

        ASSERT_EQ(TEST_KEY1, key1);
        ASSERT_EQ(TEST_VAL1, val1);

        ASSERT_EQ(TEST_KEY2, key2);
        ASSERT_EQ(TEST_VAL2, val2);
        // txn.abort();
    }
}

