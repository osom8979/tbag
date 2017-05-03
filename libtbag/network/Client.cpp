/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/network/Client.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

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
    return SharedClient();
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

