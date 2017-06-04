/**
 * @file   Lmdb.cpp
 * @brief  Lmdb class implementation.
 * @author zer0
 * @date   2017-06-04
 */

#include <libtbag/database/Lmdb.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <exception>
#include <iostream>

#include <lmdb.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace database {

// ---------------
namespace __impl {
// ---------------

inline static MDB_cursor_op convertCursorOp(LmdbCursorOperations op) TBAG_NOEXCEPT
{
    switch (op) {
#define _TBAG_XX(name, msg) case LmdbCursorOperations::CO##name: return MDB##name;
        TBAG_LMDB_CURSOR_OP_MAP(_TBAG_XX)
#undef _TBAG_XX
    }
}

// ------------------
} // namespace __impl
// ------------------

template <typename T>
inline MDB_env * cast_mdb_env(T * v) TBAG_NOEXCEPT
{
    return static_cast<MDB_env*>(v);
}

template <typename T>
inline MDB_txn * cast_mdb_txn(T * v) TBAG_NOEXCEPT
{
    return static_cast<MDB_txn*>(v);
}

STATIC_ASSERT_CHECK_IS_SAME(MDB_dbi, unsigned int);

template <typename T>
inline MDB_dbi cast_mdb_dbi(T v) TBAG_NOEXCEPT
{
    return static_cast<MDB_dbi>(v);
}

template <typename T>
inline MDB_cursor * cast_mdb_cursor(T * v) TBAG_NOEXCEPT
{
    return static_cast<MDB_cursor*>(v);
}

// --------------------
// Lmdb implementation.
// --------------------

Lmdb::Lmdb() : Parent(nullptr)
{
    if (create() != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

Lmdb::~Lmdb()
{
    close();
}

Err Lmdb::setMaxReaders(unsigned int size)
{
    int const CODE = ::mdb_env_set_maxreaders(cast_mdb_env(ptr), size);
    return convertMdbErrorToErrWithLogging("Lmdb::setMaxReaders()", CODE);
}

Err Lmdb::setMapSize(std::size_t size)
{
    int const CODE = ::mdb_env_set_mapsize(cast_mdb_env(ptr), size);
    return convertMdbErrorToErrWithLogging("Lmdb::setMapSize()", CODE);
}

Err Lmdb::setMaxIndividualDatabase(unsigned int size)
{
    int const CODE = ::mdb_env_set_maxdbs(cast_mdb_env(ptr), size);
    return convertMdbErrorToErrWithLogging("Lmdb::setMaxIndividualDatabase()", CODE);
}

Err Lmdb::create()
{
    int const CODE = ::mdb_env_create(reinterpret_cast<MDB_env**>(&ptr));
    return convertMdbErrorToErrWithLogging("Lmdb::create()", CODE);
}

void Lmdb::close()
{
    if (ptr != nullptr) {
        ::mdb_env_close(cast_mdb_env(ptr));
        ptr = nullptr;
    }
}

Err Lmdb::open(std::string const & path, LmdbOpenFlags flags, int mode)
{
    if (filesystem::Path(path).isDirectory() == false) {
        tDLogE("Lmdb::open() Not found directory.");
        return Err::E_ENOENT;
    }

    int const CODE = ::mdb_env_open(cast_mdb_env(ptr), path.c_str(), flags, static_cast<mdb_mode_t>(mode));
    return convertMdbErrorToErrWithLogging("Lmdb::open()", CODE);
}

// -------------------------------
// LmdbTransaction implementation.
// -------------------------------

LmdbTransaction::LmdbTransaction() : Parent(nullptr)
{
    // EMPTY.
}

LmdbTransaction::LmdbTransaction(Lmdb & db, LmdbTransaction & parent, LmdbOpenFlags flags) : Parent(nullptr)
{
    if (begin(db, parent, flags) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbTransaction::LmdbTransaction(Lmdb & db, LmdbOpenFlags flags) : Parent(nullptr)
{
    if (begin(db, flags) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbTransaction::~LmdbTransaction()
{
    // EMPTY.
}

Err LmdbTransaction::begin(Lmdb & db, LmdbTransaction & parent, LmdbOpenFlags flags)
{
    int const CODE = ::mdb_txn_begin(cast_mdb_env(db.get()), cast_mdb_txn(parent.get()),
                                     flags, reinterpret_cast<MDB_txn**>(&ptr));
    return convertMdbErrorToErrWithLogging("LmdbTransaction::begin()", CODE);
}

Err LmdbTransaction::begin(Lmdb & db, LmdbOpenFlags flags)
{
    LmdbTransaction NULL_PARENT;
    return begin(db, NULL_PARENT, flags);
}

Err LmdbTransaction::commit()
{
    int const CODE = ::mdb_txn_commit(cast_mdb_txn(ptr));
    return convertMdbErrorToErrWithLogging("Lmdb::commit()", CODE);
}

void LmdbTransaction::abort()
{
    ::mdb_txn_abort(cast_mdb_txn(ptr));
}

// --------------------------------------
// LmdbIndividualDatabase implementation.
// --------------------------------------

LmdbIndividualDatabase::LmdbIndividualDatabase() : _index(0)
{
    // EMPTY.
}

LmdbIndividualDatabase::~LmdbIndividualDatabase()
{
    // EMPTY.
}

Err LmdbIndividualDatabase::open(LmdbTransaction & txn, std::string const & name, DbiOpenFlags flags)
{
    int const CODE = ::mdb_dbi_open(cast_mdb_txn(txn.get()), name.c_str(), flags, (MDB_dbi*)&_index);
    return convertMdbErrorToErrWithLogging("LmdbIndividualDatabase::open()", CODE);
}

void LmdbIndividualDatabase::close(Lmdb & db)
{
    ::mdb_dbi_close(cast_mdb_env(db.get()), cast_mdb_dbi(_index));
}

Err LmdbIndividualDatabase::put(LmdbTransaction & txn,
                                void const * key_data, std::size_t key_size,
                                void const * value_data, std::size_t value_size,
                                unsigned int flags)
{
    MDB_val key;
    key.mv_data = const_cast<void*>(key_data);
    key.mv_size = key_size;

    MDB_val val;
    val.mv_data = const_cast<void*>(value_data);
    val.mv_size = value_size;

    int const CODE = ::mdb_put(cast_mdb_txn(txn.get()), cast_mdb_dbi(_index), &key, &val, flags);
    return convertMdbErrorToErrWithLogging("LmdbIndividualDatabase::put()", CODE);
}

Err LmdbIndividualDatabase::put(LmdbTransaction & txn,
                                std::vector<char> const & key,
                                std::vector<char> const & value,
                                unsigned int flags)
{
    return put(txn, (void const *)&key[0], key.size(), (void const *)&value[0], value.size(), flags);
}

Err LmdbIndividualDatabase::put(LmdbTransaction & txn,
                                std::string const & key,
                                std::string const & value,
                                unsigned int flags)
{
    return put(txn, (void const *)&key[0], key.size(), (void const *)&value[0], value.size(), flags);
}

// --------------------------
// LmdbCursor implementation.
// --------------------------

LmdbCursor::LmdbCursor() : Parent(nullptr)
{
    // EMPTY.
}

LmdbCursor::LmdbCursor(LmdbTransaction & txn, LmdbIndividualDatabase & dbi) : Parent(nullptr)
{
    if (open(txn, dbi) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbCursor::~LmdbCursor()
{
    close();
}

Err LmdbCursor::open(LmdbTransaction & txn, LmdbIndividualDatabase & dbi)
{
    int const CODE = ::mdb_cursor_open(cast_mdb_txn(txn.get()), dbi.getIndex(), reinterpret_cast<MDB_cursor**>(&ptr));
    return convertMdbErrorToErrWithLogging("LmdbCursor::open()", CODE);
}

void LmdbCursor::close()
{
    if (ptr != nullptr) {
        ::mdb_cursor_close(cast_mdb_cursor(ptr));
        ptr = nullptr;
    }
}

Err LmdbCursor::get(void const * key_data, std::size_t key_size,
                    void * value_data, std::size_t value_size,
                    LmdbCursorOperations op)
{
    MDB_val key;
    key.mv_data = const_cast<void*>(key_data);
    key.mv_size = key_size;

    MDB_val val;
    val.mv_data = value_data;
    val.mv_size = value_size;

    int const CODE = ::mdb_cursor_get(cast_mdb_cursor(ptr), &key, &val, __impl::convertCursorOp(op));
    return convertMdbErrorToErrWithLogging("LmdbCursor::get()", CODE);
}

Err LmdbCursor::get(std::vector<char> const & key,
                    std::vector<char> & value,
                    LmdbCursorOperations op)
{
    return get((void const *)&key[0], key.size(), (void*)&value[0], value.size(), op);
}

Err LmdbCursor::get(std::string const & key,
                    std::string & value,
                    LmdbCursorOperations op)
{
    return get((void const *)&key[0], key.size(), (void*)&value[0], value.size(), op);
}

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

