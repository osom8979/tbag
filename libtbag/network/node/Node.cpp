/**
 * @file   Node.cpp
 * @brief  Node class implementation.
 * @author zer0
 * @date   2018-10-26
 */

#include <libtbag/network/node/Node.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace node    {

/** @warning Don't change this variable. */
TBAG_CONSTEXPR static unsigned int const _POOL_SIZE = 1U;

/**
 * Node::Impl class implementation.
 *
 * @author zer0
 * @date   2018-10-26
 */
struct Node::Impl : private Noncopyable
{
public:
    using ThreadPool = libtbag::thread::ThreadPool;

public:
    using HttpServer = libtbag::network::http::HttpServer;
    using HttpClient = libtbag::network::http::HttpClient;
    using StreamType = libtbag::network::details::StreamType;
    using Loop       = libtbag::uvpp::Loop;

public:
    struct Server;
    struct Client;

    friend struct Server;
    friend struct Client;

public:
    /**
     * Node::Impl::Server class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     */
    struct Server : public HttpServer
    {
        Server(Loop & loop, StreamType type = StreamType::TCP) : HttpServer(loop, type)
        {
            // EMPTY.
        }

        virtual ~Server()
        {
            // EMPTY.
        }
    };

    /**
     * Node::Impl::Client class implementation.
     *
     * @author zer0
     * @date   2018-10-26
     */
    struct Client : public HttpClient
    {
        Client(Loop & loop, StreamType type = StreamType::TCP) : HttpClient(loop, type)
        {
            // EMPTY.
        }

        virtual ~Client()
        {
            // EMPTY.
        }
    };

    using SharedServer = std::shared_ptr<Server>;
    using SharedClient = std::shared_ptr<Client>;

private:
    Node * _parent;

private:
    ThreadPool _pool;

private:
    NodeType _type = NodeType::NT_NONE;

public:
    Impl(Node * parent) : _parent(parent), _pool(_POOL_SIZE)
    {
        assert(_parent != nullptr);
        assert(_pool.sizeOfThreads() == _POOL_SIZE);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
};

// ----------------------------
// Node implementation.
// ----------------------------

Node::Node() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

Node::Node(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Node::Node(Node const & obj) TBAG_NOEXCEPT : Node(nullptr)
{
    (*this) = obj;
}

Node::Node(Node && obj) TBAG_NOEXCEPT : Node(nullptr)
{
    (*this) = std::move(obj);
}

Node::~Node()
{
    // EMPTY.
}

Node & Node::operator =(Node const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Node & Node::operator =(Node && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Node::copy(Node const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
        _event = obj._event;
    }
}

void Node::swap(Node & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
        _event.swap(obj._event);
    }
}

Err Node::open(std::string const & uri)
{
    return Err::E_UNSUPOP;
}

Err Node::close()
{
    return Err::E_UNSUPOP;
}

Err Node::connect(std::string const & server_name, std::string const & uri)
{
    return Err::E_UNSUPOP;
}

Err Node::disconnect(std::string const & server_name)
{
    return Err::E_UNSUPOP;
}

Err Node::c2s(std::string const & server_name, char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err Node::s2c(std::string const & client_name, char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

