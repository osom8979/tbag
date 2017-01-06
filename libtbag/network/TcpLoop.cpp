/**
 * @file   TcpLoop.cpp
 * @brief  TcpLoop class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpLoop.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpLoop::TcpLoop() : _loop()
{
    Parent::setTcp(_loop.newHandle<CallableTcp>(_loop, this));
    _async_close      = _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::CLOSE);
    _async_start_read = _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::START_READ);
    _async_stop_read  = _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::STOP_READ);
}

TcpLoop::~TcpLoop()
{
    _async_close.reset();
    _async_start_read.reset();
    _async_stop_read.reset();
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

