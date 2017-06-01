/**
 * @file   RwLock.hpp
 * @brief  RwLock class prototype.
 * @author zer0
 * @date   2016-05-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_RWLOCK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_RWLOCK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

/**
 * RwLock class prototype.
 *
 * @author zer0
 * @date   2016-05-12
 *
 * @remarks
 *  Use the libuv.
 */
class TBAG_API RwLock : private Noncopyable
{
private:
    void * _handle;

public:
    RwLock();
    ~RwLock();

public:
    void readLock();
    bool tryReadLock();
    void readUnlock();

public:
    void writeLock();
    bool tryWriteLock();
    void writeUnlock();
};

/**
 * Read Lock/Unlock class.
 *
 * @author zer0
 * @date   2016-05-12
 */
class ReadLockGuard : private Noncopyable
{
private:
    RwLock & _lock;

public:
    inline ReadLockGuard(RwLock & lock) : _lock(lock)
    { _lock.readLock(); }
    inline ~ReadLockGuard()
    { _lock.readUnlock(); }
};

/**
 * Write Lock/Unlock class.
 *
 * @author zer0
 * @date   2016-05-12
 */
class WriteLockGuard : private Noncopyable
{
private:
    RwLock & _lock;

public:
    inline WriteLockGuard(RwLock & lock) : _lock(lock)
    { _lock.writeLock(); }
    inline ~WriteLockGuard()
    { _lock.writeUnlock(); }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_RWLOCK_HPP__

