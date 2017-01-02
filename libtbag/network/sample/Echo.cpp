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

EchoServer::EchoServer(int count) : _write_count(count), _echo_count(count * 5)
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

        shared->setUserData(new DatagramAdapter());

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

EchoServer::binf EchoServer::onClientAlloc(Client & client, std::size_t suggested_size)
{
    DatagramAdapter * adapter = static_cast<DatagramAdapter*>(client.getUserData());
    if (adapter != nullptr) {
        adapter->alloc(suggested_size);
    }
    return uv::defaultOnAlloc(client.atReadBuffer(), suggested_size);
}

void EchoServer::onClientRead(Client & client, Err code, char const * buffer, std::size_t size)
{
    DatagramAdapter * adapter = static_cast<DatagramAdapter*>(client.getUserData());
    if (code == Err::SUCCESS && adapter != nullptr) {
        adapter->push(buffer, size);

        binf info;
        while (adapter->next(&info)) {
            std::string msg;
            msg.assign(info.buffer, info.buffer + info.size);
            std::cout << "Read message: " << msg << std::endl;

            for (int i = 0; i < _write_count; ++i) {
                adapter->asyncWrite(client, info.buffer, info.size);
            }
        }
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

    --_echo_count;
    std::cout << "ECHO COUNT: " << _echo_count << "\n";

    if ((_echo_count % _write_count) == 0) {
        client.close();
    }

    if (_echo_count <= 0) {
        close();
    }
}

void EchoServer::onClientClose(Client & client)
{
    std::cout << "CLIENT " << (void*)&client << " END.\n";
    DatagramAdapter * adapter = static_cast<DatagramAdapter*>(client.getUserData());
    if (adapter != nullptr) {
        delete adapter;
        client.setUserData(nullptr);
    }
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

EchoClient::binf EchoClient::onAlloc(std::size_t suggested_size)
{
    _datagram.alloc(suggested_size);
    return uv::defaultOnAlloc(atReadBuffer(), suggested_size);
}

void EchoClient::onConnect(ConnectRequest & request, Err code)
{
    if (code != Err::SUCCESS) {
        std::cout << "EchoClient::onConnect() Status error: " << static_cast<int>(code) << ".\n";
        return;
    }

    startRead();

    std::string msg = TEST_ECHO_MESSAGE;
    _datagram.asyncWrite(*this, &msg[0], msg.size());
}

void EchoClient::onRead(Err code, char const * buffer, std::size_t size)
{
    if (code == Err::SUCCESS) {
        _datagram.push(buffer, size);

        binf info;
        while (_datagram.next(&info)) {
            std::string msg;
            msg.assign(info.buffer, info.buffer + info.size);
            std::cout << "Echo read: " << msg << std::endl;
        }
    } else if (code == Err::END_OF_FILE) {
        std::cout << "EchoClient::onRead() End of file.\n";
        close();

    } else {
        std::cout << "EchoClient::onRead() Failure.\n";
        close();
    }
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

