/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Client.hpp>
#include <libtbag/network/details/TcpNetClient.hpp>
#include <libtbag/network/details/UdpNet.hpp>
#include <libtbag/network/details/PipeNet.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

Client::Client(Loop & loop)
{
    // EMPTY.
}

Client::~Client()
{
    // EMPTY.
}

std::shared_ptr<Client> Client::create(Loop & loop, Type type)
{
    using SharedClient = std::shared_ptr<Client>;
    // @formatter:off
    switch (type) {
    case Type::TCP:  return SharedClient(new (std::nothrow) details::TcpNetClient(loop));
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

