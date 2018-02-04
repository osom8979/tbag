/**
 * @file   HttpsClient.cpp
 * @brief  HttpsClient class implementation.
 * @author zer0
 * @date   2018-02-04
 */

#include <libtbag/network/http/HttpsClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpsClient::HttpsClient(Loop & loop, StreamType type)
        : Parent(loop, type), _reader(this), _tls(), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

HttpsClient::~HttpsClient()
{
    // EMPTY.
}

std::string HttpsClient::getKey() const
{
    return _reader.getKey();
}

Err HttpsClient::writeRequest()
{
    return writeRequest(HttpRequest());
}

Err HttpsClient::writeRequest(HttpRequest const & request)
{
    HttpRequest update_request = request;
    update_request.updateDefaultRequest();
    std::string const & REQUEST_STRING = update_request.toRequestString();
    return writeTls(REQUEST_STRING.data(), REQUEST_STRING.size());
}

Err HttpsClient::writeTls(void const * data, std::size_t size)
{
    Err encode_code = Err::E_UNKNOWN;
    auto encode_result = _tls.decode(data, size, &encode_code);
    if (isFailure(encode_code)) {
        return encode_code;
    }
    return write(encode_result.data(), encode_result.size());
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

