/**
 * @file   HttpCacheData.cpp
 * @brief  HttpCacheData class implementation.
 * @author zer0
 * @date   2017-07-02
 */

#include <libtbag/network/http/HttpCacheData.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpCacheData::HttpCacheData(ClientInterface * client)
        : _client(client), _upgrade(false),
          _send_frame(), _recv_frame(), _buffer(), _key(), _protocols(),
          _builder(), _parser()
{
    // EMPTY.
}

HttpCacheData::~HttpCacheData()
{
    // EMPTY.
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

