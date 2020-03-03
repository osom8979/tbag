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
 *
 *   # Send client's public_key -{encode ca-pub-key}-> Received by server. (decode ca-pri-key)
 *   Client->Server [label="ca_public_key/encode/client_public_key"];
 *
 *   # Send server's public_key -{encode ca-pri-key}-> Received by client. (decode ca-pub-key)
 *   Server->Client [label="ca_private_key/encode/server_public_key"];
 *
 *   --- [label="request/replay #2"];
 *
 *   # Send client's data -{encode server-pub-key}-> Received by server. (decode server-pri-key)
 *   Client->Server [label="server_public_key/encode/client_data"];
 *
 *   # Send server's data -{encode client-pub-key}-> Received by client. (decode client-pri-key)
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

/* [NO DOC] Example:
 * <pre>
 *  ------------------------------------------------------------------------------------------------------
 *                  CLIENT SIDE                         |                SERVER SIDE
 *  ------------------------------------------------------------------------------------------------------
 *                                                      |
 *  using namespace libtbag::crypto;                    | using namespace libtbag::crypto;
 *  std::string ca_public_key_pem;                      | std::string ca_private_key_pem;
 *                                                      |
 *  PublicKey ca_public_key;                            | PrivateKey ca_private_key;
 *  ca_public_key.readPem(ca_public_key_pem);           | ca_private_key.readPem(ca_private_key_pem);
 *                                                      |
 *  std::string client_data = "...";                    | std::string server_data = "...";
 *                                                      |
 *  AuthClient client;                                  | AuthServer server;
 *  client.init(ca_public_key.key(), client_data));     | server.init(ca_private_key.key(), server_data));
 *                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *                                                Initalize DONE.
 *  ------------------------------------------------------------------------------------------------------
 *                                                      |
 *  assert(client.isReady();                            | assert(server.isReady();
 *  assert(client.getServerData().empty());             | assert(server.getClientData().empty());
 *                                                      |
 *  auto const encoded_auth0 = client.encodeAuth0();    |
 *  // Send 'encoded_auth0' to the server ...           >
 *                                                      > // Recv 'encoded_auth0' from the client ...
 *                                                      | server.decodeAuth0(encoded_auth0)
 *                                                      |
 *                                                      | auto const encoded_auth1 = server.encodeAuth1();
 *                                                      < // Send 'encoded_auth1' to the client ...
 *  // Recv 'encoded_auth1' from the server ...         <
 *  client.decodeAuth1(encoded_auth1);                  |
 *                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *                                          REQUEST and REPLAY #1 DONE.
 *  ------------------------------------------------------------------------------------------------------
 *                                                      |
 *  auto const encoded_auth2 = client.encodeAuth2();    |
 *  // Send 'encoded_auth2' to the server ...           >
 *                                                      > // Recv 'encoded_auth2' from the client ...
 *                                                      | server.decodeAuth2(encoded_auth2);
 *                                                      |
 *                                                      | auto const encoded_auth3 = server.encodeAuth3();
 *                                                      < // Send 'encoded_auth3' to the client ...
 *  // Recv 'encoded_auth3' from the server ...         <
 *  client.decodeAuth3(encoded_auth3);                  |
 *                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *                                         REQUEST and REPLAY #2 DONE.
 *  ------------------------------------------------------------------------------------------------------
 *                                                      |
 *  auto const server_data = client.getServerData();    | auto const client_data = server.getClientData();
 *                                                      |
 *  // Test 'server_data' validation ...                | // Test 'client_data' validation ...
 *                                                      |
 *  client.ok();                                        | server.ok();
 *                                                      |
 *  assert(client.validation());                        | assert(server.validation());
 *                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *                                                Validation OK.
 *  ------------------------------------------------------------------------------------------------------
 *                                                      |
 *  std::string data1 = "...";                          |
 *  auto const encode_data1 = client.encode(data1);     |
 *  // Send 'encode_data1' to the server ...            >
 *                                                      > // Recv 'encode_data1' from the client ...
 *                                                      | auto const data1 = server.decode(encode_data1);
 *                                                      |
 *                                                      | // Use 'data1' ...
 *                                                      |
 *                                                      | std::string data2 = "...";
 *                                                      | auto const encode_data2 = server.encode(data2);
 *                                                      < // Send 'encode_data2' to the client ...
 *  // Recv 'encode_data2' from the server ...          <
 *  auto const data2 = client.decode(encode_data2);     |
 *                                                      |
 *  // Use 'data2' ...                                  |
 *                                                      |
 *  ------------------------------------------------------------------------------------------------------
 *                                         Sensitive Data Delivery DONE.
 *  ------------------------------------------------------------------------------------------------------
 */

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
    AuthClient(AuthClient && obj) TBAG_NOEXCEPT;
    ~AuthClient();

public:
    AuthClient & operator =(AuthClient && obj) TBAG_NOEXCEPT;

public:
    void swap(AuthClient & obj) TBAG_NOEXCEPT;

    inline friend void swap(AuthClient & lh, AuthClient & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    bool init(std::string const & ca_pem_public_key, std::string const & client_data);
    bool isReady() const;

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
    AuthServer(AuthServer && obj) TBAG_NOEXCEPT;
    ~AuthServer();

public:
    AuthServer & operator =(AuthServer && obj) TBAG_NOEXCEPT;

public:
    void swap(AuthServer & obj) TBAG_NOEXCEPT;

    inline friend void swap(AuthServer & lh, AuthServer & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    bool init(std::string const & ca_pem_private_key, std::string const & server_data);
    bool isReady() const;

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

