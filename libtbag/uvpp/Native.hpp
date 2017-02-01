/**
 * @file   Native.hpp
 * @brief  Native class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_NATIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_NATIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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
    { return uvpp::isHandle(TYPE); }
    inline bool isRequest() const TBAG_NOEXCEPT
    { return uvpp::isRequest(TYPE); }
    inline bool isEtc() const TBAG_NOEXCEPT
    { return uvpp::isEtc(TYPE); }
    inline bool isStream() const TBAG_NOEXCEPT
    { return uvpp::isStream(TYPE); }
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_NATIVE_HPP__

