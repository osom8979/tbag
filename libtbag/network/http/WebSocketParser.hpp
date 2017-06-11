/**
 * @file   WebSocketParser.hpp
 * @brief  WebSocketParser class prototype.
 * @author zer0
 * @date   2017-06-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>

#include <vector>
#include <set>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * WebSocketParser class prototype.
 *
 * @author zer0
 * @date   2017-06-11
 */
class TBAG_API WebSocketParser
{
public:

public:
    WebSocketParser();
    virtual ~WebSocketParser();
};

// ----------
// Utilities.
// ----------

TBAG_API std::string upgradeWebSocketKey(std::string const & base64_key);

TBAG_API bool existsWebSocketVersion13(std::string const & versions);
TBAG_API std::string getWebSocketProtocol(std::string const & protocols, std::set<std::string> const & accept_protocols);
TBAG_API std::string getWebSocketProtocolWithTbag(std::string const & protocols);

TBAG_API Err getResponseWebSocket(HttpParser const & request, HttpBuilder & response);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETPARSER_HPP__

