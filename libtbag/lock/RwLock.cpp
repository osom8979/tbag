/**
 * @file   RwLock.cpp
 * @brief  RwLock class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/lock/RwLock.hpp>
#include <cassert>
#include <new> // std::nothrow

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_rwlock_t * uv_rwlock_cast(T * object)
{
    return static_cast<uv_rwlock_t*>(object);
}

// ----------------------
// RwLock implementation.
// ----------------------

RwLock::RwLock() throw (UnsupportedRwLockException) : _is_init(false), _lock(new uv_rwlock_t)
{
    int error_code = uv_rwlock_init(uv_rwlock_cast(_lock));
    if (error_code == 0) {
        _is_init = true;
    } else {
        throw UnsupportedRwLockException(error_code);
    }
}

RwLock::~RwLock()
{
    if (_is_init) {
        uv_rwlock_destroy(uv_rwlock_cast(_lock));
    }

    assert(_lock != nullptr);
    delete uv_rwlock_cast(_lock);
}

void RwLock::readLock()
{
    uv_rwlock_rdlock(uv_rwlock_cast(_lock));
}

bool RwLock::tryReadLock()
{
    return (uv_rwlock_tryrdlock(uv_rwlock_cast(_lock)) == 0 ? true : false);
}

void RwLock::readUnlock()
{
    uv_rwlock_rdunlock(uv_rwlock_cast(_lock));
}

void RwLock::writeLock()
{
    uv_rwlock_wrlock(uv_rwlock_cast(_lock));
}

bool RwLock::tryWriteLock()
{
    return (uv_rwlock_trywrlock(uv_rwlock_cast(_lock)) == 0 ? true : false);
}

void RwLock::writeUnlock()
{
    uv_rwlock_wrunlock(uv_rwlock_cast(_lock));
}

// -----------------------------
// ReadLockGuard implementation.
// -----------------------------

ReadLockGuard::ReadLockGuard(RwLock & lock) : _lock(lock)
{
    _lock.readLock();
}

ReadLockGuard::~ReadLockGuard()
{
    _lock.readUnlock();
}

// ------------------------------
// WriteLockGuard implementation.
// ------------------------------

WriteLockGuard::WriteLockGuard(RwLock & lock) : _lock(lock)
{
    _lock.writeLock();
}

WriteLockGuard::~WriteLockGuard()
{
    _lock.writeUnlock();
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

