/**
 * @file   Native.hpp
 * @brief  Native class prototype.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
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
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */
class TBAG_API Native : public container::Pointer<void>, private Noncopyable
{
public:
    using Parent = container::Pointer<void>;

private:
    utype const TYPE;
    void * _user;

public:
    explicit Native(utype type);
    explicit Native(int type);
    virtual ~Native();

public:
    inline utype getType() const TBAG_NOEXCEPT
    { return TYPE; }

public:
    // @formatter:off
    inline void         setUserData(void * data) TBAG_NOEXCEPT { _user = data; }
    inline void       * getUserData()            TBAG_NOEXCEPT { return _user; }
    inline void const * getUserData()      const TBAG_NOEXCEPT { return _user; }
    // @formatter:on

public:
    // @formatter:off
    inline bool isHandle () const TBAG_NOEXCEPT { return uvpp::isHandle (TYPE); }
    inline bool isRequest() const TBAG_NOEXCEPT { return uvpp::isRequest(TYPE); }
    inline bool isEtc    () const TBAG_NOEXCEPT { return uvpp::isEtc    (TYPE); }
    inline bool isStream () const TBAG_NOEXCEPT { return uvpp::isStream (TYPE); }
    // @formatter:on
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_NATIVE_HPP__

