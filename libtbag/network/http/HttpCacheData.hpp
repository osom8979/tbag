/**
 * @file   HttpCacheData.hpp
 * @brief  HttpCacheData class prototype.
 * @author zer0
 * @date   2017-07-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpCacheData class prototype.
 *
 * @author zer0
 * @date   2017-07-02
 */
class TBAG_API HttpCacheData : private Noncopyable
{
public:
    using Frame   = WebSocketFrame;
    using Buffer  = std::vector<uint8_t>;
    using Strings = std::vector<std::string>;

    using ClientInterface = details::ClientInterface;

private:
    ClientInterface * _client;

public:
    HttpBuilder builder;
    HttpParser  parser;

public:
    bool upgrade;

    Frame send_frame;
    Frame recv_frame;

    Buffer buffer;

    std::string key;
    Strings protocols;

public:
    HttpCacheData(ClientInterface * client = nullptr);
    virtual ~HttpCacheData();
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__

