/**
 * @file   Handshake.hpp
 * @brief  Handshake class prototype.
 * @author zer0
 * @date   2019-12-24
 *
 * Handshake sequence:
 * @msc
 *   Client,Server;
 *   --- [label="request/replay #1"];
 *   Client->Server [label="ca_public_key/encode/client_public_key"];
 *   Server->Client [label="ca_private_key/encode/server_public_key"];
 *
 *   --- [label="request/replay #2"];
 *   Client->Server [label="server_public_key/encode/client_data"];
 *   Server->Client [label="client_public_key/encode/server_data"];
 * @endmsc
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_AUTH_HANDSHAKE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_AUTH_HANDSHAKE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/crypto/Rsa.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace auth {

struct AuthData
{
    AuthData() { /* EMPTY. */ }
    virtual ~AuthData() { /* EMPTY. */ }

    virtual std::string toString() const = 0;
    virtual bool fromString(std::string const & data) = 0;
};

struct StringAuthData : public AuthData
{
    std::string auth_data;

    StringAuthData() { /* EMPTY. */ }
    StringAuthData(std::string const & data) : auth_data(data) { /* EMPTY. */ }
    virtual ~StringAuthData() { /* EMPTY. */ }

    std::string toString() const override
    { return auth_data; }

    bool fromString(std::string const & data) override
    { auth_data = data; return true; }
};

using SharedAuthData = std::shared_ptr<AuthData>;
using PublicKey = libtbag::crypto::PublicKey;
using PrivateKey = libtbag::crypto::PrivateKey;
using Padding = libtbag::crypto::Rsa::Padding;
using CipherAlgorithm = libtbag::crypto::Rsa::CipherAlgorithm;

TBAG_CONSTEXPR Padding const DEFAULT_PADDING = Padding::P_PKCS1;
TBAG_CONSTEXPR CipherAlgorithm const DEFAULT_CIPHER = CipherAlgorithm::CA_NONE;
TBAG_CONSTEXPR int const DEFAULT_KEY_LENGTH = 2048;

/**
 * HandshakeClient class prototype.
 *
 * @author zer0
 * @date   2019-12-24
 */
class TBAG_API HandshakeClient : private Noncopyable
{
private:
    PublicKey _ca_public_key;

private:
    PublicKey _client_public_key;
    PrivateKey _client_private_key;
    SharedAuthData _client_data;

private:
    PublicKey _server_public_key;
    SharedAuthData _server_data;

public:
    HandshakeClient();
    ~HandshakeClient();

public:
    bool init(std::string const & ca_pem_public_key, SharedAuthData const & client_data);
    bool init(std::string const & ca_pem_public_key, std::string const & client_data);

public:
    /**
     * Handshake #1 request. (Client->Server)
     *
     * - Algorithm: ca_public_key/encode
     * - Send data: client_public_key
     */
    std::string getHandshake1RequestData() const;

    /**
     * Handshake #1 response. (Server->Client)
     *
     * - Algorithm: ca_public_key/decode
     * - Recv data: server_public_key
     */
    bool setHandshake1ResponseData(std::string const & data);

public:
    /**
     * Handshake #2 request. (Client->Server)
     *
     * - Algorithm: server_public_key/encode
     * - Send data: client_data
     */
    std::string getHandshake2RequestData() const;

    /**
     * Handshake #2 response. (Server->Client)
     *
     * - Algorithm: client_private_key/decode
     * - Recv data: server_data
     */
    bool setHandshake2ResponseData(std::string const & data);
};

/**
 * HandshakeServer class prototype.
 *
 * @author zer0
 * @date   2019-12-24
 */
class TBAG_API HandshakeServer : private Noncopyable
{
private:
    PrivateKey _ca_private_key;

private:
    PublicKey _server_public_key;
    PrivateKey _server_private_key;
    SharedAuthData _server_data;

private:
    PublicKey _client_public_key;
    SharedAuthData _client_data;

public:
    HandshakeServer();
    ~HandshakeServer();

public:
    bool init(std::string const & ca_pem_private_key, SharedAuthData const & server_data);
    bool init(std::string const & ca_pem_private_key, std::string const & server_data);

public:
    /**
     * Handshake #1 response. (Client->Server)
     *
     * - Algorithm: ca_private_key/decode
     * - Recv data: client_public_key
     */
    bool setHandshake1ResponseData(std::string const & data);

    /**
     * Handshake #1 replay. (Server->Client)
     *
     * - Algorithm: ca_private_key/encode
     * - Send data: server_public_key
     */
    std::string getHandshake1ReplayData() const;

public:
    /**
     * Handshake #2 response. (Client->Server)
     *
     * - Algorithm: server_private_key/decode
     * - Recv data: client_data
     */
    bool setHandshake2ResponseData(std::string const & data);

    /**
     * Handshake #2 replay. (Server->Client)
     *
     * - Algorithm: client_public_key/encode
     * - Send data: server_data
     */
    std::string getHandshake2ReplayData() const;
};

} // namespace auth

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_AUTH_HANDSHAKE_HPP__

