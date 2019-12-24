/**
 * @file   Handshake.cpp
 * @brief  Handshake class implementation.
 * @author zer0
 * @date   2019-12-24
 */

#include <libtbag/auth/Handshake.hpp>

#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/id/Uuid.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace auth {

static std::string __zip_base64(std::string const & data)
{
    assert(!data.empty());
    auto const buffer = libtbag::util::Buffer(data.begin(), data.end());
    std::string result;
    auto const code = libtbag::archive::ex::encodeZipBase64(buffer, result);
    assert(isSuccess(code));
    return result;
}

static std::string __unzip_base64(std::string const & data)
{
    assert(!data.empty());
    libtbag::util::Buffer result;
    auto const code = libtbag::archive::ex::decodeZipBase64(data, result);
    assert(isSuccess(code));
    return std::string(result.begin(), result.end());
}

// ---------------
// HandshakeClient
// ---------------

HandshakeClient::HandshakeClient()
{
    // EMPTY.
}

HandshakeClient::~HandshakeClient()
{
    // EMPTY.
}

bool HandshakeClient::init(std::string const & ca_pem_public_key, SharedAuthData const & client_data)
{
    if (!_ca_public_key.readPem(ca_pem_public_key)) {
        return false;
    }
    auto keys = libtbag::crypto::generateKeys(DEFAULT_CIPHER, DEFAULT_KEY_LENGTH);
    _client_public_key = std::move(keys.first);
    _client_private_key = std::move(keys.second);
    _client_data = client_data;
    return true;
}

bool HandshakeClient::init(std::string const & ca_pem_public_key, std::string const & client_data)
{
    return init(ca_pem_public_key, std::make_shared<StringAuthData>(client_data));
}

// ---------------
// HandshakeServer
// ---------------

HandshakeServer::HandshakeServer()
{
    // EMPTY.
}

HandshakeServer::~HandshakeServer()
{
    // EMPTY.
}

bool HandshakeServer::init(std::string const & ca_pem_private_key, SharedAuthData const & server_data)
{
    if (!_ca_private_key.readPem(ca_pem_private_key)) {
        return false;
    }
    auto keys = libtbag::crypto::generateKeys(DEFAULT_CIPHER, DEFAULT_KEY_LENGTH);
    _server_public_key = std::move(keys.first);
    _server_private_key = std::move(keys.second);
    _server_data = server_data;
    return true;
}

bool HandshakeServer::init(std::string const & ca_pem_private_key, std::string const & server_data)
{
    return init(ca_pem_private_key, std::make_shared<StringAuthData>(server_data));
}

} // namespace auth

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

