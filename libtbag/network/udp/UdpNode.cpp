/**
 * @file   UdpNode.cpp
 * @brief  UdpNode class implementation.
 * @author zer0
 * @date   2017-06-15
 */

#include <libtbag/network/udp/UdpNode.hpp>
#include <libtbag/log/Log.hpp>
#include "UdpNode.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpNode::UdpNode(Loop & loop) : UdpNode(loop, SharedSafetyAsync())
{
    // EMPTY.
}

UdpNode::UdpNode(Loop & loop, SharedSafetyAsync async)
{
    _client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async.reset();
    }
}

UdpNode::~UdpNode()
{
    // EMPTY.
}

UdpNode::WeakClientBackend UdpNode::getClient()
{
    Guard g(_mutex);
    return WeakClientBackend(_client);
}

UdpNode::WeakSafetyAsync UdpNode::getAsync()
{
    Guard g(_mutex);
    return WeakSafetyAsync(_async);
}

// ------------------
// PROTECTED SECTION.
// ------------------

bool UdpNode::_initInternalHandles()
{
    assert(_client != nullptr);
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (static_cast<bool>(_async) == false) {
        _async = loop->newHandle<SafetyAsync>(*loop);
    }
    assert(static_cast<bool>(_async));
    return true;
}

Err UdpNode::_writeReal(binf const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));
    if (_addr.ipv4.sin_family == AF_INET) {
        return _client->send(_writer.send_req, const_cast<binf*>(buffer), size, &_addr.ipv4);
    } else if (_addr.ipv6.sin6_family == AF_INET6) {
        return _client->send(_writer.send_req, const_cast<binf*>(buffer), size, &_addr.ipv6);
    }
    return Err::E_EAFNOSUPPORT;
}

void UdpNode::_copyToWriteBuffer(binf const * buffer, std::size_t size)
{
    // COPY TO TEMP BUFFER ...
    _writer.buffers.resize(size);
    for (std::size_t i = 0; i < size; ++i) {
        char const * b = (buffer + i)->buffer;
        std::size_t bs = (buffer + i)->size;
        _writer.buffers[i].assign(b, b + bs);
    }
}

std::vector<UdpNode::binf> UdpNode::_getWriteBufferInfo()
{
    std::size_t SIZE = _writer.buffers.size();
    std::vector<binf> result(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i) {
        result[i].buffer = &_writer.buffers[i][0];
        result[i].size   =  _writer.buffers[i].size();
    }
    return result;
}

Err UdpNode::_autoWrite(binf const * buffer, std::size_t size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        Err const CODE = _writeReal(buffer, size);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("UdpNode::_autoWrite() Write {} error.", getErrName(CODE));
            return CODE;
        }

    } else {
        _copyToWriteBuffer(buffer, size);
        auto job = _async->newSendFunc(std::bind(&UdpNode::onAsyncWrite, this));
        if (static_cast<bool>(job) == false) {
            tDLogE("UdpNode::_autoWrite() New job error.");
            return Err::E_BADALLOC;
        }
    }

    return Err::E_SUCCESS;
}

void UdpNode::_closeAll()
{
    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }

    if (_owner_async && static_cast<bool>(_async)) {
        if (_async->isClosing() == false) {
            _async->close();
        }
        _async.reset();
    }
}

void UdpNode::onAsyncWrite()
{
    Guard guard(_mutex);

    auto binfs = _getWriteBufferInfo();
    Err const CODE = _writeReal(&binfs[0], binfs.size());

    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpNode::onAsyncWrite() {} error.", getErrName(CODE));
    }
}

UdpNode::Id UdpNode::getId() const
{
    return _client->id();
}

bool UdpNode::init(char const * destination, int port, uint64_t UNUSED_PARAM(millisec))
{
    Guard guard(_mutex);

    assert(static_cast<bool>(_client));
    if (static_cast<bool>(_client) == false) {
        return false;
    }

    // uvpp::initAddress(destination, port, &_addr.ipv4);

    bool is_init = false;
    if (is_init == false) {
        tDLogE("UdpNode::init() Initialize fail.");
        return false;
    }

    if (_initInternalHandles() == false) {
        tDLogE("UdpNode::init() Initialize fail (internal handles).");
        return false;
    }

    return true;
}

bool UdpNode::start()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->startRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpNode::start() {} error", getErrName(CODE));
        return false;
    }
    return true;
}

bool UdpNode::stop()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->stopRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpNode::stop() {} error", getErrName(CODE));
        return false;
    }
    return true;
}

void UdpNode::close()
{
    assert(static_cast<bool>(_client));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("UdpNode::close() request.");
        _closeAll();
    } else {
        tDLogD("UdpNode::close() async request.");
        _async->newSendFunc([&](){
            Guard guard(_mutex);
            _closeAll();
        });
    }
}

void UdpNode::cancel()
{
    // EMPTY.
}

bool UdpNode::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    Guard guard(_mutex);
    return _autoWrite(buffer, size, millisec) == Err::E_SUCCESS;
}

bool UdpNode::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;

    Guard guard(_mutex);
    return _autoWrite(&info, 1U, millisec) == Err::E_SUCCESS;
}

void * UdpNode::getUserData()
{
    assert(static_cast<bool>(_client));

    Guard guard(_mutex);
    return _client->getUserData();
}

std::string UdpNode::getDestination() const
{
    Guard guard(_mutex);
    return _client->getSockIp();
}

int UdpNode::getPort() const
{
    Guard guard(_mutex);
    return _client->getSockPort();
}

// --------------
// Event backend.
// --------------

void UdpNode::runBackendConnect(Err code)
{
    onConnect(code);
}

void UdpNode::runBackendShutdown(Err code)
{
    onShutdown(code);
}

void UdpNode::runBackendWrite(Err code)
{
    onWrite(code);
}

void UdpNode::runBackendRead(Err code, char const * buffer, std::size_t size,
                             sockaddr const * addr, unsigned int flags)
{
    onRead(code, buffer, size);
}

void UdpNode::runBackendClose()
{
    _mutex.lock();
    _closeAll();
    _mutex.unlock();
    onClose();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

