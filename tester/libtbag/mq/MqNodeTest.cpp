/**
 * @file   MqNodeTest.cpp
 * @brief  MqNode class tester.
 * @author zer0
 * @date   2018-11-11
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/MqNode.hpp>
#include <libtbag/log/Log.hpp>

#include <string>
#include <set>

TBAG_CONSTEXPR static char const * const EXTENSION_NODE_PARAMS = "?verbose=true&wait_closing=1";

using namespace libtbag;
using namespace libtbag::mq;
using namespace libtbag::mq::details;

TEST(MqNodeTest, Bind)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    libtbag::log::SeverityGuard guard;
    MqNode node(PIPE_PATH, MqMode::MM_BIND);
}

TEST(MqNodeTest, ClientRel_ServerRel)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    // Client release -> Server release!
    client.reset();
    server.reset();
}

TEST(MqNodeTest, ServerRel_ClientRel)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    // Server release -> Client release!
    server.reset();
    client.reset();
}

TEST(MqNodeTest, Reconnect)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS +
            "&reconnect=0&wait_activation=0";

    libtbag::log::SeverityGuard guard;
    std::unique_ptr<MqNode> client;
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    std::unique_ptr<MqNode> server;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);

    ASSERT_EQ(E_SUCCESS, client->waitEnable());
}

TEST(MqNodeTest, ClientToServerMessage)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    std::string const TEST1_VAL = "T1E1ST1";
    std::string const TEST2_VAL = "T2E2ST2";

    MqNode::MqMsg msg_send1(TEST1_VAL);
    MqNode::MqMsg msg_send2(TEST2_VAL);

    ASSERT_EQ(E_SUCCESS, client->send(msg_send1));
    ASSERT_EQ(E_SUCCESS, client->send(msg_send2));

    MqNode::MqMsg msg_recv1;
    MqNode::MqMsg msg_recv2;

    server->recvWait(msg_recv1);
    server->recvWait(msg_recv2);

    std::string const RESULT1_VAL(msg_recv1.begin(), msg_recv1.end());
    std::string const RESULT2_VAL(msg_recv2.begin(), msg_recv2.end());

    server.reset();
    client.reset();

    ASSERT_EQ(TEST1_VAL, RESULT1_VAL);
    ASSERT_EQ(TEST2_VAL, RESULT2_VAL);
}

TEST(MqNodeTest, ServerToClientMessage)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    std::string const TEST1_VAL = "T1E1ST1";
    std::string const TEST2_VAL = "T2E2ST2";

    MqNode::MqMsg msg_send1(TEST1_VAL);
    MqNode::MqMsg msg_send2(TEST2_VAL);

    ASSERT_EQ(E_SUCCESS, server->send(msg_send1));
    ASSERT_EQ(E_SUCCESS, server->send(msg_send2));

    MqNode::MqMsg msg_recv1;
    MqNode::MqMsg msg_recv2;

    client->recvWait(msg_recv1);
    client->recvWait(msg_recv2);

    std::string const RESULT1_VAL(msg_recv1.begin(), msg_recv1.end());
    std::string const RESULT2_VAL(msg_recv2.begin(), msg_recv2.end());

    client.reset();
    server.reset();

    ASSERT_EQ(TEST1_VAL, RESULT1_VAL);
    ASSERT_EQ(TEST2_VAL, RESULT2_VAL);
}

TEST(MqNodeTest, SendAndClose1)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    ASSERT_EQ(E_SUCCESS, server->send(MqNode::MqMsg(std::string("T1E1ST1"))));
    ASSERT_EQ(E_SUCCESS, client->send(MqNode::MqMsg(std::string("T2E2ST2"))));

    // Client release -> Server release!
    client.reset();
    server.reset();
}

TEST(MqNodeTest, SendAndClose2)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    ASSERT_EQ(E_SUCCESS, server->send(MqNode::MqMsg(std::string("T1E1ST1"))));
    ASSERT_EQ(E_SUCCESS, client->send(MqNode::MqMsg(std::string("T2E2ST2"))));

    // Server release -> Client release!
    server.reset();
    client.reset();
}

TEST(MqNodeTest, MultiClient)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + EXTENSION_NODE_PARAMS;

    std::unique_ptr<MqNode> server;
    std::unique_ptr<MqNode> client1;
    std::unique_ptr<MqNode> client2;

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_BIND);
    client1 = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);
    client2 = std::make_unique<MqNode>(PIPE_PATH, MqMode::MM_CONNECT);

    std::string const TEST1_VAL = "T1E1ST1";
    std::string const TEST2_VAL = "T2E2ST2";
    std::string const TEST3_VAL = "T3E3ST3";
    std::string const TEST4_VAL = "T4E4ST4";
    std::string const TEST5_VAL = "T5E5ST5";
    std::string const TEST6_VAL = "T6E6ST6";

    MqNode::MqMsg msg_send1(TEST1_VAL);
    MqNode::MqMsg msg_send2(TEST2_VAL);
    MqNode::MqMsg msg_send3(TEST3_VAL);
    MqNode::MqMsg msg_send4(TEST4_VAL);
    MqNode::MqMsg msg_send5(TEST5_VAL);
    MqNode::MqMsg msg_send6(TEST6_VAL);

    ASSERT_EQ(E_SUCCESS, client1->send(msg_send1));
    ASSERT_EQ(E_SUCCESS, client2->send(msg_send2));
    ASSERT_EQ(E_SUCCESS, client1->send(msg_send3));
    ASSERT_EQ(E_SUCCESS, client2->send(msg_send4));
    ASSERT_EQ(E_SUCCESS, server->send(msg_send5));
    ASSERT_EQ(E_SUCCESS, server->send(msg_send6));

    MqNode::MqMsg msg_recv1;
    MqNode::MqMsg msg_recv2;
    MqNode::MqMsg msg_recv3;
    MqNode::MqMsg msg_recv4;
    server->recvWait(msg_recv1);
    server->recvWait(msg_recv2);
    server->recvWait(msg_recv3);
    server->recvWait(msg_recv4);

    std::string const RESULT1_VAL(msg_recv1.begin(), msg_recv1.end());
    std::string const RESULT2_VAL(msg_recv2.begin(), msg_recv2.end());
    std::string const RESULT3_VAL(msg_recv3.begin(), msg_recv3.end());
    std::string const RESULT4_VAL(msg_recv4.begin(), msg_recv4.end());

    std::set<std::string> client_to_server_vals;
    ASSERT_TRUE(client_to_server_vals.insert(RESULT1_VAL).second);
    ASSERT_TRUE(client_to_server_vals.insert(RESULT2_VAL).second);
    ASSERT_TRUE(client_to_server_vals.insert(RESULT3_VAL).second);
    ASSERT_TRUE(client_to_server_vals.insert(RESULT4_VAL).second);

    MqNode::MqMsg msg_client1_recv5;
    MqNode::MqMsg msg_client1_recv6;
    client1->recvWait(msg_client1_recv5);
    client1->recvWait(msg_client1_recv6);
    std::string const CLIENT1_RESULT5_VAL(msg_client1_recv5.begin(), msg_client1_recv5.end());
    std::string const CLIENT1_RESULT6_VAL(msg_client1_recv6.begin(), msg_client1_recv6.end());

    MqNode::MqMsg msg_client2_recv5;
    MqNode::MqMsg msg_client2_recv6;
    client2->recvWait(msg_client2_recv5);
    client2->recvWait(msg_client2_recv6);
    std::string const CLIENT2_RESULT5_VAL(msg_client2_recv5.begin(), msg_client2_recv5.end());
    std::string const CLIENT2_RESULT6_VAL(msg_client2_recv6.begin(), msg_client2_recv6.end());

    server.reset();
    client1.reset();
    client2.reset();

    auto itr = client_to_server_vals.find(TEST1_VAL);
    ASSERT_NE(itr, client_to_server_vals.end());
    client_to_server_vals.erase(itr);

    itr = client_to_server_vals.find(TEST2_VAL);
    ASSERT_NE(itr, client_to_server_vals.end());
    client_to_server_vals.erase(itr);

    itr = client_to_server_vals.find(TEST3_VAL);
    ASSERT_NE(itr, client_to_server_vals.end());
    client_to_server_vals.erase(itr);

    itr = client_to_server_vals.find(TEST4_VAL);
    ASSERT_NE(itr, client_to_server_vals.end());
    client_to_server_vals.erase(itr);

    ASSERT_TRUE(client_to_server_vals.empty());

    ASSERT_EQ(TEST5_VAL, CLIENT1_RESULT5_VAL);
    ASSERT_EQ(TEST5_VAL, CLIENT2_RESULT5_VAL);

    ASSERT_EQ(TEST6_VAL, CLIENT1_RESULT6_VAL);
    ASSERT_EQ(TEST6_VAL, CLIENT2_RESULT6_VAL);
}

