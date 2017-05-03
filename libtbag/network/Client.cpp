/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Client.hpp>
#include <libtbag/network/details/TcpNet.hpp>
#include <libtbag/network/details/UdpNet.hpp>
#include <libtbag/network/details/PipeNet.hpp>
#include <libtbag/network/details/PollNet.hpp>
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

Client::SharedClient Client::create(Loop & loop, NetType type)
{
    // @formatter:off
    switch (type) {
    case NetType::TCP:  return SharedClient(new (std::nothrow) details::TcpNetClient(loop));
    case NetType::UDP:  return SharedClient(new (std::nothrow) details::UdpNetClient(loop));
    case NetType::PIPE: return SharedClient(new (std::nothrow) details::PipeNetClient(loop));
    case NetType::POLL: return SharedClient(new (std::nothrow) details::PollNetClient(loop));
    default:            return SharedClient();
    }
    // @formatter:on
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

