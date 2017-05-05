/**
 * @file   TcpNetClient.cpp
 * @brief  TcpNetClient class implementation.
 * @author zer0
 * @date   2017-05-05
 */

#include <libtbag/network/details/TcpNetClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// ----------------------------
// TcpNetClient implementation.
// ----------------------------

TcpNetClient::TcpNetClient(Loop & loop)
{
    // EMPTY.
}

TcpNetClient::~TcpNetClient()
{
    // EMPTY.
}

TcpNetClient::Type TcpNetClient::getType() const
{
    return Type::TCP;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

