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
#include <libtbag/util/BufferInfo.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

using  binf  = util::binf;
using cbinf = util::cbinf;

// ----------------
// LMDB Open flags:
// ----------------

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

// --------------------
// LMDB DBI Open flags:
// --------------------

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

// ---------------------
// LMDB DBI Write flags:
// ---------------------

#ifndef TBAG_LMDB_WRITE_FLAGS_MAP
#define TBAG_LMDB_WRITE_FLAGS_MAP(_TBAG_XX) \
    /* For put: Don't write if the key already exists. */               \
    _TBAG_XX(_NOOVERWRITE,    0x10, "LMDB_NOOVERWRITE")                 \
    /* Only for #MDB_DUPSORT */                                         \
    /* For put: don't write if the key and data pair already exist. */  \
    /* For mdb_cursor_del: remove all duplicate data items. */          \
    _TBAG_XX(_NODUPDATA  ,    0x20, "LMDB_NODUPDATA")                   \
    /* For mdb_cursor_put: overwrite the current key/data pair */       \
    _TBAG_XX(_CURRENT    ,    0x40, "LMDB_CURRENT")                     \
    /* For put: Just reserve space for data, don't copy it. */          \
    /* Return a pointer to the reserved space. */                       \
    _TBAG_XX(_RESERVE    , 0x10000, "LMDB_RESERVE")                     \
    /* Data is being appended, don't split full pages. */               \
    _TBAG_XX(_APPEND     , 0x20000, "LMDB_APPEND")                      \
    /* Duplicate data is being appended, don't split full pages. */     \
    _TBAG_XX(_APPENDDUP  , 0x40000, "LMDB_APPENDDUP")                   \
    /* Store multiple data items in one call. */                        \
    /* Only for #MDB_DUPFIXED. */                                       \
    _TBAG_XX(_MULTIPLE   , 0x80000, "LMDB_MULTIPLE")                    \
    /* -- END -- */
#endif

using LmdbWriteFlags = unsigned int;

#define _TBAG_XX(name, value, msg) TBAG_CONSTEXPR LmdbWriteFlags const TBAG_DBI_WF##name = value;
TBAG_LMDB_DBI_OPEN_FLAG_MAP(_TBAG_XX)
#undef _TBAG_XX

// ----------------------
// LMDB Cursor operators:
// ----------------------

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

// ------------------------
// LMDB classes prototypes.
// ------------------------

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
 *  All database operations require a transaction handle. Transactions may be read-only or read-write.
 */
class TBAG_API LmdbTxn : public container::Pointer<void>, private Noncopyable
{
public:
    using Parent = container::Pointer<void>;
    using OpenFlags = LmdbOpenFlags;

private:
    Lmdb & _db;
    bool _end;

public:
    LmdbTxn(Lmdb & db);
    LmdbTxn(Lmdb & db, LmdbTxn & parent, LmdbOpenFlags flags = 0);
    LmdbTxn(Lmdb & db, LmdbOpenFlags flags);
    virtual ~LmdbTxn();

public:
    inline Lmdb       & getDb()       TBAG_NOEXCEPT { return _db; }
    inline Lmdb const & getDb() const TBAG_NOEXCEPT { return _db; }

public:
    /** Return the transaction's ID. */
    std::size_t getId() const;

public:
    /** Create a transaction for use with the environment. */
    Err begin(LmdbTxn & parent, OpenFlags flags = 0);
    Err begin(OpenFlags flags = 0);

public:
    /** Renew a read-only transaction. */
    Err renew();

    /** Reset a read-only transaction. */
    void reset();

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
class TBAG_API LmdbDbi : private Noncopyable
{
public:
    using Buffer = std::vector<char>;
    using OpenFlags = LmdbOpenFlags;
    using WriteFlags = LmdbWriteFlags;

private:
    LmdbTxn & _txn;
    unsigned int _index;

public:
    LmdbDbi(LmdbTxn & txn);
    virtual ~LmdbDbi();

public:
    inline unsigned int getIndex() const TBAG_NOEXCEPT
    { return _index; }
    inline operator unsigned int() const TBAG_NOEXCEPT
    { return _index; }

    /** Retrieve the DB flags for a database handle. */
    Err getFlags(unsigned int * flags) const;

public:
    /** Open a database in the environment. */
    Err open(std::string const & name, OpenFlags flags = 0);

    /** Close a database handle. Normally unnecessary. */
    void close();

    /** Empty or delete+close a database. */
    Err drop(int del);

public:
    // /** Retrieve statistics for a database. */
    // Err stat(MDB_stat *stat);

    // /** Set a custom key comparison function for a database. */
    // Err setCompare(MDB_cmp_func *cmp);

    // /** Set a custom data comparison function for a #MDB_DUPSORT database. */
    // Err setDupsort(MDB_cmp_func *cmp);

    // /** Set a relocation function for a #MDB_FIXEDMAP database. */
    // Err setRelfunc(MDB_rel_func *rel);

    // /** Set a context pointer for a #MDB_FIXEDMAP database's relocation function. */
    // Err setRelctx(void * ctx);

public:
    Err put(binf const & key, binf const & val, WriteFlags flags = 0);
    Err put(Buffer const & key, Buffer const & val, WriteFlags flags = 0);
    Err put(std::string const & key, std::string const & val, WriteFlags flags = 0);

    /** Get items from a database. */
    Err get(binf & key, binf & val);
    Err get(Buffer & key, Buffer & val);
    Err get(std::string & key, std::string & val);

    /** Delete items from a database. */
    Err del(binf const & key, binf & val);
    Err del(Buffer const & key, Buffer & val);
    Err del(std::string const & key, std::string & val);
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
    using Buffer = std::vector<char>;
    using WriteFlags = LmdbWriteFlags;
    using Operation = LmdbCursorOperations;

private:
    LmdbTxn & _txn;
    LmdbDbi & _dbi;

public:
    LmdbCursor();
    LmdbCursor(LmdbTxn & txn, LmdbDbi & dbi);
    virtual ~LmdbCursor();

private:
    /** Create a cursor handle. */
    Err open();

    /** Close a cursor handle. */
    void close();

public:
    Err get(binf & key, binf & val, Operation op);
    Err get(Buffer & key, Buffer & val, Operation op);
    Err get(std::string & key, std::string & val, Operation op);

    /** Renew a cursor handle. */
    Err renew();

    /** Store by cursor. */
    Err put(binf const & key, binf const & val, WriteFlags flags = 0);
    Err put(Buffer const & key, Buffer const & val, WriteFlags flags = 0);
    Err put(std::string const & key, std::string const & val, WriteFlags flags = 0);

    /** Delete current key/data pair */
    Err del(unsigned int flags);

    /** Return count of duplicates for current key. */
    Err count(std::size_t * result);
};

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DATABASE_LMDB_HPP__

