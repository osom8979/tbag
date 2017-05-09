/**
 * @file   Server.hpp
 * @brief  Server class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/Client.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * Server class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
struct Server : public details::NetCommon
{
    using SharedClient = std::shared_ptr<Client>;
    using   WeakClient =   std::weak_ptr<Client>;

    // @formatter:off
    virtual Type getType() const
    { return Type::UNKNOWN; }
    virtual Id getId() const
    { return id::UNKNOWN_ID; }

    virtual bool init(String const & destination, int port = 0)
    { return false; }

    virtual WeakClient accept()
    { return WeakClient(); }

    virtual void close()
    { /* EMPTY. */ }

    virtual void onConnection(uerr code)
    { /* EMPTY. */ }
    virtual void onClientShutdown(WeakClient node, uerr code)
    { /* EMPTY. */ }
    virtual void onClientWrite(WeakClient node, uerr code)
    { /* EMPTY. */ }
    virtual void onClientRead(WeakClient node, uerr code, char const * buffer, Size size)
    { /* EMPTY. */ }
    virtual void onClientClose(WeakClient node)
    { /* EMPTY. */ }
    virtual void onServerClose()
    { /* EMPTY. */ }
    // @formatter:on
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

