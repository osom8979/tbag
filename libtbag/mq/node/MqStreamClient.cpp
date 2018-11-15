/**
 * @file   MqStreamClient.cpp
 * @brief  MqStreamClient class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamClient::binf;

MqStreamClient::MqStreamClient(Loop & loop, pipe_t const & UNUSED_PARAM(x)) : TYPE(MqType::MT_PIPE)
{
    // EMPTY.
}

MqStreamClient::MqStreamClient(Loop & loop, tcp_t const & UNUSED_PARAM(x)) : TYPE(MqType::MT_TCP)
{
    // EMPTY.
}

MqStreamClient::~MqStreamClient()
{
    // EMPTY.
}

void MqStreamClient::onConnect(ConnectRequest & request, Err code)
{
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
}

binf MqStreamClient::onAlloc(std::size_t suggested_size)
{
    return binf();
}

void MqStreamClient::onRead(Err code, char const * buffer, std::size_t size)
{
}

void MqStreamClient::onClose()
{
}

Err MqStreamClient::send(MqMsg const & msg)
{
    return Err::E_UNSUPOP;
}

Err MqStreamClient::recv(MqMsg & msg)
{
    return Err::E_UNSUPOP;
}

Err MqStreamClient::recvWait(MqMsg & msg)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

