/**
 * @file   SqliteTest.cpp
 * @brief  Sqlite class tester.
 * @author zer0
 * @date   2016-04-29
 * @date   2016-07-06 (Rename: SqliteContextTest -> SqliteTest)
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/database/Sqlite.hpp>

using namespace libtbag;
using namespace libtbag::database;

TEST(SqliteTest, Default)
{
    tttDir(true, true);
    auto const PATH = tttDirGet() / "db.sqlite";

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

    Sqlite db;
    ASSERT_TRUE(db.open(PATH.toString()));
    ASSERT_EQ(0 , db.getErrorCode());
    ASSERT_LT(0U, db.getErrorMessage().size());

    ASSERT_TRUE(db.execute(CREATE_TABLE));
    ASSERT_TRUE(db.execute(INSERT_ITEM1));
    ASSERT_TRUE(db.execute(INSERT_ITEM2));

    using RowType = std::tuple<int, std::string>;
    auto callback = [](Sqlite::Statement const & statement) -> RowType {
        return RowType(statement.getInt(0), statement.getString(1));
    };
    auto result = db.prepare<RowType>(SELECT_ITEM, callback);

    ASSERT_EQ(2U, result.size());
    ASSERT_EQ(1, std::get<0>(result[0]));
    ASSERT_EQ(2, std::get<0>(result[1]));
    ASSERT_STREQ("temp_name1", std::get<1>(result[0]).c_str());
    ASSERT_STREQ("temp_name2", std::get<1>(result[1]).c_str());
    ASSERT_TRUE(db.execute(DELETE_ITEM));

    db.close();
}

