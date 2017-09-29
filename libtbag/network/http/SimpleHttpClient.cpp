/**
 * @file   SimpleHttpClient.cpp
 * @brief  SimpleHttpClient class implementation.
 * @author zer0
 * @date   2017-09-29
 */

#include <libtbag/network/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

SimpleHttpClient::SimpleHttpClient(Loop & loop, StreamType type) : HttpClient(loop, type)
{
    // EMPTY.
}

SimpleHttpClient::~SimpleHttpClient()
{
    // EMPTY.
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
    uvpp::Loop loop;
    HttpClient http(loop, type);

    Err const INIT_CODE = http.init(host.c_str(), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        return INIT_CODE;
    }

    if (timeout > 0) {
        Err const TIMER_CODE = http.startTimer(timeout);
        if (TIMER_CODE != Err::E_SUCCESS) {
            http.close();
            loop.run();
            return TIMER_CODE;
        }
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
    });

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

