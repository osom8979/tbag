/**
 * @file   client.cpp
 * @brief  client class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <tester/main/client.hpp>
#include <libtbag/network/socket/Client.hpp>
#include <cstdlib>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

using namespace network;
using namespace network::socket;

/**
 * Echo client implementation.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 */
struct EchoClientTester : public Client, public Client::EventCallback
{
public:
    inline static TBAG_CONSTEXPR char const * const getTestMessage() TBAG_NOEXCEPT
    { return "THIS_IS_TEST_ECHO_MESSAGE"; }

public:
    EchoClientTester() : Client(this)
    { /* EMPTY. */ }
    virtual ~EchoClientTester()
    { /* EMPTY. */ }

public:
    virtual void onConnect(int status) override
    {
        std::cout << "EchoClientTester::onConnect(" << (status == 0 ? true : false) << ")\n";

        if (status != 0) {
            std::cout << "Status error: " << status << ".\n";
            return;
        }

        std::string msg = getTestMessage();
        std::cout << "[WRITE MESSAGE] " << msg << std::endl;

        this->read();
        this->write(&msg[0], msg.size());
    }

    virtual void onClose() override
    {
        std::cout << "EchoClientTester::onClose()\n";
    }

    virtual void onRead(Code code, char const * buffer, std::size_t size) override
    {
        std::cout << "EchoClientTester::onRead() ";
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

    virtual void onWrite(Code code) override
    {
        std::cout << "EchoClientTester::onWrite() ";
        if (code == Code::SUCCESS) {
            std::cout << "Success.\n";
        } else {
            std::cout << "Failure.\n";
            this->close();
        }
    }
};

int main_client(std::string const & ip, int port)
{
    std::cout.setf(std::ios_base::boolalpha);

    EchoClientTester client;
    if (client.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

