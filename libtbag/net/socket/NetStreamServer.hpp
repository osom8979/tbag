/**
 * @file   NetStreamServer.hpp
 * @brief  NetStreamServer class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/MqNode.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

/**
 * NetStreamServer class prototype.
 *
 * @author zer0
 * @date   2018-12-09
 * @date   2019-01-15 (Merge with NetStreamServer::Impl class)
 */
class TBAG_API NetStreamServer TBAG_FINAL : public libtbag::mq::MqNode
{
public:
    using OnBegin  = std::function<void(void)>;
    using OnEnd    = std::function<void(void)>;
    using OnAccept = std::function<bool(std::intptr_t, std::string const &)>;
    using OnRecv   = std::function<void(std::intptr_t, char const *, std::size_t)>;
    using OnClose  = std::function<void(std::intptr_t)>;

public:
    struct Callbacks
    {
        OnBegin   begin_cb;
        OnEnd     end_cb;
        OnAccept  accept_cb;
        OnRecv    recv_cb;
        OnClose   close_cb;
    };

private:
    Callbacks _callbacks;

public:
    NetStreamServer(MqParams const & params, Callbacks const & cbs);
    NetStreamServer(std::string const & uri, Callbacks const & cbs);
    ~NetStreamServer();

private:
    using size = std::size_t;
    friend void onBindCb(void * parent);
    friend bool onAcceptCb(void * node, std::string const & peer, void * parent);
    friend void onCloseServerCb(void * parent);
    friend size onWriteNodeCb(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent);
    friend void onReadNodeCb(void * node, char const * buffer, std::size_t size, void * parent);
    friend void onCloseNodeCb(void * node, void * parent);
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

