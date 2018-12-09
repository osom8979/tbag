/**
 * @file   NetStreamServer.cpp
 * @brief  NetStreamServer class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/net/NetStreamServer.hpp>
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

namespace net {

/**
 * NetStreamServer::Impl class implementation.
 *
 * @author zer0
 * @date   2018-12-09
 */
struct NetStreamServer::Impl : private Noncopyable
{
public:
    using Pool   = libtbag::thread::ThreadPool;
    using Loop   = libtbag::uvpp::Loop;
    using Stream = libtbag::uvpp::Stream;

public:
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
    MqParams const PARAMS;

private:
    NetStreamServer * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

public:
    Impl(NetStreamServer * parent, MqParams const & params)
            : PARAMS(params), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        assert(_parent != nullptr);

        MqInternal internal;
        internal.accept_cb     = &__on_accept_cb__;
        internal.default_write = &__on_default_write_cb__;
        internal.default_read  = &__on_default_read_cb__;
        internal.close_node    = &__on_close_node_cb__;
        internal.parent        = this;

        _mq = std::make_shared<MqStreamServer>(_loop, internal, params);
        if (!_loop || !_mq) {
            throw std::bad_alloc();
        }

        auto idle = _loop.newHandle<libtbag::uvpp::func::FuncIdle>(_loop);
        assert(static_cast<bool>(idle));
        idle->idle_cb = [&](){
            _parent->onBegin();
            idle->close();
        };
        auto const IDLE_START_CODE = idle->start();
        assert(isSuccess(IDLE_START_CODE));

        bool const PUSH_RESULT = _pool.push([&](){
            runner();
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

        _pool.exit();
        tDLogIfD(PARAMS.verbose, "NetStreamServer::Impl::~Impl({}) Wait for Pool to exit.", TYPE_NAME);

        _pool.join();
        tDLogIfN(PARAMS.verbose, "NetStreamServer::Impl::~Impl({}) Done.", TYPE_NAME);

        _mq.reset();
    }

public:
    void runner()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);

        tDLogIfI(PARAMS.verbose, "NetStreamServer::Impl::runner({}/{}) Loop start", TYPE_NAME);
        _last = _loop.run();

        if (isSuccess(_last)) {
            tDLogIfI(PARAMS.verbose, "NetStreamServer::Impl::runner({}/{}) Loop end success.", TYPE_NAME);
        } else {
            tDLogE("NetStreamServer::Impl::runner({}/{}) Loop end error: {}", _last, TYPE_NAME);
        }

        assert(_parent != nullptr);
        _parent->onEnd();
    }

private:
    static bool __on_accept_cb__(void * node, std::string const & peer, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;
        assert(impl->_parent != nullptr);
        return impl->_parent->onAccept(reinterpret_cast<std::intptr_t>(node), peer);
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
            tDLogE("__on_default_write_cb__() Write error: {}", code);
        }
        return isSuccess(code) ? 1U : 0U;
    }

    static void __on_default_read_cb__(void * node, char const * buffer, std::size_t size, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;
        assert(impl->_parent != nullptr);
        impl->_parent->onRecv(reinterpret_cast<std::intptr_t>(node), buffer, size);
    }

    static void __on_close_node_cb__(void * node, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamServer::Impl*)parent;
        assert(impl->_parent != nullptr);
        impl->_parent->onClose(reinterpret_cast<std::intptr_t>(node));
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

bool NetStreamServer::onRecv(std::intptr_t id, char const * buffer, std::size_t size)
{
    return true;
}

void NetStreamServer::onClose(std::intptr_t id)
{
    // EMPTY.
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

