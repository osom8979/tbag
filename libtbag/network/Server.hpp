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
    struct NodeInterface : public details::NetCommon
    {
        // @formatter:off
        virtual Id getId() const { return id::UNKNOWN_ID; }

        virtual bool  start() { return false; }
        virtual bool   stop() { return false; }
        virtual bool  close() { return false; }
        virtual bool cancel() { return false; }

        virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) { return false; }
        // @formatter:on
    };

    using SharedServer = std::shared_ptr<Server>;

    // @formatter:off
    virtual Type getType() const
    { return Type::UNKNOWN; }

    virtual bool init(String const & destination, int port = 0, int timeout = 0)
    { return false; }
    virtual bool close()
    { return false; }

    virtual void onClientConnect(NodeInterface * node, uerr code) { /* EMPTY. */ }
    virtual void onClientWrite  (NodeInterface * node, uerr code) { /* EMPTY. */ }
    virtual void onClientRead   (NodeInterface * node, uerr code,
                                 char const * buffer, Size size)  { /* EMPTY. */ }
    virtual void onClientClose  (NodeInterface * node)            { /* EMPTY. */ }
    virtual void onServerClose  ()                                { /* EMPTY. */ }
    // @formatter:on

public:
    static SharedServer create(Loop & loop, Type type);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

