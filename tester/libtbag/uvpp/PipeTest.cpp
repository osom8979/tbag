/**
 * @file   PipeTest.cpp
 * @brief  Pipe class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/func/FunctionalPipe.hpp>

#include <cstring>
#include <cassert>

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <memory>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::func;

TBAG_CONSTEXPR char const * const PIPE_WRITE_TEST_MSG = "TEST";

struct PipeClientTest : public Pipe
{
    std::vector<char> buffer;
    ConnectRequest connect_req;
    WriteRequest write_req;

    int step1 = 0;
    int step2 = 0;
    int step3 = 0;
    int step4 = 0;

    PipeClientTest(Loop & loop) : Pipe(loop)
    { /* EMPTY. */ }

    void connect(char const * name)
    {
        Pipe::connect(connect_req, name);
    }

    virtual void onConnect(ConnectRequest & request, Err code) override
    {
        assert(step1 == 0);
        assert(step2 == 0);
        assert(step3 == 0);
        assert(step4 == 0);

        if (startRead() == E_SUCCESS) {
            if (write(write_req, PIPE_WRITE_TEST_MSG, strlen(PIPE_WRITE_TEST_MSG)) == E_SUCCESS) {
                ++step1;
            }
        }
    }

    virtual void onWrite(WriteRequest & request, Err code) override
    {
        assert(step1 == 1);
        assert(step2 == 0);
        assert(step3 == 0);
        assert(step4 == 0);
        ++step2;
    }

    virtual binf onAlloc(std::size_t suggested_size) override
    {
        return defaultOnAlloc(buffer, suggested_size);
    }

    virtual void onRead(Err code, char const * buffer, std::size_t size) override
    {
        assert(step1 == 1);
        assert(step2 == 1);
        assert(step3 == 0);
        assert(step4 == 0);

        if (code == E_SUCCESS) {
            std::string const ECHO_MESSAGE(buffer, buffer + size);
            if (ECHO_MESSAGE == std::string(PIPE_WRITE_TEST_MSG)) {
                ++step3;
            }
        }

        close();
    }

    virtual void onClose() override
    {
        assert(step1 == 1);
        assert(step2 == 1);
        assert(step3 == 1);
        assert(step4 == 0);
        ++step4;
    }
};

struct PipeServerTest : public Pipe
{
    struct Client : public Pipe
    {
        std::vector<char> buffer;
        WriteRequest write_req;

        PipeServerTest & server;

        Client(Loop & loop, PipeServerTest & s) : Pipe(loop), server(s)
        { /* EMPTY. */ }

        virtual binf onAlloc(std::size_t suggested_size) override
        {
            return defaultOnAlloc(buffer, suggested_size);
        }

        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        {
            assert(server.step1 == 1);
            assert(server.step2 == 0);
            assert(server.step3 == 0);
            assert(server.step4 == 0);
            assert(server.step5 == 0);

            if (code == E_SUCCESS) {
                if (std::string(buffer, buffer + size) == std::string(PIPE_WRITE_TEST_MSG)) {
                    if (write(write_req, buffer, size) == E_SUCCESS) {
                        ++server.step2;
                    }
                }
            }
        }

        virtual void onWrite(WriteRequest & request, Err code) override
        {
            assert(server.step1 == 1);
            assert(server.step2 == 1);
            assert(server.step3 == 0);
            assert(server.step4 == 0);
            assert(server.step5 == 0);
            ++server.step3;

            close();
        }

        virtual void onClose() override
        {
            assert(server.step1 == 1);
            assert(server.step2 == 1);
            assert(server.step3 == 1);
            assert(server.step4 == 0);
            assert(server.step5 == 0);
            ++server.step4;

            server.close();
        }
    };

    using SharedClient = std::shared_ptr<Client>;
    using ClientVector = std::vector<SharedClient>;

    ClientVector clients;

    int step1 = 0;
    int step2 = 0;
    int step3 = 0;
    int step4 = 0;
    int step5 = 0;

    PipeServerTest(Loop & loop) : Pipe(loop)
    { /* EMPTY. */ }

    virtual void onConnection(Err code) override
    {
        assert(step1 == 0);
        assert(step2 == 0);
        assert(step3 == 0);
        assert(step4 == 0);
        assert(step5 == 0);

        auto client = getLoop()->newHandle<Client>(*getLoop(), *this);
        if (accept(*client.get()) == E_SUCCESS) {
            if (client->startRead() == E_SUCCESS) {
                ++step1;
            }
        }
    }

    virtual void onClose() override
    {
        assert(step1 == 1);
        assert(step2 == 1);
        assert(step3 == 1);
        assert(step4 == 1);
        assert(step5 == 0);
        ++step5;
    }
};

TEST(PipeTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    char const * PATH = "\\\\.\\pipe\\PIPE_TEST";
#else
    char const * const TEST_FILENAME = "echo.sock";
    tttDir(true, true);
    auto const TEST_FILE_PATH = tttDir_Get() / TEST_FILENAME;
    char const * PATH = TEST_FILE_PATH.c_str();
#endif

    Loop server;
    auto server_pipe = server.newHandle<PipeServerTest>(server);
    ASSERT_EQ(E_SUCCESS, server_pipe->bind(PATH));
    ASSERT_EQ(E_SUCCESS, server_pipe->listen());

    std::cout << "Sock name: " << server_pipe->getSockName() << std::endl;

    std::thread thread1 = std::thread([&](){
        server.run();
    });

    Loop client;
    auto client_pipe = client.newHandle<PipeClientTest>(client);
    client_pipe->connect(PATH);

    std::thread thread2 = std::thread([&](){
        client.run();
    });

    thread1.join();
    thread2.join();

    ASSERT_EQ(1, client_pipe->step1);
    ASSERT_EQ(1, client_pipe->step2);
    ASSERT_EQ(1, client_pipe->step3);
    ASSERT_EQ(1, client_pipe->step4);

    ASSERT_EQ(1, server_pipe->step1);
    ASSERT_EQ(1, server_pipe->step2);
    ASSERT_EQ(1, server_pipe->step3);
    ASSERT_EQ(1, server_pipe->step4);
    ASSERT_EQ(1, server_pipe->step5);
}

TEST(PipeTest, ClientShutdown)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    auto const TEST_FILE_PATH = std::string(R"(\\.\pipe\)") + test_info_->test_case_name() + "_" + test_info_->name();
    char const * PATH = TEST_FILE_PATH.c_str();
#else
    tttDir(true, true);
    auto const TEST_FILE_PATH = tttDir_Get() / "pipe_shutdown.sock";
    char const * PATH = TEST_FILE_PATH.c_str();
#endif

    Loop loop;
    ConnectRequest  connect_req;
    ShutdownRequest shutdown_req;

    int   connection_count = 0;
    int      connect_count = 0;
    int     shutdown_count = 0;
    int client_close_count = 0;
    int    node_read_count = 0;
    int   node_close_count = 0;
    int server_close_count = 0;

    auto server_pipe = loop.newHandle<FuncPipe>(loop);
    auto client_pipe = loop.newHandle<FuncPipe>(loop);
    auto node_pipe   = loop.newHandle<FuncPipe>(loop);

    server_pipe->connection_cb = [&](Err code){
        ++connection_count;
        auto const CODE = server_pipe->accept(*(node_pipe.get()));
        assert(CODE == E_SUCCESS);
        auto const START_CODE = node_pipe->startRead();
        assert(START_CODE == E_SUCCESS);
        std::cout << "01. Connection" << std::endl;
    };
    client_pipe->connect_cb = [&](ConnectRequest & req, Err code){
        ++connect_count;
        auto const CODE = client_pipe->shutdown(shutdown_req);
        assert(CODE == E_SUCCESS);
        std::cout << "02. Connect" << std::endl;
    };
    client_pipe->shutdown_cb = [&](ShutdownRequest & req, Err code){
        ++shutdown_count;
        client_pipe->close();
        std::cout << "03. Shutdown" << std::endl;
    };
    client_pipe->close_cb = [&](){
        ++client_close_count;
        std::cout << "04. Close (Client)" << std::endl;
    };
    std::vector<char> node_buffer;
    node_pipe->alloc_cb = [&](std::size_t suggested_size) -> binf{
        std::cout << "05. Alloc (Node)" << std::endl;
        return libtbag::uvpp::defaultOnAlloc(node_buffer, suggested_size);
    };
    node_pipe->read_cb = [&](Err code, char const * data, std::size_t size){
        ++node_read_count;
        assert(code == E_EOF);
        node_pipe->close();
        std::cout << "06. Read EOF (Node)" << std::endl;
    };
    node_pipe->close_cb = [&](){
        ++node_close_count;
        server_pipe->close();
        std::cout << "07. Close (Node)" << std::endl;
    };
    server_pipe->close_cb = [&](){
        ++server_close_count;
        std::cout << "08. Close (Server)" << std::endl;
    };

    ASSERT_EQ(E_SUCCESS, server_pipe->bind(PATH));
    ASSERT_EQ(E_SUCCESS, server_pipe->listen());
    client_pipe->connect(connect_req, PATH);
    ASSERT_EQ(E_SUCCESS, loop.run());

    ASSERT_EQ(1,   connection_count);
    ASSERT_EQ(1,      connect_count);
    ASSERT_EQ(1,     shutdown_count);
    ASSERT_EQ(1, client_close_count);
    ASSERT_EQ(1,    node_read_count);
    ASSERT_EQ(1,   node_close_count);
    ASSERT_EQ(1, server_close_count);
}

