/**
 * @file   Lmdb.hpp
 * @brief  Lmdb class prototype.
 * @author zer0
 * @date   2017-06-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_LMDB_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_LMDB_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

#ifndef TBAG_LMDB_OPEN_FLAG_MAP
#define TBAG_LMDB_OPEN_FLAG_MAP(_TBAG_XX) \
    _TBAG_XX(_FIXEDMAP  ,      0x01, "mmap at a fixed address (experimental)"                       ) \
    _TBAG_XX(_NOSUBDIR  ,    0x4000, "no environment directory"                                     ) \
    _TBAG_XX(_NOSYNC    ,   0x10000, "don't fsync after commit"                                     ) \
    _TBAG_XX(_RDONLY    ,   0x20000, "read only"                                                    ) \
    _TBAG_XX(_NOMETASYNC,   0x40000, "don't fsync metapage after commit"                            ) \
    _TBAG_XX(_WRITEMAP  ,   0x80000, "use writable mmap"                                            ) \
    _TBAG_XX(_MAPASYNC  ,  0x100000, "use asynchronous msync when #MDB_WRITEMAP is used"            ) \
    _TBAG_XX(_NOTLS     ,  0x200000, "tie reader locktable slots to #MDB_txn objects instead of to threads" ) \
    _TBAG_XX(_NOLOCK    ,  0x400000, "don't do any locking, caller must manage their own locks"     ) \
    _TBAG_XX(_NORDAHEAD ,  0x800000, "don't do readahead (no effect on Windows)"                    ) \
    _TBAG_XX(_NOMEMINIT , 0x1000000, "don't initialize malloc'd memory before writing to datafile"  ) \
    /* -- END -- */
#endif

using LmdbOpenFlags = unsigned int;

#define _TBAG_XX(name, value, msg) TBAG_CONSTEXPR LmdbOpenFlags const TBAG_LMDB_OF##name = value;
TBAG_LMDB_OPEN_FLAG_MAP(_TBAG_XX)
#undef _TBAG_XX

#ifndef TBAG_LMDB_DBI_OPEN_FLAG_MAP
#define TBAG_LMDB_DBI_OPEN_FLAG_MAP(_TBAG_XX) \
    _TBAG_XX(_REVERSEKEY,    0x02, "use reverse string keys"                    ) \
    _TBAG_XX(_DUPSORT   ,    0x04, "use sorted duplicates"                      ) \
    _TBAG_XX(_INTEGERKEY,    0x08, "numeric keys in native byte order"          ) \
    _TBAG_XX(_DUPFIXED  ,    0x10, "with #MDB_DUPSORT, sorted dup items have fixed size") \
    _TBAG_XX(_INTEGERDUP,    0x20, "with #MDB_DUPSORT, dups are #MDB_INTEGERKEY-style integers") \
    _TBAG_XX(_REVERSEDUP,    0x40, "with #MDB_DUPSORT, use reverse string dups" ) \
    _TBAG_XX(_CREATE    , 0x40000, "create DB if not already existing"          ) \
    /* -- END -- */
#endif

using DbiOpenFlags = unsigned int;

#define _TBAG_XX(name, value, msg) TBAG_CONSTEXPR DbiOpenFlags const TBAG_DBI_OF##name = value;
TBAG_LMDB_DBI_OPEN_FLAG_MAP(_TBAG_XX)
#undef _TBAG_XX

/**
 * Opaque structure for a database environment.
 *
 * @author zer0
 * @date   2017-06-04
 *
 * @remarks
 *  A DB environment supports multiple databases, all residing in the same shared-memory map.
 */
class TBAG_API Lmdb : public container::Pointer<void>, private Noncopyable
{
public:
    using Parent = container::Pointer<void>;

public:
    Lmdb();
    virtual ~Lmdb();

public:
    inline bool isCreate() const TBAG_NOEXCEPT
    { return ptr != nullptr; }

public:
    /** Set the maximum number of threads/reader slots for the environment. */
    Err setMaxReaders(unsigned int size);

    /** Set the size of the memory map to use for this environment. */
    Err setMapSize(std::size_t size);

    /** Set the maximum number of named databases for the environment. */
    Err setMaxIndividualDatabase(unsigned int size);

public:
    /** Create an LMDB environment handle. */
    Err create();

    /** Close the environment and release the memory map. */
    void close();

    /** Open an environment handle. */
    Err open(std::string const & path, LmdbOpenFlags flags, int mode = 0664);
};

/**
 * Opaque structure for a transaction handle.
 *
 * @author zer0
 * @date   2017-06-04
 *
 * @remarks
 * All database operations require a transaction handle. Transactions may be read-only or read-write.
 */
class TBAG_API LmdbTransaction : public container::Pointer<void>, private Noncopyable
{
public:
    using Parent = container::Pointer<void>;

public:
    LmdbTransaction();
    LmdbTransaction(Lmdb & db, LmdbTransaction & parent, LmdbOpenFlags flags = 0);
    LmdbTransaction(Lmdb & db, LmdbOpenFlags flags = 0);
    virtual ~LmdbTransaction();

public:
    /** Create a transaction for use with the environment. */
    Err begin(Lmdb & db, LmdbTransaction & parent, LmdbOpenFlags flags = 0);
    Err begin(Lmdb & db, LmdbOpenFlags flags = 0);

public:
    /** Commit all the operations of a transaction into the database. */
    Err commit();

    /** Abandon all the operations of the transaction instead of saving them. */
    void abort();
};

/**
 * A handle for an individual database in the DB environment.
 *
 * @author zer0
 * @date   2017-06-04
 */
class TBAG_API LmdbIndividualDatabase
{
private:
    unsigned int _index;

public:
    LmdbIndividualDatabase();
    virtual ~LmdbIndividualDatabase();

public:
    inline unsigned int getIndex() const TBAG_NOEXCEPT
    { return _index; }

public:
    /** Open a database in the environment. */
    Err open(LmdbTransaction & txn, std::string const & name, DbiOpenFlags flags = 0);

    /** Close a database handle. Normally unnecessary. */
    void close(Lmdb & db);

public:
    Err put(LmdbTransaction & txn,
            void const * key_data, std::size_t key_size,
            void const * value_data, std::size_t value_size,
            unsigned int flags = 0);
    Err put(LmdbTransaction & txn,
            std::vector<char> const & key,
            std::vector<char> const & value,
            unsigned int flags = 0);
    Err put(LmdbTransaction & txn,
            std::string const & key,
            std::string const & value,
            unsigned int flags = 0);
};

#ifndef TBAG_LMDB_CURSOR_OP_MAP
#define TBAG_LMDB_CURSOR_OP_MAP(_TBAG_XX) \
    _TBAG_XX(_FIRST         , "Position at first key/data item"                     ) \
    _TBAG_XX(_FIRST_DUP     , "Position at first data item of current key."         ) \
    _TBAG_XX(_GET_BOTH      , "Position at key/data pair. Only for #MDB_DUPSORT"    ) \
    _TBAG_XX(_GET_BOTH_RANGE, "position at key, nearest data. Only for #MDB_DUPSORT") \
    _TBAG_XX(_GET_CURRENT   , "Return key/data at current cursor position"          ) \
    _TBAG_XX(_GET_MULTIPLE  , "Return key and up to a page of duplicate data items from current cursor position.") \
    _TBAG_XX(_LAST          , "Position at last key/data item"                      ) \
    _TBAG_XX(_LAST_DUP      , "Position at last data item of current key."          ) \
    _TBAG_XX(_NEXT          , "Position at next data item"                          ) \
    _TBAG_XX(_NEXT_DUP      , "Position at next data item of current key."          ) \
    _TBAG_XX(_NEXT_MULTIPLE , "Return key and up to a page of duplicate data items from next cursor position.") \
    _TBAG_XX(_NEXT_NODUP    , "Position at first data item of next key"             ) \
    _TBAG_XX(_PREV          , "Position at previous data item"                      ) \
    _TBAG_XX(_PREV_DUP      , "Position at previous data item of current key."      ) \
    _TBAG_XX(_PREV_NODUP    , "Position at last data item of previous key"          ) \
    _TBAG_XX(_SET           , "Position at specified key"                           ) \
    _TBAG_XX(_SET_KEY       , "Position at specified key, return key + data"        ) \
    _TBAG_XX(_SET_RANGE     , "Position at first key greater than or equal to specified key.") \
    /* -- END -- */
#endif

/**
 * Cursor Get operations.
 *
 * @remarks
 *  This is the set of all operations for retrieving data using a cursor.
 */
enum class LmdbCursorOperations
{
#define _TBAG_XX(name, msg) CO##name,
    TBAG_LMDB_CURSOR_OP_MAP(_TBAG_XX)
#undef _TBAG_XX
};

/**
 * Opaque structure for navigating through a database.
 *
 * @author zer0
 * @date   2017-06-04
 */
class TBAG_API LmdbCursor : public container::Pointer<void>, private Noncopyable
{
public:
    using Parent = container::Pointer<void>;

public:
    LmdbCursor();
    LmdbCursor(LmdbTransaction & txn, LmdbIndividualDatabase & dbi);
    virtual ~LmdbCursor();

public:
    /** Create a cursor handle. */
    Err open(LmdbTransaction & txn, LmdbIndividualDatabase & dbi);

    /** Close a cursor handle. */
    void close();

public:
    Err get(void const * key_data, std::size_t key_size,
            void * value_data, std::size_t value_size,
            LmdbCursorOperations op);
    Err get(std::vector<char> const & key,
            std::vector<char> & value,
            LmdbCursorOperations op);
    Err get(std::string const & key,
            std::string & value,
            LmdbCursorOperations op);
};

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_LMDB_HPP__

