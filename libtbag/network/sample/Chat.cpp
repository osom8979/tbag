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
#include <cassert>

#include <iostream>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------
// Chat server implementation.
// ---------------------------

ChatServer::ChatServer() : _id_count(0)
{
}

ChatServer::~ChatServer()
{
}

void ChatServer::onConnection(Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "STATUS ERROR: " << static_cast<int>(code) << std::endl;
        return;
    }

    auto client = createAcceptedClient();
    if (auto shared = client.lock()) {
        std::cout << "CLIENT CONNECTION: " << shared->getSockName() << std::endl;

        UserInfo * user = new UserInfo;
        user->id = _id_count++;
        user->ip = shared->getSockName();
        user->name = std::to_string(user->id);
        shared->setUserData(user);

        std::cout << "JOIN: " << user->name << "(" << user->ip << ")\n";

        if (shared->startRead() == false) {
            std::cout << "Start read error.\n";
            eraseClient(*shared);
        }
    } else {
        std::cout << "Not found client key error.\n";
    }
}

void ChatServer::onClose()
{
    std::cout << "CLOSE THIS SERVER.\n";
}

void ChatServer::onClientRead(Client & client, Err code, char const * buffer, std::size_t size)
{
    if (code == Err::END_OF_FILE) {
        client.close();
        return;
    }
    if (code == Err::FAILURE) {
        std::cout << "UNKNOWN READ ERROR: " << (void*)&client << std::endl;
        return;
    }

    auto msg = msg::GetChatPacket(buffer);
    std::cout << "[" << msg->name()->c_str() << "]"
              << "(" << (int)msg->ver()->major() << "." << (int)msg->ver()->minor() << ")"
              << msg->msg()->c_str() << std::endl;

    for (auto & cursor : _clients) {
        if (cursor.second.get() != &client) {
            cursor.second->asyncWrite(buffer, size);
        }
    }
}

void ChatServer::onClientWrite(Client & client, WriteRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "SEND FAILURE.\n";
        client.close();
    }
}

void ChatServer::onClientClose(Client & client)
{
    UserInfo * user = static_cast<UserInfo*>(client.getUserData());
    if (user == nullptr) {
        std::cout << "Not found user info.\n";
        return;
    }

    assert(user != nullptr);
    std::cout << "OUT: " << user->name << "(" << user->ip << ")\n";
    delete user;
    client.setUserData(nullptr);
}

// ---------------------------
// Chat client implementation.
// ---------------------------

ChatClient::ChatClient()
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::onConnect(ConnectRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "STATUS ERROR: " << static_cast<int>(code) << std::endl;
        return;
    }
    startRead();
}

void ChatClient::onRead(Err code, char const * buffer, std::size_t size)
{
    if (code == Err::END_OF_FILE) {
        close();
        return;
    }
    if (code == Err::FAILURE) {
        std::cout << "UNKNOWN READ ERROR.\n";
        return;
    }

    flatbuffers::Verifier verifier((uint8_t const *)buffer, size);
    if (msg::VerifyChatPacketBuffer(verifier) == false) {
        std::cout << "PARSER ERROR.\n";
        return;
    }

    auto msg = msg::GetChatPacket(buffer);
    std::cout << "[" << msg->name()->c_str() << "]"
              << "(" << (int)msg->ver()->major() << "." << (int)msg->ver()->minor() << ")"
              << msg->msg()->c_str()
              << std::endl;
}

void ChatClient::onWrite(WriteRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "SEND FAILURE.\n";
        close();
    }
}

void ChatClient::onClose()
{
    std::cout << "CLOSE.\n";
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

    std::string name;
    std::cout << "Enter the your name: ";
    std::cin >> name;

    std::mutex mutex;
    std::atomic_bool is_exit(false);

    ChatClient client;
    std::thread t([&](){
        while (true) {
            std::string msg;
            std::cin >> msg;

            if (is_exit == false) {
                msg::Version version = msg::Version(1, 1);
                flatbuffers::FlatBufferBuilder builder;
                auto packet = msg::CreateChatPacket(builder, &version, builder.CreateString(name), builder.CreateString(msg));
                builder.Finish(packet);
                client.tryWrite((char const *)builder.GetBufferPointer(), builder.GetSize());
            } else {
                break;
            }
        }
    });
    bool result = client.run(ip, port);

    is_exit = true;
    t.join();

    return (result ? EXIT_SUCCESS : EXIT_FAILURE);
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

