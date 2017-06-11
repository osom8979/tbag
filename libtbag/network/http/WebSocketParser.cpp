/**
 * @file   WebSocketParser.cpp
 * @brief  WebSocketParser class implementation.
 * @author zer0
 * @date   2017-06-11
 */

#include <libtbag/network/http/WebSocketParser.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/encrypt/Base64.hpp>
#include <libtbag/encrypt/Sha1.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketParser::WebSocketParser()
{
    // EMPTY.
}

WebSocketParser::~WebSocketParser()
{
    // EMPTY.
}

// ----------
// Utilities.
// ----------

std::string upgradeWebSocketKey(std::string const & original_key)
{
    encrypt::Sha1Hash sha1_key;
    if (encrypt::encryptSha1(original_key + WEBSOCKET_HANDSHAKE_GUID, sha1_key) == false) {
        return std::string();
    }

    std::vector<uint8_t> const SHA1_BUFFER(sha1_key.begin(), sha1_key.end());
    std::string base64_key;

    if (encrypt::encodeBase64WithBinary(SHA1_BUFFER, base64_key) == false) {
        return std::string();
    }

    return base64_key;
}

bool existsWebSocketVersion13(std::string const & versions)
{
    for (auto & ver : string::splitTokens(versions, VALUE_DELIMITER)) {
        try {
            if (std::stoi(string::trim(ver)) == WEBSOCKET_VERSION_HYBI13) {
                return true;
            }
        } catch (...) {
            // EMPTY.
        }
    }
    return false;
}

std::string getWebSocketProtocol(std::string const & protocols, std::set<std::string> const & accept_protocols)
{
    std::stringstream ss;
    for (auto & proto : string::splitTokens(protocols, VALUE_DELIMITER)) {
        if (accept_protocols.find(string::trim(proto)) != accept_protocols.end()) {
            ss << string::trim(proto) << VALUE_DELIMITER;
        }
    }
    return ss.str();
}

std::string getWebSocketProtocolWithTbag(std::string const & protocols)
{
    return getWebSocketProtocol(protocols, {VALUE_TBAG_PROTOCOL});
}

Err getResponseWebSocket(HttpParser const & request, HttpBuilder & response)
{
    if (string::lower(request.getHeader(HEADER_CONNECTION)) != string::lower(VALUE_UPGRADE)) {
        return Err::E_ILLARGS;
    }
    if (string::lower(request.getHeader(HEADER_UPGRADE)) != string::lower(VALUE_WEBSOCKET)) {
        return Err::E_ILLARGS;
    }
    if (existsWebSocketVersion13(request.getHeader(HEADER_SEC_WEBSOCKET_VERSION)) == false) {
        return Err::E_VERSION_MISMATCH;
    }

    std::string const UPGRADE_KEY = upgradeWebSocketKey(request.getHeader(HEADER_SEC_WEBSOCKET_KEY));
    if (UPGRADE_KEY.empty()) {
        return Err::E_ILLARGS;
    }

    response.setVersion(1, 1);
    response.setStatus(HttpStatus::SC_SWITCHING_PROTOCOLS);
    response.insertHeader(HEADER_UPGRADE, VALUE_WEBSOCKET);
    response.insertHeader(HEADER_CONNECTION, VALUE_UPGRADE);
    response.insertHeader(HEADER_SEC_WEBSOCKET_ACCEPT, UPGRADE_KEY);

    return Err::E_SUCCESS;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

