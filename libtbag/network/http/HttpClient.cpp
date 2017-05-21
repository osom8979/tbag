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

Err requestWithSync(Uri const & uri, HttpBuilder const & request, uint64_t timeout, HttpResponse & result)
{
    using Loop = uvpp::Loop;
    Loop loop;
    TcpHttpClient http(loop);

    std::string host;
    int port = 0;

    Uri::AddrFlags const FLAG = Uri::AddrFlags::MOST_IPV4;
    Err ADDRINFO_RESULT = uri.requestAddrInfo(host, port, FLAG);
    if (ADDRINFO_RESULT != Err::E_SUCCESS) {
        return ADDRINFO_RESULT;
    }

    if (http.init(host, port, timeout) == false) {
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
    }, TcpHttpClient::Millisec(timeout));

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

