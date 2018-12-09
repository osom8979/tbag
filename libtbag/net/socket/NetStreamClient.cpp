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
    MqParams const PARAMS;

private:
    NetStreamClient * _parent;

private:
    Pool _pool;
    Loop _loop;
    Err  _last;

private:
    SharedMq _mq;

public:
    Impl(NetStreamClient * parent, MqParams const & params)
            : PARAMS(params), _parent(parent),
              _pool(THREAD_SIZE), _loop(), _last(Err::E_EBUSY)
    {
        assert(_parent != nullptr);

        MqInternal internal;
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
    }

    ~Impl()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);
        auto const CODE = _mq->send(MqMsg(libtbag::mq::details::ME_CLOSE));
        if (isSuccess(CODE)) {
            tDLogIfD(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Send a close message.", TYPE_NAME);
        } else {
            tDLogIfW(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Failed to send close-message: {}", TYPE_NAME, CODE);
        }

        _pool.exit();
        tDLogIfD(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Wait for Pool to exit.", TYPE_NAME);

        _pool.join();
        tDLogIfN(PARAMS.verbose, "NetStreamClient::Impl::~Impl({}) Done.", TYPE_NAME);

        _mq.reset();
    }

public:
    void runner()
    {
        char const * const TYPE_NAME = libtbag::mq::details::getTypeName(PARAMS.type);

        tDLogIfI(PARAMS.verbose, "NetStreamClient::Impl::runner({}/{}) Loop start", TYPE_NAME);
        _last = _loop.run();

        if (isSuccess(_last)) {
            tDLogIfI(PARAMS.verbose, "NetStreamClient::Impl::runner({}/{}) Loop end success.", TYPE_NAME);
        } else {
            tDLogE("NetStreamClient::Impl::runner({}/{}) Loop end error: {}", _last, TYPE_NAME);
        }

        assert(_parent != nullptr);
        _parent->onEnd();
    }

private:
    static void __on_connect_cb__(void * parent)
    {
        assert(parent != nullptr);
        auto * impl = (NetStreamClient::Impl*)parent;
        assert(impl->_parent != nullptr);
        impl->_parent->onBegin();
    }

    static std::size_t __on_default_write_cb__(void * node, char const * buffer, std::size_t size, void * parent)
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
        assert(impl->_parent != nullptr);
        impl->_parent->onRecv(buffer, size);
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

NetStreamClient::MqParams NetStreamClient::getParams(std::string const & uri)
{
    return libtbag::mq::details::convertUriToParams(uri);
}

void NetStreamClient::onBegin()
{
    // EMPTY.
}

bool NetStreamClient::onRecv(char const * buffer, std::size_t size)
{
    return true;
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

