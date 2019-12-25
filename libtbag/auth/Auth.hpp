/**
 * @file   Auth.hpp
 * @brief  Auth class prototype.
 * @author zer0
 * @date   2019-12-24
 * @date   2019-12-25 (Rename: Handshake -> Auth)
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

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_AUTH_AUTH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_AUTH_AUTH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/crypto/Rsa.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace auth {

using PublicKey = libtbag::crypto::PublicKey;
using PrivateKey = libtbag::crypto::PrivateKey;
using Padding = libtbag::crypto::Rsa::Padding;
using CipherAlgorithm = libtbag::crypto::Rsa::CipherAlgorithm;

TBAG_CONSTEXPR Padding const DEFAULT_PADDING = Padding::P_PKCS1;
TBAG_CONSTEXPR CipherAlgorithm const DEFAULT_CIPHER = CipherAlgorithm::CA_NONE;
TBAG_CONSTEXPR int const DEFAULT_KEY_LENGTH = 2048;

/**
 * AuthClient class prototype.
 *
 * @author zer0
 * @date   2019-12-24
 * @date   2019-12-25 (Rename: HandshakeClient -> AuthClient)
 */
class TBAG_API AuthClient : private Noncopyable
{
private:
    PublicKey _ca_public_key;

private:
    PublicKey _client_public_key;
    PrivateKey _client_private_key;
    std::string _client_data;

private:
    PublicKey _server_public_key;
    std::string _server_data;

private:
    bool _validation = false;

public:
    AuthClient();
    ~AuthClient();

public:
    bool init(std::string const & ca_pem_public_key, std::string const & client_data);

public:
    std::string getServerData() const;
    std::string getClientData() const;

public:
    std::string encodeAuth0() const;
    bool decodeAuth1(std::string const & encoded_data);

    std::string encodeAuth2() const;
    bool decodeAuth3(std::string const & encoded_data);

public:
    void ok() TBAG_NOEXCEPT;
    void no() TBAG_NOEXCEPT;

public:
    bool validation() const TBAG_NOEXCEPT;

public:
    std::string encode(std::string const & data) const;
    std::string decode(std::string const & data) const;
};

/**
 * AuthServer class prototype.
 *
 * @author zer0
 * @date   2019-12-24
 * @date   2019-12-25 (Rename: HandshakeServer -> AuthServer)
 */
class TBAG_API AuthServer : private Noncopyable
{
private:
    PrivateKey _ca_private_key;

private:
    PublicKey _server_public_key;
    PrivateKey _server_private_key;
    std::string _server_data;

private:
    PublicKey _client_public_key;
    std::string _client_data;

private:
    bool _validation = false;

public:
    AuthServer();
    ~AuthServer();

public:
    bool init(std::string const & ca_pem_private_key, std::string const & server_data);

public:
    std::string getServerData() const;
    std::string getClientData() const;

public:
    bool decodeAuth0(std::string const & encoded_data);
    std::string encodeAuth1() const;

    bool decodeAuth2(std::string const & encoded_data);
    std::string encodeAuth3() const;

public:
    void ok() TBAG_NOEXCEPT;
    void no() TBAG_NOEXCEPT;

public:
    bool validation() const TBAG_NOEXCEPT;

public:
    std::string encode(std::string const & data) const;
    std::string decode(std::string const & data) const;
};

} // namespace auth

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_AUTH_AUTH_HPP__

