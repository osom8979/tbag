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
    if (code == Err::E_SUCCESS) {
        using namespace std::chrono;

        auto buffer = _builder.request();
        Millisec left_time = _timeout - duration_cast<Millisec>(SystemClock::now() - _start_time);
        if (this->write(buffer.data(), buffer.size(), left_time.count()) == false) {
            _response_cb(Err::E_WRERR, _parser);
            this->close();
        }
    } else {
        _response_cb(code, _parser);
        this->close();
    }
}

void HttpClient::onShutdown(Err code)
{
    _response_cb(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code, _parser);
    this->close();
}

void HttpClient::onWrite(Err code)
{
    if (code == Err::E_SUCCESS) {
        this->start();
    } else {
        _response_cb(code, _parser);
        this->close();
    }
}

void HttpClient::onRead(Err code, char const * buffer, std::size_t size)
{
    if (code == Err::E_EOF) {
        _parser.execute(buffer, size);
        _response_cb(Err::E_SUCCESS, _parser);

        this->stop();
        this->close();
    } else if (code == Err::E_SUCCESS) {
        _parser.execute(buffer, size);
        if (_parser.isComplete()) {
            _response_cb(Err::E_SUCCESS, _parser);

            this->stop();
            this->close();
        }
    } else {
        _response_cb(code, _parser);
        this->stop();
        this->close();
    }
}

// ----------
// Utilities.
// ----------

Err requestWithSync(Uri const & uri, HttpBuilder const & request, uint64_t timeout, HttpResponse & result)
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

    HttpBuilder real_request = request;
    if (real_request.method.empty()) {
        real_request.setMethod(METHOD_GET);
    }
    if (real_request.version == HttpVersion(0, 0)) {
        real_request.version.set(1, 1);
    }
    if (real_request.url.empty()) {
        real_request.url = uri.getRequestPath();
    }
    if (real_request.existsHeader(HEADER_HOST) == false) {
        real_request.insertHeader(HEADER_HOST, uri.getHost());
    }
    if (real_request.existsHeader(HEADER_USER_AGENT) == false) {
        real_request.insertHeader(HEADER_USER_AGENT, HEADER_DEFAULT_USER_AGENT);
    }
    if (real_request.existsHeader(HEADER_ACCEPT) == false) {
        real_request.insertHeader(HEADER_ACCEPT, HEADER_DEFAULT_ACCEPT);
    }

    Err http_result = Err::E_UNKNOWN;
    http.setup(real_request, [&](Err code, HttpParser const & response){
        http_result = code;
        result.version.set(response.getHttpMajor(), response.getHttpMinor());
        result.headers = response.headers;
        result.body = response.body;
        result.status = response.getStatusCode();
        result.reason = response.getErrnoDescription();
    }, HttpClient::Millisec(timeout));

    Err LOOP_RESULT = loop.run();
    if (LOOP_RESULT != Err::E_SUCCESS) {
        return LOOP_RESULT;
    }

    return http_result;
}

Err requestWithSync(Uri const & uri, uint64_t timeout, HttpResponse & result)
{
    return requestWithSync(uri, HttpBuilder(1, 1), timeout, result);
}

Err requestWithSync(std::string const & uri, uint64_t timeout, HttpResponse & result)
{
    return requestWithSync(Uri(uri), HttpBuilder(1, 1), timeout, result);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

