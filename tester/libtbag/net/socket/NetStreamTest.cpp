/**
 * @file   NetStreamTest.cpp
 * @brief  NetStream class tester.
 * @author zer0
 * @date   2018-12-09
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/net/socket/NetStreamClient.hpp>
#include <libtbag/net/socket/NetStreamServer.hpp>
#include <libtbag/log/Log.hpp>

#include <atomic>
#include <memory>

using namespace libtbag;
using namespace libtbag::net;
using namespace libtbag::net::socket;

TEST(NetStreamTest, DefaultEcho)
{
    libtbag::log::SeverityGuard guard;

    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true&wait_closing=1";

    std::atomic_int server_begin (0);
    std::atomic_int server_end   (0);
    std::atomic_int server_accept(0);
    std::atomic_int server_recv  (0);
    std::atomic_int server_close (0);

    std::atomic_int client_begin (0);
    std::atomic_int client_recv  (0);
    std::atomic_int client_close (0);

    std::string const TEST_VAL = "TEST";

    using MqMsg = libtbag::mq::details::MqMsg;
    MqMsg msg_send(TEST_VAL);
    MqMsg msg_recv;
    MqMsg msg_echo;

    Err echo_send = Err::E_UNKNOWN;

    std::unique_ptr<NetStreamServer> server;
    NetStreamServer::Callbacks server_callbacks;
    server_callbacks.begin_cb = [&](){
        ++server_begin;
    };
    server_callbacks.end_cb = [&](){
        ++server_end;
    };
    server_callbacks.accept_cb = [&](std::intptr_t id, std::string const &) -> bool {
        ++server_accept;
        return true;
    };
    server_callbacks.recv_cb = [&](std::intptr_t id, char const * data, std::size_t size){
        msg_recv.stream = id;
        msg_recv.buffer.assign(data, data + size);
        echo_send = server->send(msg_recv);
        ++server_recv;
    };
    server_callbacks.close_cb = [&](std::intptr_t id){
        ++server_close;
    };
    server = std::make_unique<NetStreamServer>(PIPE_PATH, server_callbacks);

    std::unique_ptr<NetStreamClient> client;
    NetStreamClient::Callbacks client_callbacks;
    client_callbacks.begin_cb = [&](){
        ++client_begin;
    };
    client_callbacks.recv_cb = [&](char const * data, std::size_t size){
        msg_echo.stream = 0;
        msg_echo.buffer.assign(data, data + size);
        ++client_recv;
    };
    client_callbacks.end_cb = [&](){
        ++client_close;
    };
    client = std::make_unique<NetStreamClient>(PIPE_PATH, client_callbacks);

    ASSERT_EQ(Err::E_SUCCESS, client->send(msg_send));
    while (client_recv == 0) {
        // Busy wait...
    }

    ASSERT_EQ(Err::E_SUCCESS, echo_send);
    ASSERT_EQ(TEST_VAL, std::string(msg_send.begin(), msg_send.end()));
    ASSERT_EQ(TEST_VAL, std::string(msg_recv.begin(), msg_recv.end()));
    ASSERT_EQ(TEST_VAL, std::string(msg_echo.begin(), msg_echo.end()));

    client.reset();
    server.reset();

    ASSERT_EQ(1, server_begin );
    ASSERT_EQ(1, server_end   );
    ASSERT_EQ(1, server_accept);
    ASSERT_EQ(1, server_recv  );
    ASSERT_EQ(1, server_close );

    ASSERT_EQ(1, client_begin );
    ASSERT_EQ(1, client_recv  );
    ASSERT_EQ(1, client_close );
}

