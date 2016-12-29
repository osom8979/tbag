/**
 * @file   TcpLoop.cpp
 * @brief  TcpLoop class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpLoop.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpLoop::TcpLoop() : _loop(), _tcp(_loop, this)
{
    // EMPTY.
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

