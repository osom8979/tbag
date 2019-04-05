/**
 * @file   UvSemaphore.hpp
 * @brief  UvSemaphore class prototype.
 * @author zer0
 * @date   2017-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVSEMAPHORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVSEMAPHORE_HPP__

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
 * UvSemaphore class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API UvSemaphore : private Noncopyable
{
private:
    void * _handle;

public:
    UvSemaphore(unsigned int value);
    ~UvSemaphore();

public:
    inline void * getHandle() TBAG_NOEXCEPT
    { return _handle; }

public:
    void wait();
    bool tryWait();
    void post();

// Alias methods.
public:
    // clang-format off
    inline void lock   () { wait(); }
    inline bool tryLock() { return tryWait(); }
    inline void unlock () { post(); }
    // clang-format on
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_UVSEMAPHORE_HPP__

