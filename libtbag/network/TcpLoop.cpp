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
    auto weak = _loop.insertChildHandle(new uv::ex::CallableTcp(_loop, this));
    if (weak.expired()) {
        std::bad_alloc();
    }
    _tcp = std::static_pointer_cast<CallableTcp, uv::Handle>(weak.lock());
    assert(static_cast<bool>(_tcp));
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

// -------------------------
// TcpLoopEx implementation.
// -------------------------

TcpLoopEx::TcpLoopEx()
{
    // EMPTY.
}

TcpLoopEx::~TcpLoopEx()
{
    // EMPTY.
}

TcpLoopEx::binf TcpLoopEx::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    binf info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

