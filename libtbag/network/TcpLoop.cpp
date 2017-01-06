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
}

TcpLoop::~TcpLoop()
{
    // EMPTY.
}

void TcpLoop::asyncAction(AsyncTcpHelper::ActionType type)
{
    if (_loop.getOwnerThreadId() == std::this_thread::get_id()) {
        atTcp()->close();
    } else {
        _loop.newHandle<AsyncTcpHelper>(*this, type);
    }
}

void TcpLoop::asyncClose()
{
    asyncAction(AsyncTcpHelper::ActionType::CLOSE);
}

void TcpLoop::asyncStartRead()
{
    asyncAction(AsyncTcpHelper::ActionType::START_READ);
}

void TcpLoop::asyncStopRead()
{
    asyncAction(AsyncTcpHelper::ActionType::STOP_READ);
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

