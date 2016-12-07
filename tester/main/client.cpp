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
    { return "Test echo message."; }

private:
    bool _result;

public:
    EchoClientTester() : _result(false)
    { /* EMPTY. */ }
    virtual ~EchoClientTester()
    { /* EMPTY. */ }

public:
    inline bool getResult() const TBAG_NOEXCEPT
    { return _result; }

public:
    virtual bool onConnect(int status) override
    {
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "- onConnect(" << (status == 0 ? true : false) << ")\n";

        std::string msg = getTestMessage();
        std::cout << "[WRITE MESSAGE]\n";
        this->write(&msg[0], msg.size());

        return true;
    }

    virtual void onClose() override
    {
        std::cout << "- onClose()\n";
    }

    virtual void onRead(Code code, char const * buffer, std::size_t size) override
    {
        if (code == Code::SUCCESS) {
            std::string msg;
            msg.assign(buffer, buffer + size);
            std::cout << "- onRead() success: " << msg << std::endl;

            if (msg == std::string(getTestMessage())) {
                _result = true;
            }
        } else if (code == Code::FAILURE) {
            std::cerr << "- onRead() unknown error.\n";
        } else {
            std::cout << "- onRead() End of file.\n";
        }

        std::cout << "[CLOSE]\n";
        this->close();
    }

    virtual void onWrite(Code code) override
    {
        if (code == Code::FAILURE) {
            std::cerr << "- onWrite() unknown error.\n";

            std::cout << "[CLOSE]\n";
            this->close();
        } else {
            std::cout << "- onWrite() success.\n";
        }
    }
};

int main_client(std::string const & ip, int port)
{
    EchoClientTester client;
    if (client.run(ip, port)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

