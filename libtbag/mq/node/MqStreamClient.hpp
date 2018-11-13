/**
 * @file   MqStreamClient.hpp
 * @brief  MqStreamClient class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

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
 * MqStreamClient class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqStreamClient : public libtbag::mq::details::MqInterface
{
public:
    using MqType = libtbag::mq::details::MqType;

public:
    template <typename _BaseT>
    struct Client : public _BaseT
    {
        MqStreamClient * parent = nullptr;

        Client(Loop & loop, MqStreamClient * p) : _BaseT(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Client()
        { /* EMPTY. */ }

        virtual void onConnect(ConnectRequest & request, Err code) override
        { parent->onConnect(request, code); }
        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onShutdown(request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onWrite(request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onRead(code, buffer, size); }
        virtual void onClose() override
        { parent->onClose(); }
    };

public:
    using TcpClient  = Client<Tcp>;
    using PipeClient = Client<Pipe>;

public:
    using SharedTcpClient  = std::shared_ptr<TcpClient>;
    using SharedPipeClient = std::shared_ptr<PipeClient>;

public:
    using pipe_t = libtbag::mq::details::pipe_t;
    using tcp_t  = libtbag::mq::details::tcp_t;

public:
    MqType const TYPE;

public:
    MqStreamClient(Loop & loop, pipe_t const & UNUSED_PARAM(x));
    MqStreamClient(Loop & loop, tcp_t const & UNUSED_PARAM(x));
    virtual ~MqStreamClient();

protected:
    void onConnect(ConnectRequest & request, Err code);
    void onShutdown(ShutdownRequest & request, Err code);
    void onWrite(WriteRequest & request, Err code);
    binf onAlloc(std::size_t suggested_size);
    void onRead(Err code, char const * buffer, std::size_t size);
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

