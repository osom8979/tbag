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

struct EchoClientTester : public libtbag::network::socket::Client
{
//public:
//    inline static TBAG_CONSTEXPR char const * const getTestMessage() TBAG_NOEXCEPT
//    { return "Test echo message."; }
//
//private:
//    bool _echo_result;
//
//public:
//    EchoClientTester() : _echo_result(false)
//    { /* EMPTY. */ }
//    virtual ~EchoClientTester()
//    { /* EMPTY. */ }
//
//public:
//    inline bool getEchoResult() const
//    { return _echo_result; }
//
//public:
//    virtual void onConnect(int status) override
//    {
//        std::cout.setf(std::ios_base::boolalpha);
//        std::cout << "- onConnect(" << (status == 0 ? true : false) << ")\n";
//
//        std::cout << "[START READ]\n";
//        read();
//
//        std::string msg = getTestMessage();
//        std::cout << "[WRITE MESSAGE]\n";
//        write(&msg[0], msg.size());
//    }
//
//    virtual void onClose() override
//    {
//        std::cout << "- onClose()\n";
//    }
//
//    virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override
//    {
//        if (code == ReadErrorCode::SUCCESS) {
//            std::string msg;
//            msg.assign(buffer, buffer + length);
//            std::cout << "- onRead() success: " << msg << std::endl;
//
//            if (msg == std::string(getTestMessage())) {
//                _echo_result = true;
//            }
//        } else if (code == ReadErrorCode::UNKNOWN_ERROR) {
//            std::cerr << "- onRead() unknown error.\n";
//        } else {
//            std::cout << "- onRead() End of file.\n";
//        }
//
//        std::cout << "[CLOSE]\n";
//        close();
//    }
//
//    virtual void onWrite(WriteErrorCode code) override
//    {
//        if (code == WriteErrorCode::UNKNOWN_ERROR) {
//            std::cerr << "- onWrite() unknown error.\n";
//
//            std::cout << "[CLOSE]\n";
//            close();
//        } else {
//            std::cout << "- onWrite() success.\n";
//        }
//    }
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

