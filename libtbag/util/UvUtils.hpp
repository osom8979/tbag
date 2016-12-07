/**
 * @file   UvUtils.hpp
 * @brief  UvUtils class prototype.
 * @author zer0
 * @date   2016-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__

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

namespace util {

TBAG_API void initUv();
TBAG_API char const * getUvType(void * handle);

/**
 * Table of libuv handle types.
 *
 * @author zer0
 * @date   2016-12-07
 */
enum class UvType : int
{
    // Handle types.
    LOOP,
    HANDLE,
    STREAM,
    TCP,
    UDP,
    PIPE,
    TTY,
    POLL,
    TIMER,
    PREPARE,
    CHECK,
    IDLE,
    ASYNC,
    PROCESS,
    FS_EVENT,
    FS_POLL,
    SIGNAL,

    // Request types.
    REQ,
    GETADDRINFO,
    GETNAMEINFO,
    SHUTDOWN,
    WRITE,
    CONNECT,
    UDP_SEND,
    FS,
    WORK,

    // None of the above.
    CPU_INFO,
    INTERFACE_ADDRESS,
    DIRENT,
    PASSWD,
};

/**
 * libuv handle utility class.
 *
 * @author zer0
 * @date   2016-12-07
 */
class TBAG_API UvHandle
{
private:
    UvType const TYPE;
    void * _native;

public:
    UvHandle(UvType type);
    ~UvHandle();

public:
    inline operator bool() const TBAG_NOEXCEPT
    { return _native != nullptr; }

public:
    inline UvType getType() const TBAG_NOEXCEPT
    { return TYPE; }

public:
    inline void * getNative() TBAG_NOEXCEPT
    { return _native; }
    inline void const * getNative() const TBAG_NOEXCEPT
    { return _native; }

public:
    template <typename T>
    inline T * castNative() TBAG_NOEXCEPT
    { return static_cast<T*>(_native); }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_UVUTILS_HPP__

