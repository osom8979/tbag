/**
 * @file   UdpServer.cpp
 * @brief  UdpServer class implementation.
 * @author zer0
 * @date   2017-06-19
 */

#include <libtbag/network/udp/UdpServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

UdpServer::UdpServer(Loop & loop) : _receiver(loop, this)
{
    // EMPTY.
}

UdpServer::~UdpServer()
{
    // EMPTY.
}

void UdpServer::closeAll()
{
    for (auto & cursor : _senders) {
        if (static_cast<bool>(cursor.second)) {
            cursor.second->close();
        }
    }
    _receiver.close();
}

UdpServer::WeakClient UdpServer::newSender(std::string const & destination, int port)
{
    Guard const MUTEX_GUARD(_mutex);
    auto receiver = _receiver.getClient().lock();
    if (static_cast<bool>(receiver) == false) {
        tDLogE("UdpServer::newSender() expired receiver.");
        return WeakClient();
    }

    auto async = _receiver.getAsync().lock();
    if (static_cast<bool>(async) == false) {
        tDLogE("UdpServer::newSender() expired async.");
        return WeakClient();
    }

    Loop * loop = receiver->getLoop();
    assert(loop != nullptr);

    auto sender = new (std::nothrow) UdpSender(*loop, async, this);
    assert(sender != nullptr);

    Err const INIT_CODE = sender->init(destination.c_str(), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        tDLogE("UdpServer::newSender() init {} error.", getErrName(INIT_CODE));
        return WeakClient();
    }

    auto client = SharedClient(sender);
    assert(static_cast<bool>(client));

    // Allocate User Data.
    if (auto udp = sender->getClient().lock()) {
        udp->setUserData(onClientUdataAlloc(WeakClient(client)));
    } else {
        tDLogW("UdpServer::newSender() udp client is nullptr.");
    }

    if (_senders.insert(ClientPair(client->id(), client)).second == false) {
        tDLogE("UdpServer::newSender() insert fail.");
        return WeakClient();
    }

    return WeakClient(client);
}

std::string UdpServer::dest() const
{
    Guard const MUTEX_GUARD(_mutex);
    return _receiver.dest();
}

int UdpServer::port() const
{
    Guard const MUTEX_GUARD(_mutex);
    return _receiver.port();
}

Err UdpServer::init(char const * destination, int port)
{
    Guard const MUTEX_GUARD(_mutex);
    if (_receiver.init(destination, port) != Err::E_SUCCESS) {
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

Err UdpServer::close()
{
    Guard const MUTEX_GUARD(_mutex);
    auto receiver = _receiver.getClient().lock();
    if (static_cast<bool>(receiver) == false) {
        tDLogW("UdpServer::close() expired receiver.");
        closeAll();
        return Err::E_WARNING;
    }

    auto async = _receiver.getAsync().lock();
    if (static_cast<bool>(async) == false) {
        tDLogW("UdpServer::close() expired async.");
        closeAll();
        return Err::E_WARNING;
    }

    Loop * loop = receiver->getLoop();
    assert(loop != nullptr);
    if (loop->isAliveAndThisThread() == false) {
        tDLogD("UdpServer::close() async request.");
        async->newSendFunc([&]() {
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    tDLogD("UdpServer::close() sync request.");
    closeAll();
    return Err::E_SUCCESS;
}

UdpServer::WeakClient UdpServer::accept()
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return WeakClient();
}

UdpServer::WeakClient UdpServer::get(Id id)
{
    Guard const MUTEX_GUARD(_mutex);
    auto itr = _senders.find(id);
    if (itr != _senders.end()) {
        return itr->second;
    }
    return WeakClient();
}

Err UdpServer::remove(Id id)
{
    Guard const MUTEX_GUARD(_mutex);
    return _senders.erase(id) == 1U ? Err::E_SUCCESS : Err::E_NOTFOUND;
}

// --------------
// Event backend.
// --------------

void UdpServer::backConnection(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpServer::backClose()
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

