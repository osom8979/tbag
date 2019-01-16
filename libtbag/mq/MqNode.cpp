/**
 * @file   MqNode.cpp
 * @brief  MqNode class implementation.
 * @author zer0
 * @date   2018-11-11
 */

#include <libtbag/mq/MqNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/mq/node/MqLocalQueue.hpp>
#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

using MqParams    = MqNode::MqParams;
using MqInternal  = MqNode::MqInternal;
using MqMsg       = MqNode::MqMsg;
using MqIsConsume = MqNode::MqIsConsume;

bool onAcceptCb(void * stream, std::string const & peer, void * parent)
{
    auto * mq = (MqNode*)parent;
    assert(mq != nullptr);
    if (mq->_callbacks.accept_cb) {
        return mq->_callbacks.accept_cb(peer);
    }
    return true;
}

MqIsConsume onWriteCb(MqMsg & msg, void * parent)
{
    auto * mq = (MqNode*)parent;
    assert(mq != nullptr);
    if (mq->_callbacks.write_cb) {
        return mq->_callbacks.write_cb(msg) ? MqIsConsume::MIC_CONSUMED : MqIsConsume::MIC_PASS;
    }
    return MqIsConsume::MIC_PASS;
}

MqIsConsume onRecvCb(MqMsg const & msg, void * parent)
{
    auto * mq = (MqNode*)parent;
    assert(mq != nullptr);
    if (mq->_callbacks.recv_cb) {
        return mq->_callbacks.recv_cb(msg) ? MqIsConsume::MIC_CONSUMED : MqIsConsume::MIC_PASS;
    }
    return MqIsConsume::MIC_PASS;
}

// ---------------------
// MqNode implementation
// ---------------------

MqNode::MqNode(MqParams const & params, MqMode mode, no_init_t)
        : PARAMS(params), MODE(mode), _pool(THREAD_SIZE), _last(Err::E_EBUSY), _callbacks()
{
    assert(!static_cast<bool>(_mq));
}

MqNode::MqNode(std::string const & uri, MqMode mode, no_init_t)
        : MqNode(getParams(uri), mode, no_init)
{
    assert(!static_cast<bool>(_mq));
}

MqNode::MqNode(MqParams const & params, MqMode mode, Callbacks const & cbs)
        : PARAMS(params), MODE(mode), _pool(THREAD_SIZE), _last(Err::E_EBUSY), _callbacks(cbs)
{
    MqInternal internal;
    internal.accept_cb = &onAcceptCb;
    internal.write_cb  = &onWriteCb;
    internal.recv_cb   = &onRecvCb;
    internal.parent    = this;

    if (!init(internal)) {
        throw std::bad_alloc();
    }
}

MqNode::MqNode(std::string const & uri, MqMode mode, Callbacks const & cbs)
        : MqNode(getParams(uri), mode, cbs)
{
    assert(static_cast<bool>(_mq));
}

MqNode::MqNode(MqBindMode, MqParams const & params, Callbacks const & cbs)
        : MqNode(params, MqMode::MM_BIND, cbs)
{
    assert(static_cast<bool>(_mq));
}

MqNode::MqNode(MqBindMode, std::string const & uri, Callbacks const & cbs)
        : MqNode(uri, MqMode::MM_BIND, cbs)
{
    assert(static_cast<bool>(_mq));
}

MqNode::MqNode(MqConnectMode, MqParams const & params, Callbacks const & cbs)
        : MqNode(params, MqMode::MM_CONNECT, cbs)
{
    assert(static_cast<bool>(_mq));
}

MqNode::MqNode(MqConnectMode, std::string const & uri, Callbacks const & cbs)
        : MqNode(uri, MqMode::MM_CONNECT, cbs)
{
    assert(static_cast<bool>(_mq));
}

MqNode::~MqNode()
{
    assert(static_cast<bool>(_mq));
    auto const CODE = _mq->exit();
    if (isSuccess(CODE)) {
        tDLogIfD(PARAMS.verbose, "MqNode::~MqNode({}/{}) Send a close message.",
                 getTypeName(), getModeName());
    } else {
        tDLogIfW(PARAMS.verbose, "MqNode::~MqNode({}/{}) Failed to send close-message: {}",
                 getTypeName(), getModeName(), CODE);
    }

    _pool.join();
    tDLogIfN(PARAMS.verbose, "MqNode::~MqNode({}/{}) Done.", getTypeName(), getModeName());

    _mq.reset();
}

bool MqNode::init(MqInternal const & internal)
{
    using MqLocalQueue   = libtbag::mq::node::MqLocalQueue;
    using MqStreamClient = libtbag::mq::node::MqStreamClient;
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

    assert(MODE != MqMode::MM_NONE);
    if (PARAMS.type == MqType::MT_LOCAL) {
        _mq = std::make_unique<MqLocalQueue>(_loop, internal, PARAMS);
    } else {
        if (MODE == MqMode::MM_BIND) {
            _mq = std::make_unique<MqStreamServer>(_loop, internal, PARAMS);
        } else {
            assert(MODE == MqMode::MM_CONNECT);
            _mq = std::make_unique<MqStreamClient>(_loop, internal, PARAMS);
        }
    }

    if (!_mq) {
        return false;
    }

    if (PARAMS.on_create_loop != nullptr) {
        PARAMS.on_create_loop(_loop, PARAMS);
    }

    bool const PUSH_RESULT = _pool.push([&](){
        runner();
    });
    assert(PUSH_RESULT);
    assert(static_cast<bool>(_mq));

    libtbag::mq::details::waitOnActivation(PARAMS, _mq.get());
    return true;
}

void MqNode::runner()
{
    tDLogIfI(PARAMS.verbose, "MqNode::runner({}/{}) Loop start", getTypeName(), getModeName());
    _last = _loop.run();

    if (isSuccess(_last)) {
        tDLogIfI(PARAMS.verbose, "MqNode::runner({}/{}) Loop end success.", getTypeName(), getModeName());
    } else {
        tDLogE("MqNode::runner({}/{}) Loop end error: {}", _last, getTypeName(), getModeName());
    }

    _pool.exit();
}

void MqNode::join()
{
    _pool.join();
}

Err MqNode::exit()
{
    assert(static_cast<bool>(_mq));
    return _mq->exit();
}

Err MqNode::send(MqMsg const & msg)
{
    assert(static_cast<bool>(_mq));
    return _mq->send(msg);
}

Err MqNode::send(char const * buffer, std::size_t size, std::intptr_t id)
{
    return send(MqMsg(buffer, size, id));
}

Err MqNode::send(MqEvent event, char const * buffer, std::size_t size, std::intptr_t id)
{
    return send(MqMsg(event, buffer, size, id));
}

Err MqNode::send(std::string const & text, std::intptr_t id)
{
    return send(MqMsg(text, id));
}

Err MqNode::send(MqEvent event, std::string const & text, std::intptr_t id)
{
    return send(MqMsg(event, text, id));
}

Err MqNode::send(MqMsg::Buffer const & buffer, std::intptr_t id)
{
    return send(MqMsg(buffer, id));
}

Err MqNode::send(MqEvent event, MqMsg::Buffer const & buffer, std::intptr_t id)
{
    return send(MqMsg(event, buffer, id));
}

Err MqNode::sendClose(std::intptr_t id)
{
    return send(MqMsg(libtbag::mq::details::ME_CLOSE, id));
}

Err MqNode::recv(MqMsg & msg)
{
    assert(static_cast<bool>(_mq));
    return _mq->recv(msg);
}

Err MqNode::recvWait(MqMsg & msg, uint64_t timeout_nano)
{
    assert(static_cast<bool>(_mq));
    return _mq->waitRecv(msg, timeout_nano);
}

Err MqNode::waitEnable(uint64_t timeout_nano)
{
    assert(static_cast<bool>(_mq));
    return _mq->waitEnable(timeout_nano);
}

MqParams MqNode::getParams(std::string const & uri)
{
    return libtbag::mq::details::convertUriToParams(uri);
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

