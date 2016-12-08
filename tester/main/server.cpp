/**
 * @file   server.cpp
 * @brief  server class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <tester/main/server.hpp>
#include <libtbag/network/socket/Server.hpp>
#include <cstdlib>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

using namespace network;
using namespace network::socket;

/**
 * Echo server implementation.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 */
struct EchoServerTester : public Server, public Server::EventCallback
{
private:
    int _echo_count;

public:
    EchoServerTester() : Server(this), _echo_count(5)
    { /* EMPTY. */ }
    virtual ~EchoServerTester()
    { /* EMPTY. */ }

public:
    virtual bool onConnection(ClientKey key, int status) override
    {
        std::cout << "EchoServerTester::onConnect(" << key.get() << "," << (status == 0 ? true : false) << ") ";
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

    virtual void onClose() override
    {
        std::cout << "EchoServerTester::onClose()\n";
    }

    virtual void onCloseClient(ClientKey key) override
    {
        std::cout << "EchoServerTester::onCloseClient(" << key.get() << ")\n";
    }

    virtual void onRead(ClientKey from, Code code, char * buffer, std::size_t size) override
    {
        std::cout << "EchoServerTester::onRead() ";
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

    virtual void onWrite(ClientKey to, Code code) override
    {
        std::cout << "EchoServerTester::onWrite() ";
        if (code == Code::SUCCESS) {
            std::cout << "Success.\n";
        } else {
            std::cout << "Failure.\n";
        }

        this->closeClient(to);

        --_echo_count;
        std::cout << "Echo count: " << _echo_count << "\n";

        if (_echo_count <= 0) {
            this->close();
        }
    }
};

int main_server(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

    EchoServerTester server;
    if (server.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

