/**
 * @file   NetStreamServer.cpp
 * @brief  NetStreamServer class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/net/socket/NetStreamServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/mq/node/MqStreamServer.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

using MqType     = NetStreamServer::MqType;
using MqEvent    = NetStreamServer::MqEvent;
using MqParams   = NetStreamServer::MqParams;
using MqInternal = NetStreamServer::MqInternal;

void onBindCb(void * parent)
{
    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.begin_cb) {
        sock->_callbacks.begin_cb();
    }
}

bool onAcceptCb(void * node, std::string const & peer, void * parent)
{
    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.accept_cb) {
        return sock->_callbacks.accept_cb(reinterpret_cast<std::intptr_t>(node), peer);
    }
    return true;
}

void onCloseServerCb(void * parent)
{
    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.end_cb) {
        sock->_callbacks.end_cb();
    }
}

std::size_t onWriteNodeCb(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent)
{
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);

    Err code;
    if (sock->type() == MqType::MT_PIPE) {
        auto * tcp = (MqStreamServer::TcpNode*)node;
        code = tcp->write(tcp->write_req, buffer, size);
    } else if (sock->type() == MqType::MT_TCP) {
        auto * pipe = (MqStreamServer::PipeNode*)node;
        code = pipe->write(pipe->write_req, buffer, size);
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return 0;
    }

    if (isFailure(code)) {
        tDLogE("NetStreamServer::Impl::__on_default_write_cb__() Write error: {}", code);
    }
    return isSuccess(code) ? 1U : 0U;
}

void onReadNodeCb(void * node, char const * buffer, std::size_t size, void * parent)
{
    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.recv_cb) {
        sock->_callbacks.recv_cb(reinterpret_cast<std::intptr_t>(node), buffer, size);
    }
}

void onCloseNodeCb(void * node, void * parent)
{
    auto * sock = (NetStreamServer*)parent;
    assert(sock != nullptr);
    if (sock->_callbacks.close_cb) {
        sock->_callbacks.close_cb(reinterpret_cast<std::intptr_t>(node));
    }
}

// ------------------------------
// NetStreamServer implementation
// ------------------------------

NetStreamServer::NetStreamServer(MqParams const & params, Callbacks const & cbs)
        : libtbag::mq::MqNode(params, MqMode::MM_BIND, MqNode::no_init{}), _callbacks(cbs)
{
    MqInternal internal;
    internal.active_cb     = &onBindCb;
    internal.accept_cb     = &onAcceptCb;
    internal.close_cb      = &onCloseServerCb;
    internal.default_write = &onWriteNodeCb;
    internal.default_read  = &onReadNodeCb;
    internal.close_node    = &onCloseNodeCb;
    internal.parent        = this;

    if (!init(internal)) {
        throw std::bad_alloc();
    }
}

NetStreamServer::NetStreamServer(std::string const & uri, Callbacks const & cbs)
        : NetStreamServer(getParams(uri), cbs)
{
    // EMPTY.
}

NetStreamServer::~NetStreamServer()
{
    // EMPTY.
}

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

