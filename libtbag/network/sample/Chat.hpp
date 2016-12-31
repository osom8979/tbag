/**
 * @file   Chat.hpp
 * @brief  Chat class prototype.
 * @author zer0
 * @date   2016-12-23
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
#include <libtbag/uv/Tty.hpp>
#include <libtbag/uv/Loop.hpp>

#include <map>
#include <memory>
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
 */
class TBAG_API ChatServer : public TcpServer
{
public:
    struct UserInfo
    {
        int id;
        std::string ip;
        std::string name;
    };

private:
    int _id_count;

public:
    ChatServer();
    virtual ~ChatServer();

public:
    virtual void onConnection(Err code) override;
    virtual void onClose() override;

public:
    virtual void onClientRead(Client & client, Err code, char const * buffer, std::size_t size) override;
    virtual void onClientWrite(Client & client, WriteRequest & request, Err code) override;
    virtual void onClientClose(Client & client) override;
};

// Forward declaration.
class AsyncChatInput;

/**
 * Chatting client class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 */
class TBAG_API ChatClient : public TcpClient
{
public:
    using SharedAsyncChatInput = std::shared_ptr<AsyncChatInput>;

private:
    SharedAsyncChatInput _input;

public:
    ChatClient(std::string const & name);
    virtual ~ChatClient();

public:
    virtual void onConnect(ConnectRequest & request, Err code) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onWrite(WriteRequest & request, Err code) override;
    virtual void onClose() override;
};

/**
 * Chatting client of stdin.
 *
 * @author zer0
 * @date   2016-12-31
 */
class TBAG_API AsyncChatInput : public uv::Tty
{
public:
    using Buffer = std::vector<char>;

private:
    ChatClient & _client;

private:
    Buffer      _read_buffer;
    Buffer      _last_buffer;
    std::size_t _last_index;
    std::string _name;

public:
    AsyncChatInput(uv::Loop & loop, ChatClient & client, std::string const & name);
    ~AsyncChatInput();

public:
    virtual uv::binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;

public:
    virtual void onReadLine(std::string const & msg);
};

TBAG_API int runChatServer(std::string const & ip, int port);
TBAG_API int runChatClient(std::string const & ip, int port);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__

