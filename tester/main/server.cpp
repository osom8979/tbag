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
public:
    EchoServerTester()
    { /* EMPTY. */ }
    virtual ~EchoServerTester()
    { /* EMPTY. */ }

public:
    virtual bool onConnection(std::string & client_address, int status) override
    {
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "- onConnection(" << (status == 0 ? true : false) << ") \n";

        std::cout << "[ACCEPT]\n";
        return true;
    }

    virtual void onClose() override
    {
        std::cout << "- onClose()\n";
    }

    virtual void onCloseClient(ClientKey key) override
    {
        std::cout << "- onClientClose()\n";
    }

    virtual void onRead(ClientKey from, Code code, char * buffer, std::size_t size) override
    {
        if (code == Code::SUCCESS) {
            std::string msg;
            msg.assign(buffer, buffer + size);

            std::cout << "- onClientRead() success: " << msg << std::endl;

            std::cout << "[WRITE]\n";
            this->write(from, &msg[0], msg.size());
        } else if (code == Code::END_OF_FILE) {
            std::cout << "- onClientRead() end of file.\n";
        } else {
            std::cout << "- onClientRead() unknown error.\n";
        }
    }

    virtual void onWrite(ClientKey to, Code code) override
    {
        if (code == Code::SUCCESS) {
            std::cout << "- onClientWrite() success.\n";
            this->closeClient(to);
        } else {
            std::cout << "- onClientWrite() unknown error.\n";
        }
    }
};

int main_server(std::string const & ip, int port)
{
    EchoServerTester server;
    if (server.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

