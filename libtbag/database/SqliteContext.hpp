/**
 * @file   SqliteContext.hpp
 * @brief  SqliteContext class prototype.
 * @author zer0
 * @date   2016-04-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITECONTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITECONTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <vector>
#include <tuple>
#include <functional>

#include <sqlite3.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

/**
 * SqliteContext class prototype.
 *
 * @author zer0
 * @date   2016-04-29
 */
class SqliteContext : public Noncopyable
{
public:
    /**
     * Sqlite transaction class prototype.
     *
     * @author zer0
     * @date   2016-04-29
     */
    class Transaction : public Noncopyable
    {
    private:
        SqliteContext & _context;

    public:
        Transaction(SqliteContext & context) : _context(context) {
            _context.beginTransaction();
        }

        ~Transaction() {
            _context.endTransaction();
        }
    };

    /**
     * Sqlite prepare statement class prototype.
     *
     * @author zer0
     * @date   2016-04-29
     */
    class Statement : public Noncopyable
    {
    private:
        sqlite3_stmt * _statement;

    public:
        Statement(sqlite3_stmt * statement) : _statement(statement) {
            __EMPTY_BLOCK__
        }

        ~Statement() {
            __EMPTY_BLOCK__
        }

    public:
        int getByte(int index) const {
            return sqlite3_column_bytes(_statement, index);
        }

        int getDouble(int index) const {
            return sqlite3_column_double(_statement, index);
        }

        int getInt(int index) const {
            return sqlite3_column_int(_statement, index);
        }

        std::string getString(int index) const {
            return std::string((char const *)sqlite3_column_text(_statement, index));
        }
    };

private:
    sqlite3 * _db;

public:
    SqliteContext() : _db(nullptr) {
        __EMPTY_BLOCK__
    }

    ~SqliteContext() {
        __EMPTY_BLOCK__
    }

public:
    bool open(std::string const & path) {
        if (sqlite3_open(path.c_str(), &_db) != SQLITE_OK) {
            return false;
        }
        return true;
    }

    void close() {
        sqlite3_close(_db);
    }

    inline bool isOpen() const noexcept {
        return (_db != nullptr ? true : false);
    }

public:
    bool execute(std::string const & query) {
        if (!isOpen()) {
            return false;
        }

        char * error_message = nullptr;
        if (sqlite3_exec(_db, query.c_str(), nullptr, nullptr, &error_message) != SQLITE_OK) {
            return false;
        }
        return true;
    }

    template <typename Tuple>
    std::vector<Tuple> prepare(std::string const & query
                             , std::function<Tuple(Statement const &)> const & callback) {
        if (!isOpen()) {
            return std::vector<Tuple>();
        }

        std::vector<Tuple> result;

        Transaction guard(*this);
        sqlite3_stmt * stmt = nullptr;

        if (sqlite3_prepare(_db, query.c_str(), query.size(), &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                result.push_back(callback(Statement(stmt)));
            }
        }

        sqlite3_finalize(stmt);
        return result;
    }

public:
    bool beginTransaction() {
        return this->execute("BEGIN;");
    }

    bool endTransaction() {
        return this->execute("END;");
    }

public:
    int getErrorCode() const noexcept {
        return sqlite3_errcode(this->_db);
    }

    std::string getErrorMessage() const noexcept {
        return std::string(sqlite3_errmsg(this->_db));
    }
};

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITECONTEXT_HPP__

