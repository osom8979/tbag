/**
 * @file   NetStreamClient.cpp
 * @brief  NetStreamClient class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/net/socket/NetStreamClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/thread/ThreadPool.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Stream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

/**
 * NetStreamClient::Impl class implementation.
 *
 * @author zer0
 * @date   2018-12-09
 */
struct NetStreamClient::Impl : private Noncopyable
{
public:
    using Pool   = libtbag::thread::ThreadPool;
    using Loop   = libtbag::uvpp::Loop;
    using Stream = libtbag::uvpp::Stream;

public:
    using MqStreamClient = libtbag::mq::node::MqStreamClient;

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
    MqParams  const PARAMS;

private:
    NetStreamClient * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

public:
    Impl(NetStreamClient * parent, MqParams const & params, Callbacks const & cbs = Callbacks{})
            : CALLBACKS(cbs), PARAMS(params), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        assert(_parent != nullptr);

        MqInternal internal;
        internal.connect_cb    = &__on_connect_cb__;
        internal.close_cb      = &__on_close_cb__;
        internal.default_write = &__on_default_write_cb__;
        internal.default_read  = &__on_default_read_cb__;
        internal.parent        = this;

        _mq = std::make_shared<MqStreamClient>(_loop, internal, params);
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
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);
        auto const CODE = _mq->exit();
        if (isSuccess(CODE)) {
            tDLogIfD(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Send a close message.", TYPE_NAME);
        } else {
            tDLogIfW(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Failed to send close-message: {}", TYPE_NAME, CODE);
        }

        _pool.join();
        tDLogIfN(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Done.", TYPE_NAME);

        _mq.reset();
    }

public:
    void runner()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);

        tDLogIfI(PARAMS.verbose, "NetStreamClient::Impl::runner({}) Loop start", TYPE_NAME);
        _last = _loop.run();

        if (isSuccess(_last)) {
            tDLogIfI(PARAMS.verbose, "NetStreamClient::Impl::runner({}) Loop end success.", TYPE_NAME);
        } else {
            tDLogE("NetStreamClient::Impl::runner({}) Loop end error: {}", _last, TYPE_NAME);
        }

        _pool.exit();
    }

private:
    static void __on_connect_cb__(void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamClient::Impl*)parent;

        if (impl->CALLBACKS.begin_cb) {
            impl->CALLBACKS.begin_cb();
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onBegin();
        }
    }

    static void __on_close_cb__(void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamClient::Impl*)parent;

        if (impl->CALLBACKS.end_cb) {
            impl->CALLBACKS.end_cb();
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onEnd();
        }
    }

    static std::size_t __on_default_write_cb__(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamClient::Impl*)parent;

        Err code;
        if (impl->PARAMS.type == MqType::MT_PIPE) {
            auto * tcp = (MqStreamClient::TcpClient*)node;
            code = tcp->write(tcp->write_req, buffer, size);
        } else if (impl->PARAMS.type == MqType::MT_TCP) {
            auto * pipe = (MqStreamClient::PipeClient*)node;
            code = pipe->write(pipe->write_req, buffer, size);
        } else {
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            return 0;
        }

        if (isFailure(code)) {
            tDLogE("NetStreamClient::Impl::__on_default_write_cb__() Write error: {}", code);
        }
        return isSuccess(code) ? 1U : 0U;
    }

    static void __on_default_read_cb__(void * node, char const * buffer, std::size_t size, void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamClient::Impl*)parent;

        if (impl->CALLBACKS.recv_cb) {
            impl->CALLBACKS.recv_cb(buffer, size);
        } else {
            assert(impl->_parent != nullptr);
            impl->_parent->onRecv(buffer, size);
        }
    }

public:
    void join()
    {
        _pool.join();
    }

    Err exit()
    {
        return _mq->exit();
    }

    Err send(MqMsg const & msg)
    {
        return _mq->send(msg);
    }
};

// -------------------------------
// NetStreamClient implementation.
// -------------------------------

NetStreamClient::NetStreamClient(MqParams const & params)
        : _impl(std::make_unique<Impl>(this, params))
{
    assert(static_cast<bool>(_impl));
}

NetStreamClient::NetStreamClient(std::string const & uri)
        : _impl(std::make_unique<Impl>(this, getParams(uri)))
{
    assert(static_cast<bool>(_impl));
}

NetStreamClient::NetStreamClient(MqParams const & params, Callbacks const & cbs)
        : _impl(std::make_unique<Impl>(this, params, cbs))
{
    assert(static_cast<bool>(_impl));
}

NetStreamClient::NetStreamClient(std::string const & uri, Callbacks const & cbs)
        : _impl(std::make_unique<Impl>(this, getParams(uri), cbs))
{
    assert(static_cast<bool>(_impl));
}

NetStreamClient::NetStreamClient(NetStreamClient && obj) TBAG_NOEXCEPT
        : _impl(std::move(obj._impl))
{
    // EMPTY.
}

NetStreamClient::~NetStreamClient()
{
    _impl.reset();
}

NetStreamClient & NetStreamClient::operator =(NetStreamClient && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void NetStreamClient::swap(NetStreamClient & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void NetStreamClient::join()
{
    assert(static_cast<bool>(_impl));
    _impl->join();
}

Err NetStreamClient::exit()
{
    assert(static_cast<bool>(_impl));
    return _impl->exit();
}

Err NetStreamClient::send(MqMsg const & msg)
{
    assert(static_cast<bool>(_impl));
    return _impl->send(msg);
}

Err NetStreamClient::send(char const * buffer, std::size_t size)
{
    return send(MqMsg(buffer, size));
}

Err NetStreamClient::send(MqEvent event, char const * buffer, std::size_t size)
{
    return send(MqMsg(event, buffer, size));
}

Err NetStreamClient::send(std::string const & text)
{
    return send(MqMsg(text));
}

Err NetStreamClient::send(MqEvent event, std::string const & text)
{
    return send(MqMsg(event, text));
}

Err NetStreamClient::send(MqMsg::Buffer const & buffer)
{
    return send(MqMsg(buffer));
}

Err NetStreamClient::send(MqEvent event, MqMsg::Buffer const & buffer)
{
    return send(MqMsg(event, buffer));
}

NetStreamClient::MqParams NetStreamClient::getParams(std::string const & uri)
{
    return libtbag::mq::details::convertUriToParams(uri);
}

void NetStreamClient::onBegin()
{
    // EMPTY.
}

void NetStreamClient::onRecv(char const * buffer, std::size_t size)
{
    // EMPTY.
}

void NetStreamClient::onEnd()
{
    // EMPTY.
}

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

