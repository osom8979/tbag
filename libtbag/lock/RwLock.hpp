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

#include <memory>
#include <exception>

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
public:
    static constexpr char const * const MESSAGE = "Unsupported RwLock exception.";

private:
    int _error_code;

public:
    UnsupportedRwLockException(int error_code) : _error_code(error_code)
    { /* EMPTY. */ }

public:
    inline int getErrorCode() const noexcept
    { return _error_code; }

public:
    virtual const char * what() const noexcept override
    { return MESSAGE; }
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
public:
    struct RwLockPimpl;
    using Lock = std::unique_ptr<RwLockPimpl>;

private:
    Lock _lock;

public:
    RwLock() throw (UnsupportedRwLockException);
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
class ReadLockGuard : public Noncopyable
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
class WriteLockGuard : public Noncopyable
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

