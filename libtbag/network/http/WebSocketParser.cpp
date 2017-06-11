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

std::string upgradeWebsocketKey(std::string const & original_key)
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

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

