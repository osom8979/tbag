/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-09-30
 */

#include <libtbag/network/http/base/HttpProperty.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace base    {

// ------------------------
// Miscellaneous utilities.
// ------------------------

std::vector<int> getWsVersions(HttpHeader const & header)
{
    std::vector<int> result;
    auto const VERSIONS = header.getIgnoreCase(HEADER_SEC_WEBSOCKET_VERSION);
    for (auto & ver : string::splitTokens(VERSIONS, DEFAULT_VALUE_DELIMITER)) {
        try {
            result.push_back(std::stoi(string::trim(ver)));
        } catch (...) {
            // SKIP.
        }
    }
    return result;
}

bool testWsVersion(HttpHeader const & header, int test_version)
{
    for (auto & ver : getWsVersions(header)) {
        if (ver == test_version) {
            return true;
        }
    }
    return false;
}

// ---------------------------
// HttpVersion implementation.
// ---------------------------

std::string HttpVersion::toVersionString() const
{
    return string::fformat("HTTP/{}.{}", http_major, http_minor);
}

// ----------------------------------
// HttpCommonProperty implementation.
// ----------------------------------

HttpCommonProperty & HttpCommonProperty::operator =(HttpCommonProperty const & obj)
{
    if (this != &obj) {
        HttpVersion::operator=(obj);
        HttpHeader::operator=(obj);
        body = obj.body;
    }
    return *this;
}

HttpCommonProperty & HttpCommonProperty::operator =(HttpCommonProperty && obj)
{
    swap(obj);
    return *this;
}

void HttpCommonProperty::swap(HttpCommonProperty & obj)
{
    if (this != &obj) {
        HttpVersion::swap(obj);
        HttpHeader::swap(obj);
        body.swap(obj.body);
    }
}

void HttpCommonProperty::setBody(std::string const & b)
{
    body.assign(b.begin(), b.end());
}

void HttpCommonProperty::setBody(char const * begin, std::size_t size)
{
    body.assign(begin, begin + size);
}

void HttpCommonProperty::appendBody(char const * begin, std::size_t size)
{
    body.insert(body.end(), begin, begin + size);
}

void HttpCommonProperty::appendBody(util::Buffer const & buffer)
{
    body.insert(body.end(), buffer.begin(), buffer.end());
}

void HttpCommonProperty::appendBody(std::string const & str)
{
    body.insert(body.end(), str.begin(), str.end());
}

std::string HttpCommonProperty::getBodyString() const
{
    return std::string(body.begin(), body.end());
}

std::string HttpCommonProperty::toBodyString() const
{
    return getBodyString();
}

std::string HttpCommonProperty::toDebugHeaderString() const
{
    std::stringstream ss;
    std::size_t const SIZE = sizeOfHeaders();
    auto itr = begin();
    for (std::size_t i = 0; i < SIZE; ++i, ++itr) {
        ss << "[H] " << itr->first << ": " << itr->second;
        if (i + 1 < SIZE) {
            ss << std::endl;
        }
    }
    return ss.str();
}

// ---------------------------
// HttpRequest implementation.
// ---------------------------

HttpRequest & HttpRequest::operator =(HttpRequest const & obj)
{
    if (this != &obj) {
        HttpCommonProperty::operator=(obj);
        method = obj.method;
        path = obj.path;
    }
    return *this;
}

HttpRequest & HttpRequest::operator =(HttpRequest && obj)
{
    swap(obj);
    return *this;
}

void HttpRequest::swap(HttpRequest & obj)
{
    if (this != &obj) {
        HttpCommonProperty::swap(obj);
        method.swap(obj.method);
        path.swap(obj.path);
    }
}

void HttpRequest::setHttpMethod(HttpMethod m)
{
    method = http::getHttpMethodName(m);
}

HttpMethod HttpRequest::getHttpMethod() const
{
    return http::getHttpMethod(method.c_str());
}

bool HttpRequest::checkWsRequest(int test_version) const
{
    if (getHttpMethod() != HttpMethod::M_GET) {
        return false;
    }
    if (exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return false;
    }
    if (exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return false;
    }
    if (testWsVersion(*this, test_version) == false) {
        return false;
    }
    return true;
}

void HttpRequest::updateDefaultRequest()
{
    insertIfNotExists(HEADER_USER_AGENT, DEFAULT_VALUE_OF_USER_AGENT);
    insertIfNotExists(HEADER_ACCEPT, DEFAULT_VALUE_OF_ACCEPT);
    if (body.empty() == false) {
        insertIfNotExists(HEADER_CONTENT_LENGTH, std::to_string(body.size()));
    }

    // @formatter:off
    if (method.empty()) { method = http::getHttpMethodName(HttpMethod::M_GET); }
    if (path.empty()) { path = "/"; }
    if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    // @formatter:on
}

void HttpRequest::updateDefaultWsRequest(std::string const & ws_key)
{
    insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    insertIfNotExists(HEADER_SEC_WEBSOCKET_VERSION, std::to_string(WEBSOCKET_VERSION_HYBI13));
    insertIfNotExists(HEADER_SEC_WEBSOCKET_KEY, ws_key);

    // @formatter:off
    if (method.empty()) { method = http::getHttpMethodName(HttpMethod::M_GET); }
    if (path.empty()) { path = "/"; }
    if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    // @formatter:on
}

std::string HttpRequest::toRequestLine() const
{
    std::stringstream ss;
    ss << (method.empty() ? HttpMethodGET::getMethod() : method) << SP
       << (path.empty() ? "/" : path) << SP
       << toVersionString();
    return ss.str();
}

std::string HttpRequest::toRequestString() const
{
    std::stringstream ss;
    ss << toRequestLine() << CRLF
       << toMessageHeader() /* Include CRLF */ << CRLF
       << toBodyString();
    return ss.str();
}

std::string HttpRequest::toDebugRequestString() const
{
    std::stringstream ss;
    ss << "[REQ] " << toRequestLine() << std::endl
       << toDebugHeaderString();
    return ss.str();
}

// ----------------------------
// HttpResponse implementation.
// ----------------------------

HttpResponse & HttpResponse::operator =(HttpResponse const & obj)
{
    if (this != &obj) {
        HttpCommonProperty::operator=(obj);
        code = obj.code;
        reason = obj.reason;
    }
    return *this;
}

HttpResponse & HttpResponse::operator =(HttpResponse && obj)
{
    swap(obj);
    return *this;
}

void HttpResponse::swap(HttpResponse & obj)
{
    if (this != &obj) {
        HttpCommonProperty::swap(obj);
        std::swap(code, obj.code);
        reason.swap(obj.reason);
    }
}

void HttpResponse::setHttpStatus(HttpStatus s)
{
    code   = http::getHttpStatusNumber(s);
    reason = http::getHttpStatusReason(s);
}

void HttpResponse::setHttpStatus(std::string const & name)
{
    setHttpStatus(http::getHttpStatus(name));
}

HttpStatus HttpResponse::getHttpStatus() const
{
    return http::getHttpStatus(code);
}

bool HttpResponse::checkWsResponse(std::string const & original_key) const
{
    if (code != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
        return false;
    }
    if (exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return false;
    }
    if (exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return false;
    }
    if (exists(HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
        return false;
    }
    std::string const ACCEPT_KEY = get(HEADER_SEC_WEBSOCKET_ACCEPT);
    if (ACCEPT_KEY != ws::getUpgradeWebSocketKey(original_key)) {
        return false;
    }
    return true;
}

void HttpResponse::updateDefaultResponse()
{
    insertIfNotExists(HEADER_SERVER, DEFAULT_VALUE_OF_SERVER);
    insertIfNotExists(HEADER_CONTENT_TYPE, DEFAULT_VALUE_OF_CONTENT_TYPE);
    if (body.empty() == false) {
        insertIfNotExists(HEADER_CONTENT_LENGTH, std::to_string(body.size()));
    }

    // @formatter:off
    if (code == 0) { setHttpStatus(HttpStatus::SC_OK); }
    if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    // @formatter:on
}

void HttpResponse::updateDefaultWsResponse(std::string const & key)
{
    insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
    insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
    insertIfNotExists(HEADER_SEC_WEBSOCKET_ACCEPT, ws::getUpgradeWebSocketKey(key));
    setHttpStatus(HttpStatus::SC_SWITCHING_PROTOCOLS); // Force update!

    // @formatter:off
    if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    // @formatter:on
}

void HttpResponse::updateDefaultWsResponse(HttpRequest const & request)
{
    updateDefaultWsResponse(request.get(HEADER_SEC_WEBSOCKET_KEY));
}

std::string HttpResponse::toStatusLine() const
{
    std::stringstream ss;
    ss << toVersionString() << SP
       << code << SP
       << (reason.empty() ? getErrName(Err::E_EINIT) : reason);
    return ss.str();
}

std::string HttpResponse::toResponseString() const
{
    std::stringstream ss;
    ss << toStatusLine() << CRLF
       << toMessageHeader() /* Include CRLF */ << CRLF
       << toBodyString();
    return ss.str();
}

std::string HttpResponse::toDebugResponseString() const
{
    std::stringstream ss;
    ss << "[RES] " << toStatusLine() << std::endl
       << toDebugHeaderString();
    return ss.str();
}

// ----------------------------
// HttpProperty implementation.
// ----------------------------

HttpProperty & HttpProperty::operator =(HttpProperty const & obj)
{
    if (this != &obj) {
        HttpCommonProperty::operator=(obj);

        method = obj.method;
        path = obj.path;

        code = obj.code;
        reason = obj.reason;
    }
    return *this;
}

HttpProperty & HttpProperty::operator =(HttpProperty && obj)
{
    swap(obj);
    return *this;
}

void HttpProperty::swap(HttpProperty & obj)
{
    if (this != &obj) {
        HttpCommonProperty::swap(obj);

        method.swap(obj.method);
        path.swap(obj.path);

        std::swap(code, obj.code);
        reason.swap(obj.reason);
    }
}

void HttpProperty::clear()
{
    http_major = 0;
    http_minor = 0;

    clearHeaders();
    body.clear();

    method.clear();
    path.clear();

    code = 0;
    reason.clear();
}

} // namespace base
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

