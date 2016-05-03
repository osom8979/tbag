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

    std::string const CREATE_TABLE =
            "CREATE TABLE IF NOT EXISTS test (id INT PRIMARY KEY, name CHAR(20));";
    std::string const INSERT_ITEM1 =
            "INSERT INTO test(id, name) VALUES(1, 'temp_name1');";
    std::string const INSERT_ITEM2 =
            "INSERT INTO test(id, name) VALUES(2, 'temp_name2');";
    std::string const SELECT_ITEM =
            "SELECT * FROM test;";
    std::string const DELETE_ITEM =
            "DELETE FROM test WHERE id > 0;";

    ASSERT_TRUE(db.open(PATH));
    ASSERT_EQ(db.getErrorCode(), 0);
    ASSERT_GT(db.getErrorMessage().size(), 0U);

    ASSERT_TRUE(db.execute(CREATE_TABLE));
    ASSERT_TRUE(db.execute(INSERT_ITEM1));
    ASSERT_TRUE(db.execute(INSERT_ITEM2));

    using RowType = std::tuple<int, std::string>;
    auto callback = [](SqliteContext::Statement const & statement) -> RowType {
        return RowType(statement.getInt(0), statement.getString(1));
    };
    auto result = db.prepare<RowType>(SELECT_ITEM, callback);

    ASSERT_EQ(result.size(), 2U);
    ASSERT_EQ(std::get<0>(result[0]), 1);
    ASSERT_EQ(std::get<0>(result[1]), 2);
    ASSERT_EQ(std::get<1>(result[0]), std::string("temp_name1"));
    ASSERT_EQ(std::get<1>(result[1]), std::string("temp_name2"));
    ASSERT_TRUE(db.execute(DELETE_ITEM));

    db.close();
}

