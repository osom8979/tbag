/**
 * @file   SimpleHttpClient.cpp
 * @brief  SimpleHttpClient class implementation.
 * @author zer0
 * @date   2017-09-29
 */

#include <libtbag/network/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/filesystem/details/FsTypes.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ---------------
namespace __impl {
// ---------------

template <typename ClientType>
Err requestWithSync(Uri const & uri,
                    int port,
                    libtbag::http::HttpRequest const & request,
                    libtbag::http::HttpResponse & response,
                    uint64_t timeout,
                    typename ClientType::StreamType type)
{
    std::string host;

    if (type == ClientType::StreamType::PIPE) {
        host = uri.toString();
        port = 0;
    } else {
        Err const ADDR_CODE = uri.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4);
        if (ADDR_CODE != Err::E_SUCCESS) {
            return ADDR_CODE;
        }
    }

    using Simple = SimpleClient<ClientType>;

    uvpp::Loop loop;
    std::shared_ptr<Simple> http;
    try {
        http.reset(new Simple(loop, type));
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

    libtbag::http::HttpRequest builder = request;
    if (builder.method.empty()) {
        builder.method = libtbag::http::getHttpMethodName(libtbag::http::HttpMethod::M_GET);
    }
    if (builder.path.empty()) {
        builder.path.assign(uri.getRequestPath());
    }
    if (!libtbag::http::exists(builder.header, libtbag::http::HEADER_HOST)) {
        libtbag::http::insert(builder.header, libtbag::http::HEADER_HOST, uri.getHost());
    }
    libtbag::http::updateDefaultRequest(builder);

    tDLogI("requestWithSync() Request {}: {}", builder.method, uri.toString());
    Err code = Err::E_UNKNOWN;
    http->setRequest(builder);
    http->setOnResponse([&](libtbag::http::HttpResponse const & r){
        tDLogD("requestWithSync()::OnResponse() HTTP_CODE: {}", r.code);
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

// ------------------
} // namespace __impl
// ------------------

Err requestWithSync(Uri const & uri,
                    libtbag::http::HttpRequest const & request,
                    libtbag::http::HttpResponse & response,
                    uint64_t timeout,
                    HttpClient::StreamType type)
{
    return __impl::requestWithSync<HttpClient>(uri, libtbag::http::DEFAULT_HTTP_PORT, request, response, timeout, type);
}

Err requestWithSync(std::string const & uri,
                    libtbag::http::HttpRequest const & request,
                    libtbag::http::HttpResponse & response,
                    uint64_t timeout)
{
    return requestWithSync(Uri(uri), request, response, timeout);
}

Err requestWithSync(std::string const & uri,
                    libtbag::http::HttpResponse & response,
                    uint64_t timeout)
{
    return requestWithSync(Uri(uri), libtbag::http::HttpProperty(), response, timeout);
}

Err requestWithTlsSync(Uri const & uri,
                       libtbag::http::HttpRequest const & request,
                       libtbag::http::HttpResponse & response,
                       uint64_t timeout,
                       HttpsClient::StreamType type)
{
    return __impl::requestWithSync<HttpsClient>(uri, libtbag::http::DEFAULT_HTTPS_PORT, request, response, timeout, type);
}

Err requestWithTlsSync(std::string const & uri,
                       libtbag::http::HttpRequest const & request,
                       libtbag::http::HttpResponse & response,
                       uint64_t timeout)
{
    return requestWithTlsSync(Uri(uri), request, response, timeout);
}

Err requestWithTlsSync(std::string const & uri,
                       libtbag::http::HttpResponse & response,
                       uint64_t timeout)
{
    return requestWithTlsSync(Uri(uri), libtbag::http::HttpProperty(), response, timeout);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

