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
        : _client(client), upgrade(false),
          send_frame(), recv_frame(), buffer(),
          key(), protocols(), builder(), parser()
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

