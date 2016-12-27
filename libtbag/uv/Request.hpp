/**
 * @file   Request.hpp
 * @brief  Request class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_REQUEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_REQUEST_HPP__

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
 * Request class prototype.
 *
 * @author zer0
 * @date   2016-12-27
 */
class TBAG_API Request : public Native
{
public:
    using Parent = Native;

public:
    Request(UvRequsetType type);
    virtual ~Request();

public:
    /**
     * Cancel a pending request.
     *
     * @remarks
     *  Fails if the request is executing or has finished executing.
     */
    bool cancel();

    /** Returns the size of the given request type. */
    std::size_t getNativeSize() const TBAG_NOEXCEPT;
};

#ifndef _TBAG_UV_REQUEST_EX
#define _TBAG_UV_REQUEST_EX(type, name)                   \
    struct name##Request : public Request {               \
        name##Request() : Request(UvRequsetType::type) { } \
        ~name##Request() { }                              \
    }
#endif

_TBAG_UV_REQUEST_EX(CONNECT    , Connect    );
_TBAG_UV_REQUEST_EX(WRITE      , Write      );
_TBAG_UV_REQUEST_EX(SHUTDOWN   , Shutdown   );
_TBAG_UV_REQUEST_EX(UDP_SEND   , UdpSend    );
_TBAG_UV_REQUEST_EX(FS         , Fs         );
_TBAG_UV_REQUEST_EX(WORK       , Work       );
_TBAG_UV_REQUEST_EX(GETADDRINFO, GetAddrInfo);
_TBAG_UV_REQUEST_EX(GETNAMEINFO, GetNameInfo);

#undef _TBAG_UV_REQUEST_EX

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_REQUEST_HPP__

