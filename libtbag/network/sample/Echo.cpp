/**
 * @file   Echo.cpp
 * @brief  Echo class implementation.
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#include <libtbag/network/sample/Echo.hpp>
#include <libtbag/log/Log.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

// ---------------------------
// Echo server implementation.
// ---------------------------

EchoServer::EchoServer(int count) : _count(count)
{
    std::cout.setf(std::ios_base::boolalpha);
    std::cerr.setf(std::ios_base::boolalpha);
}

EchoServer::~EchoServer()
{
    // EMPTY.
}

bool EchoServer::onNewConnection(uerr code, WeakClient client)
{
    if (auto shared = client.lock()) {
        if (shared->startRead() == uerr::UVPP_SUCCESS) {
            return true;
        }
    }
    return false;
}

void EchoServer::onClientWrite(ClientTcp & client, WriteRequest & request, uerr code)
{
    if (code != uerr::UVPP_SUCCESS) {
        client.close();
        return;
    }

    if ((--_count) == 0) {
        asyncClose();
    }
}

void EchoServer::onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size)
{
    std::string msg;
    msg.assign(buffer, buffer + size);
    std::cout << " - Read message: " << msg << std::endl;

    if (asyncWriteClient(client, &msg[0], msg.size())) {
        std::cout << "EchoServer::onClientReadDatagram() write success.\n";
    } else {
        std::cerr << "EchoServer::onClientReadDatagram() write failure.\n";
    }
}

// ---------------------------
// Echo client implementation.
// ---------------------------

EchoClient::EchoClient(int count) : _count(count)
{
    std::cout.setf(std::ios_base::boolalpha);
    std::cerr.setf(std::ios_base::boolalpha);
}

EchoClient::~EchoClient()
{
    // EMPTY.
}

void EchoClient::onClientConnect(ConnectRequest & request, uerr code)
{
    auto shared = getWeakClient().lock();

    if (code != uerr::UVPP_SUCCESS) {
        std::cerr << "EchoClient::onClientConnect() Status error: " << static_cast<int>(code) << ".\n";
        shared->close();
        return;
    }

    if (shared->startRead() == uerr::UVPP_SUCCESS) {
        std::cout << "EchoClient::onClientConnect() start read success.\n";
    } else {
        std::cerr << "EchoClient::onClientConnect() start read failure.\n";
        shared->close();
        return;
    }

    std::string msg = ECHO_MESSAGE;
    if (asyncWrite(&msg[0], msg.size())) {
        std::cout << "EchoClient::onClientConnect() write success.\n";
    } else {
        std::cerr << "EchoClient::onClientConnect() write failure.\n";
    }
}

void EchoClient::onClientReadDatagram(uerr code, char const * buffer, std::size_t size)
{
    std::string msg;
    msg.assign(buffer, buffer + size);
    std::cout << " - Echo read: " << msg << std::endl;

    if ((--_count) == 0) {
        asyncClose();
    } else {
        std::string msg = ECHO_MESSAGE;
        asyncWrite(&msg[0], msg.size());
    }
}

// -------------
// Main methods.
// -------------

int runEchoServer(std::string const & ip, int port)
{
    std::cout << "Start echo server: " << ip << " (" << port << ")\n";
    return EchoServer(ECHO_COUNT).run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runEchoClient(std::string const & ip, int port)
{
    std::cout << "Start echo client: " << ip << " (" << port << ")\n";
    return EchoClient(ECHO_COUNT).run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

