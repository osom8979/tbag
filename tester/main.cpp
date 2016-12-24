/**
 * @file   main.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/libtbag.h>
#include <libtbag/string/Commander.hpp>
#include <libtbag/filesystem/Path.hpp>
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

enum class TestMode
{
    UNKNOWN_ERROR = 0,
    TEST,
    ECHO_SERVER,
    ECHO_CLIENT,
    CHAT_SERVER,
    CHAT_CLIENT,
};

int main(int argc, char **argv)
{
    tbInitialize();

    TestMode mode = TestMode::TEST;
    std::string ip;
    int port;

    {   // COMMAND-LINE PARSING.
        using Cmd  = libtbag::string::Commander;
        using Path = libtbag::filesystem::Path;

        auto const CMD_ARGUMENT_ECHO_SERVER_LAMBDA = [&](libtbag::string::Arguments const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::ECHO_SERVER;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        };
        auto const CMD_ARGUMENT_ECHO_CLIENT_LAMBDA = [&](libtbag::string::Arguments const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::ECHO_CLIENT;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        };
        auto const CMD_ARGUMENT_CHAT_SERVER_LAMBDA = [&](libtbag::string::Arguments const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::CHAT_SERVER;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        };
        auto const CMD_ARGUMENT_CHAT_CLIENT_LAMBDA = [&](libtbag::string::Arguments const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::CHAT_CLIENT;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        };

        Cmd cmd;
        cmd.insert(CMD_ARGUMENT_ECHO_SERVER, CMD_ARGUMENT_ECHO_SERVER_LAMBDA);
        cmd.insert(CMD_ARGUMENT_ECHO_CLIENT, CMD_ARGUMENT_ECHO_CLIENT_LAMBDA);
        cmd.insert(CMD_ARGUMENT_CHAT_SERVER, CMD_ARGUMENT_CHAT_SERVER_LAMBDA);
        cmd.insert(CMD_ARGUMENT_CHAT_CLIENT, CMD_ARGUMENT_CHAT_CLIENT_LAMBDA);
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

