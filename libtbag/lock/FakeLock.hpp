/**
 * @file   FakeLock.hpp
 * @brief  FakeLock class prototype.
 * @author zer0
 * @date   2016-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_FAKELOCK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_FAKELOCK_HPP__

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
 * FakeLock class prototype.
 *
 * @author zer0
 * @date   2016-05-22
 */
class FakeLock : private Noncopyable
{
public:
    FakeLock() = default;
    ~FakeLock() = default;

public:
    inline void lock() TBAG_NOEXCEPT
    { /* EMPTY. */ }

    inline bool try_lock() TBAG_NOEXCEPT
    { return false; }

    inline void unlock() TBAG_NOEXCEPT
    { /* EMPTY. */ }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_FAKELOCK_HPP__

