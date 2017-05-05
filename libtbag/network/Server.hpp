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
    struct NodeInterface
    {
        // @formatter:off
        virtual uerr  start() { return uerr::UVPP_ENOSYS; }
        virtual uerr   stop() { return uerr::UVPP_ENOSYS; }
        virtual uerr  close() { return uerr::UVPP_ENOSYS; }
        virtual uerr cancel() { return uerr::UVPP_ENOSYS; }

        virtual uerr  syncWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }
        virtual uerr asyncWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }
        virtual uerr   tryWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }
        // @formatter:on
    };

    using SharedServer = std::shared_ptr<Server>;

    // @formatter:off
    virtual Type getType() const = 0;

    virtual uerr init(String const & arg1, int arg2)
    { return uerr::UVPP_ENOSYS; }

    virtual uerr start() { return uerr::UVPP_ENOSYS; }
    virtual uerr  stop() { return uerr::UVPP_ENOSYS; }
    virtual uerr close() { return uerr::UVPP_ENOSYS; }

    virtual void onClientConnect(NodeInterface * node, uerr code) { /* EMPTY. */ }
    virtual void onClientWrite  (NodeInterface * node, uerr code) { /* EMPTY. */ }
    virtual void onClientRead   (NodeInterface * node, uerr code,
                                 char const * buffer, Size size)  { /* EMPTY. */ }
    virtual void onClientClose  (NodeInterface * node)            { /* EMPTY. */ }
    virtual void onServerClose  ()                                { /* EMPTY. */ }
    // @formatter:on

    static SharedServer create(Loop & loop, Type type);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SERVER_HPP__

