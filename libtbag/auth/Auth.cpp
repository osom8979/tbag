/**
 * @file   Auth.cpp
 * @brief  Auth package implementation.
 * @author zer0
 * @date   2019-12-24
 * @date   2019-12-25 (Rename: Handshake -> Auth)
 */

#include <libtbag/auth/Auth.hpp>

#include <libtbag/dom/json/JsonUtils.hpp>
#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

#include <algorithm>
#include <sstream>

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

/**
 * Algorithm:
 * # data -> split_tokens
 * # for_each(tokens)
 *   ## encrypt
 *   ## zip
 *   ## encode_base64
 * # json_array
 */
template <typename KeyT>
static std::string __encrypt(KeyT const & key, std::string const & data)
{
    assert(key.exists());
    assert(!data.empty());
    auto const max_data_size = key.getMaxDataSize(DEFAULT_PADDING);
    auto const tokens = libtbag::string::splitSize(data, max_data_size);
    Json::Value json(Json::arrayValue);
    for (auto const & token : tokens) {
        auto const encrypted_token = key.encrypt(token);
        json.append(__zip_base64(encrypted_token));
    }
    return libtbag::dom::json::writeFast(json);
}

/**
 * Algorithm:
 * # json_array -> merge_tokens
 * # for_each(tokens)
 *   ## decode_base64
 *   ## unzip
 *   ## decrypt
 * # data
 */
template <typename KeyT>
static std::string __decrypt(KeyT const & key, std::string const & encrypted_data)
{
    assert(key.exists());
    assert(!encrypted_data.empty());
    Json::Value json;
    if (!libtbag::dom::json::parse(encrypted_data, json)) {
        return {};
    }
    if (!json.isArray()) {
        return {};
    }
    std::stringstream ss;
    auto const size = json.size();
    for (auto i = 0; i < size; ++i) {
        if (!json[i].isString()) {
            return {};
        }
        ss << key.decrypt(__unzip_base64(json[i].asString()), DEFAULT_PADDING);
    }
    return ss.str();
}

// clang-format off

/** Encode auth0/Client side/Handshake #1/Request data/CA Public Key/Encrypt */
std::string __encodeAuth0(PublicKey const & ca_public_key, std::string const & client_public_key)
{ return __encrypt(ca_public_key, client_public_key); }
/** Decode auth0/Server side/Handshake #1/Request data/CA Private Key/Decrypt */
std::string __decodeAuth0(PrivateKey const & ca_private_key, std::string const & encoded_client_public_key)
{ return __decrypt(ca_private_key, encoded_client_public_key); }

/** Encode auth1/Server side/Handshake #1/Replay data/CA Private Key/Encrypt */
std::string __encodeAuth1(PrivateKey const & ca_private_key, std::string const & server_public_key)
{ return __encrypt(ca_private_key, server_public_key); }
/** Decode auth1/Client side/Handshake #1/Replay data/CA Public Key/Decrypt */
std::string __decodeAuth1(PublicKey const & ca_public_key, std::string const & encoded_server_public_key)
{ return __decrypt(ca_public_key, encoded_server_public_key); }

/** Encode auth2/Client side/Handshake #2/Request data/Server Public Key/Encrypt */
std::string __encodeAuth2(PublicKey const & server_public_key, std::string const & client_data)
{ return __encrypt(server_public_key, client_data); }
/** Decode auth2/Server side/Handshake #2/Request data/Server Private Key/Decrypt */
std::string __decodeAuth2(PrivateKey const & server_private_key, std::string const & encoded_client_data)
{ return __decrypt(server_private_key, encoded_client_data); }

/** Encode auth3/Server side/Handshake #2/Replay data/Client Public Key/Encrypt */
std::string __encodeAuth3(PublicKey const & client_public_key, std::string const & server_data)
{ return __encrypt(client_public_key, server_data); }
/** Decode auth3/Client side/Handshake #2/Replay data/Client Private Key/Decrypt */
std::string __decodeAuth3(PrivateKey const & client_private_key, std::string const & encoded_server_data)
{ return __decrypt(client_private_key, encoded_server_data); }

// clang-format on

// ---------------
// AuthClient
// ---------------

AuthClient::AuthClient()
{
    // EMPTY.
}

AuthClient::AuthClient(AuthClient && obj) TBAG_NOEXCEPT
        : _ca_public_key(std::move(obj._ca_public_key)),
          _client_public_key(std::move(obj._client_public_key)),
          _client_private_key(std::move(obj._client_private_key)),
          _client_data(std::move(obj._client_data)),
          _server_public_key(std::move(obj._server_public_key)),
          _server_data(std::move(obj._server_data)),
          _validation(obj._validation)
{
    // EMPTY.
}

AuthClient::~AuthClient()
{
    // EMPTY.
}

AuthClient & AuthClient::operator =(AuthClient && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _ca_public_key = std::move(obj._ca_public_key);
        _client_public_key = std::move(obj._client_public_key);
        _client_private_key = std::move(obj._client_private_key);
        _client_data = std::move(obj._client_data);
        _server_public_key = std::move(obj._server_public_key);
        _server_data = std::move(obj._server_data);
        _validation = obj._validation;
    }
    return *this;
}

void AuthClient::swap(AuthClient & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _ca_public_key.swap(obj._ca_public_key);
        _client_public_key.swap(obj._client_public_key);
        _client_private_key.swap(obj._client_private_key);
        _client_data.swap(obj._client_data);
        _server_public_key.swap(obj._server_public_key);
        _server_data.swap(obj._server_data);
        std::swap(_validation, obj._validation);
    }
}

bool AuthClient::init(std::string const & ca_pem_public_key, std::string const & client_data)
{
    _ca_public_key = PublicKey();
    if (!_ca_public_key.readPem(ca_pem_public_key)) {
        return false;
    }
    auto keys = libtbag::crypto::generateKeys(DEFAULT_CIPHER, DEFAULT_KEY_LENGTH);
    _client_public_key = std::move(keys.first);
    _client_private_key = std::move(keys.second);
    _client_data = client_data;
    _server_public_key = PublicKey();
    _server_data.clear();
    _validation = false;
    return true;
}

std::string AuthClient::getServerData() const
{
    return _server_data;
}

std::string AuthClient::getClientData() const
{
    return _client_data;
}

std::string AuthClient::encodeAuth0() const
{
    return __encodeAuth0(_ca_public_key, _client_public_key.key());
}

bool AuthClient::decodeAuth1(std::string const & encoded_data)
{
    return _server_public_key.readPem(__decodeAuth1(_ca_public_key, encoded_data));
}

std::string AuthClient::encodeAuth2() const
{
    return __encodeAuth2(_server_public_key, _client_data);
}

bool AuthClient::decodeAuth3(std::string const & encoded_data)
{
    _server_data = __decodeAuth3(_client_private_key, encoded_data);
    return !_server_data.empty();
}

void AuthClient::ok() TBAG_NOEXCEPT
{
    _validation = true;
}

void AuthClient::no() TBAG_NOEXCEPT
{
    _validation = false;
}

bool AuthClient::validation() const TBAG_NOEXCEPT
{
    return _validation;
}

std::string AuthClient::encode(std::string const & data) const
{
    if (!_validation) {
        return {};
    }
    return __encrypt(_server_public_key, data);
}

std::string AuthClient::decode(std::string const & data) const
{
    if (!_validation) {
        return {};
    }
    return __decrypt(_client_private_key, data);
}

// ---------------
// AuthServer
// ---------------

AuthServer::AuthServer()
{
    // EMPTY.
}

AuthServer::AuthServer(AuthServer && obj) TBAG_NOEXCEPT
        : _ca_private_key(std::move(obj._ca_private_key)),
          _server_public_key(std::move(obj._server_public_key)),
          _server_private_key(std::move(obj._server_private_key)),
          _server_data(std::move(obj._server_data)),
          _client_public_key(std::move(obj._client_public_key)),
          _client_data(std::move(obj._client_data)),
          _validation(obj._validation)
{
    // EMPTY.
}

AuthServer::~AuthServer()
{
    // EMPTY.
}

AuthServer & AuthServer::operator =(AuthServer && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _ca_private_key = std::move(obj._ca_private_key);
        _server_public_key = std::move(obj._server_public_key);
        _server_private_key = std::move(obj._server_private_key);
        _server_data = std::move(obj._server_data);
        _client_public_key = std::move(obj._client_public_key);
        _client_data = std::move(obj._client_data);
        _validation =  obj._validation;
    }
    return *this;
}

void AuthServer::swap(AuthServer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _ca_private_key.swap(obj._ca_private_key);
        _server_public_key.swap(obj._server_public_key);
        _server_private_key.swap(obj._server_private_key);
        _server_data.swap(obj._server_data);
        _client_public_key.swap(obj._client_public_key);
        _client_data.swap(obj._client_data);
        std::swap(_validation, obj._validation);
    }
}

bool AuthServer::init(std::string const & ca_pem_private_key, std::string const & server_data)
{
    _ca_private_key = PrivateKey();
    if (!_ca_private_key.readPem(ca_pem_private_key)) {
        return false;
    }
    auto keys = libtbag::crypto::generateKeys(DEFAULT_CIPHER, DEFAULT_KEY_LENGTH);
    _server_public_key = std::move(keys.first);
    _server_private_key = std::move(keys.second);
    _server_data = server_data;
    _client_public_key = PublicKey();
    _client_data.clear();
    _validation = false;
    return true;
}

std::string AuthServer::getServerData() const
{
    return _server_data;
}

std::string AuthServer::getClientData() const
{
    return _client_data;
}

bool AuthServer::decodeAuth0(std::string const & encoded_data)
{
    return _client_public_key.readPem(__decodeAuth0(_ca_private_key, encoded_data));
}

std::string AuthServer::encodeAuth1() const
{
    return __encodeAuth1(_ca_private_key, _server_public_key.key());
}

bool AuthServer::decodeAuth2(std::string const & encoded_data)
{
    _client_data = __decodeAuth2(_server_private_key, encoded_data);
    return !_client_data.empty();
}

std::string AuthServer::encodeAuth3() const
{
    return __encodeAuth3(_client_public_key, _server_data);
}

void AuthServer::ok() TBAG_NOEXCEPT
{
    _validation = true;
}

void AuthServer::no() TBAG_NOEXCEPT
{
    _validation = false;
}

bool AuthServer::validation() const TBAG_NOEXCEPT
{
    return _validation;
}

std::string AuthServer::encode(std::string const & data) const
{
    if (!_validation) {
        return {};
    }
    return __encrypt(_client_public_key, data);
}

std::string AuthServer::decode(std::string const & data) const
{
    if (!_validation) {
        return {};
    }
    return __decrypt(_server_private_key, data);
}

} // namespace auth

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

