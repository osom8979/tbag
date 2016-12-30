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

static char const * const TEST_ECHO_MESSAGE = "__TEST_ECHO_MESSAGE__";

// ---------------------------
// Echo server implementation.
// ---------------------------

EchoServer::EchoServer(int count) : _count(count)
{
    std::cout.setf(std::ios_base::boolalpha);
}

EchoServer::~EchoServer()
{
    // EMPTY.
}

void EchoServer::onConnection(Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "EchoServer::onConnection() Status error: " << static_cast<int>(code) << ".\n";
        return;
    }

    auto client = createAcceptedClient();
    if (auto shared = client.lock()) {
        std::cout << "CLIENT CONNECTION: " << shared->getSockName() << std::endl;

        if (shared->startRead() == false) {
            std::cout << "Start read error.\n";
            eraseClient(*shared);
        }
    } else {
        std::cout << "Not found client key error.\n";
    }
}

void EchoServer::onClose()
{
    std::cout << "SERVER END.\n";
}

void EchoServer::onClientRead(Client & client, Err code, char const * buffer, std::size_t size)
{
    if (code == Err::SUCCESS) {

        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Read message: " << msg << std::endl;

        client.tryWrite(&msg[0], msg.size());

    } else if (code == Err::END_OF_FILE) {
        std::cout << "EchoServer::onRead() End of file.\n";
        client.close();

    } else {
        std::cout << "EchoServer::onRead() Failure.\n";
        client.close();
    }
}

void EchoServer::onClientWrite(Client & client, WriteRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "EchoServer::onWrite() Failure.\n";
    }

    client.close();

    --_count;
    std::cout << "ECHO COUNT: " << _count << "\n";

    if (_count <= 0) {
        this->close();
    }
}

void EchoServer::onClientClose(Client & client)
{
    std::cout << "CLIENT " << (void*)&client << "END.\n";
}

// ---------------------------
// Echo client implementation.
// ---------------------------

EchoClient::EchoClient()
{
    std::cout.setf(std::ios_base::boolalpha);
}

EchoClient::~EchoClient()
{
    // EMPTY.
}

void EchoClient::onConnect(ConnectRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "EchoClient::onConnect() Status error: " << static_cast<int>(code) << ".\n";
        return;
    }

    startRead();

    std::string msg = TEST_ECHO_MESSAGE;
    write(&msg[0], msg.size());
}

void EchoClient::onRead(Err code, char const * buffer, std::size_t size)
{
    if (code == Err::SUCCESS) {
        std::string msg;
        msg.assign(buffer, buffer + size);
        std::cout << "Echo read: " << msg << std::endl;
    } else if (code == Err::END_OF_FILE) {
        std::cout << "EchoClient::onRead() End of file.\n";
    } else {
        std::cout << "EchoClient::onRead() Failure.\n";
    }

    close();
}

void EchoClient::onWrite(WriteRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "EchoClient::onWrite() Failure.\n";
        close();
    }
}

void EchoClient::onClose()
{
    std::cout << "END.\n";
}

// -------------
// Main methods.
// -------------

int runEchoServer(std::string const & ip, int port)
{
    return EchoServer().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runEchoClient(std::string const & ip, int port)
{
    return EchoClient().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

