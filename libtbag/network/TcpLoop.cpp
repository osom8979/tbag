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

void TcpLoop::safeClose()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->close();
    } else {
        _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::CLOSE);
    }
}

void TcpLoop::safeStartRead()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->startRead();
    } else {
        _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::START_READ);
    }
}

void TcpLoop::safeStopRead()
{
    if (isEqualOwnerThreadId()) {
        atTcp()->stopRead();
    } else {
        _loop.newHandle<AsyncHelper>(*this, AsyncHelper::ActionType::STOP_READ);
    }
}

TcpLoop::WriteRequest * TcpLoop::safeWrite(char const * buffer, std::size_t size)
{
    if (isEqualOwnerThreadId()) {
        return asyncWrite(buffer, size);
    } else {
        WriteRequest * request = obtainWriteRequest();
        _loop.newHandle<AsyncWriteHelper>(*this, request, buffer, size);
        return request;
    }
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

