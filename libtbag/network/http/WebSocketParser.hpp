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
#include <libtbag/network/http/HttpProperty.hpp>

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

TBAG_API std::string upgradeWebsocketKey(std::string const & base64_key);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETPARSER_HPP__

