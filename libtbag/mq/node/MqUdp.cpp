/**
 * @file   MqUdp.cpp
 * @brief  MqUdp class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqUdp.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqUdp::binf;

MqUdp::MqUdp(Loop & loop) : TYPE(MqType::MT_UDP)
{
    // EMPTY.
}

MqUdp::~MqUdp()
{
    // EMPTY.
}

void MqUdp::onSend(UdpSendRequest & request, Err code)
{
}

binf MqUdp::onAlloc(std::size_t suggested_size)
{
    return binf();
}

void MqUdp::onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
{
}

void MqUdp::onClose()
{
}

Err MqUdp::open(std::string const & uri)
{
    return Err::E_UNSUPOP;
}

Err MqUdp::close()
{
    return Err::E_UNSUPOP;
}

Err MqUdp::send(char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err MqUdp::recv(std::vector<char> & buffer)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

