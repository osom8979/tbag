/**
 * @file   MqNode.cpp
 * @brief  MqNode class implementation.
 * @author zer0
 * @date   2018-11-11
 */

#include <libtbag/mq/MqNode.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/thread/ThreadPool.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * MqNode::Impl class implementation.
 *
 * @author zer0
 * @date   2018-11-11
 */
struct MqNode::Impl : private Noncopyable
{
public:
    using Uri  = libtbag::network::Uri;
    using Pool = libtbag::thread::ThreadPool;
    using Loop = libtbag::uvpp::Loop;

public:
    using MqType         = libtbag::mq::details::MqType;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqStreamClient = libtbag::mq::node::MqStreamClient;
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

public:
    using MqInterface = libtbag::mq::details::MqInterface;
    using SharedMq    = std::shared_ptr<MqInterface>;

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

public:
    MqType const TYPE;
    MqMode const MODE;

private:
    MqNode * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

private:
    static MqType getMqType(std::string const & uri)
    {
        Uri const URI(uri);
        auto const HOST = libtbag::string::lower(URI.getHost());
        using namespace libtbag::mq::details;
        if (HOST == PIPE_LOWER_NAME) {
            return MqType::MT_PIPE;
        } else if (HOST == TCP_LOWER_NAME) {
            return MqType::MT_TCP;
        } else if (HOST == UDP_LOWER_NAME) {
            return MqType::MT_UDP;
        }
        return MqType::MT_NONE;
    }

public:
    Impl(MqNode * parent, std::string const & uri, MqMode mode)
            : TYPE(getMqType(uri)), MODE(mode), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        if (!startTask()) {
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

private:
    bool startTask()
    {
        return _pool.push([&](){
            this->runner();
        });
    }

public:
    void runner()
    {
        _last = _loop.run();
        if (isFailure(_last)) {
            tDLogE("MqNode::Impl::runner() Loop error: {}", _last);
        }
        _pool.exit();
    }

public:
    Err send(char const * buffer, std::size_t size)
    {
        return Err::E_UNSUPOP;
    }

    Err recv(std::vector<char> & buffer)
    {
        return Err::E_UNSUPOP;
    }

    void recvWait(std::vector<char> & buffer)
    {
    }
};

// ----------------------
// MqNode implementation.
// ----------------------

MqNode::MqNode() : _impl(nullptr)
{
    // EMPTY.
}

MqNode::MqNode(MqNode const & obj) TBAG_NOEXCEPT : MqNode()
{
    (*this) = obj;
}

MqNode::MqNode(MqNode && obj) TBAG_NOEXCEPT : MqNode()
{
    (*this) = std::move(obj);
}

MqNode::~MqNode()
{
    _impl.reset();
}

MqNode & MqNode::operator =(MqNode const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

MqNode & MqNode::operator =(MqNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void MqNode::copy(MqNode const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void MqNode::swap(MqNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

Err MqNode::bind(std::string const & uri)
{
    try {
        _impl = std::make_shared<Impl>(this, uri, Impl::MqMode::MM_BIND);
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::connect(std::string const & uri)
{
    try {
        _impl = std::make_shared<Impl>(this, uri, Impl::MqMode::MM_CONNECT);
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::close()
{
    _impl.reset();
    return Err::E_SUCCESS;
}

Err MqNode::send(char const * buffer, std::size_t size)
{
    if (!_impl) {
        return Err::E_NREADY;
    }
    return _impl->send(buffer, size);
}

Err MqNode::send(std::vector<char> const & buffer)
{
    if (!_impl) {
        return Err::E_NREADY;
    }
    return send(buffer.data(), buffer.size());
}

Err MqNode::recv(std::vector<char> & buffer)
{
    if (!_impl) {
        return Err::E_NREADY;
    }
    return _impl->recv(buffer);
}

void MqNode::recvWait(std::vector<char> & buffer)
{
    if (_impl) {
        _impl->recvWait(buffer);
    }
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

