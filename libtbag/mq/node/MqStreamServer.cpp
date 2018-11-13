/**
 * @file   MqStreamServer.cpp
 * @brief  MqStreamServer class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamServer::binf;

MqStreamServer::MqStreamServer(Loop & loop, pipe_t const & UNUSED_PARAM(x)) : TYPE(MqType::MT_PIPE)
{
}

MqStreamServer::MqStreamServer(Loop & loop, tcp_t const & UNUSED_PARAM(x)) : TYPE(MqType::MT_TCP)
{
}

MqStreamServer::~MqStreamServer()
{
    // EMPTY.
}

void MqStreamServer::onNodeShutdown(Stream * node, ShutdownRequest & request, Err code)
{
}

void MqStreamServer::onNodeWrite(Stream * node, WriteRequest & request, Err code)
{
}

binf MqStreamServer::onNodeAlloc(Stream * node, std::size_t suggested_size)
{
    return binf();
}

void MqStreamServer::onNodeRead(Stream * node, Err code, char const * buffer, std::size_t size)
{
}

void MqStreamServer::onNodeClose(Stream * node)
{
    Loop * loop = node->getLoop();
    assert(loop != nullptr);
    auto shared = loop->findChildHandle(*node).lock();
    assert(static_cast<bool>(shared));
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
    Loop * loop = server->getLoop();
    assert(loop != nullptr);
    SharedStream stream;
    if (TYPE == MqType::MT_PIPE) {
        stream = loop->newHandle<PipeNode>(*loop, this);
    } else {
        assert(TYPE == MqType::MT_TCP);
        stream = loop->newHandle<TcpNode>(*loop, this);
    }
    auto const ACCEPT_CODE = server->accept(*stream);
    assert(ACCEPT_CODE == Err::E_SUCCESS);
}

void MqStreamServer::onServerClose(Stream * server)
{
}

Err MqStreamServer::open(std::string const & uri)
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::close()
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::send(char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::recv(std::vector<char> & buffer)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

