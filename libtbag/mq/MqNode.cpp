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

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <thread>
#include <chrono>
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
    using MqStreamClient = libtbag::mq::node::MqStreamClient;
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

public:
    using MqInterface = libtbag::mq::details::MqInterface;
    using SharedMq    = std::shared_ptr<MqInterface>;

public:
    using UvLock      = libtbag::lock::UvLock;
    using UvCondition = libtbag::lock::UvCondition;

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

public:
    MqMode const MODE;

private:
    MqNode * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

public:
    Impl(MqNode * parent, MqParams const & params, MqMode mode)
            : MODE(mode), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        assert(MODE != MqMode::MM_NONE);
        if (!_pool.waitPush([&](){ init(params, mode); })) {
            throw std::bad_alloc();
        }
        assert(static_cast<bool>(_mq));
        if (!_pool.push([&](){ runner(); })) {
            throw std::bad_alloc();
        }

        // [CLIENT ONLY]
        // Wait until connection is completed.
        if (MODE == MqMode::MM_CONNECT && params.wait_on_connection_timeout_millisec > 0) {
            auto const BEGIN_TIME = std::chrono::system_clock::now();
            auto const TIMEOUT = std::chrono::milliseconds(params.wait_on_connection_timeout_millisec);
            while (_mq->state() == MqMachineState::MMS_INITIALIZED) {
                if (std::chrono::system_clock::now() - BEGIN_TIME >= TIMEOUT) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }

    ~Impl()
    {
        _mq->send(MqMsg(MqEvent::ME_CLOSE));
        _pool.exit();
        _pool.join();
    }

public:
    void init(MqParams const & params, MqMode mode)
    {
        if (MODE == MqMode::MM_BIND) {
            _mq = std::make_shared<MqStreamServer>(_loop, params);
        } else {
            assert(MODE == MqMode::MM_CONNECT);
            _mq = std::make_shared<MqStreamClient>(_loop, params);
        }
    }

    void runner()
    {
        tDLogI("MqNode::Impl::runner() Loop start");
        _last = _loop.run();
        if (isSuccess(_last)) {
            tDLogI("MqNode::Impl::runner() Loop end success.");
        } else {
            tDLogE("MqNode::Impl::runner() Loop end error: {}", _last);
        }
        _mq.reset();
    }

public:
    Err send(MqMsg const & msg)
    {
        return _mq->send(msg);
    }

    Err recv(MqMsg & msg)
    {
        return _mq->recv(msg);
    }

    void recvWait(MqMsg & msg)
    {
        return _mq->recvWait(msg);
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

Err MqNode::bind(MqParams const & params)
{
    try {
        _impl = std::make_shared<Impl>(this, params, MqMode::MM_BIND);
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::bind(std::string const & uri)
{
    return bind(libtbag::mq::details::convertUriToParams(uri));
}

Err MqNode::connect(MqParams const & params)
{
    try {
        _impl = std::make_shared<Impl>(this, params, MqMode::MM_CONNECT);
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

Err MqNode::connect(std::string const & uri)
{
    return connect(libtbag::mq::details::convertUriToParams(uri));
}

Err MqNode::close()
{
    _impl.reset();
    return Err::E_SUCCESS;
}

Err MqNode::send(MqMsg const & msg)
{
    if (!_impl) {
        return Err::E_NREADY;
    }
    return _impl->send(msg);
}

Err MqNode::recv(MqMsg & msg)
{
    if (!_impl) {
        return Err::E_NREADY;
    }
    return _impl->recv(msg);
}

void MqNode::recvWait(MqMsg & msg)
{
    assert(static_cast<bool>(_impl));
    _impl->recvWait(msg);
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

