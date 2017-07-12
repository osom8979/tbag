/**
 * @file   UdpServer.hpp
 * @brief  UdpServer class prototype.
 * @author zer0
 * @date   2017-06-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/udp/UdpReceiver.hpp>
#include <libtbag/network/udp/UdpSender.hpp>

#include <unordered_map>
#include <string>
#include <atomic>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpServer class prototype.
 *
 * @author zer0
 * @date   2017-06-19
 */
class TBAG_API UdpServer : public details::ServerInterface
{
public:
    using ClientInterface = details::ClientInterface;
    using ServerInterface = details::ServerInterface;

    using Loop = uvpp::Loop;
    using Udp  = uvpp::Udp;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync =   std::weak_ptr<SafetyAsync>;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using ClientMap  = std::unordered_map<Id, SharedClient>;
    using ClientPair = ClientMap::value_type;

    using AtomicBool = std::atomic_bool;
    using Id = ClientInterface::Id;

private:
    UdpReceiver _receiver;
    ClientMap   _senders;

private:
    mutable Mutex _mutex;

public:
    UdpServer(Loop & loop);
    virtual ~UdpServer();

public:
    inline bool emptySenders() const TBAG_NOEXCEPT_SP_OP(_senders.empty())
    { Guard g(_mutex); return _senders.empty(); }
    inline std::size_t sizeSenders() const TBAG_NOEXCEPT_SP_OP(_senders.size())
    { Guard g(_mutex); return _senders.size(); }

private:
    void closeAll();

public:
    WeakClient newSender(std::string const & destination, int port);

public:
    virtual std::string dest() const override;
    virtual int         port() const override;

public:
    virtual Err        init  (char const * destination, int port = 0) override;
    virtual Err        close () override;
    virtual WeakClient accept() override;
    virtual WeakClient get   (Id id) override;
    virtual Err        remove(Id id) override;

public:
    virtual void backConnection(Err code) override;
    virtual void backClose() override;

public:
    template <typename Predicated>
    void foreach(Predicated predicated)
    {
        Guard guard(_mutex);
        for (auto & cursor : _senders) {
            predicated(cursor);
        }
    }

    template <typename Predicated>
    void updateClients(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_senders);
    }
};

using FunctionalUdpServer = details::FunctionalServer<UdpServer>;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPSERVER_HPP__

