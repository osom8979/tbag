/**
 * @file   UvLock.hpp
 * @brief  UvLock class prototype.
 * @author zer0
 * @date   2016-10-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVLOCK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVLOCK_HPP__

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
 * UvLock class prototype.
 *
 * @author zer0
 * @date   2016-10-12
 */
class TBAG_API UvLock : private Noncopyable
{
private:
    void * _handle;

public:
    UvLock();
    ~UvLock();

public:
    inline void * getHandle() TBAG_NOEXCEPT
    { return _handle; }

public:
    void lock();
    bool tryLock();
    void unlock();
};

/**
 * UvLock guard class template.
 *
 * @author zer0
 * @date   2018-10-19
 */
template <typename MutexType>
class UvLockGuard : private Noncopyable
{
public:
    struct AdoptLock { /* EMPTY. */ };

public:
    using Mutex = MutexType;

private:
    Mutex & _mutex;

public:
    explicit UvLockGuard(MutexType & mutex) : _mutex(mutex)
    {
        _mutex.lock();
    }

    explicit UvLockGuard(MutexType & mutex, AdoptLock UNUSED_PARAM(lock)) : _mutex(mutex)
    {
        // EMPTY.
    }

    explicit UvLockGuard(MutexType & mutex, bool lock) : _mutex(mutex)
    {
        if (lock) {
            _mutex.lock();
        }
    }

    ~UvLockGuard()
    {
        _mutex.unlock();
    }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVLOCK_HPP__

