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
#include <libtbag/Exception.hpp>

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
class TBAG_API UvLock : public Noncopyable
{
private:
    void * _handle;

public:
    UvLock() throw (InitializeException);
    ~UvLock();

public:
    inline void * getHandle() TBAG_NOEXCEPT
    { return _handle; }

public:
    void lock();
    bool tryLock();
    void unlock();
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVLOCK_HPP__

