/**
 * @file   HttpCommon.cpp
 * @brief  HttpCommon class implementation.
 * @author zer0
 * @date   2018-12-11
 */

#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <libtbag/crypto/Base64.hpp>
#include <libtbag/crypto/Sha.hpp>
#include <libtbag/id/Uuid.hpp>

#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <sstream>

#include <http_parser.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

#define _TBAG_XX(num, name, str) \
    static_assert(static_cast<int>(::http_method::HTTP_##name) == static_cast<int>(HttpMethod::M_##name), \
                  "Mismatch HTTP " #str " Method number.");
TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX

char const * getHttpMethodName(HttpMethod method) TBAG_NOEXCEPT
{
    switch (method) {
#define _TBAG_XX(num, name, str) case HttpMethod::M_##name: return #str;
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "M_UNKNOWN";
    }
}

int getHttpMethodNumber(HttpMethod method) TBAG_NOEXCEPT
{
    switch (method) {
#define _TBAG_XX(num, name, str) case HttpMethod::M_##name: return num;
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_HTTP_METHOD;
    }
}

HttpMethod getHttpMethod(std::string const & name)
{
    std::string const UPPER_NAME = string::upper(name);
#define _TBAG_XX(num, name, str) \
    if (UPPER_NAME == getHttpMethodName(HttpMethod::M_##name)) { \
        return HttpMethod::M_##name; \
    }
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    return HttpMethod::M_UNKNOWN;
}

char const * getHttpStatusName(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return #name;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown status code";
    }
}

char const * getHttpStatusReason(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return reason;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown status code";
    }
}

int getHttpStatusNumber(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return code;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return static_cast<int>(HttpStatus::SC_UNKNOWN);
    }
}

HttpStatus getHttpStatus(int status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case static_cast<int>(HttpStatus::SC_##name): return HttpStatus::SC_##name;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return HttpStatus::SC_UNKNOWN;
    }
}

HttpStatus getHttpStatus(std::string const & status)
{
    int number = 0;
    try {
        number = std::stoi(status);
    } catch (...) {
        number = 0;
    }
    return getHttpStatus(number);
}

HttpHeader getRegularization(HttpHeader const & header)
{
    HttpHeader result;
    for (auto & item : header) {
        using namespace libtbag::string;
        result.insert(std::make_pair(lower(trim(item.first)), lower(trim(item.second))));
    }
    return result;
}

std::string getHeaderValue(HttpHeader const & header, std::string const & key)
{
    if (!key.empty()) {
        auto itr = header.find(key);
        if (itr != header.end()) {
            return itr->second;
        }
    }
    return std::string();
}

std::string getIgnoreCase(HttpHeader const & header, std::string const & key)
{
    using namespace libtbag::string;
    auto const COMPARE_KEY = lower(trim(key));
    for (auto & item : header) {
        if (lower(trim(item.first)) == COMPARE_KEY) {
            return item.second;
        }
    }
    return std::string();
}

std::size_t insert(HttpHeader & header, std::string const & key, std::string const & val)
{
    return header.insert(std::make_pair(key, val)) != header.end() ? 1 : 0;
}

std::size_t insert(HttpHeader & header, HttpHeader const & insert_header)
{
    std::size_t count = 0;
    for (auto & item : insert_header) {
        if (header.insert(item) != header.end()) {
            ++count;
        }
    }
    return count;
}

std::size_t insertIfNotExists(HttpHeader & header, std::string const & key, std::string const & val)
{
    if (!exists(header, key)) {
        return header.insert(std::make_pair(key, val)) != header.end() ? 1 : 0;
    }
    return 0;
}

std::size_t insertIfNotExists(HttpHeader & header, HttpHeader const & insert_header)
{
    std::size_t count = 0;
    for (auto & item : header) {
        count += insertIfNotExists(header, item.first, item.second);
    }
    return count;
}

bool exists(HttpHeader const & header, std::string const & key)
{
    return header.find(key) != header.end();
}

bool exists(HttpHeader const & header, std::string const & key, std::string const & val)
{
    auto pair = header.equal_range(key);
    for (; pair.first != pair.second; ++pair.first) {
        if (pair.first->second == val) {
            return true;
        }
    }
    return false;
}

bool existsInSplitValues(HttpHeader const & header,
                         std::string const & key,
                         std::string const & val)
{
    return existsInSplitValues(header, key, val, VALUE_DELIMITER);
}

bool existsInSplitValues(HttpHeader const & header,
                         std::string const & key,
                         std::string const & val,
                         std::string const & delimiter)
{
    auto pair = header.equal_range(key);
    std::vector<std::string> tokens;
    for (; pair.first != pair.second; ++pair.first) {
        using namespace libtbag::string;
        tokens = splitTokens(pair.first->second, delimiter);
        for (auto & item : tokens) {
            if (trim(item) == val) {
                return true;
            }
        }
    }
    return false;
}

std::string toMessageHeader(HttpHeader const & header)
{
    std::stringstream ss;
    for (auto & item : header) {
        ss << item.first << HEADER_DELIMITER << item.second << CRLF;
    }
    return ss.str();
}

std::string toVersionString(int http_major, int http_minor)
{
    return libtbag::string::fformat("HTTP/{}.{}", http_major, http_minor);
}

std::string toVersionString(HttpCommon const & common)
{
    return toVersionString(common.http_major, common.http_minor);
}

std::string toDebugHeaderString(HttpHeader const & header)
{
    std::stringstream ss;
    std::size_t const SIZE = header.size();
    auto itr = header.begin();
    for (std::size_t i = 0; i < SIZE; ++i, ++itr) {
        ss << "[H] " << itr->first << HEADER_DELIMITER << itr->second;
        if (i + 1 < SIZE) {
            ss << std::endl;
        }
    }
    return ss.str();
}

std::string toDebugHeaderString(HttpCommon const & common)
{
    return toDebugHeaderString(common.header);
}

std::vector<int> getWsVersions(HttpHeader const & header)
{
    std::vector<int> result;
    using namespace libtbag::string;
    auto const VERSIONS = getIgnoreCase(header, HEADER_SEC_WEBSOCKET_VERSION);
    for (auto & ver : splitTokens(VERSIONS, VALUE_DELIMITER)) {
        try {
            result.push_back(std::stoi(trim(ver)));
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

std::string generateRandomWebSocketKey()
{
    using namespace libtbag::crypto;
    using namespace libtbag::id;

    std::string base64;
    encodeBase64(Uuid::ver4().toString(), base64);
    return base64;
}

std::string getUpgradeWebSocketKey(std::string const & key)
{
    using namespace libtbag::crypto;
    using namespace libtbag::util;

    Sha1Hash sha1_key;
    if (!encryptSha1(key + WEBSOCKET_HANDSHAKE_GUID, sha1_key)) {
        return std::string();
    }

    Buffer const SHA1_BUFFER(sha1_key.begin(), sha1_key.end());
    std::string base64_key;

    if (!encodeBase64(SHA1_BUFFER, base64_key)) {
        return std::string();
    }

    return base64_key;
}

void clear(HttpCommon & common)
{
    common.http_minor = DEFAULT_HTTP_VERSION_MINOR;
    common.http_major = DEFAULT_HTTP_VERSION_MAJOR;
    common.header.clear();
    common.body.clear();
}

void clear(HttpBaseRequest & request)
{
    request.method.clear();
    request.path.clear();
}

void clear(HttpBaseResponse & response)
{
    response.code = TBAG_UNKNOWN_HTTP_STATUS;
    response.reason.clear();
}

void clear(HttpRequest & request)
{
    clear((HttpCommon&)request);
    clear((HttpBaseRequest&)request);
}

void clear(HttpResponse & response)
{
    clear((HttpCommon&)response);
    clear((HttpBaseResponse&)response);
}

void clear(HttpProperty & property)
{
    clear((HttpCommon&)property);
    clear((HttpBaseRequest&)property);
    clear((HttpBaseResponse&)property);
}

void swap(HttpCommon & lh, HttpCommon & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    std::swap(lh.http_minor, rh.http_minor);
    std::swap(lh.http_major, rh.http_major);
    lh.header.swap(rh.header);
    lh.body.swap(rh.body);
}

void swap(HttpBaseRequest & lh, HttpBaseRequest & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    lh.method.swap(rh.method);
    lh.path.swap(rh.path);
}

void swap(HttpBaseResponse & lh, HttpBaseResponse & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    std::swap(lh.code, rh.code);
    lh.reason.swap(rh.reason);
}

void swap(HttpRequest & lh, HttpRequest & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    swap((HttpCommon&)lh, (HttpCommon&)rh);
    swap((HttpBaseRequest&)lh, (HttpBaseRequest&)rh);
}

void swap(HttpResponse & lh, HttpResponse & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    swap((HttpCommon&)lh, (HttpCommon&)rh);
    swap((HttpBaseResponse&)lh, (HttpBaseResponse&)rh);
}

void swap(HttpProperty & lh, HttpProperty & rh) TBAG_NOEXCEPT
{
    if (&lh == &rh) {
        return;
    }
    swap((HttpCommon&)lh, (HttpCommon&)rh);
    swap((HttpBaseRequest&)lh, (HttpBaseRequest&)rh);
    swap((HttpBaseResponse&)lh, (HttpBaseResponse&)rh);
}

bool checkWsRequest(HttpMethod method, HttpHeader const & header, int test_version)
{
    if (method != HttpMethod::M_GET) {
        return false;
    }
    if (!exists(header, HEADER_CONNECTION, VALUE_UPGRADE)) {
        return false;
    }
    if (!exists(header, HEADER_UPGRADE, VALUE_WEBSOCKET)) {
        return false;
    }
    if (!testWsVersion(header, test_version)) {
        return false;
    }
    return true;
}

bool checkWsRequest(std::string const & method, HttpHeader const & header, int test_version)
{
    return checkWsRequest(getHttpMethod(method), header, test_version);
}

bool checkWsRequest(HttpRequest const & request, int test_version)
{
    return checkWsRequest(request.method, request.header, test_version);
}

bool checkWsRequest(HttpProperty const & property, int test_version)
{
    return checkWsRequest(property.method, property.header, test_version);
}

void updateDefaultRequest(HttpCommon & common, HttpBaseRequest & request)
{
    insertIfNotExists(common.header, HEADER_USER_AGENT, DEFAULT_VALUE_OF_USER_AGENT);
    insertIfNotExists(common.header, HEADER_ACCEPT, DEFAULT_VALUE_OF_ACCEPT);

    if (!common.body.empty()) {
        insertIfNotExists(common.header, HEADER_CONTENT_LENGTH, std::to_string(common.body.size()));
    }

    // @formatter:off
    if (request.method.empty()) { request.method = getHttpMethodName(HttpMethod::M_GET); }
    if (request.path.empty()) { request.path = PATH_DELIMITER; }
    if (common.http_minor == 0) { common.http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    if (common.http_major == 0) { common.http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    // @formatter:on
}

void updateDefaultRequest(HttpRequest & request)
{
    updateDefaultRequest(request, request);
}

void updateDefaultRequest(HttpProperty & property)
{
    updateDefaultRequest(property, property);
}

void updateDefaultWsRequest(HttpCommon & common, HttpBaseRequest & request, std::string const & ws_key)
{
    insertIfNotExists(common.header, HEADER_CONNECTION, VALUE_UPGRADE);
    insertIfNotExists(common.header, HEADER_UPGRADE, VALUE_WEBSOCKET);
    insertIfNotExists(common.header, HEADER_SEC_WEBSOCKET_VERSION, std::to_string(WEBSOCKET_VERSION_HYBI13));
    insertIfNotExists(common.header, HEADER_SEC_WEBSOCKET_KEY, ws_key);

    // @formatter:off
    if (request.method.empty()) { request.method = getHttpMethodName(HttpMethod::M_GET); }
    if (request.path.empty()) { request.path = PATH_DELIMITER; }
    if (common.http_minor == 0) { common.http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    if (common.http_major == 0) { common.http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    // @formatter:on
}

void updateDefaultWsRequest(HttpRequest & request, std::string const & ws_key)
{
    updateDefaultWsRequest(request, request, ws_key);
}

void updateDefaultWsRequest(HttpProperty & property, std::string const & ws_key)
{
    updateDefaultWsRequest(property, property, ws_key);
}

std::string toRequestLine(HttpCommon const & common, HttpBaseRequest const & request)
{
    std::stringstream ss;
    ss << (request.method.empty() ? HttpMethodGET::getMethod() : request.method) << SP
       << (request.path.empty() ? PATH_DELIMITER : request.path) << SP
       << toVersionString(common);
    return ss.str();
}

std::string toRequestLine(HttpRequest const & request)
{
    return toRequestLine(request, request);
}

std::string toRequestLine(HttpProperty const & property)
{
    return toRequestLine(property, property);
}

std::string toRequestString(HttpCommon const & common, HttpBaseRequest const & request)
{
    std::stringstream ss;
    ss << toRequestLine(common, request) << CRLF
       << toMessageHeader(common.header) /* Include CRLF */ << CRLF
       << std::string(common.body.begin(), common.body.end());
    return ss.str();
}

std::string toRequestString(HttpRequest const & request)
{
    return toRequestString(request, request);
}

std::string toRequestString(HttpProperty const & property)
{
    return toRequestString(property, property);
}

std::string toDebugRequestString(HttpCommon const & common, HttpBaseRequest const & request)
{
    std::stringstream ss;
    ss << "[REQ] " << toRequestLine(common, request) << std::endl
       << toDebugHeaderString(common);
    return ss.str();
}

std::string toDebugRequestString(HttpRequest const & request)
{
    return toDebugRequestString(request, request);
}

std::string toDebugRequestString(HttpProperty const & property)
{
    return toDebugRequestString(property, property);
}

void setHttpStatus(HttpBaseResponse & response, HttpStatus s)
{
    response.code   = getHttpStatusNumber(s);
    response.reason = getHttpStatusReason(s);
}

void setHttpStatus(HttpBaseResponse & response, std::string const & name)
{
    setHttpStatus(response, getHttpStatus(name));
}

HttpStatus getHttpStatus(HttpBaseResponse const & response)
{
    return getHttpStatus(response.code);
}

bool checkWsResponse(int code, HttpHeader const & header, std::string const & original_key)
{
    if (code != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
        return false;
    }
    if (exists(header, HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        return false;
    }
    if (exists(header, HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        return false;
    }
    if (exists(header, HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
        return false;
    }

    std::string const ACCEPT_KEY = getHeaderValue(header, HEADER_SEC_WEBSOCKET_ACCEPT);
    if (ACCEPT_KEY != getUpgradeWebSocketKey(original_key)) {
        return false;
    }
    return true;
}

bool checkWsResponse(int code, HttpCommon const & common, std::string const & original_key)
{
    return checkWsResponse(code, common.header, original_key);
}

bool checkWsResponse(HttpResponse const & response, std::string const & original_key)
{
    return checkWsResponse(response.code, response.header, original_key);
}

bool checkWsResponse(HttpProperty const & property, std::string const & original_key)
{
    return checkWsResponse(property.code, property.header, original_key);
}

void updateDefaultResponse(HttpCommon & common, HttpBaseResponse & response)
{
    insertIfNotExists(common.header, HEADER_SERVER, DEFAULT_VALUE_OF_SERVER);
    insertIfNotExists(common.header, HEADER_CONTENT_TYPE, DEFAULT_VALUE_OF_CONTENT_TYPE);
    insertIfNotExists(common.header, HEADER_CONTENT_LENGTH, std::to_string(common.body.size()));

    // @formatter:off
    if (response.code == 0) { setHttpStatus(response, HttpStatus::SC_OK); }
    if (common.http_major == 0) { common.http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    if (common.http_minor == 0) { common.http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    // @formatter:on
}

void updateDefaultResponse(HttpResponse & response)
{
    updateDefaultResponse(response, response);
}

void updateDefaultResponse(HttpProperty & property)
{
    updateDefaultResponse(property, property);
}

void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, std::string const & key)
{
    insertIfNotExists(common.header, HEADER_UPGRADE, VALUE_WEBSOCKET);
    insertIfNotExists(common.header, HEADER_CONNECTION, VALUE_UPGRADE);
    insertIfNotExists(common.header, HEADER_SEC_WEBSOCKET_ACCEPT, getUpgradeWebSocketKey(key));

    setHttpStatus(response, HttpStatus::SC_SWITCHING_PROTOCOLS); // Force update!

    // @formatter:off
    if (common.http_minor == 0) { common.http_minor = DEFAULT_HTTP_VERSION_MINOR; }
    if (common.http_major == 0) { common.http_major = DEFAULT_HTTP_VERSION_MAJOR; }
    // @formatter:on
}

void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, HttpHeader const & request_header)
{
    updateDefaultWsResponse(common, response, getHeaderValue(request_header, HEADER_SEC_WEBSOCKET_KEY));
}

void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, HttpCommon const & request_common)
{
    updateDefaultWsResponse(common, response, request_common.header);
}

void updateDefaultWsResponse(HttpResponse & response, std::string const & key)
{
    updateDefaultWsResponse(response, response, key);
}

void updateDefaultWsResponse(HttpResponse & response, HttpHeader const & request_header)
{
    updateDefaultWsResponse(response, response, request_header);
}

void updateDefaultWsResponse(HttpResponse & response, HttpCommon const & request_common)
{
    updateDefaultWsResponse(response, response, request_common);
}

void updateDefaultWsResponse(HttpProperty & property, std::string const & key)
{
    updateDefaultWsResponse(property, property, key);
}

void updateDefaultWsResponse(HttpProperty & property, HttpHeader const & request_header)
{
    updateDefaultWsResponse(property, property, request_header);
}

void updateDefaultWsResponse(HttpProperty & property, HttpCommon const & request_common)
{
    updateDefaultWsResponse(property, property, request_common);
}

std::string toStatusLine(HttpCommon const & common, HttpBaseResponse const & response)
{
    std::stringstream ss;
    ss << toVersionString(common) << SP
       << response.code << SP
       << (response.reason.empty() ? getErrName(Err::E_EINIT) : response.reason);
    return ss.str();
}

std::string toStatusLine(HttpResponse const & response)
{
    return toStatusLine(response, response);
}

std::string toStatusLine(HttpProperty const & property)
{
    return toStatusLine(property, property);
}

std::string toResponseString(HttpCommon const & common, HttpBaseResponse const & response)
{
    std::stringstream ss;
    ss << toStatusLine(common, response) << CRLF
       << toMessageHeader(common.header) /* Include CRLF */ << CRLF
       << std::string(common.body.begin(), common.body.end());
    return ss.str();
}

std::string toResponseString(HttpResponse const & response)
{
    return toResponseString(response, response);
}

std::string toResponseString(HttpProperty const & property)
{
    return toResponseString(property, property);
}

std::string toDebugResponseString(HttpCommon const & common, HttpBaseResponse const & response)
{
    std::stringstream ss;
    ss << "[RES] " << toStatusLine(common, response) << std::endl
       << toDebugHeaderString(common);
    return ss.str();
}

std::string toDebugResponseString(HttpResponse const & response)
{
    return toDebugResponseString(response, response);
}

std::string toDebugResponseString(HttpProperty const & property)
{
    return toDebugResponseString(property, property);
}

char const * getWsOpCodeName(WsOpCode code) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (code) {
    case WsOpCode::WSOC_CONTINUATION_FRAME          : return "WSOC_CONTINUE";
    case WsOpCode::WSOC_TEXT_FRAME                  : return "WSOC_TEXT";
    case WsOpCode::WSOC_BINARY_FRAME                : return "WSOC_BINARY";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_1: return "WSOC_NCF1";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_2: return "WSOC_NCF2";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_3: return "WSOC_NCF3";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_4: return "WSOC_NCF4";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_5: return "WSOC_NCF5";
    case WsOpCode::WSOC_CONNECTION_CLOSE            : return "WSOC_CLOSE";
    case WsOpCode::WSOC_DENOTES_PING                : return "WSOC_PING";
    case WsOpCode::WSOC_DENOTES_PONG                : return "WSOC_PONG";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_1    : return "WSOC_CF1";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_2    : return "WSOC_CF2";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_3    : return "WSOC_CF3";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_4    : return "WSOC_CF4";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_5    : return "WSOC_CF5";
    default: return "WSOC_UNKNOWN";
    }
    // @formatter:on
}

bool isWsControlFrame(WsOpCode code) TBAG_NOEXCEPT
{
    // @formatter:off
    return (WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_1 <= COMPARE_AND(code) <= WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_5) ||
           (WsOpCode::WSOC_RESERVED_CONTROL_FRAME_1     <= COMPARE_AND(code) <= WsOpCode::WSOC_RESERVED_CONTROL_FRAME_5);
    // @formatter:on
}

static_assert(WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE == 2,
              "The minimum header size for a WebSocket is 2 byte.");

WsPayloadBit getWsPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT
{
    if (payload_length_7bit <= WS_PAYLOAD_7BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_7;
    } else if (payload_length_7bit == WS_PAYLOAD_16BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_16;
    } else if (payload_length_7bit == WS_PAYLOAD_64BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_64;
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return WsPayloadBit::WSPL_BIT_7;
}

TBAG_CONSTEXPR std::size_t const __MAX_UINT16_BYTE_SIZE = libtbag::type::TypeInfo<uint16_t>::maximum();
TBAG_CONSTEXPR std::size_t const __MAX_UINT64_BYTE_SIZE = libtbag::type::TypeInfo<uint64_t>::maximum();

WsPayloadBit getWsPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT
{
    if (payload_length <= WS_PAYLOAD_7BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_7;
    } else if (payload_length <= __MAX_UINT16_BYTE_SIZE) {
        return WsPayloadBit::WSPL_BIT_16;
    }

    assert(__MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= __MAX_UINT64_BYTE_SIZE);
    return WsPayloadBit::WSPL_BIT_64;
}

uint8_t getPayloadDataByteIndex(WsPayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT
{
    return getMaskingKeyByteIndex(payload_bit) + (is_mask ? sizeof(uint32_t) : 0);
}

uint8_t getMaskingKeyByteIndex(WsPayloadBit payload_bit) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (payload_bit) {
    case WsPayloadBit::WSPL_BIT_7 : return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE;
    case WsPayloadBit::WSPL_BIT_16: return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint16_t);
    case WsPayloadBit::WSPL_BIT_64: return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint64_t);
    }
    // @formatter:on

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

uint32_t copyMaskingKeyFromBuffer(char const * data) TBAG_NOEXCEPT
{
    uint32_t network_32byte_size = 0;
    ::memcpy(&network_32byte_size, data, sizeof(uint32_t));
    return network_32byte_size;
}

std::string getPayloadData(uint32_t mask, std::string const & data)
{
    HttpBuffer const INPUT(data.begin(), data.end());
    HttpBuffer const OUTPUT = getPayloadData(mask, INPUT);
    return std::string(OUTPUT.begin(), OUTPUT.end());
}

HttpBuffer getPayloadData(uint32_t mask, HttpBuffer const & data)
{
    return getPayloadData(mask, data.data(), data.size());
}

HttpBuffer getPayloadData(uint32_t mask, char const * data, std::size_t size)
{
    HttpBuffer result(data, data + size);
    updatePayloadData(mask, result.data(), result.size());
    return result;
}

void updatePayloadData(uint32_t mask, char * result, std::size_t size)
{
    static_assert(sizeof(uint32_t) == 4, "Why not?");
    uint8_t const * mask_ptr = reinterpret_cast<uint8_t const *>(&mask);
    for (std::size_t i = 0; i < size; ++i) {
        *(result + i) ^= mask_ptr[i % sizeof(uint32_t)];
    }
}

uint64_t getPayloadLength(char const * total_data)
{
    auto const PAYLOAD_LENGTH_7BIT = static_cast<uint8_t>(total_data[1] & 0x7F /* 0b01111111 */);
    auto const PAYLOAD_BIT = getWsPayloadBit(PAYLOAD_LENGTH_7BIT);
    return getPayloadLength(total_data + WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE, PAYLOAD_LENGTH_7BIT, PAYLOAD_BIT);
}

uint64_t getPayloadLength(char const * data, uint8_t payload_length_7bit, WsPayloadBit payload_bit)
{
    // Update payload length.
    if (payload_bit == WsPayloadBit::WSPL_BIT_7) {
        return payload_length_7bit;

    } else if (payload_bit == WsPayloadBit::WSPL_BIT_16) {
        uint16_t temp = 0;
        ::memcpy(&temp, data, sizeof(uint16_t));
        return bitwise::toHost(temp);

    } else if (payload_bit == WsPayloadBit::WSPL_BIT_64) {
        uint64_t temp = 0;
        ::memcpy(&temp, data, sizeof(uint64_t));
        return bitwise::toHost(temp);
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

std::size_t calculateBufferSize(uint64_t payload_length, bool is_mask)
{
    std::size_t const DEFAULT_SIZE = WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE
                                     + payload_length + (is_mask ? sizeof(uint32_t) : 0);
    switch (getWsPayloadBitWithPayloadLength(payload_length)) {
    case WsPayloadBit::WSPL_BIT_7:  return DEFAULT_SIZE;
    case WsPayloadBit::WSPL_BIT_16: return DEFAULT_SIZE + sizeof(uint16_t);
    case WsPayloadBit::WSPL_BIT_64: return DEFAULT_SIZE + sizeof(uint64_t);
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return DEFAULT_SIZE;
}

uint16_t getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return num;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE;
    }
}

char const * getWsStatusCodeName(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return #name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "WSSC_UNKNOWN";
    }
}

char const * getWsStatusCodeReason(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return str;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown reason";
    }
}

WsStatusCode getWsStatusCode(uint16_t code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case num: return WsStatusCode::WSSC_##name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return WsStatusCode::WSSC_UNKNOWN;
    }
}

// ------------------------
// WsStatus implementation.
// ------------------------

WsStatus::WsStatus() : code(0), reason()
{
    // EMPTY.
}

WsStatus::WsStatus(uint16_t c) : code(c), reason()
{
    // EMPTY.
}

WsStatus::WsStatus(uint16_t c, std::string const & r)
        : code(c), reason(r)
{
    // EMPTY.
}

WsStatus::WsStatus(WsStatusCode s)
        : code(getWsStatusCodeNumber(s)), reason(getWsStatusCodeReason(s))
{
    // EMPTY.
}

WsStatus::WsStatus(char const * buffer, std::size_t size) : code(0), reason()
{
    auto const PARSE_RESULT = parse(buffer, size);
    if (isFailure(PARSE_RESULT)) {
        throw ErrException(PARSE_RESULT);
    }
}

WsStatus::WsStatus(HttpBuffer const & payload) : code(0), reason()
{
    auto const PARSE_RESULT = parse(payload);
    if (isFailure(PARSE_RESULT)) {
        throw ErrException(PARSE_RESULT);
    }
}

WsStatus::~WsStatus()
{
    // EMPTY.
}

void WsStatus::setWsStatusCode(WsStatusCode s)
{
    code = getWsStatusCodeNumber(s);
    reason = getWsStatusCodeReason(s);
}

WsStatusCode WsStatus::getWsStatusCode() const TBAG_NOEXCEPT
{
    return libtbag::http::getWsStatusCode(code);
}

Err WsStatus::parse(char const * buffer, std::size_t size)
{
    Err const CODE_RESULT = getStatusCode(buffer, size, &code);
    if (isFailure(CODE_RESULT)) {
        return CODE_RESULT;
    }
    Err const REASON_RESULT = getReason(buffer, size, &reason);
    if (isFailure(REASON_RESULT)) {
        return REASON_RESULT;
    }
    return Err::E_SUCCESS;
}

Err WsStatus::parse(HttpBuffer const & payload)
{
    return parse(payload.data(), payload.size());
}

Err WsStatus::getStatusCode(char const * payload_begin, std::size_t payload_length, uint16_t * result)
{
    if (payload_length < sizeof(uint16_t)) {
        return Err::E_ILLARGS;
    }

    uint16_t temp = 0;
    memcpy(&temp, &payload_begin[0], sizeof(temp));
    if (result != nullptr) {
        *result = bitwise::toHost(temp);
    }
    return Err::E_SUCCESS;
}

Err WsStatus::getReason(char const * payload_begin, std::size_t payload_length, std::string * result)
{
    if (payload_length <= sizeof(uint16_t)) {
        return Err::E_ILLARGS;
    }

    if (result != nullptr) {
        result->assign(&payload_begin[sizeof(uint16_t)],
                       &payload_begin[sizeof(uint16_t)] + payload_length - sizeof(uint16_t));
    }
    return Err::E_SUCCESS;
}

std::string WsStatus::toString() const
{
    return getWsStatusCodeName(getWsStatusCode());
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

