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
    UdpNode();
    virtual ~UdpNode();
};

using FunctionalUdpNode = details::FunctionalClient<UdpNode>;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

