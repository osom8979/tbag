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
#include <chrono>

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
    using TimePoint = std::chrono::system_clock::time_point;

public:
    int const TIMEOUT_MILLISEC;
    TimePoint const START_TIME;

private:
    SharedTimeout _timeout;
    HttpRequest   _request;
    HttpResponse  _response;

public:
    static bool isTlsSchema(Uri const & uri)
    {
        using namespace libtbag::string;
        return lower(trim(uri.getSchema())) == HTTPS_SCHEMA_LOWER;
    }

public:
    Impl(Uri const & uri, std::string const & method, HttpCommon const & common, int timeout_millisec)
            : HttpClient(getDefaultParams(uri, timeout_millisec), false, isTlsSchema(uri)),
              TIMEOUT_MILLISEC(timeout_millisec), START_TIME(std::chrono::system_clock::now())
    {
        _request.http_major = common.http_major;
        _request.http_minor = common.http_minor;
        _request.header = common.header;
        _request.body = common.body;

        auto const REQUEST_PATH = uri.getRequestPath();
        if (REQUEST_PATH.empty()) {
            _request.path = ROOT_PATH;
        } else {
            _request.path = uri.getRequestPath();
        }

        if (method.empty()) {
            _request.method = HttpMethodGET::getMethod();
        } else {
            _request.method = method;
        }

        if (!_request.exists(HEADER_HOST)) {
            _request.insert(HEADER_HOST, uri.getHost());
        }
        _request.updateDefaultRequest();
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
    virtual void onBegin() override
    {
        if (TIMEOUT_MILLISEC > 0) {
            using namespace std::chrono;
            auto const ELAPSED_TIME = system_clock::now() - START_TIME;
            if (ELAPSED_TIME >= milliseconds(TIMEOUT_MILLISEC)) {
                tDLogW("SimpleHttpClient::Impl::onBegin() Connection timeout!");
                return;
            }

            auto const NEXT_TIMEOUT = TIMEOUT_MILLISEC - duration_cast<milliseconds>(ELAPSED_TIME).count();
            assert(NEXT_TIMEOUT >= 1);

            _timeout = loop().newHandle<Timeout>(loop(), this);
            assert(static_cast<bool>(_timeout));
            assert(_timeout->isInit());

            auto const START_CODE = _timeout->start(NEXT_TIMEOUT);
            assert(isSuccess(START_CODE));
        }

        auto const CODE = writeRequest(_request);
        assert(isSuccess(CODE));
    }

    virtual void onEnd() override
    {
        // Done.
    }

public:
    void onTimeout(Timeout * timeout)
    {
        assert(timeout != nullptr);
        assert(timeout == _timeout.get());

        tDLogW("SimpleHttpClient::Impl::onTimeout() Timeout!");
        _response.setHttpStatus(HttpStatus::SC_TBAG_LIBRARY_TIMEOUT);

        stopTimerAndExit();
    }

protected:
    virtual void onRegularHttp(HttpResponse const & response) override
    {
        _response = response;
        stopTimerAndExit();
    }

    virtual void onError(Err code) override
    {
        tDLogE("SimpleHttpClient::Impl::onError() Error code: {}", code);
        _response.setHttpStatus(HttpStatus::SC_TBAG_UNKNOWN_EXCEPTION);
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

HttpResponse request(std::string const & uri, std::string const & method, HttpCommon const & common, int timeout)
{ return SimpleHttpClient(uri, method, common, timeout).waitResponse(); }

HttpResponse requestGet(std::string const & uri, HttpCommon const & common, int timeout)
{ return request(uri, HttpMethodGET::getMethod(), common, timeout); }
HttpResponse requestPost(std::string const & uri, HttpCommon const & common, int timeout)
{ return request(uri, HttpMethodPOST::getMethod(), common, timeout); }
HttpResponse requestPut(std::string const & uri, HttpCommon const & common, int timeout)
{ return request(uri, HttpMethodPUT::getMethod(), common, timeout); }
HttpResponse requestDelete(std::string const & uri, HttpCommon const & common, int timeout)
{ return request(uri, HttpMethodDELETE::getMethod(), common, timeout); }

HttpResponse requestGet(std::string const & uri, int timeout)
{ return requestGet(uri, HttpCommon{}, timeout); }
HttpResponse requestPost(std::string const & uri, int timeout)
{ return requestPost(uri, HttpCommon{}, timeout); }
HttpResponse requestPut(std::string const & uri, int timeout)
{ return requestPut(uri, HttpCommon{}, timeout); }
HttpResponse requestDelete(std::string const & uri, int timeout)
{ return requestDelete(uri, HttpCommon{}, timeout); }

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

