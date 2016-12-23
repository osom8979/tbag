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
#include <libtbag/network/socket/Server.hpp>
#include <libtbag/network/socket/Client.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// TODO: Forward declaration.

/**
 * Chatting server class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 */
class TBAG_API ChatServer : public socket::Server, public socket::Server::EventCallback
{
private:

public:
    ChatServer();
    virtual ~ChatServer();

public:
    virtual bool onConnection(ClientKey key, int status) override;
    virtual void onClose() override;
    virtual void onCloseClient(ClientKey key) override;
    virtual void onRead(ClientKey from, Code code, char * buffer, std::size_t size) override;
    virtual void onWrite(ClientKey to, Code code) override;
};

/**
 * Chatting client class prototype.
 *
 * @author zer0
 * @date   2016-12-23
 */
class TBAG_API ChatClient : public socket::Client, public socket::Client::EventCallback
{
private:

public:
    ChatClient();
    virtual ~ChatClient();

public:
    virtual void onConnect(int status) override;
    virtual void onClose() override;
    virtual void onRead(Code code, char const * buffer, std::size_t size) override;
    virtual void onWrite(Code code) override;
};

TBAG_API int runChatServer(std::string const & ip, int port);
TBAG_API int runChatClient(std::string const & ip, int port);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_CHAT_HPP__

