/**
 * @file   TcpServer.cpp
 * @brief  TcpServer class implementation.
 * @author zer0
 * @date   2017-05-06
 */

#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/log/Log.hpp>

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
        : Tcp(loop), _parent(parent), _id(id::generator::genTimeId())
{
    _shutdown = loop.newHandle<TimeoutToShutdown>(loop, this, false);
    assert(static_cast<bool>(_shutdown));
}

TcpRealNode::~TcpRealNode()
{
    _shutdown.reset();
}

void TcpRealNode::onWrite(WriteRequest & request, uerr code)
{
    _parent.onClientWrite(this, code);
}

TcpRealNode::binf TcpRealNode::onAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_buffer, suggested_size);
}

void TcpRealNode::onRead(uerr code, char const * buffer, std::size_t size)
{
    _parent.onClientRead(this, code, buffer, size);
}

void TcpRealNode::onClose()
{
    _parent.onClientClose(this);
}

TcpRealNode::Id TcpRealNode::getId() const
{
    return _id;
}

bool TcpRealNode::start()
{
    return uvpp::Tcp::startRead() == uerr::UVPP_SUCCESS;
}

bool TcpRealNode::stop()
{
    return uvpp::Tcp::stopRead() == uerr::UVPP_SUCCESS;
}

bool TcpRealNode::close()
{
    uvpp::Tcp::close();
    return true;
}

bool TcpRealNode::cancel()
{
    return false;
}

bool TcpRealNode::write(char const * buffer, Size size, uint64_t millisec)
{
    return false;
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
    uerr const CODE = _parent._server->accept(*node);
    if (CODE == uerr::UVPP_SUCCESS) {
        _parent.onClientConnect(node.get(), code);
    } else {
        __tbag_error("TcpRealServer::onConnection() {} error.", uvpp::getErrorName(CODE));
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

TcpServer::SharedNode TcpServer::getNode(NodeKey key)
{
    Guard guard(_node_mutex);
    auto itr = _nodes.find(key);
    if (itr == _nodes.end()) {
        return SharedNode();
    }
    return itr->second;
}

TcpServer::SharedNode TcpServer::getNode(NodeKey key) const
{
    Guard guard(_node_mutex);
    auto itr = _nodes.find(key);
    if (itr == _nodes.end()) {
        return SharedNode();
    }
    return itr->second;
}

TcpServer::SharedNode TcpServer::insertNewNode()
{
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    SharedNode node(new (std::nothrow) TcpRealNode(*loop, *this));
    if (static_cast<bool>(node) == false) {
        return SharedNode();
    }

    NodeKey key = node->getId();

    Guard guard(_node_mutex);
    if (_nodes.insert(NodePair(node->getId(), node)).second) {
        if (loop->insertChildHandle(node).expired()) {
            _nodes.erase(node->getId());
            return SharedNode();
        }
    }

    return node;
}

bool TcpServer::removeNode(NodeKey key)
{
    Guard guard(_node_mutex);
    return _nodes.erase(key) == 1U;
}

bool TcpServer::init(String const & ip, int port, int UNUSED_PARAM(timeout))
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

    if (loop->isRunning() && loop->getOwnerThreadId() != std::this_thread::get_id()) {
        __tbag_debug("TcpServer::close() async request.");
        return static_cast<bool>(_async->newSendFunc(job));
    }

    __tbag_debug("TcpServer::close() sync request.");
    return job(nullptr);
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

