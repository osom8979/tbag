/**
 * @file   Tcp.hpp
 * @brief  Tcp class prototype.
 * @author zer0
 * @date   2016-10-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/macro/attributes.hpp>
#include <libtbag/loop/event/UvEventHandler.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Tcp class prototype.
 *
 * @author zer0
 * @date   2016-10-20
 *
 * @remarks
 *  Pointer to implementation of @c uv_tcp_t.
 */
class TBAG_EXPORTS Tcp : public Noncopyable
{
public:
    using Handler = libtbag::loop::event::UvEventHandler;

private:
    Handler * _handler;
    void * _tcp;

public:
    Tcp(Handler * handler);
    ~Tcp();

public:
    inline void * getNative() noexcept
    { return _tcp; }
    inline void const * getNative() const noexcept
    { return _tcp; }
};

} // namespace network
} // namespace socket

#ifndef TBAG_TCP_NATIVE_CASTING
#define TBAG_TCP_NATIVE_CASTING(v) \
    static_cast<uv_tcp_t*>(v.getNative())
#endif

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

