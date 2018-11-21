/**
 * @file   MqNodeTest.cpp
 * @brief  MqNode class tester.
 * @author zer0
 * @date   2018-11-11
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/mq/MqNode.hpp>
#include <libtbag/log/Log.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::mq;

TEST(MqNodeTest, Bind)
{
    libtbag::log::SeverityGuard guard;

    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    MqNode node;
    ASSERT_EQ(Err::E_SUCCESS, node.bind(PIPE_PATH));
}

TEST(MqNodeTest, ClientRel_ServerRel)
{
    // Client release -> Server release!
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();

    libtbag::log::SeverityGuard guard;
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));

    client.reset();
    server.reset();
}

TEST(MqNodeTest, ServerRel_ClientRel)
{
    // Server release -> Client release!
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();

    libtbag::log::SeverityGuard guard;
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));

    server.reset();
    client.reset();
}

TEST(MqNodeTest, ClientToServerMessage)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();

    libtbag::log::SeverityGuard guard;
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));

    std::string const TEST1_VAL = "T1E1ST1";
    std::string const TEST2_VAL = "T2E2ST2";

    MqNode::MqMsg msg_send1(TEST1_VAL);
    MqNode::MqMsg msg_send2(TEST2_VAL);

    ASSERT_EQ(Err::E_SUCCESS, client->send(msg_send1));
    ASSERT_EQ(Err::E_SUCCESS, client->send(msg_send2));

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
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();

    libtbag::log::SeverityGuard guard;
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));

    std::string const TEST1_VAL = "T1E1ST1";
    std::string const TEST2_VAL = "T2E2ST2";

    MqNode::MqMsg msg_send1(TEST1_VAL);
    MqNode::MqMsg msg_send2(TEST2_VAL);

    ASSERT_EQ(Err::E_SUCCESS, server->send(msg_send1));
    ASSERT_EQ(Err::E_SUCCESS, server->send(msg_send2));

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
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    libtbag::log::SeverityGuard guard;
    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, server->send(MqNode::MqMsg(std::string("T1E1ST1"))));
    ASSERT_EQ(Err::E_SUCCESS, client->send(MqNode::MqMsg(std::string("T2E2ST2"))));

    // Client release -> Server release!
    client.reset();
    server.reset();
}

TEST(MqNodeTest, SendAndClose2)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get() + "?verbose=true";

    libtbag::log::SeverityGuard guard;
    auto server = std::make_unique<MqNode>();
    auto client = std::make_unique<MqNode>();
    ASSERT_EQ(Err::E_SUCCESS, server->bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, client->connect(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, server->send(MqNode::MqMsg(std::string("T1E1ST1"))));
    ASSERT_EQ(Err::E_SUCCESS, client->send(MqNode::MqMsg(std::string("T2E2ST2"))));

    // Server release -> Client release!
    server.reset();
    client.reset();
}

