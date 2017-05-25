/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpClient::HttpClient(Loop & loop, StreamType type)
        : Parent(loop, type), _builder(), _parser(HttpParser::Type::RESPONSE)
{
    // EMPTY.
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

void HttpClient::setup(HttpBuilder const & request,
                       OnResponse const & response_cb,
                       Millisec const & timeout)
{
    _builder     = request;
    _response_cb = response_cb;
    _timeout     = timeout;
    _start_time  = SystemClock::now();
}

void HttpClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onConnect() {} error.", getErrName(code));
        _response_cb(code, _parser);
        close();
        return;
    }

    auto buffer = _builder.toDefaultRequestString();
    if (write(buffer.data(), buffer.size()) == false) {
        tDLogE("HttpClient::onConnect() {} error.", getErrName(Err::E_WRERR));
        _response_cb(Err::E_WRERR, _parser);
        close();
        return;
    }

    using namespace std::chrono;
    Millisec const LEFT_TIME = _timeout - duration_cast<Millisec>(SystemClock::now() - _start_time);

    if (LEFT_TIME.count() <= 0) {
        tDLogE("HttpClient::onConnect() {} error.", getErrName(Err::E_TIMEOUT));
        _response_cb(Err::E_TIMEOUT, _parser);
        close();
        return;
    }

    startTimeoutClose(LEFT_TIME);
}

void HttpClient::onShutdown(Err code)
{
    _response_cb(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code, _parser);
    close();
}

void HttpClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onWrite() {} error.", getErrName(code));
        _response_cb(code, _parser);
        close();
        return;
    }

    start();
}

void HttpClient::onRead(Err code, char const * buffer, std::size_t size)
{
    if (code == Err::E_EOF) {
        tDLogI("HttpClient::onRead() EOF.");
        _response_cb(code, _parser);
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onRead() {} error", getErrName(code));
        _response_cb(code, _parser);
        close();
        return;
    }

    assert(code == Err::E_SUCCESS);

    _parser.execute(buffer, size);
    if (_parser.isComplete()) {
        tDLogD("HttpClient::onRead() Completed http parsing (HTTP STATUS: {}).", _parser.getStatusCode());

        _response_cb(Err::E_SUCCESS, _parser);
        close();
    }
}

void HttpClient::onClose()
{
    tDLogD("HttpClient::onClose()");
}

// ----------
// Utilities.
// ----------

Err requestWithSync(Uri const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result)
{
    using Loop = uvpp::Loop;
    Loop loop;
    HttpClient http(loop);

    std::string host;
    int port = 0;

    Uri::AddrFlags const FLAG = Uri::AddrFlags::MOST_IPV4;
    Err ADDRINFO_RESULT = uri.requestAddrInfo(host, port, FLAG);
    if (ADDRINFO_RESULT != Err::E_SUCCESS) {
        return ADDRINFO_RESULT;
    }

    if (http.init(host.c_str(), port, timeout) == false) {
        return Err::E_EINIT;
    }

    HttpBuilder builder = request;
    if (builder.getMethod().empty()) {
        builder.setMethod(getHttpMethodName(HttpMethod::M_GET));
    }
    if (builder.getUrl().empty()) {
        builder.setUrl(uri.getRequestPath());
    }
    if (builder.existsHeader(HEADER_HOST) == false) {
        builder.insertHeader(HEADER_HOST, uri.getHost());
    }

    tDLogI("requestWithSync() Request {}: {}", builder.getMethod(), uri.getString());

    Err http_result = Err::E_UNKNOWN;
    http.setup(builder, [&](Err code, HttpParser const & response){
        tDLogI("requestWithSync() Response Err({}) HTTP STATUS: {}", getErrName(code), response.getStatusCode());
        http_result = code;
        result = response.getResponse();
    }, HttpClient::Millisec(timeout));

    Err LOOP_RESULT = loop.run();
    if (LOOP_RESULT != Err::E_SUCCESS) {
        return LOOP_RESULT;
    }

    return http_result;
}

Err requestWithSync(std::string const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result)
{
    return requestWithSync(Uri(uri), request, timeout, result);
}

Err requestWithSync(Uri const & uri, uint64_t timeout, HttpResponse & result)
{
    return requestWithSync(uri, HttpRequest(), timeout, result);
}

Err requestWithSync(std::string const & uri, uint64_t timeout, HttpResponse & result)
{
    return requestWithSync(Uri(uri), HttpRequest(), timeout, result);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

