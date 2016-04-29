/**
 * @file   SqliteContextTest.cpp
 * @brief  SqliteContext class tester.
 * @author zer0
 * @date   2016-04-29
 */

#include <gtest/gtest.h>
#include <libtbag/database/SqliteContext.hpp>

using namespace libtbag;
using namespace libtbag::database;

TEST(SqliteContextTest, Default)
{
    std::string const PATH = "__database_test_sqlite3.sqlite";
    SqliteContext db;
    ASSERT_TRUE(db.open(PATH));
    db.close();
}

