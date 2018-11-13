/**
 * @file   MqUdp.hpp
 * @brief  MqUdp class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqUdp class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqUdp : public libtbag::mq::details::MqInterface
{
public:
    using MqType = libtbag::mq::details::MqType;

public:
    struct Node : public Udp
    {
        MqUdp * parent = nullptr;

        Node(Loop & loop, MqUdp * p) : Udp(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Node()
        { /* EMPTY. */ }

        virtual void onSend(UdpSendRequest & request, Err code) override
        { parent->onSend(request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(suggested_size); }
        virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override
        { parent->onRecv(code, buffer, size, addr, flags); }
        virtual void onClose() override
        { parent->onClose(); }
    };

public:
    using SharedNode = std::shared_ptr<Node>;

public:
    MqType const TYPE;

public:
    MqUdp(Loop & loop);
    virtual ~MqUdp();

protected:
    void onSend(UdpSendRequest & request, Err code);
    binf onAlloc(std::size_t suggested_size);
    void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags);
    void onClose();

public:
    virtual Err open(std::string const & uri) override;
    virtual Err close() override;

public:
    virtual Err send(char const * buffer, std::size_t size) override;
    virtual Err recv(std::vector<char> & buffer) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__

