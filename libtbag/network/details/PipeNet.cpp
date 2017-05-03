/**
 * @file   PipeNet.cpp
 * @brief  PipeNet class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/network/details/PipeNet.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

using Loop = uvpp::Loop;

// -----------------------------
// PipeNetClient implementation.
// -----------------------------

PipeNetClient::PipeNetClient(Loop & loop) : Client(loop)
{
}

PipeNetClient::~PipeNetClient()
{
}

PipeNetClient::Type PipeNetClient::getType() const
{
    return Type::PIPE;
}

// -----------------------------
// PipeNetServer implementation.
// -----------------------------

PipeNetServer::PipeNetServer(Loop & loop) : Server(loop)
{
}

PipeNetServer::~PipeNetServer()
{
}

PipeNetServer::Type PipeNetServer::getType() const
{
    return Type::PIPE;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

