/**
 * @file   Sqlite.hpp
 * @brief  Sqlite class prototype.
 * @author zer0
 * @date   2016-04-29
 * @date   2016-07-06 (Rename: SqliteContext -> Sqlite)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <vector>
#include <tuple>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

/**
 * Sqlite class prototype.
 *
 * @author zer0
 * @date   2016-04-29
 * @date   2016-07-06 (Rename: SqliteContext -> Sqlite)
 */
class TBAG_API Sqlite : public Noncopyable
{
public:
    /** Fake @c sqlite3_stmt struct. */
    using FakeStatement = void;

    /** Fake @c sqlite3 struct. */
    using FakeContext = void;

public:
    /**
     * Sqlite transaction class prototype.
     *
     * @author zer0
     * @date   2016-04-29
     */
    class TBAG_API Transaction : public Noncopyable
    {
    private:
        Sqlite & _context;

    public:
        Transaction(Sqlite & context);
        ~Transaction();
    };

    /**
     * Sqlite prepare statement class prototype.
     *
     * @author zer0
     * @date   2016-04-29
     */
    class TBAG_API Statement : public Noncopyable
    {
    public:
        using str = std::string;

    private:
        FakeStatement * _statement;

    public:
        Statement(FakeStatement * statement);
        ~Statement();

    public:
        int getByte(int index) const;
        double getDouble(int index) const;
        int getInt(int index) const;
        str getString(int index) const;
    };

private:
    FakeContext * _db;

public:
    Sqlite();
    ~Sqlite();

public:
    bool open(std::string const & path);
    void close();

public:
    bool isOpen() const TBAG_NOEXCEPT;

public:
    bool execute(std::string const & query);

public:
    FakeStatement * prepareStatement(std::string const & query);
    bool stepRowStatement(FakeStatement * statement);
    void finalizeStatement(FakeStatement * statement);

public:
    bool beginTransaction();
    bool endTransaction();

public:
    int getErrorCode() const TBAG_NOEXCEPT;
    std::string getErrorMessage() const TBAG_NOEXCEPT;

public:
    template <typename Tuple>
    std::vector<Tuple> prepare(std::string const & query
                             , std::function<Tuple(Statement const &)> const & callback)
    {
        Transaction guard(*this);
        FakeStatement * stmt = prepareStatement(query);
        if (stmt != nullptr) {
            std::vector<Tuple> result;
            while (stepRowStatement(stmt)) {
                result.push_back(callback(Statement(stmt)));
            }
            finalizeStatement(stmt);
            return result;
        }
        return std::vector<Tuple>();
    }
};

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_SQLITE_HPP__

