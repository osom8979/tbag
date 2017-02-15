/**
 * @file   Chat.hpp
 * @brief  Chat class prototype.
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/TcpClient.hpp>
#include <libtbag/network/TcpServer.hpp>

#include <libtbag/uvpp/Tty.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

/**
 * Chatting server class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API ChatServer : public TcpServer
{
public:
    ChatServer();
    virtual ~ChatServer();

public:
    virtual bool onNewConnection(uerr code, WeakClient client) override;
    virtual void onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size) override;
};

// Forward declaration.
class ChatTty;

/**
 * Chatting client class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API ChatClient : public TcpClient
{
private:
    std::shared_ptr<ChatTty> _input;

public:
    ChatClient();
    virtual ~ChatClient();

public:
    virtual void onClientConnect(ConnectRequest & request, uerr code) override;
    virtual void onClientReadDatagram(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClientClose() override;
};

/**
 * Chatting client of stdin.
 *
 * @author zer0
 * @date   2016-12-31
 */
class TBAG_API ChatTty : public uvpp::Tty
{
public:
    using uerr = uvpp::uerr;
    using binf = uvpp::binf;
    using Loop = uvpp::Loop;
    using Tty  = uvpp::Tty;

    using Buffer = std::vector<char>;

private:
    ChatClient & _client;

private:
    Buffer _read_buffer;
    Buffer _last_buffer;
    std::size_t _last_index;

public:
    ChatTty(uvpp::Loop & loop, ChatClient & client);
    virtual ~ChatTty();

public:
    virtual uvpp::binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;

public:
    void onReadLine(std::string const & msg);
};

char const * const TEST_CHAT_SERVER_ADDRESS =   "0.0.0.0";
char const * const TEST_CHAT_CLIENT_ADDRESS = "127.0.0.1";
int          const TEST_CHAT_PORT           =       19998;

TBAG_API int runChatServer(std::string const & ip = TEST_CHAT_SERVER_ADDRESS, int port = TEST_CHAT_PORT);
TBAG_API int runChatClient(std::string const & ip = TEST_CHAT_CLIENT_ADDRESS, int port = TEST_CHAT_PORT);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__

