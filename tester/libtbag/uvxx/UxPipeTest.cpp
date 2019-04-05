/**
 * @file   UxPipeTest.cpp
 * @brief  UxPipe class tester.
 * @author zer0
 * @date   2018-12-07
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxPipe.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxPipeTest, Default)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = tttDir_Pipe_Get();

    UxPipe server_pipe;
    ASSERT_FALSE(server_pipe.isInit());

    UxPipe client_pipe;
    ASSERT_FALSE(client_pipe.isInit());

    UxPipe node_pipe;
    ASSERT_FALSE(node_pipe.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    UxPipe::ConnectRequest  connect_req;
    UxPipe::ShutdownRequest shutdown_req;

    int   connection_count = 0;
    int      connect_count = 0;
    int     shutdown_count = 0;
    int client_close_count = 0;
    int    node_read_count = 0;
    int   node_close_count = 0;
    int server_close_count = 0;

    ASSERT_EQ(E_SUCCESS, server_pipe.init(loop));
    ASSERT_TRUE(server_pipe.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, client_pipe.init(loop));
    ASSERT_TRUE(client_pipe.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    ASSERT_EQ(E_SUCCESS, node_pipe.init(loop));
    ASSERT_TRUE(node_pipe.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(3, loop.size());

    server_pipe.setOnConnection([&](Err code){
        ++connection_count;
        auto const CODE = server_pipe.accept(node_pipe);
        assert(CODE == E_SUCCESS);
        auto const START_CODE = node_pipe.startRead();
        assert(START_CODE == E_SUCCESS);
        std::cout << "01. Connection" << std::endl;
    });
    server_pipe.setOnClose([&](){
        ++server_close_count;
        std::cout << "08. Close (Server)" << std::endl;
    });

    client_pipe.setOnConnect([&](UxPipe::ConnectRequest & req, Err code){
        ++connect_count;
        auto const CODE = client_pipe.shutdown(shutdown_req);
        assert(CODE == E_SUCCESS);
        std::cout << "02. Connect" << std::endl;
    });
    client_pipe.setOnShutdown([&](UxPipe::ShutdownRequest & req, Err code){
        ++shutdown_count;
        client_pipe.close();
        std::cout << "03. Shutdown" << std::endl;
    });
    client_pipe.setOnClose([&](){
        ++client_close_count;
        std::cout << "04. Close (Client)" << std::endl;
    });

    std::vector<char> node_buffer;
    node_pipe.setOnAlloc([&](std::size_t suggested_size) -> UxPipe::binf {
        std::cout << "05. Alloc (Node)" << std::endl;
        return libtbag::uvpp::defaultOnAlloc(node_buffer, suggested_size);
    });
    node_pipe.setOnRead([&](Err code, char const * data, std::size_t size){
        ++node_read_count;
        assert(code == E_EOF);
        node_pipe.close();
        std::cout << "06. Read EOF (Node)" << std::endl;
    });
    node_pipe.setOnClose([&](){
        ++node_close_count;
        server_pipe.close();
        std::cout << "07. Close (Node)" << std::endl;
    });

    ASSERT_EQ(E_SUCCESS, server_pipe.bind(PIPE_PATH));
    ASSERT_EQ(E_SUCCESS, server_pipe.listen());
    client_pipe.connect(connect_req, PIPE_PATH);

    ASSERT_EQ(E_SUCCESS, loop.run());
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(1,   connection_count);
    ASSERT_EQ(1,      connect_count);
    ASSERT_EQ(1,     shutdown_count);
    ASSERT_EQ(1, client_close_count);
    ASSERT_EQ(1,    node_read_count);
    ASSERT_EQ(1,   node_close_count);
    ASSERT_EQ(1, server_close_count);
}

