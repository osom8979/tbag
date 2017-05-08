/**
 * @file   TcpServer.cpp
 * @brief  TcpServer class implementation.
 * @author zer0
 * @date   2017-05-06
 */

#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// ---------------------------
// TcpRealNode implementation.
// ---------------------------

TcpRealNode::TcpRealNode(Loop & loop, TcpServer & parent)
        : TcpClient(loop), _parent(parent)
{
    // EMPTY.
}

TcpRealNode::~TcpRealNode()
{
    // EMPTY.
}

void TcpRealNode::onConnect(uerr code)
{
    _parent.onClientConnect(this, code);
}

void TcpRealNode::onWrite(uerr code)
{
    _parent.onClientWrite(this, code);
}

void TcpRealNode::onRead(uerr code, char const * buffer, Size size)
{
    _parent.onClientRead(this, code, buffer, size);
}

void TcpRealNode::onClose()
{
    _parent.onClientClose(this);
}

// -----------------------------
// TcpRealServer implementation.
// -----------------------------

TcpRealServer::TcpRealServer(Loop & loop, TcpServer & parent) : Tcp(loop), _parent(parent)
{
    // EMPTY.
}

TcpRealServer::~TcpRealServer()
{
    // EMPTY.
}

bool TcpRealServer::init(String const & ip, int port)
{
    return uvpp::initCommonServer(*this, ip, port);
}

void TcpRealServer::onConnection(uerr code)
{
    auto node = _parent.insertNewNode();
    if (auto shared = node->getClient().lock()) {
        uerr const CODE = _parent._server->accept(*shared);
        if (CODE == uerr::UVPP_SUCCESS) {
            if (_parent.onClientConnect(node.get(), code)) {
                __tbag_debug("TcpRealServer::onConnection() client connect (Sock:{}[{}]/Peer:{}[{}]).",
                             shared->getSockIp(), shared->getSockPort(),
                             shared->getPeerIp(), shared->getPeerPort());
            } else {
                __tbag_error("TcpRealServer::onConnection() Accept denied (Sock:{}[{}]/Peer:{}[{}]).",
                             shared->getSockIp(), shared->getSockPort(),
                             shared->getPeerIp(), shared->getPeerPort());
            }
        } else {
            __tbag_error("TcpRealServer::onConnection() {} error.", uvpp::getErrorName(CODE));
        }
    } else {
        __tbag_error("TcpRealServer::onConnection() node is nullptr.");
    }
}

void TcpRealServer::onClose()
{
    _parent.onServerClose();
}

// -------------------------
// TcpServer implementation.
// -------------------------

TcpServer::TcpServer(Loop & loop)
{
    _server = loop.newHandle<TcpRealServer>(loop, *this);
    _async  = loop.newHandle<SafetyWriteAsync>(loop);
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));
}

TcpServer::~TcpServer()
{
    _server.reset();
    _async.reset();
}

TcpServer::SharedNode TcpServer::insertNewNode()
{
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    SharedNode node(new (std::nothrow) TcpRealNode(*loop, *this));
    if (static_cast<bool>(node) == false) {
        return SharedNode();
    }

    Guard guard(_node_mutex);
    bool const INSERT_RESULT = _nodes.insert(NodePair(node->getId(), node)).second;
    assert(INSERT_RESULT);
    return node;
}

bool TcpServer::removeNode(NodeKey key)
{
    Guard guard(_node_mutex);
    return _nodes.erase(key) == 1U;
}

bool TcpServer::init(String const & ip, int port)
{
    return _server->init(ip, port);
}

bool TcpServer::close()
{
    auto job = [&](SafetyAsync * UNUSED_PARAM(async)) -> bool {
        foreach([](NodePair & cursor){
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        });

        _server->close();
        _async->close();
        return true;
    };

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        __tbag_debug("TcpServer::close() sync request.");
        return job(nullptr);
    }

    __tbag_debug("TcpServer::close() async request.");
    return static_cast<bool>(_async->newSendFunc(job));
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

