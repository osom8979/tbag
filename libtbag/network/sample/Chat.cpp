/**
 * @file   Chat.cpp
 * @brief  Chat class implementation.
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#include <libtbag/network/sample/Chat.hpp>
#include <libtbag/log/Log.hpp>

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

ChatServer::ChatServer()
{
    std::cout.setf(std::ios_base::boolalpha);
    std::cerr.setf(std::ios_base::boolalpha);
}

ChatServer::~ChatServer()
{
    // EMPTY.
}

bool ChatServer::onNewConnection(uerr code, WeakClient client)
{
    if (auto shared = client.lock()) {
        if (shared->startRead() == uerr::UVPP_SUCCESS) {
            std::cout << "ChatServer::onNewConnection() start read success.\n";
            return true;
        } else {
            std::cerr << "ChatServer::onNewConnection() start read failure.\n";
            shared->close();
        }
    }
    return false;
}

void ChatServer::onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size)
{
    std::string msg;
    msg.assign(buffer, buffer + size);
    std::cout << "ChatServer::onClientReadDatagram() read message: " << msg << std::endl;

    foreachClient([&](ClientTcp & tcp){
        this->asyncWriteClient(tcp, buffer, size);
    });
}

// ---------------------------
// Chat client implementation.
// ---------------------------

ChatClient::ChatClient()
{
    std::cout.setf(std::ios_base::boolalpha);
    std::cerr.setf(std::ios_base::boolalpha);

    _input = atLoop().newHandle<ChatTty>(atLoop(), *this);
}

ChatClient::~ChatClient()
{
    // EMPTY.
}

void ChatClient::onClientConnect(ConnectRequest & request, uerr code)
{
    auto shared = getWeakClient().lock();

    if (code != uerr::UVPP_SUCCESS) {
        std::cerr << "ChatClient::onClientConnect() Status error: " << static_cast<int>(code) << ".\n";
        shared->close();
        return;
    }

    if (shared->startRead() == uerr::UVPP_SUCCESS) {
        std::cout << "ChatClient::onClientConnect() start read success.\n";
    } else {
        std::cerr << "ChatClient::onClientConnect() start read failure.\n";
        shared->close();
    }
}

void ChatClient::onClientReadDatagram(uerr code, char const * buffer, std::size_t size)
{
    std::string msg;
    msg.assign(buffer, buffer + size);
    std::cout << " - message: " << msg << std::endl;
}

void ChatClient::onClientClose()
{
    _input->close();
    if (auto shared = getWeakAsync().lock()) {
        shared->close();
    }
}

// -----------------------
// ChatTty implementation.
// -----------------------

ChatTty::ChatTty(Loop & loop, ChatClient & client)
        : Tty(loop, GeneralFile::FILE_STDIN), _client(client)
{
    setMode(TtyMode::TTY_NORMAL);
    startRead();
}

ChatTty::~ChatTty()
{
    stopRead();
    resetMode();
}

uvpp::binf ChatTty::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
        _last_buffer.resize(suggested_size);
    }

    uvpp::binf info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}

void ChatTty::onRead(uerr code, char const * buffer, std::size_t size)
{
    char const LF = '\n';

    std::cout << "ChatTty::onRead() message: ";
    for (std::size_t index = 0; index < size; ++index) {
        std::cout << buffer[index];

        if (buffer[index] == LF) {
            std::string msg(&_last_buffer[0], _last_index);
            onReadLine(msg);
            _last_index = 0;
        } else {
            _last_buffer[_last_index] = buffer[index];
            ++_last_index;
        }
    }
}

void ChatTty::onReadLine(std::string const & msg)
{
    char const * const EXIT_COMMAND = "exit";

    if (msg == EXIT_COMMAND) {
        _client.asyncClose();
        return;
    }

    std::cout << "ChatTty::onReadLine() write message(" << msg.size() << "): " << msg << std::endl;

    if (_client.asyncWrite(&msg[0], msg.size())) {
        std::cout << "ChatTty::onReadLine() write success.\n";
    } else {
        std::cerr << "ChatTty::onReadLine() write failure.\n";
    }
}

// -------------
// Main methods.
// -------------

int runChatServer(std::string const & ip, int port)
{
    std::cout << "Start chatting server: " << ip << " (" << port << ")\n";
    return ChatServer().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runChatClient(std::string const & ip, int port)
{
    std::cout << "Start chatting client: " << ip << " (" << port << ")\n";
    return ChatClient().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

