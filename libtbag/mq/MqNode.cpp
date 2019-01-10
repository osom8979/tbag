/**
 * @file   MqNode.cpp
 * @brief  MqNode class implementation.
 * @author zer0
 * @date   2018-11-11
 */

#include <libtbag/mq/MqNode.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/node/MqLocalQueue.hpp>
#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/thread/ThreadPool.hpp>

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
    using MqLocalQueue   = libtbag::mq::node::MqLocalQueue;
    using MqStreamClient = libtbag::mq::node::MqStreamClient;
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

public:
    using MqInternal  = libtbag::mq::details::MqInternal;
    using MqIsConsume = libtbag::mq::details::MqIsConsume;
    using MqInterface = libtbag::mq::details::MqInterface;
    using SharedMq    = std::shared_ptr<MqInterface>;

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

public:
    Callbacks const CALLBACKS;
    MqMode    const MODE;
    MqParams  const PARAMS;

private:
    MqNode * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

public:
    Impl(MqNode * parent, MqParams const & params, MqMode mode, Callbacks const & cbs = Callbacks{})
            : CALLBACKS(cbs), MODE(mode), PARAMS(params), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        MqInternal internal;
        internal.accept_cb = &__on_accept_cb__;
        internal.write_cb  = &__on_write_cb__;
        internal.recv_cb   = &__on_recv_cb__;
        internal.parent    = this;

        assert(MODE != MqMode::MM_NONE);
        if (params.type == MqType::MT_LOCAL) {
            _mq = std::make_shared<MqLocalQueue>(_loop, internal, params);
        } else {
            if (MODE == MqMode::MM_BIND) {
                _mq = std::make_shared<MqStreamServer>(_loop, internal, params);
            } else {
                assert(MODE == MqMode::MM_CONNECT);
                _mq = std::make_shared<MqStreamClient>(_loop, internal, params);
            }
        }

        if (!_loop || !_mq) {
            throw std::bad_alloc();
        }

        bool const PUSH_RESULT = _pool.push([&](){
            runner();
        });
        assert(PUSH_RESULT);

        libtbag::mq::details::waitOnActivation(params, _mq.get());
    }

    ~Impl()
    {
        using namespace libtbag::mq::details;
        char const * const TYPE_NAME = getTypeName(PARAMS.type);
        char const * const MODE_NAME = getModeName(MODE);

        auto const CODE = _mq->send(MqMsg(ME_CLOSE));
        if (isSuccess(CODE)) {
            tDLogIfD(PARAMS.verbose, "MqNode::Impl::~Impl({}/{}) Send a close message.",
                     TYPE_NAME, MODE_NAME);
        } else {
            tDLogIfW(PARAMS.verbose, "MqNode::Impl::~Impl({}/{}) Failed to send close-message: {}",
                     TYPE_NAME, MODE_NAME, CODE);
        }

        _pool.join();
        tDLogIfN(PARAMS.verbose, "MqNode::Impl::~Impl({}/{}) Done.", TYPE_NAME, MODE_NAME);

        _mq.reset();
    }

public:
    void runner()
    {
        using namespace libtbag::mq::details;
        char const * const TYPE_NAME = getTypeName(PARAMS.type);
        char const * const MODE_NAME = getModeName(MODE);

        tDLogIfI(PARAMS.verbose, "MqNode::Impl::runner({}/{}) Loop start", TYPE_NAME, MODE_NAME);
        _last = _loop.run();

        if (isSuccess(_last)) {
            tDLogIfI(PARAMS.verbose, "MqNode::Impl::runner({}/{}) Loop end success.", TYPE_NAME, MODE_NAME);
        } else {
            tDLogE("MqNode::Impl::runner({}/{}) Loop end error: {}", _last, TYPE_NAME, MODE_NAME);
        }

        _pool.exit();
    }

private:
    static bool __on_accept_cb__(void * node, std::string const & peer, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (MqNode::Impl*)parent;

        if (impl->CALLBACKS.accept_cb) {
            return impl->CALLBACKS.accept_cb(peer);
        } else {
            assert(impl->_parent != nullptr);
            return impl->_parent->onAccept(peer);
        }
    }

    static MqIsConsume __on_write_cb__(MqMsg & msg, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (MqNode::Impl*)parent;

        bool result;
        if (impl->CALLBACKS.write_cb) {
            result = impl->CALLBACKS.write_cb(msg);
        } else {
            assert(impl->_parent != nullptr);
            result = impl->_parent->onWrite(msg);
        }
        return result ? MqIsConsume::MIC_CONSUMED : MqIsConsume::MIC_PASS;
    }

    static MqIsConsume __on_recv_cb__(MqMsg const & msg, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (MqNode::Impl*)parent;

        bool result;
        if (impl->CALLBACKS.recv_cb) {
            result = impl->CALLBACKS.recv_cb(msg);
        } else {
            assert(impl->_parent != nullptr);
            result = impl->_parent->onRecv(msg);
        }
        return result ? MqIsConsume::MIC_CONSUMED : MqIsConsume::MIC_PASS;
    }

public:
    MqParams params() const
    {
        return PARAMS;
    }

    Err waitEnable(uint64_t timeout_nano)
    {
        return _mq->waitEnable(timeout_nano);
    }

    Err join()
    {
        _pool.join();
        return Err::E_SUCCESS;
    }

    Err send(MqMsg const & msg)
    {
        return _mq->send(msg);
    }

    Err recv(MqMsg & msg)
    {
        return _mq->recv(msg);
    }

    Err recvWait(MqMsg & msg, uint64_t timeout_nano)
    {
        return _mq->waitRecv(msg, timeout_nano);
    }
};

// ----------------------
// MqNode implementation.
// ----------------------

MqNode::MqNode(MqParams const & params, MqMode mode, Callbacks const & cbs)
        : _impl(std::make_unique<Impl>(this, params, mode, cbs))
{
    assert(static_cast<bool>(_impl));
}

MqNode::MqNode(std::string const & uri, MqMode mode, Callbacks const & cbs)
        : MqNode(getParams(uri), mode, cbs)
{
    assert(static_cast<bool>(_impl));
}

MqNode::MqNode(MqNode && obj) TBAG_NOEXCEPT
        : _impl(std::move(obj._impl))
{
    // EMPTY.
}

MqNode::~MqNode()
{
    _impl.reset();
}

MqNode & MqNode::operator =(MqNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void MqNode::swap(MqNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

bool MqNode::onAccept(std::string const & peer)
{
    return true;
}

bool MqNode::onWrite(MqMsg & msg)
{
    return false;
}

bool MqNode::onRecv(MqMsg const & msg)
{
    return false;
}

MqParams MqNode::params() const
{
    assert(static_cast<bool>(_impl));
    return _impl->params();
}

Err MqNode::waitEnable(uint64_t timeout_nano)
{
    assert(static_cast<bool>(_impl));
    return _impl->waitEnable(timeout_nano);
}

Err MqNode::join()
{
    assert(static_cast<bool>(_impl));
    return _impl->join();
}

Err MqNode::send(MqMsg const & msg)
{
    assert(static_cast<bool>(_impl));
    return _impl->send(msg);
}

Err MqNode::send(char const * buffer, std::size_t size)
{
    return send(MqMsg(buffer, size));
}

Err MqNode::send(MqEvent event, char const * buffer, std::size_t size)
{
    return send(MqMsg(event, buffer, size));
}

Err MqNode::send(std::string const & text)
{
    return send(MqMsg(text));
}

Err MqNode::send(MqEvent event, std::string const & text)
{
    return send(MqMsg(event, text));
}

Err MqNode::send(MqMsg::Buffer const & buffer)
{
    return send(MqMsg(buffer));
}

Err MqNode::send(MqEvent event, MqMsg::Buffer const & buffer)
{
    return send(MqMsg(event, buffer));
}

Err MqNode::sendCloseMsg()
{
    return send(MqMsg(libtbag::mq::details::ME_CLOSE, 0U));
}

Err MqNode::recv(MqMsg & msg)
{
    assert(static_cast<bool>(_impl));
    return _impl->recv(msg);
}

Err MqNode::recvWait(MqMsg & msg, uint64_t timeout_nano)
{
    assert(static_cast<bool>(_impl));
    return _impl->recvWait(msg, timeout_nano);
}

MqNode::MqParams MqNode::getParams(std::string const & uri)
{
    return libtbag::mq::details::convertUriToParams(uri);
}

MqNode MqNode::bind(MqParams const & params)
{
    return MqNode(params, MqMode::MM_BIND);
}

MqNode MqNode::bind(std::string const & uri)
{
    return bind(getParams(uri));
}

MqNode MqNode::connect(MqParams const & params)
{
    return MqNode(params, MqMode::MM_CONNECT);
}

MqNode MqNode::connect(std::string const & uri)
{
    return connect(getParams(uri));
}

// ----------
// Utilities.
// ----------

MqUniqueNode bindUniqueNode(MqParams const & params, MqNodeCbs const & cbs)
{
    return std::make_unique<MqNode>(params, MqNode::MqMode::MM_BIND, cbs);
}

MqUniqueNode bindUniqueNode(std::string const & uri, MqNodeCbs const & cbs)
{
    return std::make_unique<MqNode>(uri, MqNode::MqMode::MM_BIND, cbs);
}

MqUniqueNode connectUniqueNode(MqParams const & params, MqNodeCbs const & cbs)
{
    return std::make_unique<MqNode>(params, MqNode::MqMode::MM_CONNECT, cbs);
}

MqUniqueNode connectUniqueNode(std::string const & uri, MqNodeCbs const & cbs)
{
    return std::make_unique<MqNode>(uri, MqNode::MqMode::MM_CONNECT, cbs);
}

MqSharedNode bindSharedNode(MqParams const & params, MqNodeCbs const & cbs)
{
    return std::make_shared<MqNode>(params, MqNode::MqMode::MM_BIND, cbs);
}

MqSharedNode bindSharedNode(std::string const & uri, MqNodeCbs const & cbs)
{
    return std::make_shared<MqNode>(uri, MqNode::MqMode::MM_BIND, cbs);
}

MqSharedNode connectSharedNode(MqParams const & params, MqNodeCbs const & cbs)
{
    return std::make_shared<MqNode>(params, MqNode::MqMode::MM_CONNECT, cbs);
}

MqSharedNode connectSharedNode(std::string const & uri, MqNodeCbs const & cbs)
{
    return std::make_shared<MqNode>(uri, MqNode::MqMode::MM_CONNECT, cbs);
}

#ifndef __TBAG_MQ_DEFAULT_TRY
#define __TBAG_MQ_DEFAULT_TRY(body) \
    try {                           \
        body;                       \
    } catch (ErrException e) {      \
        return e.CODE;              \
    } catch (std::exception e) {    \
        return Err::E_UNKEXCP;      \
    } catch (...) {                 \
        return Err::E_UNKNOWN;      \
    }                               \
    return Err::E_SUCCESS;
#endif

Err bind(MqUniqueNode & node, MqParams const & params, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = bindUniqueNode(params, cbs));
}

Err bind(MqUniqueNode & node, std::string const & uri, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = bindUniqueNode(uri, cbs));
}

Err bind(MqSharedNode & node, MqParams const & params, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = bindSharedNode(params, cbs));
}

Err bind(MqSharedNode & node, std::string const & uri, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = bindSharedNode(uri, cbs));
}

Err connect(MqUniqueNode & node, MqParams const & params, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = connectUniqueNode(params, cbs));
}

Err connect(MqUniqueNode & node, std::string const & uri, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = connectUniqueNode(uri, cbs));
}

Err connect(MqSharedNode & node, MqParams const & params, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = connectSharedNode(params, cbs));
}

Err connect(MqSharedNode & node, std::string const & uri, MqNodeCbs const & cbs)
{
    __TBAG_MQ_DEFAULT_TRY(node = connectSharedNode(uri, cbs));
}

#undef __TBAG_MQ_DEFAULT_TRY

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

