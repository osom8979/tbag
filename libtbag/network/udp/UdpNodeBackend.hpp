/**
 * @file   UdpNodeBackend.hpp
 * @brief  UdpNodeBackend class prototype.
 * @author zer0
 * @date   2017-06-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODEBACKEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODEBACKEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/NetInterface.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/Udp.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpNodeBackend class prototype.
 *
 * @author zer0
 * @date   2017-06-15
 */
class TBAG_API UdpNodeBackend : public uvpp::Udp
{
public:
    using ClientInterface = details::ClientInterface;
    using ReadPacket      = details::ReadPacket;

    using Parent = uvpp::Udp;
    using Loop   = uvpp::Loop;
    using binf   = uvpp::binf;
    using Buffer = std::vector<char>;

    using UdpSendRequest = uvpp::UdpSendRequest;

private:
    ClientInterface * _parent;
    Buffer _buffer;

public:
    UdpNodeBackend(Loop & loop, ClientInterface * parent);
    virtual ~UdpNodeBackend();

public:
    virtual void onSend(UdpSendRequest & request, Err code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override;
    virtual void onClose() override;
};

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODEBACKEND_HPP__

