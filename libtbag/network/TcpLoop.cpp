/**
 * @file   TcpLoop.cpp
 * @brief  TcpLoop class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpLoop.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Request.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpLoop::TcpLoop()
{
    // EMPTY.
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

TcpLoop::WriteRequest & TcpLoop::obtainWriteRequest(Tcp & tcp)
{
    auto weak = _writers.create(tcp);
    assert(weak.expired() == false);
    return *static_cast<WriteRequest*>(weak.lock().get());
}

void TcpLoop::releaseWriteRequest(WriteRequest & request)
{
    _writers.release(static_cast<uv::Request*>(&request));
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

