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
}

EchoServer::~EchoServer()
{
}

bool EchoServer::onConnection(ClientKey key, int status)
{
    std::cout << "EchoServer::onConnect(" << key.get() << "," << (status == 0 ? true : false) << ") ";
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

void EchoServer::onClose()
{
    std::cout << "EchoServer::onClose()\n";
}

void EchoServer::onCloseClient(ClientKey key)
{
    std::cout << "EchoServer::onCloseClient(" << key.get() << ")\n";
}

void EchoServer::onRead(ClientKey from, Code code, char * buffer, std::size_t size)
{
    std::cout << "EchoServer::onRead() ";

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

void EchoServer::onWrite(ClientKey to, Code code)
{
    std::cout << "EchoServer::onWrite() ";
    if (code == Code::SUCCESS) {
        std::cout << "Success.\n";
    } else {
        std::cout << "Failure.\n";
    }

    this->closeClient(to);

    --_count;
    std::cout << "Echo count: " << _count << "\n";

    if (_count <= 0) {
        this->close();
    }
}

// ---------------------------
// Echo client implementation.
// ---------------------------

EchoClient::EchoClient(): socket::Client(this)
{
}

EchoClient::~EchoClient()
{
}

void EchoClient::onConnect(int status)
{
    std::cout << "EchoClient::onConnect(" << (status == 0 ? true : false) << ")\n";

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

void EchoClient::onClose()
{
    std::cout << "EchoClient::onClose()\n";
}

void EchoClient::onRead(Code code, char const * buffer, std::size_t size)
{
    std::cout << "EchoClient::onRead() ";

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

void EchoClient::onWrite(Code code)
{
    std::cout << "EchoClient::onWrite() ";

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

int runEchoServer(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

    EchoServer server;
    if (server.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int runEchoClient(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

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

