/**
 * @file   SimpleHttpClient.cpp
 * @brief  SimpleHttpClient class implementation.
 * @author zer0
 * @date   2019-01-14
 */

#include <libtbag/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/network/Uri.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Timer.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * SimpleHttpClient::Impl class implementation.
 *
 * @author zer0
 * @date   2019-01-14
 */
struct SimpleHttpClient::Impl : public HttpClient
{
public:
    using MqParams = libtbag::mq::details::MqParams;
    using MqType   = libtbag::mq::details::MqType;
    using Uri      = libtbag::network::Uri;
    using Loop     = libtbag::uvpp::Loop;
    using Timer    = libtbag::uvpp::Timer;

public:
    struct Timeout : public Timer
    {
        Impl * parent = nullptr;

        Timeout(Loop & loop, Impl * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Timeout()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onTimeout(this); }
    };

public:
    using SharedTimeout = std::shared_ptr<Timeout>;

public:
    int const TIMEOUT_MILLISEC;

private:
    SharedTimeout _timeout;

private:
    HttpRequest  _request;
    HttpResponse _response;

public:
    static void __on_create_loop__(Loop & loop, MqParams const & params)
    {
        auto * impl = (Impl*)(params.user);
        assert(impl != nullptr);
        impl->onCreateLoop(loop);
    }

    static bool isTls(Uri const & uri)
    {
        using namespace libtbag::string;
        return lower(trim(uri.getSchema())) == HTTPS_SCHEMA_LOWER;
    }

    static MqParams getParams(Uri const & uri, void * user)
    {
        MqParams params;
        params.type = MqType::MT_TCP;
        params.send_queue_size = 4;
        params.recv_queue_size = 8;
        params.wait_closing_millisec = 1 * 1000;
        params.continuous_read_error_count = 4;
        params.connect_timeout_millisec = 4 * 1000;
        params.reconnect_count = 3;
        params.reconnect_delay_millisec = 1 * 1000;
        params.wait_on_activation_timeout_millisec = 8 * 1000;
        params.wait_next_opcode_nanosec = 1000;
        params.verbose = false;
        params.user = user;
        params.on_create_loop = &__on_create_loop__;

        if (libtbag::net::isIp(uri.getHost()) && uri.isPort()) {
            params.address = uri.getHost();
            params.port    = uri.getPortNumber();
        } else {
            std::string host;
            int port;
            if (isSuccess(uri.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4))) {
                params.address = host;
                params.port    = port;
            } else {
                params.address = uri.getHost();
                params.port    = uri.getPortNumber();
            }
        }

        return params;
    }

public:
    Impl(Uri const & uri, std::string const & method, HttpCommon const & common, int timeout_millisec)
            : HttpClient(getParams(uri, this), false, isTls(uri)), TIMEOUT_MILLISEC(timeout_millisec)
    {
        // @formatter:off
        _request.path       = uri.getRequestPath();
        _request.method     = method;
        _request.http_major = common.http_major;
        _request.http_minor = common.http_minor;
        _request.header     = common.header;
        _request.body       = common.body;
        // @formatter:on
    }

    ~Impl()
    {
        // MEPTY.
    }

public:
    HttpResponse const & atResponse() const
    {
        return _response;
    }

protected:
    void onCreateLoop(Loop & loop)
    {
        if (TIMEOUT_MILLISEC <= 0) {
            return;
        }

        _timeout = loop.newHandle<Timeout>(loop, this);
        assert(static_cast<bool>(_timeout));
        assert(_timeout->isInit());

        auto const CODE = _timeout->start(TIMEOUT_MILLISEC);
        assert(isSuccess(CODE));

        tDLogD("SimpleHttpClient::Impl::onCreateLoop() Start timer: {}ms", TIMEOUT_MILLISEC);
    }

    void onTimeout(Timeout * timeout)
    {
        assert(timeout != nullptr);
        assert(timeout == _timeout.get());
        tDLogW("SimpleHttpClient::Impl::onTimeout() Timeout!");
        stopTimerAndExit();
    }

    virtual void onRegularHttp(HttpResponse const & response) override
    {
        _response = response;
        stopTimerAndExit();
    }

    virtual void onError(Err code) override
    {
        tDLogE("SimpleHttpClient::Impl::onError() Error code: {}", code);
        stopTimerAndExit();
    }

    void stopTimerAndExit()
    {
        if (TIMEOUT_MILLISEC > 0) {
            assert(static_cast<bool>(_timeout));
            if (!_timeout->isClosing()) {
                _timeout->close();
            }
        }
        auto const EXIT_CODE = exit();
        assert(isSuccess(EXIT_CODE));
    }
};

// --------------------------------
// SimpleHttpClient implementation.
// --------------------------------

SimpleHttpClient::SimpleHttpClient(std::string const & uri, std::string const & method,
                                   HttpCommon const & common, int timeout_millisec)
        : _impl(std::make_unique<Impl>(libtbag::network::Uri(uri), method, common, timeout_millisec))
{
    assert(static_cast<bool>(_impl));
}

SimpleHttpClient::~SimpleHttpClient()
{
    // EMPTY.
}

HttpResponse SimpleHttpClient::waitResponse() const
{
    assert(static_cast<bool>(_impl));
    _impl->join();
    return _impl->atResponse();
}

// -----------------------
// Miscellaneous utilities
// -----------------------

HttpResponse request(std::string const & uri, std::string const & method,
                     HttpCommon const & common, int timeout_millisec)
{
    return SimpleHttpClient(uri, method, common, timeout_millisec).waitResponse();
}

HttpResponse requestGet(std::string const & uri, HttpCommon const & common, int timeout)
{
    return request(uri, HttpMethodGET::getMethod(), common, timeout);
}

HttpResponse requestPost(std::string const & uri, HttpCommon const & common, int timeout)
{
    return request(uri, HttpMethodPOST::getMethod(), common, timeout);
}

HttpResponse requestPut(std::string const & uri, HttpCommon const & common, int timeout)
{
    return request(uri, HttpMethodPUT::getMethod(), common, timeout);
}

HttpResponse requestDelete(std::string const & uri, HttpCommon const & common, int timeout)
{
    return request(uri, HttpMethodDELETE::getMethod(), common, timeout);
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

