/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Client.hpp>
#include <libtbag/network/details/TcpClient.hpp>
#include <libtbag/network/details/UdpNet.hpp>
#include <libtbag/network/details/PipeNet.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

Client::SharedClient Client::create(Loop & loop, Type type)
{
    // @formatter:off
    switch (type) {
    case Type::TCP:  return SharedClient(new (std::nothrow) details::TcpClient(loop));
    case Type::UDP:  return SharedClient(new (std::nothrow) details::UdpNetClient(loop));
    case Type::PIPE: return SharedClient(new (std::nothrow) details::PipeNetClient(loop));
    default:         return SharedClient();
    }
    // @formatter:on
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

