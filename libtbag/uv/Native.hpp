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
class TBAG_API Native : public Noncopyable
{
public:
    using Type = UvType;

private:
    Type const TYPE;
    void * _native;
    void * _user;

public:
    Native(Type type);
    virtual ~Native();

public:
    bool isInit() const;

public:
    inline operator bool() const TBAG_NOEXCEPT
    { return _native != nullptr; }

public:
    inline Type getType() const TBAG_NOEXCEPT
    { return TYPE; }
    inline bool isHandle() const TBAG_NOEXCEPT
    { return uv::isHandle(TYPE); }
    inline bool isRequest() const TBAG_NOEXCEPT
    { return uv::isRequest(TYPE); }
    inline bool isEtc() const TBAG_NOEXCEPT
    { return uv::isEtc(TYPE); }
    inline bool isStream() const TBAG_NOEXCEPT
    { return uv::isStream(TYPE); }

public:
    inline void * getNative() TBAG_NOEXCEPT
    { return _native; }
    inline void const * getNative() const TBAG_NOEXCEPT
    { return _native; }

public:
    inline void setUserData(void * data) TBAG_NOEXCEPT
    { _user = data; }
    inline void * getUserData() TBAG_NOEXCEPT
    { return _user; }
    inline void const * getUserData() const TBAG_NOEXCEPT
    { return _user; }

public:
    template <typename T>
    inline T * castNative() const TBAG_NOEXCEPT
    { return static_cast<T*>(_native); }
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_NATIVE_HPP__

