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

/**
 * Pointer to implementation of @c uv_rwlock_t.
 *
 * @author zer0
 * @date   2016-10-12
 *
 * @remarks
 *  Use the libuv.
 */
struct RwLock::RwLockPimpl
{
    uv_rwlock_t rwlock;

    RwLockPimpl()
    {
        int error_code = ::uv_rwlock_init(&rwlock);
        if (error_code != 0) {
            throw UnsupportedRwLockException(error_code);
        }
    }

    ~RwLockPimpl()
    {
        ::uv_rwlock_destroy(&rwlock);
    }

    inline uv_rwlock_t * getp() noexcept
    { return &rwlock; }
};

// ----------------------
// RwLock implementation.
// ----------------------

RwLock::RwLock() throw (UnsupportedRwLockException) : _lock(new RwLockPimpl)
{
    // EMPTY.
}

RwLock::~RwLock()
{
    // EMPTY.
}

void RwLock::readLock()
{
    ::uv_rwlock_rdlock(_lock->getp());
}

bool RwLock::tryReadLock()
{
    return ::uv_rwlock_tryrdlock(_lock->getp()) == 0;
}

void RwLock::readUnlock()
{
    ::uv_rwlock_rdunlock(_lock->getp());
}

void RwLock::writeLock()
{
    ::uv_rwlock_wrlock(_lock->getp());
}

bool RwLock::tryWriteLock()
{
    return ::uv_rwlock_trywrlock(_lock->getp()) == 0;
}

void RwLock::writeUnlock()
{
    ::uv_rwlock_wrunlock(_lock->getp());
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

