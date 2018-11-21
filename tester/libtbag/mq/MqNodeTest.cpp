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

