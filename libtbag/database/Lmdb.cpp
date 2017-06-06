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

// -----------------------
// LmdbTxn implementation.
// -----------------------

LmdbTxn::LmdbTxn(Lmdb & db) : Parent(nullptr), _db(db), _end(false)
{
    // EMPTY.
}

LmdbTxn::LmdbTxn(Lmdb & db, LmdbTxn & parent, LmdbOpenFlags flags) : Parent(nullptr), _db(db), _end(false)
{
    if (begin(parent, flags) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbTxn::LmdbTxn(Lmdb & db, LmdbOpenFlags flags) : Parent(nullptr), _db(db), _end(false)
{
    if (begin(flags) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbTxn::~LmdbTxn()
{
    reset();
}

std::size_t LmdbTxn::getId() const
{
    return ::mdb_txn_id(cast_mdb_txn(ptr));
}

Err LmdbTxn::begin(LmdbTxn & parent, OpenFlags flags)
{
    int const CODE = ::mdb_txn_begin(cast_mdb_env(_db.get()), cast_mdb_txn(parent.get()),
                                     flags, reinterpret_cast<MDB_txn**>(&ptr));
    return convertMdbErrorToErrWithLogging("LmdbTxn::begin()", CODE);
}

Err LmdbTxn::begin(OpenFlags flags)
{
    int const CODE = ::mdb_txn_begin(cast_mdb_env(_db.get()), nullptr,
                                     flags, reinterpret_cast<MDB_txn**>(&ptr));
    return convertMdbErrorToErrWithLogging("LmdbTxn::begin()", CODE);
}

Err LmdbTxn::renew()
{
    int const CODE = ::mdb_txn_renew(cast_mdb_txn(ptr));
    return convertMdbErrorToErrWithLogging("LmdbTxn::renew()", CODE);
}

void LmdbTxn::reset()
{
    ::mdb_txn_reset(cast_mdb_txn(ptr));
}

Err LmdbTxn::commit()
{
    int const CODE = ::mdb_txn_commit(cast_mdb_txn(ptr));
    return convertMdbErrorToErrWithLogging("Lmdb::commit()", CODE);
}

void LmdbTxn::abort()
{
    ::mdb_txn_abort(cast_mdb_txn(ptr));
    ptr = nullptr;
}

// -----------------------
// LmdbDbi implementation.
// -----------------------

LmdbDbi::LmdbDbi(LmdbTxn & txn) : _txn(txn), _index(0)
{
    // EMPTY.
}

LmdbDbi::~LmdbDbi()
{
    close();
}

Err LmdbDbi::getFlags(unsigned int * flags) const
{
    int const CODE = ::mdb_dbi_flags(cast_mdb_txn(_txn.get()), _index, flags);
    return convertMdbErrorToErrWithLogging("LmdbDbi::getFlags()", CODE);
}

Err LmdbDbi::open(std::string const & name, OpenFlags flags)
{
    int const CODE = ::mdb_dbi_open(cast_mdb_txn(_txn.get()), name.c_str(), flags, (MDB_dbi*)&_index);
    return convertMdbErrorToErrWithLogging("LmdbDbi::open()", CODE);
}

void LmdbDbi::close()
{
    ::mdb_dbi_close(cast_mdb_env(_txn.getDb().get()), cast_mdb_dbi(_index));
    _index = 0;
}

Err LmdbDbi::drop(int del)
{
    int const CODE = ::mdb_drop(cast_mdb_txn(_txn.get()), cast_mdb_dbi(_index), del);
    return convertMdbErrorToErrWithLogging("LmdbDbi::drop()", CODE);
}

Err LmdbDbi::put(binf const & key, binf const & val, WriteFlags flags)
{
    MDB_val mdb_key;
    mdb_key.mv_data = key.buffer;
    mdb_key.mv_size = key.size;

    MDB_val mdb_val;
    mdb_val.mv_data = val.buffer;
    mdb_val.mv_size = val.size;

    int const CODE = ::mdb_put(cast_mdb_txn(_txn.get()), cast_mdb_dbi(_index), &mdb_key, &mdb_val, flags);
    return convertMdbErrorToErrWithLogging("LmdbDbi::put()", CODE);
}

Err LmdbDbi::put(Buffer const & key, Buffer const & val, WriteFlags flags)
{
    binf const KEY(const_cast<char*>(&key[0]), key.size());
    binf const VAL(const_cast<char*>(&val[0]), val.size());
    return put(KEY, VAL, flags);
}

Err LmdbDbi::put(std::string const & key, std::string const & val, WriteFlags flags)
{
    binf const KEY(const_cast<char*>(&key[0]), key.size());
    binf const VAL(const_cast<char*>(&val[0]), val.size());
    return put(KEY, VAL, flags);
}

Err LmdbDbi::get(binf & key, binf & val)
{
    MDB_val mdb_key = {0,};
    MDB_val mdb_val = {0,};

    int const CODE = ::mdb_get(cast_mdb_txn(_txn.get()), cast_mdb_dbi(_index), &mdb_key, &mdb_val);
    if (CODE == MDB_SUCCESS) {
        key.set(static_cast<char*>(mdb_key.mv_data), mdb_key.mv_size);
        val.set(static_cast<char*>(mdb_val.mv_data), mdb_val.mv_size);
    }
    return convertMdbErrorToErr(CODE);
}

Err LmdbDbi::get(Buffer & key, Buffer & val)
{
    binf binf_key, binf_val;
    Err const RESULT = get(binf_key, binf_val);
    if (RESULT == Err::E_SUCCESS) {
        key.assign(binf_key.buffer, binf_key.buffer + binf_key.size);
        val.assign(binf_val.buffer, binf_val.buffer + binf_val.size);
    }
    return RESULT;
}

Err LmdbDbi::get(std::string & key, std::string & val)
{
    binf binf_key, binf_val;
    Err const RESULT = get(binf_key, binf_val);
    if (RESULT == Err::E_SUCCESS) {
        key.assign(binf_key.buffer, binf_key.buffer + binf_key.size);
        val.assign(binf_val.buffer, binf_val.buffer + binf_val.size);
    }
    return RESULT;
}

Err LmdbDbi::del(binf const & key, binf & val)
{
    MDB_val mdb_key;
    mdb_key.mv_data = key.buffer;
    mdb_key.mv_size = key.size;

    MDB_val mdb_val;
    mdb_val.mv_data = val.buffer;
    mdb_val.mv_size = val.size;

    int const CODE = ::mdb_del(cast_mdb_txn(_txn.get()), cast_mdb_dbi(_index), &mdb_key, &mdb_val);
    return convertMdbErrorToErrWithLogging("LmdbDbi::del()", CODE);
}

Err LmdbDbi::del(Buffer const & key, Buffer & val)
{
    binf result((&val[0]), val.size());
    return del(binf(const_cast<char*>(&key[0]), key.size()), result);
}

Err LmdbDbi::del(std::string const & key, std::string & val)
{
    binf result((&val[0]), val.size());
    return del(binf(const_cast<char*>(&key[0]), key.size()), result);
}

// --------------------------
// LmdbCursor implementation.
// --------------------------

LmdbCursor::LmdbCursor(LmdbTxn & txn, LmdbDbi & dbi) : Parent(nullptr), _txn(txn), _dbi(dbi)
{
    if (open() != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

LmdbCursor::~LmdbCursor()
{
    close();
}

Err LmdbCursor::open()
{
    int const CODE = ::mdb_cursor_open(cast_mdb_txn(_txn.get()), _dbi.getIndex(), reinterpret_cast<MDB_cursor**>(&ptr));
    return convertMdbErrorToErrWithLogging("LmdbCursor::open()", CODE);
}

void LmdbCursor::close()
{
    if (ptr != nullptr) {
        ::mdb_cursor_close(cast_mdb_cursor(ptr));
        ptr = nullptr;
    }
}

Err LmdbCursor::get(binf & key, binf & val, Operation op)
{
    MDB_val mdb_key = {0,};
    MDB_val mdb_val = {0,};

    int const CODE = ::mdb_cursor_get(cast_mdb_cursor(ptr), &mdb_key, &mdb_val, __impl::convertCursorOp(op));
    if (CODE == MDB_SUCCESS) {
        key.set(static_cast<char*>(mdb_key.mv_data), mdb_key.mv_size);
        val.set(static_cast<char*>(mdb_val.mv_data), mdb_val.mv_size);
    }
    return convertMdbErrorToErr(CODE);
}

Err LmdbCursor::get(Buffer & key, Buffer & val, Operation op)
{
    binf binf_key, binf_val;
    Err const RESULT = get(binf_key, binf_val, op);
    if (RESULT == Err::E_SUCCESS) {
        key.assign(binf_key.buffer, binf_key.buffer + binf_key.size);
        val.assign(binf_val.buffer, binf_val.buffer + binf_val.size);
    }
    return RESULT;
}

Err LmdbCursor::get(std::string & key, std::string & val, Operation op)
{
    binf binf_key, binf_val;
    Err const RESULT = get(binf_key, binf_val, op);
    if (RESULT == Err::E_SUCCESS) {
        key.assign(binf_key.buffer, binf_key.buffer + binf_key.size);
        val.assign(binf_val.buffer, binf_val.buffer + binf_val.size);
    }
    return RESULT;
}

Err LmdbCursor::renew()
{
    int const CODE = ::mdb_cursor_renew(cast_mdb_txn(_txn.get()), cast_mdb_cursor(ptr));
    return convertMdbErrorToErrWithLogging("LmdbCursor::renew()", CODE);
}

Err LmdbCursor::put(binf const & key, binf const & val, WriteFlags flags)
{
    MDB_val mdb_key;
    mdb_key.mv_data = key.buffer;
    mdb_key.mv_size = key.size;

    MDB_val mdb_val;
    mdb_val.mv_data = val.buffer;
    mdb_val.mv_size = val.size;

    int const CODE = ::mdb_cursor_put(cast_mdb_cursor(ptr), &mdb_key, &mdb_val, flags);
    return convertMdbErrorToErrWithLogging("LmdbDbi::put()", CODE);
}

Err LmdbCursor::put(Buffer const & key, Buffer const & val, WriteFlags flags)
{
    binf const KEY(const_cast<char*>(&key[0]), key.size());
    binf const VAL(const_cast<char*>(&val[0]), val.size());
    return put(KEY, VAL, flags);
}

Err LmdbCursor::put(std::string const & key, std::string const & val, WriteFlags flags)
{
    binf const KEY(const_cast<char*>(&key[0]), key.size());
    binf const VAL(const_cast<char*>(&val[0]), val.size());
    return put(KEY, VAL, flags);
}

Err LmdbCursor::del(unsigned int flags)
{
    int const CODE = ::mdb_cursor_del(cast_mdb_cursor(ptr), flags);
    return convertMdbErrorToErrWithLogging("LmdbDbi::put()", CODE);
}

Err LmdbCursor::count(std::size_t * result)
{
    int const CODE = ::mdb_cursor_count(cast_mdb_cursor(ptr), result);
    return convertMdbErrorToErrWithLogging("LmdbDbi::put()", CODE);
}

} // namespace database

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

