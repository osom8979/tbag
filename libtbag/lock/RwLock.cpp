/**
 * @file   RwLock.cpp
 * @brief  RwLock class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/lock/RwLock.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Type.hpp>

#include <cassert>
#include <new>
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

RwLock::RwLock() : _handle(new (std::nothrow) uv_rwlock_t)
{
    assert(_handle != nullptr);
    int error_code = ::uv_rwlock_init(cast_rwlock(_handle));
    if (error_code != 0) {
        __tbag_error("RwLock::RwLock() error[{}] {}", error_code, uv::getUvErrorName(error_code));
    }
}

RwLock::~RwLock()
{
    assert(_handle != nullptr);
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

