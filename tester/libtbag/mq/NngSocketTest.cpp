/**
 * @file   NngSocketTest.cpp
 * @brief  NngSocket class tester.
 * @author zer0
 * @date   2019-12-26
 */

#include <gtest/gtest.h>
#include <libtbag/mq/NngSocket.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/lock/UvLock.hpp>

#include <string>
#include <vector>
#include <future>
#include <iostream>

using namespace libtbag;
using namespace libtbag::mq;

TEST(NngSocketTest, ClientRecvTimeout_After)
{
    std::string const send_data = "test";
    auto const buffer_size = 1024;
    auto const server_wait_ms = 10;
    auto const client_recv_timeout_ms = 1;

    NngSocket sock_server;
    NngSocket sock_client;

    std::vector<char> server_buffer(buffer_size);
    std::vector<char> client_buffer(buffer_size);

    ASSERT_EQ(E_SUCCESS, sock_server.open(NngSocket::SocketType::ST_REP0));
    ASSERT_EQ(E_SUCCESS, sock_client.open(NngSocket::SocketType::ST_REQ0));

    auto const socket_url = std::string("inproc://") + test_info_->test_case_name() + test_info_->name();
    ASSERT_EQ(E_SUCCESS, sock_server.listen(socket_url));
    ASSERT_EQ(E_SUCCESS, sock_client.dial(socket_url));

    ASSERT_EQ(E_SUCCESS, sock_server.setSendTimeout(NngSocket::DURATION_INFINITE));
    ASSERT_EQ(E_SUCCESS, sock_client.setSendTimeout(NngSocket::DURATION_INFINITE));

    ASSERT_EQ(E_SUCCESS, sock_server.setRecvTimeout(NngSocket::DURATION_INFINITE));
    ASSERT_EQ(E_SUCCESS, sock_client.setRecvTimeout(client_recv_timeout_ms));

    int server_step = 0;
    int client_step = 0;

    using namespace libtbag::lock;
    UvLock output_lock;
    auto printer = [&](int step, char const * prefix){
        UvLockGuard<UvLock> const g(output_lock);
        std::cout << prefix << " (step=" << step << ")" << std::endl;
    };

    UvLock signal_lock;
    UvCondition signal;
    bool server_wait_end = false;

    auto server_thread = std::async(std::launch::async, [&]() -> Err {
        printer(server_step++, "Server recv");
        std::size_t recv_size = server_buffer.size();
        auto const recv_code = sock_server.recv(server_buffer.data(), &recv_size);
        if (isFailure(recv_code)) {
            return recv_code;
        }

        printer(server_step++, "Server wait");
        signal_lock.lock();
        while (!server_wait_end) {
            signal.wait(signal_lock);
        }
        signal_lock.unlock();

        printer(server_step++, "Server send");
        auto const send_code = sock_server.send((void*)send_data.data(), send_data.size());
        if (isFailure(send_code)) {
            return send_code;
        }

        printer(server_step++, "Server done");
        return E_SUCCESS;
    });

    auto client_thread = std::async(std::launch::async, [&]() -> Err {
        printer(client_step++, "Client send");
        auto const send_code = sock_client.send((void*)send_data.data(), send_data.size());
        if (isFailure(send_code)) {
            return send_code;
        }

        printer(client_step++, "Client recv1");
        std::size_t recv_size = client_buffer.size();
        auto const recv_code = sock_client.recv(client_buffer.data(), &recv_size);
        if (recv_code != E_ETIMEDOUT) {
            return recv_code;
        }

        printer(client_step++, "Client update recv timeout inf");
        auto const opt_code = sock_client.setRecvTimeout(NngSocket::DURATION_INFINITE);
        if (isFailure(opt_code)) {
            return opt_code;
        }

        signal_lock.lock();
        server_wait_end = true;
        signal.signal();
        signal_lock.unlock();

        printer(client_step++, "Client recv2");
        auto const recv_code2 = sock_client.recv(client_buffer.data(), &recv_size);
        if (isFailure(recv_code2)) {
            return recv_code2;
        }

        printer(client_step++, "Client done");
        return E_SUCCESS;
    });

    ASSERT_EQ(E_SUCCESS, server_thread.get());
    ASSERT_EQ(E_ILLSTATE, client_thread.get());

    ASSERT_EQ(4, server_step);
    ASSERT_EQ(4, client_step);

    ASSERT_EQ(E_SUCCESS, sock_server.close());
    ASSERT_EQ(E_SUCCESS, sock_client.close());
}

