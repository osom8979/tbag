/**
 * @file   Echo.cpp
 * @brief  Echo class implementation.
 * @author zer0
 * @date   2016-12-23
 */

#include <libtbag/network/sample/Echo.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------
// Echo server implementation.
// ---------------------------

EchoServer::EchoServer(int count) : socket::Server(this), _count(count)
{
    std::cout.setf(std::ios_base::boolalpha);
}

EchoServer::~EchoServer()
{
    // EMPTY.
}

bool EchoServer::onConnection(ClientKey key, int status)
{
    if (status != 0) {
        std::cout << "EchoServer::onConnection() Status error: " << status << ".\n";
        return false;
    }

    ClientValue client = this->getClient(key);
    if (static_cast<bool>(client) == false) {
        std::cout << "Not found client key error.\n";
        return false;
    }

    std::cout << "CLIENT CONNECTION: " << client->getSocketName() << std::endl;
    if (this->read(key) == false) {
        std::cout << "Start read error.\n";
        return false;
    }

    return true; // ACCEPT OK!
}

void EchoServer::onClose()
{
    std::cout << "SERVER END.\n";
}

void EchoServer::onCloseClient(ClientKey key)
{
    std::cout << "CLIENT " << key.get() << "END.\n";
}

void EchoServer::onRead(ClientKey from, Code code, char * buffer, std::size_t size)
{
    if (code == Code::SUCCESS) {
        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Read message: " << msg << std::endl;
        this->write(from, &msg[0], msg.size());
    } else if (code == Code::END_OF_FILE) {
        std::cout << "EchoServer::onRead() End of file.\n";
        this->closeClient(from);
    } else {
        std::cout << "EchoServer::onRead() Failure.\n";
        this->closeClient(from);
    }
}

void EchoServer::onWrite(ClientKey to, Code code)
{
    if (code != Code::SUCCESS) {
        std::cout << "EchoServer::onWrite() Failure.\n";
    }

    this->closeClient(to);

    --_count;
    std::cout << "ECHO COUNT: " << _count << "\n";

    if (_count <= 0) {
        this->close();
    }
}

// ---------------------------
// Echo client implementation.
// ---------------------------

EchoClient::EchoClient(): socket::Client(this)
{
    std::cout.setf(std::ios_base::boolalpha);
}

EchoClient::~EchoClient()
{
    // EMPTY.
}

void EchoClient::onConnect(int status)
{
    if (status != 0) {
        std::cout << "EchoClient::onConnect() Status error: " << status << ".\n";
        return;
    }

    std::string msg;
    std::cout << "MESSAGE: ";
    std::cin >> msg;

    this->read();
    this->write(&msg[0], msg.size());
}

void EchoClient::onClose()
{
    std::cout << "END.\n";
}

void EchoClient::onRead(Code code, char const * buffer, std::size_t size)
{
    if (code == Code::SUCCESS) {
        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Echo read: " << msg << std::endl;
    } else if (code == Code::END_OF_FILE) {
        std::cout << "EchoClient::onRead() End of file.\n";
    } else {
        std::cout << "EchoClient::onRead() Failure.\n";
    }

    this->close();
}

void EchoClient::onWrite(Code code)
{
    if (code != Code::SUCCESS) {
        std::cout << "EchoClient::onWrite() Failure.\n";
        this->close();
    }
}

// -------------
// Main methods.
// -------------

int runEchoServer(std::string const & ip, int port)
{
    EchoServer server;
    if (server.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int runEchoClient(std::string const & ip, int port)
{
    EchoClient client;
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

