/**
 * @file   NetworkActiveCheckerTest.cpp
 * @brief  NetworkActiveChecker class tester.
 * @author zer0
 * @date   2017-07-27
 * @date   2017-07-29 (Rename: ActiveCheckerTest -> NetworkActiveCheckerTest)
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/ex/ActiveChecker.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::ex;
using namespace libtbag::network::details;
using namespace libtbag::uvpp::func;

using ActiveCheckerRequest  = ActiveChecker::Request;
using ActiveCheckerResponse = ActiveChecker::Response;

TEST(NetworkActiveCheckerTest, Request)
{
    ActiveCheckerRequest request1;
    request1.version   = util::getTbagPacketVersion();
    request1.recv_ip   = LOOPBACK_IPV4;
    request1.recv_port = 1000;

    std::string const JSON_REQUEST = request1.toJsonString();
    ASSERT_FALSE(JSON_REQUEST.empty());

    ActiveCheckerRequest request2;
    ASSERT_TRUE(request2.fromJsonString(JSON_REQUEST));
    ASSERT_EQ(request1.version  , request2.version  );
    ASSERT_EQ(request1.recv_ip  , request2.recv_ip  );
    ASSERT_EQ(request1.recv_port, request2.recv_port);
}

TEST(NetworkActiveCheckerTest, Response)
{
    ActiveCheckerResponse response1;
    response1.version     = util::getTbagPacketVersion();
    response1.server_type = ActiveChecker::EndPointType::EPT_ROASTER;
    response1.server_ip   = LOOPBACK_IPV4;
    response1.server_port = 1000;

    std::string const JSON_REQUEST = response1.toJsonString();
    ASSERT_FALSE(JSON_REQUEST.empty());

    ActiveCheckerResponse response2;
    ASSERT_TRUE(response2.fromJsonString(JSON_REQUEST));
    ASSERT_EQ(response1.version    , response2.version    );
    ASSERT_EQ(response1.server_type, response2.server_type);
    ASSERT_EQ(response1.server_ip  , response2.server_ip  );
    ASSERT_EQ(response1.server_port, response2.server_port);
}

TEST(NetworkActiveCheckerTest, ClientInitFailure)
{
    log::SeverityGuard guard;
    uvpp::Loop loop;
    FuncActiveChecker server;
    ASSERT_FALSE(server.initServer(loop, std::string(), 0, ActiveCheckerResponse()));
    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());
}

TEST(NetworkActiveCheckerTest, ServerInitFailure)
{
    log::SeverityGuard guard;
    uvpp::Loop loop;
    FuncActiveChecker client;
    ASSERT_FALSE(client.initClient(loop, std::string(), 0, 1, ActiveCheckerRequest()));
    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());
}

TEST(NetworkActiveCheckerTest, Default)
{
    if (isWindowsPlatform()) {
        std::cout << "Skip this test in Windows Platform.\n";
        return;
    }

    uvpp::Loop loop;
    int server_close_counter    = 0;
    int client_response_counter = 0;
    int client_timeout_counter  = 0;
    int client_close_counter    = 0;
    int idle_counter = 0;

    FuncActiveChecker server;
    server.setOnClose([&](){
        ++server_close_counter;
    });
    ASSERT_TRUE(server.initServer(loop, ANY_IPV4, 0, ActiveCheckerResponse()));
    ActiveCheckerResponse response;
    response.server_type = ActiveChecker::EndPointType::EPT_ROASTER;
    response.server_ip   = LOOPBACK_IPV4;
    response.server_port = server.getRecvPort();
    server.setJsonString(response.toJsonString());
    ASSERT_LT(0, response.server_port);

    std::cout << "Server bind port: " << response.server_port << std::endl;

    FuncActiveChecker::EndPointType result_type;
    std::string result_ip;
    int result_port;

    FuncActiveChecker client;
    client.setOnResponse([&](FuncActiveChecker::EndPointType type, std::string const & ip, int port){
        result_type = type;
        result_ip   = ip;
        result_port = port;
        ++client_response_counter;
        client.close();
    });
    client.setOnTimeout([&](){
        ++client_timeout_counter;
        //client.close();
    });
    client.setOnClose([&](){
        ++client_close_counter;
        server.close();
    });
    ASSERT_TRUE(client.initClient(loop, ANY_IPV4, 0, 100, ActiveCheckerRequest()));
    ActiveCheckerRequest request;
    request.recv_ip   = LOOPBACK_IPV4;
    request.recv_port = client.getRecvPort();
    client.setJsonString(request.toJsonString());
    ASSERT_LT(0, request.recv_port);

    std::cout << "Client bind port: " << request.recv_port << std::endl;

    auto idle = loop.newHandle<FuncIdle>(loop);
    idle->setOnIdle([&](){
        ++idle_counter;
        client.broadcast(response.server_port);
        idle->close();
    });
    ASSERT_EQ(Err::E_SUCCESS, idle->start());

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, server_close_counter);
    ASSERT_EQ(1, client_response_counter);
    ASSERT_EQ(0, client_timeout_counter);
    ASSERT_EQ(1, client_close_counter);
    ASSERT_EQ(1, idle_counter);
    ASSERT_EQ(response.server_type, result_type);
    ASSERT_EQ(response.server_ip  , result_ip);
    ASSERT_EQ(response.server_port, result_port);
}

