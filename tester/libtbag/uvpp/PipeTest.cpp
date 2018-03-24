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

#include <cstring>
#include <cassert>

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <memory>

using namespace libtbag;
using namespace libtbag::uvpp;

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

        if (startRead() == Err::E_SUCCESS) {
            if (write(write_req, PIPE_WRITE_TEST_MSG, strlen(PIPE_WRITE_TEST_MSG)) == Err::E_SUCCESS) {
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

        if (code == Err::E_SUCCESS) {
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

            if (code == Err::E_SUCCESS) {
                if (std::string(buffer, buffer + size) == std::string(PIPE_WRITE_TEST_MSG)) {
                    if (write(write_req, buffer, size) == Err::E_SUCCESS) {
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
        if (accept(*client.get()) == Err::E_SUCCESS) {
            if (client->startRead() == Err::E_SUCCESS) {
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
    char const * const PATH = "\\\\.\\pipe\\PIPE_TEST";
#else
    char const * const TEST_FILENAME = "echo.sock";
    tttDir(true, true);
    auto const PATH = tttDir_Get() / TEST_FILENAME;
#endif

    Loop server;
    auto server_pipe = server.newHandle<PipeServerTest>(server);
    ASSERT_EQ(Err::E_SUCCESS, server_pipe->bind(PATH));
    ASSERT_EQ(Err::E_SUCCESS, server_pipe->listen());

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

