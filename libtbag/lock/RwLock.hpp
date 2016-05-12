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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

/**
 * Unsupported RwLock exception class.
 *
 * @author zer0
 * @date   2016-05-12
 */
struct UnsupportedRwLockException : public std::exception
{
private:
    int _error_code;

public:
    static constexpr char const * const MESSAGE = "Unsupported RwLock exception.";

public:
    UnsupportedRwLockException(int error_code) : _error_code(error_code) {
        __EMPTY_BLOCK__
    }

public:
    inline int getErrorCode() const noexcept {
        return _error_code;
    }

public:
    virtual const char * what() const noexcept override {
        return MESSAGE;
    }
};

/**
 * RwLock class prototype.
 *
 * @author zer0
 * @date   2016-05-12
 *
 * @remarks
 *  Use the libuv.
 */
class RwLock : public Noncopyable
{
private:
    bool _is_init;
    uv_rwlock_t _lock;

public:
    RwLock() throw (UnsupportedRwLockException) : _is_init(false) {
        int error_code = uv_rwlock_init(&_lock);
        if (error_code == 0) {
            _is_init = true;
        } else {
            throw UnsupportedRwLockException(error_code);
        }
    }

    ~RwLock() {
        if (_is_init) {
            uv_rwlock_destroy(&_lock);
        }
    }

public:
    void readLock() {
        uv_rwlock_rdlock(&_lock);
    }

    bool tryReadLock() {
        return (uv_rwlock_tryrdlock(&_lock) == 0 ? true : false);
    }

    void readUnlock() {
        uv_rwlock_rdunlock(&_lock);
    }

public:
    void writeLock() {
        uv_rwlock_wrlock(&_lock);
    }

    bool tryWriteLock() {
        return (uv_rwlock_trywrlock(&_lock) == 0 ? true : false);
    }

    void writeUnlock() {
        uv_rwlock_wrunlock(&_lock);
    }
};

/**
 * Read Lock/Unlock class.
 *
 * @author zer0
 * @date   2016-05-12
 */
class ReadLockGuard : public Noncopyable
{
private:
    RwLock & _lock;

public:
    ReadLockGuard(RwLock & lock) : _lock(lock) {
        _lock.readLock();
    }

    ~ReadLockGuard() {
        _lock.readUnlock();
    }
};

/**
 * Write Lock/Unlock class.
 *
 * @author zer0
 * @date   2016-05-12
 */
class WriteLockGuard : public Noncopyable
{
private:
    RwLock & _lock;

public:
    WriteLockGuard(RwLock & lock) : _lock(lock) {
        _lock.writeLock();
    }

    ~WriteLockGuard() {
        _lock.writeUnlock();
    }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_RWLOCK_HPP__

