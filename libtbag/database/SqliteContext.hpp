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
        if (sqlite3_open(path.c_str(), &_db) != 0) {
            return false;
        }
        return true;
    }

    void close() {
        sqlite3_close(_db);
    }
};

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITECONTEXT_HPP__

