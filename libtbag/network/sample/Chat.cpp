/**
 * @file   Chat.cpp
 * @brief  Chat class implementation.
 * @author zer0
 * @date   2016-12-23
 */

#include <libtbag/network/sample/Chat.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/sample/msg/chat_generated.h>

#include <cstdlib>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------
// Chat server implementation.
// ---------------------------

ChatServer::ChatServer() : socket::Server(this)
{
}

ChatServer::~ChatServer()
{
}

bool ChatServer::onConnection(ClientKey key, int status)
{
    std::cout << "ChatServer::onConnect(" << key.get() << "," << (status == 0 ? true : false) << ") ";
    if (status != 0) {
        std::cout << "Status error: " << status << ".\n";
        return false;
    }

    ClientValue client = this->getClient(key);
    if (static_cast<bool>(client) == false) {
        std::cout << "Not found client key error.\n";
        return false;
    }

    std::cout << "Client information: " << client->getSocketName() << ".\n";
    if (this->read(key) == false) {
        std::cout << "Read error.\n";
        return false;
    }

    return true; // ACCEPT OK!
}

void ChatServer::onClose()
{
    std::cout << "ChatServer::onClose()\n";
}

void ChatServer::onCloseClient(ClientKey key)
{
    std::cout << "ChatServer::onCloseClient(" << key.get() << ")\n";
}

void ChatServer::onRead(ClientKey from, Code code, char * buffer, std::size_t size)
{
    std::cout << "ChatServer::onRead() ";

    if (code == Code::SUCCESS) {
        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Success: " << msg << std::endl;

        std::cout << "[WRITE MESSAGE] " << msg << std::endl;
        this->write(from, &msg[0], msg.size());
    } else if (code == Code::END_OF_FILE) {
        std::cout << "End of file.\n";
        this->closeClient(from);
    } else {
        std::cout << "Failure.\n";
        this->closeClient(from);
    }
}

void ChatServer::onWrite(ClientKey to, Code code)
{
    std::cout << "ChatServer::onWrite() ";
    if (code == Code::SUCCESS) {
        std::cout << "Success.\n";
    } else {
        std::cout << "Failure.\n";
    }

    this->closeClient(to);
    this->close();
}

// ---------------------------
// Chat client implementation.
// ---------------------------

ChatClient::ChatClient(): socket::Client(this)
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::onConnect(int status)
{
    std::cout << "ChatClient::onConnect(" << (status == 0 ? true : false) << ")\n";

    if (status != 0) {
        std::cout << "Status error: " << status << ".\n";
        return;
    }

    std::string msg;
    std::cout << "[WRITE MESSAGE] :";
    std::cin >> msg;

    this->read();
    this->write(&msg[0], msg.size());
}

void ChatClient::onClose()
{
    std::cout << "ChatClient::onClose()\n";
}

void ChatClient::onRead(Code code, char const * buffer, std::size_t size)
{
    std::cout << "ChatClient::onRead() ";

    if (code == Code::SUCCESS) {
        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Success: " << msg << std::endl;
    } else if (code == Code::END_OF_FILE) {
        std::cout << "End of file.\n";
    } else {
        std::cout << "Failure.\n";
    }

    this->close();
}

void ChatClient::onWrite(Code code)
{
    std::cout << "ChatClient::onWrite() ";

    if (code == Code::SUCCESS) {
        std::cout << "Success.\n";
    } else {
        std::cout << "Failure.\n";
        this->close();
    }
}

// -------------
// Main methods.
// -------------

int runChatServer(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

    ChatServer server;
    if (server.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int runChatClient(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

    ChatClient client;
    if (client.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

