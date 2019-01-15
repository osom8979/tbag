/**
 * @file   SimpleHttpClient.cpp
 * @brief  SimpleHttpClient class implementation.
 * @author zer0
 * @date   2019-01-14
 */

#include <libtbag/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

SimpleHttpClient::SimpleHttpClient(Uri const & uri, std::string const & method,
                                   HttpCommon const & common, int timeout_millisec)
        : TIMEOUT_MILLISEC(timeout_millisec), START_TIME(std::chrono::system_clock::now())
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

    using namespace libtbag::string;
    auto const ENABLE_TLS = (lower(trim(uri.getSchema())) == HTTPS_SCHEMA_LOWER);
    auto const PARAMS = HttpClient::getDefaultParams(uri, timeout_millisec);

    HttpClient::Callbacks cbs;
    cbs.begin_cb = [&](){
        onBegin();
    };
    cbs.end_cb = [&](){
        onEnd();
    };
    cbs.http_cb = [&](HttpResponse const & response){
        onRegularHttp(response);
    };
    cbs.error_cb = [&](Err code){
        onError(code);
    };
    _client = std::make_unique<HttpClient>(PARAMS, cbs, false, ENABLE_TLS);
    assert(static_cast<bool>(_client));
}

SimpleHttpClient::SimpleHttpClient(std::string const & uri, std::string const & method,
                                   HttpCommon const & common, int timeout_millisec)
        : SimpleHttpClient(libtbag::network::Uri(uri), method, common, timeout_millisec)
{
    assert(static_cast<bool>(_client));
}

SimpleHttpClient::~SimpleHttpClient()
{
    _client.reset();
}

void SimpleHttpClient::onBegin()
{
    if (TIMEOUT_MILLISEC > 0) {
        using namespace std::chrono;
        auto const ELAPSED_TIME = system_clock::now() - START_TIME;
        if (ELAPSED_TIME >= milliseconds(TIMEOUT_MILLISEC)) {
            tDLogW("SimpleHttpClient::onBegin() Connection timeout!");
            return;
        }

        auto const NEXT_TIMEOUT = TIMEOUT_MILLISEC - duration_cast<milliseconds>(ELAPSED_TIME).count();
        assert(NEXT_TIMEOUT >= 1);

        _timeout = _client->loop().newHandle<Timeout>(_client->loop(), this);
        assert(static_cast<bool>(_timeout));
        assert(_timeout->isInit());

        auto const START_CODE = _timeout->start(NEXT_TIMEOUT);
        assert(isSuccess(START_CODE));
    }

    auto const CODE = _client->writeRequest(_request);
    assert(isSuccess(CODE));
    tDLogI("SimpleHttpClient::onBegin() Write request: {}", _request.toRequestLine());
}

void SimpleHttpClient::onEnd()
{
    tDLogD("SimpleHttpClient::onEnd() Done.");
}

void SimpleHttpClient::onTimeout(Timeout * timeout)
{
    assert(timeout != nullptr);
    assert(timeout == _timeout.get());

    tDLogW("SimpleHttpClient::onTimeout() Timeout!");
    _response.setHttpStatus(HttpStatus::SC_TBAG_LIBRARY_TIMEOUT);

    stopTimerAndExit();
}

void SimpleHttpClient::onRegularHttp(HttpResponse const & response)
{
    tDLogD("SimpleHttpClient::onRegularHttp() Response result: {}", response.toStatusLine());

    _response = response;
    stopTimerAndExit();
}

void SimpleHttpClient::onError(Err code)
{
    tDLogE("SimpleHttpClient::onError() Error code: {}", code);
    _response.setHttpStatus(HttpStatus::SC_TBAG_UNKNOWN_EXCEPTION);
    stopTimerAndExit();
}

void SimpleHttpClient::stopTimerAndExit()
{
    if (TIMEOUT_MILLISEC > 0) {
        assert(static_cast<bool>(_timeout));
        if (!_timeout->isClosing()) {
            _timeout->close();
        }
    }

    auto const EXIT_CODE = _client->exit();
    assert(isSuccess(EXIT_CODE));
}

HttpResponse SimpleHttpClient::waitResponse() const
{
    assert(static_cast<bool>(_client));
    _client->join();
    return _response;
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

