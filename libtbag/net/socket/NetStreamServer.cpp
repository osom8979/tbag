/**
 * @file   NetStreamServer.cpp
 * @brief  NetStreamServer class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/net/socket/NetStreamServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/thread/ThreadPool.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

/**
 * NetStreamServer::Impl class implementation.
 *
 * @author zer0
 * @date   2018-12-09
 */
struct NetStreamServer::Impl : private Noncopyable
{
public:
    using Pool     = libtbag::thread::ThreadPool;
    using Loop     = libtbag::uvpp::Loop;
    using Stream   = libtbag::uvpp::Stream;
    using FuncIdle = libtbag::uvpp::func::FuncIdle;

public:
    using MqStreamServer = libtbag::mq::node::MqStreamServer;

public:
    using MqInternal  = libtbag::mq::details::MqInternal;
    using MqIsConsume = libtbag::mq::details::MqIsConsume;
    using MqInterface = libtbag::mq::details::MqInterface;
    using SharedMq    = std::shared_ptr<MqInterface>;
    using SharedIdle  = std::shared_ptr<FuncIdle>;

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

public:
    Callbacks const CALLBACKS;
    MqParams  const PARAMS;

private:
    NetStreamServer * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq   _mq;
    SharedIdle _idle;

public:
    Impl(NetStreamServer * parent, MqParams const & params, Callbacks const & cbs = Callbacks{})
            : CALLBACKS(cbs), PARAMS(params), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        assert(_parent != nullptr);

        MqInternal internal;
        internal.accept_cb     = &__on_accept_cb__;
        internal.close_cb      = &__on_close_cb__;
        internal.default_write = &__on_default_write_cb__;
        internal.default_read  = &__on_default_read_cb__;
        internal.close_node    = &__on_close_node_cb__;
        internal.parent        = this;

        _mq = std::make_shared<MqStreamServer>(_loop, internal, params);
        if (!_loop || !_mq) {
            throw std::bad_alloc();
        }

        _idle = _loop.newHandle<libtbag::uvpp::func::FuncIdle>(_loop);
        assert(static_cast<bool>(_idle));
        _idle->idle_cb = [&](){
            if (CALLBACKS.begin_cb) {
                CALLBACKS.begin_cb();
            } else {
                assert(_parent != nullptr);
                _parent->onBegin();
            }
            _idle->close();
        };
        auto const IDLE_START_CODE = _idle->start();
        assert(isSuccess(IDLE_START_CODE));

        bool const PUSH_RESULT = _pool.push([&](){
            runner();
            _pool.exit();
        });
        assert(PUSH_RESULT);
    }

    ~Impl()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);
        auto const CODE = _mq->send(MqMsg(libtbag::mq::details::ME_CLOSE));
        if (isSuccess(CODE)) {
            tDLogIfD(PARAMS.verbose, "NetStreamServer::Impl::~Impl({}) Send a close message.", TYPE_NAME);
        } else {
            tDLogIfW(PARAMS.verbose, "NetStreamServer::Impl::~Impl({}) Failed to send close-message: {}", TYPE_NAME, CODE);
        }

        _pool.join();
        tDLogIfN(PARAMS.verbose, "NetStreamServer::Impl::~Impl({}) Done.", TYPE_NAME);

        _idle.reset();
        _mq.reset();
    }

public:
    void runner()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);

        tDLogIfI(PARAMS.verbose, "NetStreamServer::Impl::runner({}) Loop start", TYPE_NAME);
        _last = _loop.run();

        if (isSuccess(_last)) {
            tDLogIfI(PARAMS.verbose, "NetStreamServer::Impl::runner({}) Loop end success.", TYPE_NAME);
        } else {
            tDLogE("NetStreamServer::Impl::runner({}) Loop end error: {}", _last, TYPE_NAME);
        }

        _pool.exit();
    }

private:
    static bool __on_accept_cb__(void * node, std::string const & peer, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;

        if (impl->CALLBACKS.accept_cb) {
            return impl->CALLBACKS.accept_cb(reinterpret_cast<std::intptr_t>(node), peer);
        } else {
            assert(impl->_parent != nullptr);
            return impl->_parent->onAccept(reinterpret_cast<std::intptr_t>(node), peer);
        }
    }

    static void __on_close_cb__(void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;

        if (impl->CALLBACKS.end_cb) {
            impl->CALLBACKS.end_cb();
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onEnd();
        }
    }

    static std::size_t __on_default_write_cb__(void * node, char const * buffer, std::size_t size, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;

        Err code;
        if (impl->PARAMS.type == MqType::MT_PIPE) {
            auto * tcp = (MqStreamServer::TcpNode*)node;
            code = tcp->write(tcp->write_req, buffer, size);
        } else if (impl->PARAMS.type == MqType::MT_TCP) {
            auto * pipe = (MqStreamServer::PipeNode*)node;
            code = pipe->write(pipe->write_req, buffer, size);
        } else {
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            return 0;
        }

        if (isFailure(code)) {
            tDLogE("NetStreamServer::Impl::__on_default_write_cb__() Write error: {}", code);
        }
        return isSuccess(code) ? 1U : 0U;
    }

    static void __on_default_read_cb__(void * node, char const * buffer, std::size_t size, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;

        if (impl->CALLBACKS.recv_cb) {
            return impl->CALLBACKS.recv_cb(reinterpret_cast<std::intptr_t>(node), buffer, size);
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onRecv(reinterpret_cast<std::intptr_t>(node), buffer, size);
        }
    }

    static void __on_close_node_cb__(void * node, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;

        if (impl->CALLBACKS.close_cb) {
            impl->CALLBACKS.close_cb(reinterpret_cast<std::intptr_t>(node));
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onClose(reinterpret_cast<std::intptr_t>(node));
        }
    }

public:
    void join()
    {
        _pool.join();
    }

    Err send(MqMsg const & msg)
    {
        return _mq->send(msg);
    }
};

// -------------------------------
// NetStreamServer implementation.
// -------------------------------

NetStreamServer::NetStreamServer(MqParams const & params)
        : _impl(std::make_unique<Impl>(this, params))
{
    assert(static_cast<bool>(_impl));
}

NetStreamServer::NetStreamServer(std::string const & uri)
        : _impl(std::make_unique<Impl>(this, getParams(uri)))
{
    assert(static_cast<bool>(_impl));
}

NetStreamServer::NetStreamServer(MqParams const & params, Callbacks const & cbs)
        : _impl(std::make_unique<Impl>(this, params, cbs))
{
    assert(static_cast<bool>(_impl));
}

NetStreamServer::NetStreamServer(std::string const & uri, Callbacks const & cbs)
        : _impl(std::make_unique<Impl>(this, getParams(uri), cbs))
{
    assert(static_cast<bool>(_impl));
}

NetStreamServer::NetStreamServer(NetStreamServer && obj) TBAG_NOEXCEPT
        : _impl(std::move(obj._impl))
{
    // EMPTY.
}

NetStreamServer::~NetStreamServer()
{
    _impl.reset();
}

NetStreamServer & NetStreamServer::operator =(NetStreamServer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void NetStreamServer::swap(NetStreamServer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void NetStreamServer::join()
{
    _impl->join();
}

Err NetStreamServer::send(MqMsg const & msg)
{
    assert(static_cast<bool>(_impl));
    return _impl->send(msg);
}

Err NetStreamServer::send(char const * buffer, std::size_t size)
{
    return send(MqMsg(buffer, size));
}

Err NetStreamServer::send(MqEvent event, char const * buffer, std::size_t size)
{
    return send(MqMsg(event, buffer, size));
}

Err NetStreamServer::send(std::string const & text)
{
    return send(MqMsg(text));
}

Err NetStreamServer::send(MqEvent event, std::string const & text)
{
    return send(MqMsg(event, text));
}

Err NetStreamServer::send(MqMsg::Buffer const & buffer)
{
    return send(MqMsg(buffer));
}

Err NetStreamServer::send(MqEvent event, MqMsg::Buffer const & buffer)
{
    return send(MqMsg(event, buffer));
}

Err NetStreamServer::sendCloseMsg()
{
    return send(MqMsg(libtbag::mq::details::ME_CLOSE, 0U));
}

NetStreamServer::MqParams NetStreamServer::getParams(std::string const & uri)
{
    return libtbag::mq::details::convertUriToParams(uri);
}

void NetStreamServer::onBegin()
{
    // EMPTY.
}

void NetStreamServer::onEnd()
{
    // EMPTY.
}

bool NetStreamServer::onAccept(std::intptr_t id, std::string const & ip)
{
    return true;
}

void NetStreamServer::onRecv(std::intptr_t id, char const * buffer, std::size_t size)
{
    // EMPTY.
}

void NetStreamServer::onClose(std::intptr_t id)
{
    // EMPTY.
}

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

