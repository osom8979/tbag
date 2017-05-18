/**
 * @file   NetworkPipeTest.cpp
 * @brief  Pipe network tester.
 * @author zer0
 * @date   2017-05-09
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/pipe/PipeServer.hpp>
#include <libtbag/network/pipe/PipeClient.hpp>

#include <thread>
#include <memory>
#include <vector>
#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;
using namespace libtbag::network::pipe;

TEST(NetworkPipeTest, MultiEcho)
{
    tttDir(true, true);
    auto path = tttDirGet() / "temp.pipe";

    log::SeverityGuard guard;

    using namespace uvpp;

    std::size_t const CLIENT_SIZE  = 100;
    std::string const ECHO_MESSAGE = "ECHO MESSAGE";

    // ---------------
    // SERVER PROCESS.
    // ---------------

    Loop loop_server;
    FunctionalPipeServer server(loop_server);

    int server_connection   = 0;
    int server_client_read  = 0;
    int server_client_write = 0;
    int server_client_close = 0;
    int server_close        = 0;
    uerr server_result = uerr::UVPP_UNKNOWN;

    server.setOnConnection([&](uerr code){
        if (auto shared = server.accept().lock()) {
            if (shared->start()) {
                server_connection++;
            }
        }
    });
    server.setOnClientRead([&](FunctionalPipeServer::WeakClient node, uerr code,
                               char const * buffer, FunctionalPipeServer::Size size){
        if (code == uerr::UVPP_SUCCESS) {
            if (auto shared = node.lock()) {
                if (shared->stop()) {
                    server_client_read++;
                    shared->write(buffer, size);
                }
            }
        }
    });
    server.setOnClientWrite([&](FunctionalPipeServer::WeakClient node, uerr code){
        if (code == uerr::UVPP_SUCCESS) {
            if (auto shared = node.lock()) {
                server_client_write++;
                shared->close();
            }
        }
    });
    server.setOnClientClose([&](FunctionalPipeServer::WeakClient node){
        if (auto shared = node.lock()) {
            server_client_close++;
        }
        if (server_client_close >= CLIENT_SIZE) {
            server.close();
        }
    });
    server.setOnServerClose([&](){
        server_close++;
    });
    server.init(path, 0);

    std::thread thread_server([&](){
        server_result = loop_server.run();
    });

    // ---------------
    // CLIENT PROCESS.
    // ---------------

    using SharedLoop = std::shared_ptr<Loop>;
    using LoopVector = std::vector<SharedLoop>;

    using SharedFuncClient = std::shared_ptr<FunctionalPipeClient>;
    using ClientVector = std::vector<SharedFuncClient>;

    using ThreadVector = std::vector<std::thread>;

    std::vector<uerr> connect_result(CLIENT_SIZE, uerr::UVPP_UNKNOWN);
    std::vector<uerr>   write_result(CLIENT_SIZE, uerr::UVPP_UNKNOWN);
    std::vector<uerr>    read_result(CLIENT_SIZE, uerr::UVPP_UNKNOWN);
    std::vector<uerr>   close_result(CLIENT_SIZE, uerr::UVPP_UNKNOWN);
    std::vector<uerr>    loop_result(CLIENT_SIZE, uerr::UVPP_UNKNOWN);

    ThreadVector cthreads(CLIENT_SIZE);
    LoopVector     cloops(CLIENT_SIZE);
    ClientVector  clients(CLIENT_SIZE);

    std::size_t i = 0;

    for (i = 0; i < CLIENT_SIZE; ++i) {
        cloops.at(i).reset(new Loop());
        clients.at(i).reset(new FunctionalPipeClient(*(cloops.at(i))));
        clients.at(i)->setOnConnect([&, i](uerr code){
            if (clients.at(i)->write(ECHO_MESSAGE.data(), ECHO_MESSAGE.size())) {
                connect_result.at(i) = code;
            }
        });
        clients.at(i)->setOnWrite([&, i](uerr code){
            if (clients.at(i)->start()) {
                write_result.at(i) = code;
            }
        });
        clients.at(i)->setOnRead([&, i](uerr code, char const * buffer, PipeClient::Size size){
            if (clients.at(i)->stop()) {
                read_result.at(i) = code;
                clients.at(i)->close();
            }
        });
        clients.at(i)->setOnClose([&, i](){
            close_result.at(i) = uerr::UVPP_SUCCESS;
        });
        clients.at(i)->init(path);

        cthreads.at(i) = std::thread([&, i](){
            loop_result.at(i) = cloops.at(i)->run();
        });
    }

    for (i = 0; i < CLIENT_SIZE; ++i) {
        cthreads.at(i).join();
    }
    thread_server.join();

    for (i = 0; i < CLIENT_SIZE; ++i) {
        ASSERT_EQ(uerr::UVPP_SUCCESS, connect_result.at(i));
        ASSERT_EQ(uerr::UVPP_SUCCESS,   write_result.at(i));
        ASSERT_EQ(uerr::UVPP_SUCCESS,    read_result.at(i));
        ASSERT_EQ(uerr::UVPP_SUCCESS,   close_result.at(i));
        ASSERT_EQ(uerr::UVPP_SUCCESS,    loop_result.at(i));
    }

    ASSERT_EQ(uerr::UVPP_SUCCESS, server_result);
    ASSERT_EQ(CLIENT_SIZE, server_connection  );
    ASSERT_EQ(CLIENT_SIZE, server_client_read );
    ASSERT_EQ(CLIENT_SIZE, server_client_write);
    ASSERT_EQ(CLIENT_SIZE, server_client_close);
    ASSERT_EQ(1, server_close);
}