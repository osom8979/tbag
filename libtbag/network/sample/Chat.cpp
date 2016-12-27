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
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------
// Chat server implementation.
// ---------------------------

ChatServer::ChatServer() : socket::Server(this), _id_count(0)
{
}

ChatServer::~ChatServer()
{
}

bool ChatServer::onConnection(ClientKey key, int status)
{
    if (status != 0) {
        std::cout << "STATUS ERROR: " << status << std::endl;
        return false;
    }

    ClientValue client = this->getClient(key);
    if (static_cast<bool>(client) == false) {
        std::cout << "NOT FOUND CLIENT KEY ERROR.\n";
        return false;
    }

    if (this->read(key) == false) {
        std::cout << "READ ERROR.\n";
        return false;
    }

    UserInfo user;
    user.id = _id_count++;
    user.ip = client->getSocketName();
    user.name = std::to_string(user.id);

    _users.insert(Users::value_type(key, user));
    std::cout << "JOIN: " << user.name << "(" << user.ip << ")\n";
    return true; // ACCEPT OK!
}

void ChatServer::onClose()
{
    std::cout << "CLOSE THIS SERVER.\n";
}

void ChatServer::onCloseClient(ClientKey key)
{
    auto find_itr = _users.find(key);
    if (find_itr == _users.end()) {
        std::cout << "UNKNOWN KEY ERROR: " << key.get() << std::endl;
        return;
    }

    std::cout << "OUT: " << find_itr->second.name << "(" << find_itr->second.ip << ")\n";
    _users.erase(find_itr);
}

void ChatServer::onRead(ClientKey from, Code code, char * buffer, std::size_t size)
{
    if (code == Code::END_OF_FILE) {
        this->closeClient(from);
        return;
    }
    if (code == Code::FAILURE) {
        std::cout << "UNKNOWN READ ERROR: " << from.get() << std::endl;
        return;
    }

    auto find_itr = _users.find(from);
    if (find_itr == _users.end()) {
        std::cout << "UNKNOWN READ/KEY ERROR: " << from.get() << std::endl;
        return;
    }

    auto msg = msg::GetChatPacket(buffer);
    std::cout << "[" << msg->name()->c_str() << "]"
              << "(" << (int)msg->ver()->major() << "." << (int)msg->ver()->minor() << ")"
              << msg->msg()->c_str() << std::endl;

    for (auto itr = _users.begin(); itr != _users.end(); ++itr) {
        if (itr != find_itr) {
            this->write(itr->first, buffer, size);
        }
    }
}

void ChatServer::onWrite(ClientKey to, Code code)
{
    if (code != Code::SUCCESS) {
        std::cout << "SEND FAILURE.\n";
        this->closeClient(to);
    }
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
    if (status != 0) {
        std::cout << "STATUS ERROR: " << status << std::endl;
        return;
    }

    this->read();
}

void ChatClient::onClose()
{
    std::cout << "CLOSE.\n";
}

void ChatClient::onRead(Code code, char const * buffer, std::size_t size)
{
    if (code == Code::END_OF_FILE) {
        this->close();
        return;
    }
    if (code == Code::FAILURE) {
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
              << msg->msg()->c_str() << std::endl;
}

void ChatClient::onWrite(Code code)
{
    if (code != Code::SUCCESS) {
        std::cout << "SEND FAILURE.\n";
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
                client.try_write((char const *)builder.GetBufferPointer(), builder.GetSize());
            } else {
                break;
            }
        }
    });

    bool const RESULT = client.run(ip, port);
    is_exit = true;

    t.join();

    return (RESULT ? EXIT_SUCCESS : EXIT_FAILURE);
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

