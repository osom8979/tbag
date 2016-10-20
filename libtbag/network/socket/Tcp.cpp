/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-10-20
 */

#include <libtbag/network/socket/Tcp.hpp>
#include <cstdlib>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

Tcp::Tcp(Handler * handler) : _handler(handler), _tcp(new uv_tcp_t)
{
    ::memset(_tcp, 0x00, sizeof(uv_tcp_t));
    _handler->add(_tcp);
}

Tcp::~Tcp()
{
    _handler->remove(_tcp);
    delete _tcp;
}

} // namespace network
} // namespace socket

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

