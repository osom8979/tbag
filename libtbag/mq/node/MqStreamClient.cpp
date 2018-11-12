/**
 * @file   MqStreamClient.cpp
 * @brief  MqStreamClient class implementation.
 * @author zer0
 * @date   2018-11-12
 */

#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamClient::binf;

MqStreamClient::MqStreamClient(Loop & loop, tcp_t const &)
{
    // EMPTY.
}

MqStreamClient::MqStreamClient(Loop & loop, pipe_t const &)
{
    // EMPTY.
}

MqStreamClient::~MqStreamClient()
{
    // EMPTY.
}

void MqStreamClient::onConnect(Stream * stream, ConnectRequest & request, Err code)
{
}

void MqStreamClient::onShutdown(Stream * stream, ShutdownRequest & request, Err code)
{
}

void MqStreamClient::onWrite(Stream * stream, WriteRequest & request, Err code)
{
}

binf MqStreamClient::onAlloc(Stream * stream, std::size_t suggested_size)
{
    return binf();
}

void MqStreamClient::onRead(Stream * stream, Err code, char const * buffer, std::size_t size)
{
}

void MqStreamClient::onClose(Stream * stream)
{
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

