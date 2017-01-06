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
    std::cout.setf(std::ios_base::boolalpha);
}

ChatServer::~ChatServer()
{
    // EMPTY.
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

    flatbuffers::Verifier verifier((uint8_t const *)buffer, size);
    if (msg::VerifyChatPacketBuffer(verifier) == false) {
        std::cout << "PARSER ERROR.\n";
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

ChatClient::ChatClient(std::string const & name)
{
    std::cout.setf(std::ios_base::boolalpha);
    auto weak = atLoop().insertChildHandle(new AsyncChatInput(atLoop(), *this, name));
    _input = std::static_pointer_cast<AsyncChatInput, uv::Handle>(weak.lock());
}

ChatClient::~ChatClient()
{
    // EMPTY.
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
    _input->close();
}

// ------------------------------
// AsyncChatInput implementation.
// ------------------------------

AsyncChatInput::AsyncChatInput(uv::Loop & loop, ChatClient & client, std::string const & name)
        : uv::Tty(loop, GeneralFile::FILE_STDIN), _client(client), _name(name)
{
    setMode(TtyMode::TTY_NORMAL);
    startRead();
}

AsyncChatInput::~AsyncChatInput()
{
    stopRead();
    resetMode();
}

uv::binf AsyncChatInput::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
        _last_buffer.resize(suggested_size);
    }

    uv::binf info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}

void AsyncChatInput::onRead(Err code, char const * buffer, std::size_t size)
{
    std::cout << "onRead(): ";
    for (std::size_t index = 0; index < size; ++index) {
        std::cout << buffer[index];

        if (buffer[index] == '\n') {
            std::string msg(&_last_buffer[0], _last_index);
            onReadLine(msg);
            _last_index = 0;
        } else {
            _last_buffer[_last_index] = buffer[index];
            ++_last_index;
        }
    }
}

void AsyncChatInput::onReadLine(std::string const & msg)
{
    if (msg == "exit") {
        _client.asyncClose();
    }

    msg::Version version = msg::Version(1, 1);
    flatbuffers::FlatBufferBuilder builder;
    auto packet = msg::CreateChatPacket(builder, &version, builder.CreateString(_name), builder.CreateString(msg));
    builder.Finish(packet);

    if (_client.asyncWrite((char const *)builder.GetBufferPointer(), builder.GetSize()) == nullptr) {
        std::cout << "onReadLine() result error.\n";
        return;
    }
}

// -------------
// Main methods.
// -------------

int runChatServer(std::string const & ip, int port)
{
    return ChatServer().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runChatClient(std::string const & ip, int port)
{
    std::string name;
    std::cout << "Enter the your name: ";
    std::cin >> name;

    return ChatClient(name).run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

