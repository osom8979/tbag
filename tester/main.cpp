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

#include <tester/main/server.hpp>
#include <tester/main/client.hpp>

#include <iostream>
#include <string>

static char const * const CMD_ARGUMENT_SERVER = "server";
static char const * const CMD_ARGUMENT_CLIENT = "client";

enum class TestMode
{
    UNKNOWN_ERROR = 0,
    TEST,
    SERVER,
    CLIENT
};

int main(int argc, char **argv)
{
    tbInitialize();

    using Commander = libtbag::string::Commander;
    using Args = libtbag::string::Arguments;
    using Path = libtbag::filesystem::Path;

    TestMode mode = TestMode::TEST;
    std::string ip;
    int port;

    {   // COMMAND-LINE PARSING.
        Commander cmd;
        cmd.insert(CMD_ARGUMENT_SERVER, [&](Args const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::SERVER;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        });
        cmd.insert(CMD_ARGUMENT_CLIENT, [&](Args const & args){
            if (args.optString(0, &ip) && args.optInteger(1, &port)) {
                mode = TestMode::CLIENT;
            } else {
                mode = TestMode::UNKNOWN_ERROR;
            }
        });
        cmd.request(argc, argv);
    }

    if (mode == TestMode::TEST) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else if (mode == TestMode::SERVER) {
        return libtbag::main_server(argc, argv);
    } else if (mode == TestMode::CLIENT) {
        return libtbag::main_client(argc, argv);
    }

    std::cout << "Argument error.\n";
    return 1; // ERROR!
}

