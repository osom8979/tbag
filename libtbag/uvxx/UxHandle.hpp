/**
 * @file   UxHandle.hpp
 * @brief  UxHandle class prototype.
 * @author zer0
 * @date   2018-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXHANDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXHANDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace uvpp { class Loop; }

namespace uvxx {

/**
 * UxHandle class prototype.
 *
 * @author zer0
 * @date   2018-12-06
 */
class TBAG_API UxHandle
{
public:
    using Loop       = libtbag::uvpp::Loop;
    using Handle     = libtbag::uvpp::Handle;
    using WeakHandle = std::weak_ptr<Handle>;

protected:
    WeakHandle _handle;

public:
    UxHandle();
    UxHandle(UxHandle const & obj) TBAG_NOEXCEPT;
    UxHandle(UxHandle && obj) TBAG_NOEXCEPT;
    ~UxHandle();

public:
    UxHandle & operator =(UxHandle const & obj) TBAG_NOEXCEPT;
    UxHandle & operator =(UxHandle && obj) TBAG_NOEXCEPT;

public:
    void copy(UxHandle const & obj) TBAG_NOEXCEPT;
    void swap(UxHandle & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxHandle & lh, UxHandle & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return !_handle.expired(); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    void release();

public:
    char const * getName() const;

public:
    Loop * getLoop();
    Loop const * getLoop() const;

public:
    void close();

public:
    bool isInit() const;
    bool isActive() const;
    bool isClosing() const;
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXHANDLE_HPP__

