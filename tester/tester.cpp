/**
 * @file   tester.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/libtbag.h>
#include <libtbag/string/Commander.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

static char const * const DEFAULT_GTEST_FILTER = "-NetworkTest.*";

static char const * const DEFAULT_NETWORK_TEST_SERVER_IP = "0.0.0.0";
static char const * const DEFAULT_NETWORK_TEST_CLIENT_IP = "127.0.0.1";
static  int const         DEFAULT_NETWORK_TEST_PORT      = 20000;

enum class TestMode
{
    UNKNOWN_ERROR = 0,
    TEST,
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

    libtbag::DemoAsset asset;
    asset.create_temp_dir();

    TestMode mode = TestMode::TEST;
    std::string ip;
    int port;

    {   // COMMAND-LINE PARSING.
        using namespace libtbag::string;
        Commander cmd;
        cmd.insert("", [&](Arguments const & args){
        });
        cmd.request(argc, argv);
    }

    switch (mode) {
    case TestMode::TEST:
        testing::GTEST_FLAG(filter) = DEFAULT_GTEST_FILTER;
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    default: break;
    }

    std::cout << "Argument error.\n";
    return EXIT_FAILURE;
}

