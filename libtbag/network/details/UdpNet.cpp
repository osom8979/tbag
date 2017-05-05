/**
 * @file   UdpNet.cpp
 * @brief  UdpNet class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/network/details/UdpNet.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

using Loop = uvpp::Loop;

// ----------------------------
// UdpNetClient implementation.
// ----------------------------

UdpNetClient::UdpNetClient(Loop & loop)
{
}

UdpNetClient::~UdpNetClient()
{
}

UdpNetClient::Type UdpNetClient::getType() const
{
    return Type::UDP;
}

// ----------------------------
// UdpNetServer implementation.
// ----------------------------

UdpNetServer::UdpNetServer(Loop & loop)
{
}

UdpNetServer::~UdpNetServer()
{
}

UdpNetServer::Type UdpNetServer::getType() const
{
    return Type::UDP;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

