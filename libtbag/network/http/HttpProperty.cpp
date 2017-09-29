/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <http_parser.h>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ----------------------------------
// HttpCommonProperty implementation.
// ----------------------------------

void HttpCommonProperty::setBody(std::string const & val)
{
    body.assign(val.begin(), val.end());
}

std::string HttpCommonProperty::getBody() const
{
    return std::string(body.begin(), body.end());
}

std::string HttpCommonProperty::getHeader(std::string const & key) const
{
    auto itr = headers.find(key);
    if (itr != headers.end()) {
        return itr->second;
    }
    return std::string();
}

bool HttpCommonProperty::existsHeaderValue(std::string const & key, std::string const & value, bool ignore_case) const
{
    return existsHeaderValueFromHeaderMap(headers, key, value, ignore_case);
}

void HttpCommonProperty::insertIfNotExists(std::string const & key, std::string const & val)
{
    return insertIfNotExists(headers, key, val);
}

bool HttpCommonProperty::existsHeaderValueFromHeaderMap(
        HttpHeaderMap const & headers,
        std::string const & key,
        std::string const & value,
        bool ignore_case)
{
    auto itr = headers.find(key);
    if (itr != headers.end()) {
        std::string const VALUE = (ignore_case ? string::lower(value) : value);
        for (auto & cursor : string::splitTokens(itr->second, VALUE_DELIMITER)) {
            if (VALUE == string::lower(string::trim(cursor))) {
                return true;
            }
        }
    }
    return false;
}

void HttpCommonProperty::insertIfNotExists(HttpHeaderMap & headers, std::string const & key, std::string const & val)
{
    if (headers.find(key) == headers.end()) {
        headers.insert(HttpHeaderPair(key, val));
    }
}

// -----------------------------------
// HttpRequestProperty implementation.
// -----------------------------------

Uri HttpRequestProperty::getUri() const
{
    return Uri(url);
}

void HttpRequestProperty::setMethod(HttpMethod m)
{
    method = getHttpMethodName(m);
}

// ------------------------------------
// HttpResponseProperty implementation.
// ------------------------------------

void HttpResponseProperty::setStatus(HttpStatus s)
{
    status = getHttpStatusNumber(s);
    reason = getHttpStatusReason(s);
}

void HttpResponseProperty::setStatus(std::string const & str)
{
    try {
        status = std::stoi(str);
    } catch (...) {
        status = 0;
    }
}

// ------------------
// Debugging methods.
// ------------------

std::string toDebugString(HttpHeaderMap const & obj)
{
    std::stringstream ss;
    std::size_t const SIZE = obj.size();
    auto itr = obj.begin();
    for (std::size_t i = 0; i < SIZE; ++i, ++itr) {
        ss << "[H] " << itr->first << ": " << itr->second;
        if (i + 1 < SIZE) {
            ss << std::endl;
        }
    }
    return ss.str();
}

std::string toDebugString(HttpCommonProperty const & obj)
{
    std::stringstream ss;
    ss << toDebugString(obj.headers) << std::endl;
    ss << "[BODY] " << obj.getBody();
    return ss.str();
}

std::string toDebugString(HttpCommon const & obj)
{
    std::stringstream ss;
    ss << "HTTP/" << obj.http_minor << "." << obj.http_minor << std::endl;
    ss << toDebugString(static_cast<HttpCommonProperty const &>(obj));
    return ss.str();
}

std::string toDebugString(HttpRequest const & obj)
{
    std::stringstream ss;
    ss << "REQUEST: " << obj.method << " " << obj.url << " ";
    ss << toDebugString(static_cast<HttpCommon const &>(obj));
    return ss.str();
}

std::string toDebugString(HttpResponse const & obj)
{
    std::stringstream ss;
    ss << "RESPONSE: " << obj.status << " " << obj.reason << " ";
    ss << toDebugString(static_cast<HttpCommon const &>(obj));
    return ss.str();
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

std::string convertRegularString(std::string const & source)
{
    return string::lower(string::trim(source));
}

std::set<std::string> convertRegularSet(std::vector<std::string> const & sources)
{
    std::set<std::string> result;
    for (auto & cursor : sources) {
        result.insert(convertRegularString(cursor));
    }
    return result;
}

std::vector<std::string> findAccept(std::vector<std::string> const & sources,
                                    std::vector<std::string> const & accepts)
{
    std::vector<std::string> result;
    auto regulars = convertRegularSet(accepts);
    for (auto & proto : sources) {
        if (regulars.find(string::lower(string::trim(proto))) != regulars.end()) {
            result.push_back(proto);
        }
    }
    return result;
}

std::vector<std::string> findAccept(std::string const & sources,
                                    std::vector<std::string> const & accepts)
{
    return findAccept(fromDelimiterString(sources), accepts);
}

std::vector<std::string> fromDelimiterString(std::string const & sources, std::string const & delimiter)
{
    return string::splitTokens(sources, delimiter);
}

std::string toDelimiterString(std::vector<std::string> const & sources)
{
    std::stringstream ss;
    for (auto & cursor : sources) {
        ss << cursor << VALUE_DELIMITER;
    }
    return ss.str();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

