/**
 * @file   Handle.hpp
 * @brief  Handle class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Native.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

/**
 * libuv handle type utility class.
 *
 * @author zer0
 * @date   2016-12-17
 * @date   2016-12-27 (Move package: libtbag/util -> libtbag/uv)
 */
class TBAG_API Handle : public Native
{
public:
    using Parent = Native;

public:
    Handle(UvHandleType type);
    virtual ~Handle();

public:
    inline char const * getName() const TBAG_NOEXCEPT
    { return getUvNativeHandleName(getNative()); }

public:
    /**
     * @remarks
     *  This function should only be used between the
     *  initialization of the handle
     *  and the arrival of the close callback.
     */
    void close();

public:
    /** Returns true if the handle is active, false if it’s inactive. */
    bool isActive() const TBAG_NOEXCEPT;

    /** Returns true if the handle is closing or closed, false otherwise. */
    bool isClosing() const TBAG_NOEXCEPT;

    /** Reference the given handle. */
    void ref() TBAG_NOEXCEPT;

    /** Un-reference the given handle. */
    void unref() TBAG_NOEXCEPT;

    /** Returns true if the handle referenced, false otherwise. */
    bool hasRef() const TBAG_NOEXCEPT;

    /** Returns the size of the given handle type. */
    std::size_t getNativeSize() const  TBAG_NOEXCEPT;

public:
    virtual void onClose();
};


} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_HANDLE_HPP__

