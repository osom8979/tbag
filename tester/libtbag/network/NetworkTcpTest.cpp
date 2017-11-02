/**
 * @file   NetworkTcpTest.cpp
 * @brief  Tcp network tester.
 * @author zer0
 * @date   2017-05-06
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/stream/StreamClient.hpp>

#include <thread>
#include <memory>
#include <vector>
#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;
using namespace libtbag::network::stream;

TBAG_CONSTEXPR static bool const TEST_IPV4 = true;
TBAG_CONSTEXPR static bool const TEST_IPV6 = false;

struct TestTcpServer : public TcpServer
{
    using Loop = uvpp::Loop;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    int      connection_cb_counter = 0;
    int client_shutdown_cb_counter = 0;
    int    client_write_cb_counter = 0;
    int     client_read_cb_counter = 0;
    int    client_close_cb_counter = 0;
    int    client_timer_cb_counter = 0;
    int    server_close_cb_counter = 0;

    using OnConnection     = std::function<void(Err)>;
    using OnClientShutdown = std::function<void(WeakClient, Err)>;
    using OnClientWrite    = std::function<void(WeakClient, Err)>;
    using OnClientRead     = std::function<void(WeakClient, Err, ReadPacket const &)>;
    using OnClientClose    = std::function<void(WeakClient)>;
    using OnClientTimer    = std::function<void(WeakClient)>;
    using OnServerClose    = std::function<void(void)>;

    OnConnection     connection_cb;
    OnClientShutdown client_shutdown_cb;
    OnClientWrite    client_write_cb;
    OnClientRead     client_read_cb;
    OnClientClose    client_close_cb;
    OnClientTimer    client_timer_cb;
    OnServerClose    server_close_cb;

    TestTcpServer(Loop & loop) : TcpServer(loop)
    { /* EMPTY */ }
    virtual ~TestTcpServer()
    { /* EMPTY */ }

    inline void setOnConnection    (OnConnection     const & cb) { connection_cb      = cb; }
    inline void setOnClientShutdown(OnClientShutdown const & cb) { client_shutdown_cb = cb; }
    inline void setOnClientWrite   (OnClientWrite    const & cb) { client_write_cb    = cb; }
    inline void setOnClientRead    (OnClientRead     const & cb) { client_read_cb     = cb; }
    inline void setOnClientClose   (OnClientClose    const & cb) { client_close_cb    = cb; }
    inline void setOnClientTimer   (OnClientTimer    const & cb) { client_timer_cb    = cb; }
    inline void setOnClose         (OnServerClose    const & cb) { server_close_cb    = cb; }

    virtual void onConnection(Err code) override
    { connection_cb_counter++; if (connection_cb) { connection_cb(code); } }

    virtual void onClientShutdown(WeakClient node, Err code) override
    { client_shutdown_cb_counter++; if (client_shutdown_cb) { client_shutdown_cb(node, code); } }
    virtual void onClientWrite(WeakClient node, Err code) override
    { client_write_cb_counter++; if (client_write_cb) { client_write_cb(node, code); } }
    virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override
    { client_read_cb_counter++; if (client_read_cb) { client_read_cb(node, code, packet); } }
    virtual void onClientClose(WeakClient node) override
    { client_close_cb_counter++; if (client_close_cb) { client_close_cb(node); } }
    virtual void onClientTimer(WeakClient node) override
    { client_timer_cb_counter++; if (client_timer_cb) { client_timer_cb(node); } }
    virtual void onServerClose() override
    { server_close_cb_counter++; if (server_close_cb) { server_close_cb(); } }
};

TEST(NetworkTcpTest, JustCreateClient)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TcpClient client(loop);
    client.close();
    ASSERT_EQ(Err::E_SUCCESS, loop.run());
}

TEST(NetworkTcpTest, JustCreateServer)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TcpServer server(loop);
    server.close();
    ASSERT_EQ(Err::E_SUCCESS, loop.run());
}

TEST(NetworkTcpTest, InitAndCloseClientIpv4)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;

    if (TEST_IPV4) {
        TcpClient client(loop);
        ASSERT_EQ(Err::E_SUCCESS, client.init("127.0.0.1", 9999));
        client.close();
        ASSERT_EQ(Err::E_SUCCESS, loop.run());
    }

    if (TEST_IPV6) {
        TcpClient client(loop);
        ASSERT_EQ(Err::E_SUCCESS, client.init("::1", 9999));
        client.close();
        ASSERT_EQ(Err::E_SUCCESS, loop.run());
    }
}

TEST(NetworkTcpTest, BindError)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;

    if (TEST_IPV4) {
        TcpServer server(loop);
        // It seems that Linux is broken here - bind succeeds.
        Err const code = server.init("127.255.255.255", 9999);
        ASSERT_TRUE(code == Err::E_SUCCESS || code == Err::E_EADDRNOTAVAIL);
        server.close();
        ASSERT_EQ(Err::E_SUCCESS, loop.run());
    }

    if (TEST_IPV6) {
        TcpServer server(loop);
        ASSERT_EQ(Err::E_EADDRNOTAVAIL, server.init("4:4:4:4:4:4:4:4", 9999));
        server.close();
        ASSERT_EQ(Err::E_SUCCESS, loop.run());
    }
}

TEST(NetworkTcpTest, AddressAlreadyInUse)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TcpServer server1(loop);
    TcpServer server2(loop);

    ASSERT_EQ(Err::E_SUCCESS, server1.init("0.0.0.0", 0));
    int const SERVER_PORT = server1.port();
    ASSERT_LT(0, SERVER_PORT);

    ASSERT_NE(Err::E_SUCCESS, server2.init("0.0.0.0", SERVER_PORT));

    server1.close();
    server2.close();

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
}

TEST(NetworkTcpTest, ClientTimeout)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    FuncTcpClient client(loop);

    Err const INIT_CODE = client.init("8.8.8.8", 9999);
    if (INIT_CODE == Err::E_ENETUNREACH) {
        std::cout << "Network unreachable.\n";
        return;
    }
    ASSERT_EQ(Err::E_SUCCESS, INIT_CODE);
    client.startTimer(50);

    int connect = 0;
    int close   = 0;
    int timer   = 0;

    Err connect_result = Err::E_UNKNOWN;
    Err result = Err::E_UNKNOWN;

    client.set_onConnect([&](Err code){
        connect_result = code;
        connect++;
    });
    client.set_onClose([&](){
        close++;
    });
    client.set_onTimer([&](){
        timer++;
        client.close();
    });

    std::thread thread([&](){
        result = loop.run();
    });
    thread.join();

    ASSERT_EQ(Err::E_ECANCELED, connect_result);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(1, connect);
    ASSERT_EQ(1, close);
    ASSERT_EQ(1, timer);
}

TEST(NetworkTcpTest, CloseWhileConnecting)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    FuncTcpClient client(loop);

    Err const INIT_CODE = client.init("1.2.3.4", 9999);
    if (INIT_CODE == Err::E_ENETUNREACH) {
        std::cout << "Network unreachable.\n";
        return;
    }

    ASSERT_EQ(Err::E_SUCCESS, INIT_CODE);
    client.startTimer(1);

    int connect_cb_called = 0;
    int close_cb_called = 0;
    int timer_cb_called = 0;
    int netunreach_errors = 0;

    client.set_onConnect([&](Err code){
        ASSERT_TRUE(code == Err::E_ECANCELED || code == Err::E_ENETUNREACH);
        connect_cb_called++;
        if (code == Err::E_ENETUNREACH) {
            netunreach_errors++;
        }
    });
    client.set_onClose([&](){
        close_cb_called++;
    });
    client.set_onTimer([&](){
        timer_cb_called++;
        client.close();
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, connect_cb_called);
    ASSERT_EQ(1, close_cb_called);
    ASSERT_EQ(1, timer_cb_called);

    if (netunreach_errors > 0) {
        std::cout << "Network unreachable in onConnect() callback.\n";
    }
}

TEST(NetworkTcpTest, ReadStop)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TestTcpServer server(loop);

    ASSERT_EQ(Err::E_SUCCESS, server.init("0.0.0.0"));
    int const SERVER_PORT = server.port();

    server.setOnConnection([&](Err code){
        auto shared = server.accept().lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->start());
    });
    server.setOnClientRead([&](TestTcpServer::WeakClient node, Err code, ReadPacket const & packet){
        ASSERT_TRUE(Err::E_SUCCESS == code || Err::E_EOF == code);
        auto shared = node.lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->write(packet.buffer, packet.size));
    });
    server.setOnClientWrite([&](TestTcpServer::WeakClient node, Err code){
        ASSERT_EQ(Err::E_SUCCESS, code);
        auto shared = node.lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        shared->close();
    });

    FuncTcpClient client(loop);
    ASSERT_EQ(Err::E_SUCCESS, client.init(LOOPBACK_IPV4, SERVER_PORT));

    int connect_cb_called = 0;
    int timer_cb_called = 0;
    int write_cb_called = 0;
    int read_cb_called = 0;
    int close_cb_called = 0;

    client.set_onConnect([&](Err code){
        connect_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, code);
        ASSERT_EQ(Err::E_SUCCESS, client.startTimer(50));
        ASSERT_EQ(Err::E_SUCCESS, client.start());
    });
    client.set_onTimer([&](){
        if (timer_cb_called == 0) {
            ASSERT_EQ(Err::E_SUCCESS, client.write("PING", 4));
            ASSERT_EQ(Err::E_SUCCESS, client.stop());
        } else if (timer_cb_called == 1) {
            client.close();
        }
        timer_cb_called++;
    });
    client.set_onWrite([&](Err code){
        write_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, code);
        ASSERT_EQ(Err::E_SUCCESS, client.startTimer(50));
    });
    client.set_onRead([&](Err code, ReadPacket const & packet){
        read_cb_called++;
    });
    client.set_onClose([&](){
        close_cb_called++;
        server.close();
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, server.connection_cb_counter);
    ASSERT_EQ(1, server.client_read_cb_counter);
    ASSERT_EQ(1, server.client_write_cb_counter);
    ASSERT_EQ(1, server.client_close_cb_counter);
    ASSERT_EQ(1, server.server_close_cb_counter);
    ASSERT_EQ(1, connect_cb_called);
    ASSERT_EQ(2, timer_cb_called);
    ASSERT_EQ(1, write_cb_called);
    ASSERT_EQ(0, read_cb_called);
    ASSERT_EQ(1, close_cb_called);
}

TEST(NetworkTcpTest, ShutdownAfterWrite)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TestTcpServer server(loop);

    ASSERT_EQ(Err::E_SUCCESS, server.init("0.0.0.0"));
    int const SERVER_PORT = server.port();

    server.setOnConnection([&](Err code){
        auto shared = server.accept().lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->start());
    });
    server.setOnClientRead([&](TestTcpServer::WeakClient node, Err code, ReadPacket const & packet){
        ASSERT_TRUE(Err::E_SUCCESS == code || Err::E_EOF == code);
    });
    server.setOnClientWrite([&](TestTcpServer::WeakClient node, Err code){
        ASSERT_EQ(Err::E_SUCCESS, code);
        auto shared = node.lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->close());
    });

    FuncTcpClient client(loop);
    ASSERT_EQ(Err::E_SUCCESS, client.init(LOOPBACK_IPV4, SERVER_PORT));
    ASSERT_EQ(Err::E_SUCCESS, client.startTimer(125));

    int connect_cb_called = 0;
    int timer_cb_called = 0;
    int write_cb_called = 0;
    int read_cb_called = 0;
    int shutdown_cb_called = 0;
    int close_cb_called = 0;

    client.set_onConnect([&](Err code){
        connect_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, code);
        ASSERT_EQ(Err::E_SUCCESS, client.start());
    });
    client.set_onTimer([&](){
        timer_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, client.write("PING", 4));
        ASSERT_EQ(Err::E_SUCCESS, client.cancel());
    });
    client.set_onShutdown([&](Err code){
        shutdown_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, code);
        ASSERT_EQ(Err::E_SUCCESS, client.close());
    });
    client.set_onWrite([&](Err code){
        write_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, code);
    });
    client.set_onRead([&](Err code, ReadPacket const & packet){
        read_cb_called++;
    });
    client.set_onClose([&](){
        close_cb_called++;
        ASSERT_EQ(Err::E_SUCCESS, server.close());
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, server.connection_cb_counter);
    ASSERT_EQ(1, server.client_read_cb_counter);
    ASSERT_EQ(0, server.client_write_cb_counter);
    ASSERT_EQ(1, server.client_close_cb_counter);
    ASSERT_EQ(1, server.server_close_cb_counter);
    ASSERT_EQ(1, connect_cb_called);
    ASSERT_EQ(1, timer_cb_called);
    ASSERT_EQ(1, write_cb_called);
    ASSERT_EQ(0, read_cb_called);
    ASSERT_EQ(1, shutdown_cb_called);
    ASSERT_EQ(1, close_cb_called);
}

TEST(NetworkTcpTest, WriteOnServerToMultiClients)
{
    log::SeverityGuard guard;
    using namespace uvpp;
    Loop loop;
    TestTcpServer server(loop);

    std::string const MESSAGE = "message";

    ASSERT_EQ(Err::E_SUCCESS, server.init("0.0.0.0"));
    int const SERVER_PORT = server.port();

    int connection_count = 0;

    server.setOnConnection([&](Err code){
        auto shared = server.accept().lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->start());
        ++connection_count;

        if (connection_count == 2) {
            server.forEach([&](TestTcpServer::value_type const & value){
                value.second->write(MESSAGE.c_str(), MESSAGE.size());
            });
        }
    });

    FuncTcpClient client1(loop);
    FuncTcpClient client2(loop);
    ASSERT_EQ(Err::E_SUCCESS, client1.init(LOOPBACK_IPV4, SERVER_PORT));
    ASSERT_EQ(Err::E_SUCCESS, client2.init(LOOPBACK_IPV4, SERVER_PORT));

    int client1_read_count = 0;
    int client2_read_count = 0;
    std::vector<std::string> client1_msgs;
    std::vector<std::string> client2_msgs;

    ASSERT_EQ(Err::E_SUCCESS, client1.start());
    ASSERT_EQ(Err::E_SUCCESS, client2.start());

    client1.set_onRead([&](Err code, ReadPacket const & packet){
        if (code == Err::E_EOF) {
            client1.close();
        }
        if (code == Err::E_SUCCESS) {
            client1_msgs.push_back(std::string(packet.buffer, packet.buffer + packet.size));
            ++client1_read_count;
            if (client1_read_count + client2_read_count == 2) {
                server.close();
            }
        }
    });
    client2.set_onRead([&](Err code, ReadPacket const & packet){
        if (code == Err::E_EOF) {
            client2.close();
        }
        if (code == Err::E_SUCCESS) {
            client2_msgs.push_back(std::string(packet.buffer, packet.buffer + packet.size));
            ++client2_read_count;
            if (client1_read_count + client2_read_count == 2) {
                server.close();
            }
        }
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, client1_read_count);
    ASSERT_EQ(1, client2_read_count);
    ASSERT_EQ(1, client1_msgs.size());
    ASSERT_EQ(1, client2_msgs.size());
    ASSERT_EQ(MESSAGE, client1_msgs[0]);
    ASSERT_EQ(MESSAGE, client2_msgs[0]);
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
    FuncTcpServer server(loop_server);

    int server_connection    = 0;
    int server_client_read   = 0;
    int server_client_write  = 0;
    int server_client_close  = 0;
    int server_close         = 0;
    Err server_result = Err::E_UNKNOWN;

    server.set_onConnection([&](Err code){
        auto shared = server.accept().lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->start());
        server_connection++;
    });
    server.set_onClientRead([&](FuncTcpServer::WeakClient node, Err code, ReadPacket const & packet){
        ASSERT_TRUE(code == Err::E_SUCCESS || code == Err::E_EOF);
        if (code == Err::E_SUCCESS) {
            auto shared = node.lock();
            ASSERT_TRUE(static_cast<bool>(shared));
            ASSERT_EQ(Err::E_SUCCESS, shared->stop());
            server_client_read++;
            ASSERT_EQ(Err::E_SUCCESS, shared->write(packet.buffer, packet.size));
        }
    });
    server.set_onClientWrite([&](FuncTcpServer::WeakClient node, Err code){
        ASSERT_EQ(Err::E_SUCCESS, code);
        auto shared = node.lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        ASSERT_EQ(Err::E_SUCCESS, shared->close());
        server_client_write++;
    });
    server.set_onClientClose([&](FuncTcpServer::WeakClient node){
        auto shared = node.lock();
        ASSERT_TRUE(static_cast<bool>(shared));
        server_client_close++;
        if (server_client_close >= CLIENT_SIZE) {
            ASSERT_EQ(Err::E_SUCCESS, server.close());
        }
    });
    server.set_onServerClose([&](){
        server_close++;
    });

    server.init(details::ANY_IPV4, 0);
    int const SERVER_PORT = server.port();
    std::cout << "Server port number: " << SERVER_PORT << std::endl;
    std::thread thread_server([&](){
        server_result = loop_server.run();
    });

    // ---------------
    // CLIENT PROCESS.
    // ---------------

    using SharedLoop = std::shared_ptr<Loop>;
    using LoopVector = std::vector<SharedLoop>;

    using SharedFuncClient = std::shared_ptr<FuncTcpClient>;
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
        clients.at(i).reset(new FuncTcpClient(*(cloops.at(i))));
        clients.at(i)->set_onConnect([&, i](Err code){
            if (clients.at(i)->write(ECHO_MESSAGE.data(), ECHO_MESSAGE.size()) == Err::E_SUCCESS) {
                connect_result.at(i) = code;
            }
        });
        clients.at(i)->set_onWrite([&, i](Err code){
            if (clients.at(i)->start() == Err::E_SUCCESS) {
                write_result.at(i) = code;
            }
        });
        clients.at(i)->set_onRead([&, i](Err code, ReadPacket const & packet){
            if (clients.at(i)->stop() == Err::E_SUCCESS) {
                read_result.at(i) = code;
                clients.at(i)->close();
            }
        });
        clients.at(i)->set_onClose([&, i](){
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
    ASSERT_EQ(CLIENT_SIZE, server_connection  );
    ASSERT_EQ(CLIENT_SIZE, server_client_read );
    ASSERT_EQ(CLIENT_SIZE, server_client_write);
    ASSERT_EQ(CLIENT_SIZE, server_client_close);
    ASSERT_EQ(1, server_close);
}

