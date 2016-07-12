/**
 * @file   Sqlite.cpp
 * @brief  Sqlite class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/database/Sqlite.hpp>
#include <libtbag/database/sqlite3/sqlite3.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

template <typename T>
inline sqlite3 * sqlite3_cast(T * object)
{
    return static_cast<sqlite3*>(object);
}

template <typename T>
inline sqlite3_stmt * sqlite3_stmt_cast(T * object)
{
    return static_cast<sqlite3_stmt*>(object);
}

// ---------------------------
// Transaction implementation.
// ---------------------------

Sqlite::Transaction::Transaction(Sqlite & context) : _context(context)
{
    _context.beginTransaction();
}

Sqlite::Transaction::~Transaction()
{
    _context.endTransaction();
}

// -------------------------
// Statement implementation.
// -------------------------

Sqlite::Statement::Statement(FakeStatement * statement) : _statement(statement)
{
    // EMPTY.
}

Sqlite::Statement::~Statement()
{
    // EMPTY.
}

int Sqlite::Statement::getByte(int index) const
{
    return sqlite3_column_bytes(sqlite3_stmt_cast(_statement), index);
}

double Sqlite::Statement::getDouble(int index) const
{
    return sqlite3_column_double(sqlite3_stmt_cast(_statement), index);
}

int Sqlite::Statement::getInt(int index) const
{
    return sqlite3_column_int(sqlite3_stmt_cast(_statement), index);
}

Sqlite::Statement::str Sqlite::Statement::getString(int index) const
{
    return std::string((char const *) sqlite3_column_text(sqlite3_stmt_cast(_statement), index));
}

// ----------------------
// Sqlite implementation.
// ----------------------

Sqlite::Sqlite() : _db(nullptr)
{
    // EMPTY.
}

Sqlite::~Sqlite()
{
    if (this->isOpen()) {
        this->close();
    }
}

bool Sqlite::open(std::string const & path)
{
    return (sqlite3_open(path.c_str(), (sqlite3**) &_db) == SQLITE_OK);
}

void Sqlite::close()
{
    if (_db != nullptr) {
        sqlite3_close(sqlite3_cast(_db));
        _db = nullptr;
    }
}

bool Sqlite::isOpen() const noexcept
{
    return (_db != nullptr);
}

bool Sqlite::execute(std::string const & query)
{
    if (!isOpen()) {
        return false;
    }

    char * error_message = nullptr;
    return (sqlite3_exec(sqlite3_cast(_db)
                       , query.c_str()
                       , nullptr
                       , nullptr
                       , &error_message) == SQLITE_OK);
}

Sqlite::FakeStatement * Sqlite::prepareStatement(std::string const & query)
{
    if (isOpen() == false) {
        return nullptr;
    }

    sqlite3_stmt * stmt = nullptr;
    if (sqlite3_prepare(sqlite3_cast(_db)
                      , query.c_str()
                      , query.size()
                      , &stmt
                      , nullptr) == SQLITE_OK) {
        return static_cast<FakeStatement*>(stmt);
    }

    return nullptr;
}

bool Sqlite::stepRowStatement(FakeStatement * statement)
{
    return (sqlite3_step(sqlite3_stmt_cast(statement)) == SQLITE_ROW);
}

void Sqlite::finalizeStatement(FakeStatement * statement)
{
    if (statement != nullptr) {
        sqlite3_finalize(sqlite3_stmt_cast(statement));
    }
}

bool Sqlite::beginTransaction()
{
    return this->execute("BEGIN;");
}

bool Sqlite::endTransaction()
{
    return this->execute("END;");
}

int Sqlite::getErrorCode() const noexcept
{
    return sqlite3_errcode(sqlite3_cast(_db));
}

std::string Sqlite::getErrorMessage() const noexcept
{
    return std::string(sqlite3_errmsg(sqlite3_cast(_db)));
}

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

