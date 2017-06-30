/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpClient::HttpClient(Loop & loop, StreamType type)
        : Parent(loop, type), _builder(), _parser(HttpParser::Type::RESPONSE), _call_once(false)
{
    setSkipTimeout();
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

void HttpClient::setup(HttpBuilder const & request, Callback const & cb, uint64_t const & timeout)
{
    _builder    = request;
    _timeout    = Millisec(timeout);
    _start_time = SystemClock::now(); // Markup timer.
    _callback   = cb;
}

void HttpClient::runCallback(EventStep step, Err code)
{
    if (static_cast<bool>(_callback)) {
        _callback(Event(step, code, _parser));
    }
    _call_once = true;
}

void HttpClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onConnect() {} error.", getErrName(code));
        runCallback(EventStep::ET_CONNECT, code);
        close();
        return;
    }

    auto buffer = _builder.toDefaultRequestString();
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogE("HttpClient::onConnect() write {} error.", getErrName(WRITE_CODE));
        runCallback(EventStep::ET_CONNECT, WRITE_CODE);
        close();
        return;
    }

    using namespace std::chrono;
    Millisec const LEFT_TIME = _timeout - duration_cast<Millisec>(SystemClock::now() - _start_time);

    if (LEFT_TIME.count() <= 0) {
        tDLogE("HttpClient::onConnect() timeout.");
        runCallback(EventStep::ET_CONNECT, Err::E_TIMEOUT);
        close();
        return;
    }
}

void HttpClient::onShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    close();
}

void HttpClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onWrite() {} error.", getErrName(code));
        runCallback(EventStep::ET_WRITE, code);
        close();
        return;
    }

    Err const START_CODE = start();
    if (START_CODE != Err::E_SUCCESS) {
        tDLogE("HttpClient::onWrite() start {} error.", getErrName(START_CODE));
        runCallback(EventStep::ET_WRITE, START_CODE);
        close();
        return;
    }
}

void HttpClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        tDLogI("HttpClient::onRead() EOF.");
        if (_call_once == false) {
            runCallback(EventStep::ET_READ, Err::E_NO_RESPONSE);
        }
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onRead() {} error", getErrName(code));
        runCallback(EventStep::ET_READ, code);
        close();
        return;
    }

    assert(code == Err::E_SUCCESS);
    _parser.execute(packet.buffer, packet.size);

    if (_parser.isComplete()) {
        tDLogD("HttpClient::onRead() Completed http parsing (HTTP STATUS: {}).", _parser.getStatusCode());
        runCallback(EventStep::ET_READ, Err::E_SUCCESS);
        close();
        return;
    }

    tDLogD("HttpClient::onRead() Incomplete. Wait for response data.");
}

void HttpClient::onClose()
{
    tDLogD("HttpClient::onClose()");
    if (_call_once == false) {
        runCallback(EventStep::ET_CLOSE, Err::E_TIMEOUT);
    }
}

// ----------
// Utilities.
// ----------

Err requestWithSync(HttpClient::StreamType type,
                    std::string const & host,
                    int port,
                    Uri const & uri,
                    HttpRequest const & request,
                    uint64_t timeout,
                    HttpResponse & result)
{
    using Loop = uvpp::Loop;
    Loop loop;

    HttpClient http(loop, type);

    if (http.init(host.c_str(), port, timeout) != Err::E_SUCCESS) {
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

    http.setup(builder, [&](HttpClient::Event const & e){
        http_result = e.code;
        if (e.step == HttpClient::EventStep::ET_READ && e.code == Err::E_SUCCESS) {
            tDLogI("requestWithSync() Response Err({}) HTTP STATUS: {}",
                   getErrName(e.code), e.response.getStatusCode());
            result = e.response.getResponse();
        } else {
            tDLogE("requestWithSync() HttpClient Err({})", getErrName(e.code));
        }
    }, timeout);

    Err LOOP_RESULT = loop.run();
    if (LOOP_RESULT != Err::E_SUCCESS) {
        return LOOP_RESULT;
    }

    return http_result;
}

Err requestWithSync(Uri const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result)
{
    std::string host;
    int port = 0;

    Uri::AddrFlags const FLAG = Uri::AddrFlags::MOST_IPV4;
    Err ADDRINFO_RESULT = uri.requestAddrInfo(host, port, FLAG);
    if (ADDRINFO_RESULT != Err::E_SUCCESS) {
        return ADDRINFO_RESULT;
    }
    return requestWithSync(HttpClient::StreamType::TCP, host, port, uri, request, timeout, result);
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

