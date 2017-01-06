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

void TcpLoop::safeAsync(AsyncHelper::ActionType type)
{
    auto shared = _loop.newHandle<AsyncHelper>(*this, type);
    if (static_cast<bool>(shared)) {
        shared->send();
    }
}

void TcpLoop::safeClose()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->close();
    } else {
        safeAsync(AsyncHelper::ActionType::CLOSE);
    }
}

void TcpLoop::safeStartRead()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->close();
    } else {
        safeAsync(AsyncHelper::ActionType::START_READ);
    }
}

void TcpLoop::safeStopRead()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->close();
    } else {
        safeAsync(AsyncHelper::ActionType::STOP_READ);
    }
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

