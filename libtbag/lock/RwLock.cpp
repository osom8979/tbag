/**
 * @file   RwLock.cpp
 * @brief  RwLock class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/lock/RwLock.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_rwlock_t * cast_rwlock(T * v) TBAG_NOEXCEPT
{
    return static_cast<uv_rwlock_t*>(v);
}

// ----------------------
// RwLock implementation.
// ----------------------

RwLock::RwLock() throw (InitializeException) : _handle(new uv_rwlock_t)
{
    int error_code = ::uv_rwlock_init(cast_rwlock(_handle));
    if (error_code != 0) {
        throw InitializeException(error_code);
    }
}

RwLock::~RwLock()
{
    ::uv_rwlock_destroy(cast_rwlock(_handle));
    delete cast_rwlock(_handle);
}

void RwLock::readLock()
{
    ::uv_rwlock_rdlock(cast_rwlock(_handle));
}

bool RwLock::tryReadLock()
{
    return ::uv_rwlock_tryrdlock(cast_rwlock(_handle)) == 0;
}

void RwLock::readUnlock()
{
    ::uv_rwlock_rdunlock(cast_rwlock(_handle));
}

void RwLock::writeLock()
{
    ::uv_rwlock_wrlock(cast_rwlock(_handle));
}

bool RwLock::tryWriteLock()
{
    return ::uv_rwlock_trywrlock(cast_rwlock(_handle)) == 0;
}

void RwLock::writeUnlock()
{
    ::uv_rwlock_wrunlock(cast_rwlock(_handle));
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

