/**
 * @file   Request.hpp
 * @brief  Request class prototype.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_REQUEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_REQUEST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Native.hpp>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Handle;

/**
 * Request class prototype.
 *
 * @author zer0
 * @date   2016-12-27
 */
class TBAG_API Request : public Native
{
public:
    using Parent = Native;

private:
    Handle * _owner;

public:
    Request(ureq type, Handle * owner = nullptr);
    virtual ~Request();

public:
    inline void setOwner(Handle * owner) TBAG_NOEXCEPT
    { _owner = owner; }
    inline Handle * getOwner() const TBAG_NOEXCEPT
    { return _owner; }

public:
    /**
     * Cancel a <strong>pending</strong> request.
     *
     * @remarks
     *  Fails if the request is executing or has finished executing.
     */
    Err cancel();

    /** Returns the size of the given request type. */
    std::size_t getNativeSize() const TBAG_NOEXCEPT;
};

#ifndef _TBAG_UV_REQUEST_EX
#define _TBAG_UV_REQUEST_EX(type, name)         \
    struct name##Request : public Request {     \
        name##Request(Handle * owner = nullptr) \
            : Request(ureq::type, owner) { }    \
        ~name##Request() { }                    \
    }
#endif

_TBAG_UV_REQUEST_EX(CONNECT , Connect );
_TBAG_UV_REQUEST_EX(WRITE   , Write   );
_TBAG_UV_REQUEST_EX(SHUTDOWN, Shutdown);
_TBAG_UV_REQUEST_EX(UDP_SEND, UdpSend );
_TBAG_UV_REQUEST_EX(FS      , Fs      );
_TBAG_UV_REQUEST_EX(WORK    , Work    );

#undef _TBAG_UV_REQUEST_EX

template <typename T, bool is_base>
struct __is_request_type;

template <typename T>
struct __is_request_type<T, true> : public std::true_type
{ /* EMPTY. */ };

template <typename T>
struct __is_request_type<T, false> : public std::false_type
{ /* EMPTY. */ };

template <typename T>
struct is_request_type : public __is_request_type<T, std::is_base_of<Request, T>::value>
{ /* EMPTY. */ };

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_REQUEST_HPP__

