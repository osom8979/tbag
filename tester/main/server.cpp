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

class EchoServerTester : public libtbag::network::socket::Server
{
public:
    EchoServerTester()
    {
        // EMPTY.
    }

    ~EchoServerTester()
    {
        // EMPTY.
    }

public:
    virtual void onConnection(int status) override
    {
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "- onConnection(" << (status == 0 ? true : false) << ")\n";

        std::cout << "[ACCEPT]\n";
        if (SharedClient shared_client = accept().lock()) {
            std::cout << "[READ]\n";
            shared_client->read();
        }
    }

    virtual void onClose() override
    {
        std::cout << "- onClose()\n";
    }

public:
    virtual void onClientRead(WeakClient client, ReadErrorCode code, char * buffer, std::size_t length) override
    {
        if (code == ReadErrorCode::SUCCESS) {
            std::string msg;
            msg.assign(buffer, buffer + length);

            std::cout << "- onClientRead() success: " << msg << std::endl;

            if (SharedClient shared_client = client.lock()) {
                std::cout << "[WRITE]\n";
                shared_client->write(&msg[0], msg.size());
            }
        } else if (code == ReadErrorCode::SUCCESS) {
            std::cout << "- onClientRead() end of file.\n";
        } else {
            std::cout << "- onClientRead() unknown error.\n";
        }
    }

    virtual void onClientWrite(WeakClient client, WriteErrorCode code) override
    {
        if (code == WriteErrorCode::SUCCESS) {
            std::cout << "- onClientWrite() success.\n";

            if (SharedClient shared_client = client.lock()) {
                shared_client->close();
            }
        } else {
            std::cout << "- onClientWrite() unknown error.\n";
        }
    }

    virtual void onClientClose(WeakClient client) override
    {
        std::cout << "- onClientClose()\n";
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

