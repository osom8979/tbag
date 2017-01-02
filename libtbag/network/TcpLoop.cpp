/**
 * @file   TcpLoop.cpp
 * @brief  TcpLoop class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpLoop.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpLoop::TcpLoop() : _loop()
{
    Parent::setTcp(_loop.createAndInsertChildHandle<CallableTcp>(_loop, this));
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

