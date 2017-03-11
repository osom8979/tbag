/**
 * @file   tester.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/libtbag.h>
#include <libtbag/string/Commander.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/sample/Echo.hpp>
#include <libtbag/network/sample/Chat.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

static char const * const DEFAULT_GTEST_FILTER = "-NetworkTest.*";

static char const * const CMD_ARGUMENT_ECHO_SERVER = "es"; ///< Echo Server.
static char const * const CMD_ARGUMENT_ECHO_CLIENT = "ec"; ///< Echo Client.
static char const * const CMD_ARGUMENT_CHAT_SERVER = "cs"; ///< Chatting Server.
static char const * const CMD_ARGUMENT_CHAT_CLIENT = "cc"; ///< Chatting Client.

static char const * const DEFAULT_NETWORK_TEST_SERVER_IP = "0.0.0.0";
static char const * const DEFAULT_NETWORK_TEST_CLIENT_IP = "127.0.0.1";
static  int const         DEFAULT_NETWORK_TEST_PORT      = 20000;

enum class TestMode
{
    UNKNOWN_ERROR = 0,
    TEST,
    ECHO_SERVER,
    ECHO_CLIENT,
    CHAT_SERVER,
    CHAT_CLIENT,
};

bool checkNetworkArgument(libtbag::string::Arguments const & args, std::string & ip, int & port)
{
    if (args.optString(0, &ip)) {
        if (args.optInteger(1, &port) == false) {
            port = DEFAULT_NETWORK_TEST_PORT;
        }
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    tbInitialize();
    libtbag::signal::registerDefaultStdTerminateHandler();
    libtbag::signal::registerDefaultHandler();

    TestMode mode = TestMode::TEST;
    std::string ip;
    int port;

    {   // COMMAND-LINE PARSING.
        libtbag::string::Commander cmd;
        cmd.insert(CMD_ARGUMENT_ECHO_SERVER, [&](libtbag::string::Arguments const & args){
            if (checkNetworkArgument(args, ip, port) == false) {
                ip   = DEFAULT_NETWORK_TEST_SERVER_IP;
                port = DEFAULT_NETWORK_TEST_PORT;
            }
            mode = TestMode::ECHO_SERVER;
        });
        cmd.insert(CMD_ARGUMENT_ECHO_CLIENT, [&](libtbag::string::Arguments const & args){
            if (checkNetworkArgument(args, ip, port) == false) {
                ip   = DEFAULT_NETWORK_TEST_CLIENT_IP;
                port = DEFAULT_NETWORK_TEST_PORT;
            }
            mode = TestMode::ECHO_CLIENT;
        });
        cmd.insert(CMD_ARGUMENT_CHAT_SERVER, [&](libtbag::string::Arguments const & args){
            if (checkNetworkArgument(args, ip, port) == false) {
                ip   = DEFAULT_NETWORK_TEST_SERVER_IP;
                port = DEFAULT_NETWORK_TEST_PORT;
            }
            mode = TestMode::CHAT_SERVER;
        });
        cmd.insert(CMD_ARGUMENT_CHAT_CLIENT, [&](libtbag::string::Arguments const & args){
            if (checkNetworkArgument(args, ip, port) == false) {
                ip   = DEFAULT_NETWORK_TEST_CLIENT_IP;
                port = DEFAULT_NETWORK_TEST_PORT;
            }
            mode = TestMode::CHAT_CLIENT;
        });
        cmd.request(argc, argv);
    }

    switch (mode) {
    case TestMode::TEST:
        testing::GTEST_FLAG(filter) = DEFAULT_GTEST_FILTER;
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

    case TestMode::ECHO_SERVER: return libtbag::network::sample::runEchoServer(ip, port);
    case TestMode::ECHO_CLIENT: return libtbag::network::sample::runEchoClient(ip, port);
    case TestMode::CHAT_SERVER: return libtbag::network::sample::runChatServer(ip, port);
    case TestMode::CHAT_CLIENT: return libtbag::network::sample::runChatClient(ip, port);
    default: break;
    }

    std::cout << "Argument error.\n";
    return EXIT_FAILURE;
}

