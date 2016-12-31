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

// -----------------------
// TcpLoop implementation.
// -----------------------

TcpLoop::TcpLoop() : _loop(), _tcp()
{
    _tcp = _loop.createAndInsertChildHandle<CallableTcp>(_loop, this);
    assert(static_cast<bool>(_tcp));
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

