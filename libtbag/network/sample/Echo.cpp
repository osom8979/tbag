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
#include <limits>

#define TEST_MASSIVE_MESSAGE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

static char const * const TEST_ECHO_MESSAGE  = "__TEST_ECHO_MESSAGE__";
static  std::size_t const PRINT_MINIMUM_SIZE = 32;
static  std::size_t const MASSIVE_SIZE       = 500 * 500 * 3;
static         char const MASSIVE_VALUE      = static_cast<char>(0x0F);

static TBAG_CONSTEXPR bool isTestMassiveMessage() TBAG_NOEXCEPT
{
#if defined(TEST_MASSIVE_MESSAGE)
    return true;
#else
    return false;
#endif
}

// ---------------------------
// Echo server implementation.
// ---------------------------

EchoServer::EchoServer(int count, bool massive) : _write_count(count), _echo_count(count * 5), _massive(massive)
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
    suggested_size = MASSIVE_SIZE;
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
        std::cout << "EchoServer::onClientRead() Success size(" << size << ").\n";
        adapter->push(buffer, size);

        binf info;
        while (adapter->next(&info)) {
            std::string msg;
            if (info.size <= PRINT_MINIMUM_SIZE) {
                msg.assign(info.buffer, info.buffer + info.size);
                std::cout << " - Read message: " << msg << std::endl;
            } else {
                std::cout << " - Read message size(" << info.size << ")\n";
            }

            if (_massive) {
                // CREATE MASSIVE DATA.
                Buffer const MASSIVE_BUFFER(MASSIVE_SIZE, static_cast<char>(0x0F));
                for (int i = 0; i < _write_count; ++i) {
                    adapter->asyncWrite(client, &MASSIVE_BUFFER[0], MASSIVE_BUFFER.size());
                }
            } else {
                for (int i = 0; i < _write_count; ++i) {
                    adapter->asyncWrite(client, info.buffer, info.size);
                }
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

EchoClient::EchoClient() : _read_count(0), _debugging_count(0)
{
    std::cout.setf(std::ios_base::boolalpha);
}

EchoClient::~EchoClient()
{
    // EMPTY.
}

EchoClient::binf EchoClient::onAlloc(std::size_t suggested_size)
{
    suggested_size = MASSIVE_SIZE;
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
        std::cout << "EchoClient::onRead() Success(" << _read_count++ << ") size(" << size << ")";
        _datagram.push(buffer, size);

        std::cout << " - Datagram buffer size(" << _datagram.atDataBuffer().size() << ") free(" << _datagram.atDataBuffer().free() << ")";

        ++_debugging_count;

        binf info;
        while (_datagram.next(&info)) {
            if (info.size <= PRINT_MINIMUM_SIZE) {
                std::string msg;
                msg.assign(info.buffer, info.buffer + info.size);
                std::cout << " - Echo read: " << msg << std::endl;
            } else {
                std::cout << " - Echo read size(" << info.size << ")\n";
                assert((info.size == MASSIVE_SIZE) && "DatagramAdapter binf.size error!!\n");
            }

            _debugging_count = 0;
        }

        if (_debugging_count >= 10) {
            assert(false && "DatagramAdapter bug!!\n");
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
    int  count   = 3;
    bool massive = false;

    if (isTestMassiveMessage()) {
        count   = 50000;
        massive = true;
    }

    std::cout << "Start echo server: " << ip << " (" << port << ")\n";
    return EchoServer(count, massive).run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runEchoClient(std::string const & ip, int port)
{
    std::cout << "Start echo client: " << ip << " (" << port << ")\n";
    return EchoClient().run(ip, port) ? EXIT_FAILURE : EXIT_SUCCESS;
}

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

