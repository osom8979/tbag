/**
 * @file   MqStreamServer.hpp
 * @brief  MqStreamServer class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/container/Pointer.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/ex/AsyncSingleRequestQueue.hpp>

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqStreamServer class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqStreamServer : public libtbag::mq::details::MqInterface
{
public:
    using MqType          = libtbag::mq::details::MqType;
    using MqMsg           = libtbag::mq::details::MqMsg;
    using MqEventQueue    = libtbag::mq::details::MqEventQueue;
    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;

public:
    TBAG_CONSTEXPR static std::size_t DEFAULT_QUEUE_SIZE    = MqEventQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_PACKET_SIZE   = MqEventQueue::DEFAULT_PACKET_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_MAX_NODE_SIZE = 100000; // C10K

public:
    template <typename T, typename R>
    using AsyncSingleRequestQueue = libtbag::uvpp::ex::AsyncSingleRequestQueue<T, R>;

public:
    using WriteMsgQueue = AsyncSingleRequestQueue<AsyncMsgPointer, WriteRequest>;

    static_assert(std::is_same<AsyncMsgPointer, typename WriteMsgQueue::Value>::value, "");
    static_assert(std::is_same<WriteRequest, typename WriteMsgQueue::Request>::value, "");

private:
    struct EventQueue : public MqEventQueue
    {
        MqStreamServer * parent = nullptr;

        EventQueue(Loop & loop, std::size_t size, std::size_t msg_size, MqStreamServer * p)
                : MqEventQueue(loop, size, msg_size), parent(p)
        { assert(parent != nullptr); }
        virtual ~EventQueue()
        { /* EMPTY. */ }

        virtual AfterAction onMsg(AsyncMsg * msg) override
        { return parent->onMsg(msg); }
    };

    template <typename _BaseT>
    struct Node : public _BaseT
    {
        struct Write : public WriteMsgQueue
        {
            using Value   = WriteMsgQueue::Value;
            using Request = WriteMsgQueue::Request;

            Node * node = nullptr;

            Write(Loop & loop, Node * p) : WriteMsgQueue(loop), node(p)
            { assert(node != nullptr); }
            virtual ~Write()
            { /* EMPTY. */ }

            virtual void doRequest(Request & request, Value const & value) override
            { node->doWriteRequest(request, value); }
            virtual void onDequeue(Value const & value) override
            { node->onWriteDequeue(value); }
            virtual void onClose() override
            { node->onWriteClose(); }
        };

        MqStreamServer * parent = nullptr;
        Write writer;
        std::vector<char> read_buffer;

        Node(Loop & loop, MqStreamServer * p) : _BaseT(loop), parent(p), writer(loop, this)
        { assert(parent != nullptr); }
        virtual ~Node()
        { /* EMPTY. */ }

        void doWriteRequest(typename Write::Request & request, typename Write::Value const & value)
        { parent->doWriteRequest(this, request, value); }
        void onWriteDequeue(typename Write::Value const & value)
        { parent->onWriteDequeue(this, value); }
        void onWriteClose()
        { parent->onWriteClose(this); }

        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onNodeShutdown(this, request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onNodeWrite(this, request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onNodeAlloc(this, suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onNodeRead(this, code, buffer, size); }
        virtual void onClose() override
        { parent->onNodeClose(this); }
    };

    template <typename _BaseT>
    struct Server : public _BaseT
    {
        MqStreamServer * parent = nullptr;

        Server(Loop & loop, MqStreamServer * p) : _BaseT(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Server()
        { /* EMPTY. */ }

        virtual void onConnection(Err code) override
        { parent->onServerConnection(this, code); }
        virtual void onClose() override
        { parent->onServerClose(this); }
    };

public:
    using TcpNode  = Node<Tcp>;
    using PipeNode = Node<Pipe>;

    using TcpServer  = Server<Tcp>;
    using PipeServer = Server<Pipe>;

public:
    using SharedTcpNode  = std::shared_ptr<TcpNode>;
    using SharedPipeNode = std::shared_ptr<PipeNode>;

    using SharedTcpServer  = std::shared_ptr<TcpServer>;
    using SharedPipeServer = std::shared_ptr<PipeServer>;

public:
    using StreamPointer = libtbag::container::Pointer<Stream>;
    using NodeSet       = std::unordered_set<StreamPointer, StreamPointer::Hash, StreamPointer::EqualTo>;
    using SharedStream  = std::shared_ptr<Stream>;
    using ThreadId      = std::thread::id;

public:
    struct Params
    {
        std::string  uri;
        int          port       = 0;
        MqType       type       = MqType::MT_TCP;
        std::size_t  queue_size = DEFAULT_QUEUE_SIZE;
        std::size_t  msg_size   = DEFAULT_PACKET_SIZE;
        std::size_t  max_nodes  = DEFAULT_MAX_NODE_SIZE;

        Params() { /* EMPTY. */ }
        ~Params() { /* EMPTY. */ }
    };

public:
    MqType   const TYPE;
    ThreadId const THREAD_ID;

private:
    Params _params;

private:
    EventQueue   _events;
    SharedStream _server;
    NodeSet      _nodes;

public:
    MqStreamServer(Loop & loop, Params const & params);
    virtual ~MqStreamServer();

protected:
    AfterAction onMsg(AsyncMsg * msg);

protected:
    void doWriteRequest(Stream * node, WriteRequest & request, AsyncMsgPointer const & value);
    void onWriteDequeue(Stream * node, AsyncMsgPointer const & value);
    void onWriteClose  (Stream * node);

protected:
    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

protected:
    void onServerConnection(Stream * server, Err code);
    void onServerClose(Stream * server);

public:
    virtual Err send(char const * buffer, std::size_t size) override;
    virtual Err recv(std::vector<char> & buffer) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

