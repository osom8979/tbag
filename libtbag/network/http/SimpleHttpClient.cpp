/**
 * @file   SimpleHttpClient.cpp
 * @brief  SimpleHttpClient class implementation.
 * @author zer0
 * @date   2017-09-29
 */

#include <libtbag/network/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

SimpleHttpClient::SimpleHttpClient(Loop & loop, StreamType type)
        : HttpClient(loop, type)
{
    // EMPTY.
}

SimpleHttpClient::~SimpleHttpClient()
{
    // EMPTY.
}

void SimpleHttpClient::callOnErrorAndClose(Err code)
{
    if (static_cast<bool>(_error_cb)) {
        _error_cb(code);
    }
    close();
}

void SimpleHttpClient::onShutdown(Err code)
{
    callOnErrorAndClose(Err::E_INACCES);
}

void SimpleHttpClient::onWrite(Err code)
{
    if (isFailure(code)) {
        callOnErrorAndClose(code);
    }
}

void SimpleHttpClient::onClose()
{
    // EMPTY.
}

void SimpleHttpClient::onTimer()
{
    callOnErrorAndClose(Err::E_TIMEOUT);
}

void SimpleHttpClient::onContinue(void * arg)
{
    // EMPTY.
}

bool SimpleHttpClient::onSwitchingProtocol(HttpProperty const & response, void * arg)
{
    callOnErrorAndClose(Err::E_INACCES);
    return false;
}

void SimpleHttpClient::onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg)
{
    callOnErrorAndClose(Err::E_INACCES);
}

void SimpleHttpClient::onRegularHttp(HttpProperty const & response, void * arg)
{
    if (static_cast<bool>(_response_cb)) {
        _response_cb(response);
    }
    stopTimer();
    close();
}

void SimpleHttpClient::onParseError(Err code, void * arg)
{
    callOnErrorAndClose(code);
}

void SimpleHttpClient::onOpen()
{
    Err const WRITE_CODE = writeRequest(_request);
    if (isFailure(WRITE_CODE)) {
        callOnErrorAndClose(WRITE_CODE);
    }
}

void SimpleHttpClient::onEof()
{
    close();
}

void SimpleHttpClient::onError(EventType from, Err code)
{
    callOnErrorAndClose(code);
}

// ----------
// Utilities.
// ----------

Err requestWithSync(Uri const & uri,
                    HttpRequest const & request,
                    HttpResponse & response,
                    uint64_t timeout,
                    HttpClient::StreamType type)
{
    std::string host;
    int port = DEFAULT_HTTP_PORT;

    if (type == HttpClient::StreamType::PIPE) {
        host = uri.toString();
        port = 0;
    } else {
        Err const ADDR_CODE = uri.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4);
        if (ADDR_CODE != Err::E_SUCCESS) {
            return ADDR_CODE;
        }
    }

    uvpp::Loop loop;
    std::shared_ptr<SimpleHttpClient> http;
    try {
        http.reset(new SimpleHttpClient(loop, type));
    } catch (...) {
        return Err::E_BADALLOC;
    }

    assert(static_cast<bool>(http));

    Err const INIT_CODE = http->init(host.c_str(), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        return INIT_CODE;
    }

    if (timeout > 0) {
        Err const TIMER_CODE = http->startTimer(timeout);
        if (TIMER_CODE != Err::E_SUCCESS) {
            http->close();
            loop.run();
            return TIMER_CODE;
        }
    }

    HttpRequest builder = request;
    if (builder.method.empty()) {
        builder.setHttpMethod(HttpMethod::M_GET);
    }
    if (builder.path.empty()) {
        builder.path.assign(uri.getRequestPath());
    }
    if (builder.exists(HEADER_HOST) == false) {
        builder.insert(HEADER_HOST, uri.getHost());
    }
    builder.updateDefaultRequest();

    tDLogI("requestWithSync() Request {}: {}", builder.method, uri.toString());
    Err code = Err::E_UNKNOWN;
    http->setRequest(builder);
    http->setOnResponse([&](HttpResponse const & r){
        code = Err::E_SUCCESS;
        response = r;
    });
    http->setOnError([&](Err const & e){
        code = e;
    });

    Err const LOOP_RESULT = loop.run();
    if (LOOP_RESULT != Err::E_SUCCESS) {
        return LOOP_RESULT;
    }

    return code;
}

Err requestWithSync(std::string const & uri, HttpRequest const & request, HttpResponse & response, uint64_t timeout)
{
    return requestWithSync(Uri(uri), request, response, timeout);
}

Err requestWithSync(std::string const & uri, HttpResponse & response, uint64_t timeout)
{
    return requestWithSync(Uri(uri), HttpProperty(), response, timeout);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

