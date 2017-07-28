/**
 * @file   ActiveCheckerTest.cpp
 * @brief  ActiveChecker class tester.
 * @author zer0
 * @date   2017-07-27
 */

#include <gtest/gtest.h>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/ex/ActiveChecker.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::ex;
using namespace libtbag::network::details;
using namespace libtbag::uvpp::func;

using ActiveCheckerRequest  = ActiveChecker::Request;
using ActiveCheckerResponse = ActiveChecker::Response;

TEST(ActiveCheckerTest, Request)
{
    ActiveCheckerRequest request1;
    request1.version   = util::getTbagPacketVersion();
    request1.recv_ip   = "0.0.0.0";
    request1.recv_port = 1000;

    std::string const JSON_REQUEST = request1.toJsonString();
    ASSERT_FALSE(JSON_REQUEST.empty());

    ActiveCheckerRequest request2;
    ASSERT_TRUE(request2.fromJsonString(JSON_REQUEST));
    ASSERT_EQ(request1.version  , request2.version  );
    ASSERT_EQ(request1.recv_ip  , request2.recv_ip  );
    ASSERT_EQ(request1.recv_port, request2.recv_port);
}

TEST(ActiveCheckerTest, Response)
{
    ActiveCheckerResponse response1;
    response1.version     = util::getTbagPacketVersion();
    response1.server_type = ActiveChecker::EndPointType::EPT_ROASTER;
    response1.server_ip   = "0.0.0.0";
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

TEST(ActiveCheckerTest, Default)
{
    std::string const SERVER_BIND_IP = ANY_IPV4;
    std::string const CLIENT_BIND_IP = ANY_IPV4;
    int const SERVER_BIND_PORT     = 4100;
    int const SERVER_RESPONSE_PORT = 4101;
    int const CLIENT_BIND_PORT     = 4102;

    ActiveCheckerResponse response;
    response.server_type = ActiveChecker::EndPointType::EPT_ROASTER;
    response.server_ip   = LOOPBACK_IPV4;
    response.server_port = SERVER_RESPONSE_PORT;

    ActiveCheckerRequest request;
    request.recv_ip   = LOOPBACK_IPV4;
    request.recv_port = CLIENT_BIND_PORT;

    uvpp::Loop loop;

    int server_close_counter    = 0;
    int client_response_counter = 0;
    int client_timeout_counter  = 0;
    int client_close_counter    = 0;

    FuncActiveChecker::EndPointType result_type;
    std::string result_ip;
    int result_port;

    FuncActiveChecker server;
    server.setOnClose([&](){
        ++server_close_counter;
    });
    ASSERT_TRUE(server.initServer(loop, SERVER_BIND_IP, SERVER_BIND_PORT, response));

    FuncActiveChecker client;
    client.setOnResponse([&](FuncActiveChecker::EndPointType type, std::string const & ip, int port){
        result_type = type;
        result_ip   = ip;
        result_port = port;
        ++client_response_counter;
        server.close();
    });
    client.setOnTimeout([&](){
        ++client_timeout_counter;
        client.close();
    });
    client.setOnClose([&](){
        ++client_close_counter;
        server.close();
    });
    ASSERT_TRUE(client.initClient(loop, CLIENT_BIND_IP, CLIENT_BIND_PORT, 50, request));

    auto idle = loop.newHandle<FuncIdle>(loop);
    idle->setOnIdle([&](){
        client.broadcast(SERVER_BIND_PORT);
        idle->close();
    });
    ASSERT_EQ(Err::E_SUCCESS, idle->start());

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, server_close_counter);
    ASSERT_EQ(1, client_response_counter);
    ASSERT_EQ(1, client_timeout_counter);
    ASSERT_EQ(1, client_close_counter);
    ASSERT_EQ(response.server_type, result_type);
    ASSERT_EQ(response.server_ip  , result_ip);
    ASSERT_EQ(response.server_port, result_port);
}

