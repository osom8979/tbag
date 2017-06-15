/**
 * @file   UdpNode.hpp
 * @brief  UdpNode class prototype.
 * @author zer0
 * @date   2017-06-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/id/Id.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/udp/UdpNodeBackend.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <string>
#include <vector>
#include <memory>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpNode class prototype.
 *
 * @author zer0
 * @date   2017-06-15
 */
class TBAG_API UdpNode : public details::ClientInterface
{
public:
    using Loop = uvpp::Loop;
    using binf = uvpp::binf;
    using UdpSendRequest = uvpp::UdpSendRequest;

public:
    using SharedClientBackend = std::shared_ptr<UdpNodeBackend>;
    using   WeakClientBackend =   std::weak_ptr<UdpNodeBackend>;

public:
    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync = std::shared_ptr<SafetyAsync>;

public:
    using Id      = id::Id;
    using Buffer  = std::vector<char>;
    using Buffers = std::vector<Buffer>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    bool _owner_async;

    union {
        sockaddr_in  ipv4;
        sockaddr_in6 ipv6;
    } _addr;

private:
    SharedClientBackend _client;
    SharedSafetyAsync   _async;

private:
    mutable Mutex _mutex;

    struct {
        UdpSendRequest send_req;
        Buffers buffers;
    } _writer;

public:
    UdpNode(Loop & loop);
    UdpNode(Loop & loop, SharedSafetyAsync async);
    virtual ~UdpNode();

public:
    WeakClientBackend getClient();
    WeakSafetyAsync   getAsync();

// ===============================
// === PROTECTED SECTION BEGIN ===
// [WARNING] Don't mutex guard in this protected section.
protected:
    bool _initInternalHandles();

    Err _writeReal(binf const * buffer, std::size_t size);
    void _copyToWriteBuffer(binf const * buffer, std::size_t size);
    std::vector<binf> _getWriteBufferInfo();
    Err _autoWrite(binf const * buffer, std::size_t size, uint64_t millisec = 0);

    void _closeAll();
// === PROTECTED SECTION END ===
// =============================

private:
    void onAsyncWrite();

public:
    /** Obtain the Client handle id. */
    virtual Id getId() const override;

public:
    virtual bool init(char const * destination, int port = 0, uint64_t millisec = 0) override;

public:
    virtual bool  start() override;
    virtual bool   stop() override;
    virtual void  close() override;
    virtual void cancel() override;

public:
    virtual bool write(binf const * buffer, std::size_t size, uint64_t millisec = 0) override;
    virtual bool write(char const * buffer, std::size_t size, uint64_t millisec = 0) override;

public:
    virtual void * getUserData() override;

public:
    virtual std::string getDestination() const override;
    virtual int getPort() const override;

public:
    template <typename Predicated>
    void updateUserData(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_client->getUserData());
    }

// Event backend.
public:
    virtual void runBackendConnect(Err code) override;
    virtual void runBackendShutdown(Err code) override;
    virtual void runBackendWrite(Err code) override;
    virtual void runBackendRead(Err code, char const * buffer, std::size_t size,
                                sockaddr const * addr = nullptr, unsigned int flags = 0) override;
    virtual void runBackendClose() override;
};

using FunctionalUdpNode = details::FunctionalClient<UdpNode>;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

