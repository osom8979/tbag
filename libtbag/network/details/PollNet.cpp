/**
 * @file   PollNet.cpp
 * @brief  PollNet class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/network/details/PollNet.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

using Loop = uvpp::Loop;

// -----------------------------
// PollNetClient implementation.
// -----------------------------

PollNetClient::PollNetClient(Loop & loop) : Client(loop)
{
}

PollNetClient::~PollNetClient()
{
}

PollNetClient::Type PollNetClient::getType() const
{
    return Type::POLL;
}

// -----------------------------
// PollNetServer implementation.
// -----------------------------

PollNetServer::PollNetServer(Loop & loop) : Server(loop)
{
}

PollNetServer::~PollNetServer()
{
}

PollNetServer::Type PollNetServer::getType() const
{
    return Type::POLL;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

