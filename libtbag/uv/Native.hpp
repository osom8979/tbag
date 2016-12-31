/**
 * @file   Native.hpp
 * @brief  Native class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_NATIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_NATIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/uv/Type.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

/**
 * libuv native type utility class.
 *
 * @author zer0
 * @date   2016-12-07
 * @date   2016-12-27 (Move package: libtbag/util -> libtbag/uv)
 */
class TBAG_API Native : public container::Pointer<void>, public Noncopyable
{
public:
    using Parent = container::Pointer<void>;

private:
    UvType const TYPE;
    void * _user;

public:
    explicit Native(UvType type);
    explicit Native(UvPodType type);
    virtual ~Native();

public:
    inline UvType getType() const TBAG_NOEXCEPT
    { return TYPE; }

public:
    inline void setUserData(void * data) TBAG_NOEXCEPT
    { _user = data; }
    inline void * getUserData() TBAG_NOEXCEPT
    { return _user; }
    inline void const * getUserData() const TBAG_NOEXCEPT
    { return _user; }

public:
    inline bool isHandle() const TBAG_NOEXCEPT
    { return uv::isHandle(TYPE); }
    inline bool isRequest() const TBAG_NOEXCEPT
    { return uv::isRequest(TYPE); }
    inline bool isEtc() const TBAG_NOEXCEPT
    { return uv::isEtc(TYPE); }
    inline bool isStream() const TBAG_NOEXCEPT
    { return uv::isStream(TYPE); }
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_NATIVE_HPP__

