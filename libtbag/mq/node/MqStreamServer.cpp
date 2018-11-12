/**
 * @file   MqStreamServer.cpp
 * @brief  MqStreamServer class implementation.
 * @author zer0
 * @date   2018-11-12
 */

#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamServer::binf;

MqStreamServer::MqStreamServer(Loop & loop, tcp_t const &)
{
}

MqStreamServer::MqStreamServer(Loop & loop, pipe_t const &)
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
}

void MqStreamServer::onConnection(Stream * server, Err code)
{
}

void MqStreamServer::onClose(Stream * server)
{
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

