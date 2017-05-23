/**
 * @file   NetworkTcpTest.cpp
 * @brief  Tcp network tester.
 * @author zer0
 * @date   2017-05-06
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/tcp/TcpServer.hpp>
#include <libtbag/network/tcp/TcpClient.hpp>

#include <thread>
#include <memory>
#include <vector>
#include <iostream>

#define DISABLE_IDLE_SERVER

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;
using namespace libtbag::network::tcp;

TEST(NetworkTcpTest, ClientTimeout)
{
    using namespace uvpp;
    Loop loop;
    FunctionalTcpClient client(loop);

    if (client.init("8.8.8.8", 9999, 50) == false) {
        std::cout << "Network unreachable.\n";
        return;
    }

    int connect = 0;
    int close   = 0;

    Err connect_result = Err::E_UNKNOWN;
    Err result = Err::E_UNKNOWN;

    client.setOnConnect([&](Err code){
        connect_result = code;
        connect++;
    });
    client.setOnClose([&](){
        close++;
    });

    std::thread thread([&](){
        result = loop.run();
    });

    thread.join();

    ASSERT_EQ(Err::E_ECANCELED, connect_result);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(1, connect);
    ASSERT_EQ(1, close);
}

TEST(NetworkTcpTest, MultiEcho)
{
    log::SeverityGuard guard;

    using namespace uvpp;

    std::size_t const CLIENT_SIZE  = 100;
    std::string const ECHO_MESSAGE = "ECHO MESSAGE";

    // ---------------
    // SERVER PROCESS.
    // ---------------

    Loop loop_server;
    FunctionalTcpServer server(loop_server);

    int server_connection    = 0;
    int server_client_read   = 0;
    int server_client_write  = 0;
    int server_client_close  = 0;
    int server_close         = 0;
    int server_udata_alloc   = 0;
    int server_udata_dealloc = 0;
    Err server_result = Err::E_UNKNOWN;

    server.setOnConnection([&](Err code){
        if (auto shared = server.accept().lock()) {
            if (shared->start()) {
                server_connection++;
            }
        }
    });
    server.setOnClientRead([&](FunctionalTcpServer::WeakClient node, Err code,
                               char const * buffer, FunctionalTcpServer::Size size){
        if (code == Err::E_SUCCESS) {
            if (auto shared = node.lock()) {
                if (shared->stop()) {
                    server_client_read++;
                    shared->write(buffer, size);
                }
            }
        }
    });
    server.setOnClientWrite([&](FunctionalTcpServer::WeakClient node, Err code){
        if (code == Err::E_SUCCESS) {
            if (auto shared = node.lock()) {
                server_client_write++;
                shared->close();
            }
        }
    });
    server.setOnClientClose([&](FunctionalTcpServer::WeakClient node){
        if (auto shared = node.lock()) {
            server_client_close++;
        }
        if (server_client_close >= CLIENT_SIZE) {
#if defined(DISABLE_IDLE_SERVER)
            server.close();
#endif
        }
    });
    server.setOnServerClose([&](){
        server_close++;
    });

    server.setOnClientUserDataAlloc([&](FunctionalTcpServer::WeakClient node) -> void *{
        server_udata_alloc++;
        return new int (100);
    });
    server.setOnClientUserDataDealloc([&](FunctionalTcpServer::WeakClient node, void * data){
        server_udata_dealloc++;
        delete static_cast<int*>(data);
    });

    server.init(details::ANY_IPV4, 0);
    int const SERVER_PORT = server.getPort();

    std::cout << "Server port number: " << SERVER_PORT << std::endl;

    std::thread thread_server([&](){
        server_result = loop_server.run();
    });

    // ---------------
    // CLIENT PROCESS.
    // ---------------

    using SharedLoop = std::shared_ptr<Loop>;
    using LoopVector = std::vector<SharedLoop>;

    using SharedFuncClient = std::shared_ptr<FunctionalTcpClient>;
    using ClientVector = std::vector<SharedFuncClient>;

    using ThreadVector = std::vector<std::thread>;

    std::vector<Err> connect_result(CLIENT_SIZE, Err::E_UNKNOWN);
    std::vector<Err>   write_result(CLIENT_SIZE, Err::E_UNKNOWN);
    std::vector<Err>    read_result(CLIENT_SIZE, Err::E_UNKNOWN);
    std::vector<Err>   close_result(CLIENT_SIZE, Err::E_UNKNOWN);
    std::vector<Err>    loop_result(CLIENT_SIZE, Err::E_UNKNOWN);

    ThreadVector cthreads(CLIENT_SIZE);
    LoopVector     cloops(CLIENT_SIZE);
    ClientVector  clients(CLIENT_SIZE);

    std::size_t i = 0;

    for (i = 0; i < CLIENT_SIZE; ++i) {
        cloops.at(i).reset(new Loop());
        clients.at(i).reset(new FunctionalTcpClient(*(cloops.at(i))));
        clients.at(i)->setOnConnect([&, i](Err code){
            if (clients.at(i)->write(ECHO_MESSAGE.data(), ECHO_MESSAGE.size())) {
                connect_result.at(i) = code;
            }
        });
        clients.at(i)->setOnWrite([&, i](Err code){
            if (clients.at(i)->start()) {
                write_result.at(i) = code;
            }
        });
        clients.at(i)->setOnRead([&, i](Err code, char const * buffer, TcpClient::Size size){
            if (clients.at(i)->stop()) {
                read_result.at(i) = code;
                clients.at(i)->close();
            }
        });
        clients.at(i)->setOnClose([&, i](){
            close_result.at(i) = Err::E_SUCCESS;
        });
        clients.at(i)->init(details::LOOPBACK_IPV4, SERVER_PORT);

        cthreads.at(i) = std::thread([&, i](){
            loop_result.at(i) = cloops.at(i)->run();
        });
    }

    for (i = 0; i < CLIENT_SIZE; ++i) {
        cthreads.at(i).join();
    }
    thread_server.join();

    for (i = 0; i < CLIENT_SIZE; ++i) {
        ASSERT_EQ(Err::E_SUCCESS, connect_result.at(i));
        ASSERT_EQ(Err::E_SUCCESS,   write_result.at(i));
        ASSERT_EQ(Err::E_SUCCESS,    read_result.at(i));
        ASSERT_EQ(Err::E_SUCCESS,   close_result.at(i));
        ASSERT_EQ(Err::E_SUCCESS,    loop_result.at(i));
    }

    ASSERT_EQ(Err::E_SUCCESS, server_result);
    ASSERT_EQ(CLIENT_SIZE, server_connection   );
    ASSERT_EQ(CLIENT_SIZE, server_client_read  );
    ASSERT_EQ(CLIENT_SIZE, server_client_write );
    ASSERT_EQ(CLIENT_SIZE, server_client_close );
    ASSERT_EQ(CLIENT_SIZE, server_udata_alloc  );
    ASSERT_EQ(CLIENT_SIZE, server_udata_dealloc);
    ASSERT_EQ(1, server_close);
}

