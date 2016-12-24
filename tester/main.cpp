/**
 * @file   main.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/libtbag.h>
#include <libtbag/string/Commander.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/sample/Echo.hpp>
#include <libtbag/network/sample/Chat.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

static char const * const CMD_ARGUMENT_ECHO_SERVER = "echo_server";
static char const * const CMD_ARGUMENT_ECHO_CLIENT = "echo_client";
static char const * const CMD_ARGUMENT_CHAT_SERVER = "chat_server";
static char const * const CMD_ARGUMENT_CHAT_CLIENT = "chat_client";

static int const DEFAULT_NETWORK_TEST_PORT = 98765;

enum class TestMode
{
    UNKNOWN_ERROR = 0,
    TEST,
    ECHO_SERVER,
    ECHO_CLIENT,
    CHAT_SERVER,
    CHAT_CLIENT,
};

TestMode checkNetworkArgument(libtbag::string::Arguments const & args, TestMode mode, std::string & ip, int & port)
{
    if (args.optString(0, &ip)) {
        if (args.optInteger(1, &port) == false) {
            port = DEFAULT_NETWORK_TEST_PORT;
        }
        return mode;
    }
    return TestMode::UNKNOWN_ERROR;
}

int main(int argc, char **argv)
{
    tbInitialize();

    TestMode mode = TestMode::TEST;
    std::string ip;
    int port;

    {   // COMMAND-LINE PARSING.
        libtbag::string::Commander cmd;
        cmd.insert(CMD_ARGUMENT_ECHO_SERVER, [&](libtbag::string::Arguments const & args){
            mode = checkNetworkArgument(args, TestMode::ECHO_SERVER, ip, port);
        });
        cmd.insert(CMD_ARGUMENT_ECHO_CLIENT, [&](libtbag::string::Arguments const & args){
            mode = checkNetworkArgument(args, TestMode::ECHO_CLIENT, ip, port);
        });
        cmd.insert(CMD_ARGUMENT_CHAT_SERVER, [&](libtbag::string::Arguments const & args){
            mode = checkNetworkArgument(args, TestMode::CHAT_SERVER, ip, port);
        });
        cmd.insert(CMD_ARGUMENT_CHAT_CLIENT, [&](libtbag::string::Arguments const & args){
            mode = checkNetworkArgument(args, TestMode::CHAT_CLIENT, ip, port);
        });
        cmd.request(argc, argv);
    }

    switch (mode) {
    case TestMode::TEST:
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

