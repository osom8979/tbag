/**
 * @file   WsUtils.cpp
 * @brief  WsUtils class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/http/base/HttpCommon.hpp>
#include <libtbag/encrypt/Base64.hpp>
#include <libtbag/encrypt/Sha1.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/id/Uuid.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

std::string generateRandomWebSocketKey()
{
    std::string base64;
    encrypt::encodeBase64(id::Uuid::ver4().toString(), base64);
    return base64;
}

std::string getUpgradeWebSocketKey(std::string const & key)
{
    encrypt::Sha1Hash sha1_key;
    if (encrypt::encryptSha1(key + WEBSOCKET_HANDSHAKE_GUID, sha1_key) == false) {
        return std::string();
    }

    util::Buffer const SHA1_BUFFER(sha1_key.begin(), sha1_key.end());
    std::string base64_key;

    if (encrypt::encodeBase64(SHA1_BUFFER, base64_key) == false) {
        return std::string();
    }

    return base64_key;
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

