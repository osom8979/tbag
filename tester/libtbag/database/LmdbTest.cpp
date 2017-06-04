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

    std::string const TEST_DB_NAME = "TEST_DB";
    std::string const TEST_KEY     = "KEY";
    std::string const TEST_DATA    = "DATA";

    LmdbTransaction txn;
    ASSERT_EQ(Err::E_SUCCESS, txn.begin(db));

    LmdbIndividualDatabase dbi;
    ASSERT_EQ(Err::E_SUCCESS, dbi.open(txn, TEST_DB_NAME, TBAG_DBI_OF_CREATE));

    std::string key  = TEST_KEY;
    std::string data = TEST_DATA;

    ASSERT_EQ(Err::E_SUCCESS, dbi.put(txn, key, data));
    ASSERT_EQ(Err::E_SUCCESS, txn.commit());

    {   // READ.
        LmdbTransaction read_txn;
        ASSERT_EQ(Err::E_SUCCESS, read_txn.begin(db, TBAG_LMDB_OF_RDONLY));

        LmdbCursor cursor;
        ASSERT_EQ(Err::E_SUCCESS, cursor.open(read_txn, dbi));

        std::string key = TEST_KEY;
        std::string read_value;
        read_value.resize(100);

        int read_count = 0;
        while (cursor.get(key, read_value, LmdbCursorOperations::CO_NEXT) == Err::E_SUCCESS) {
            ++read_count;
        }

        ASSERT_EQ(1, read_count);
        ASSERT_EQ(TEST_DATA, read_value);
        read_txn.abort();
    }

    dbi.close(db);
}

