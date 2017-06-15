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
class TBAG_API UdpNode
{
private:

public:
    UdpNode();
    virtual ~UdpNode();
};

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

