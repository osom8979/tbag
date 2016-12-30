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

#include <map>

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

/**
 * Chatting client class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 */
class TBAG_API ChatClient : public TcpClient
{
public:
    ChatClient();
    virtual ~ChatClient();

public:
    virtual void onConnect(ConnectRequest & request, Err code) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onWrite(WriteRequest & request, Err code) override;
    virtual void onClose() override;
};

TBAG_API int runChatServer(std::string const & ip, int port);
TBAG_API int runChatClient(std::string const & ip, int port);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__

