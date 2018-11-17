/**
 * @file   MqNodeTest.cpp
 * @brief  MqNode class tester.
 * @author zer0
 * @date   2018-11-11
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/mq/MqNode.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::mq;

TEST(MqNodeTest, Bind_Release)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get();

    MqNode node;
    ASSERT_EQ(Err::E_SUCCESS, node.bind(PIPE_PATH));
}

//TEST(MqNodeTest, Connect_Release)
//{
//    tttDir_Automatic_Pipe();
//    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get();
//
//    MqNode node;
//    ASSERT_EQ(Err::E_SUCCESS, node.connect(PIPE_PATH));
//}

TEST(MqNodeTest, BindAndConnection_Release)
{
    tttDir_Automatic_Pipe();
    auto const PIPE_PATH = std::string("pipe://") + tttDir_Pipe_Get();

    MqNode node1;
    MqNode node2;
    ASSERT_EQ(Err::E_SUCCESS, node1.bind(PIPE_PATH));
    ASSERT_EQ(Err::E_SUCCESS, node2.connect(PIPE_PATH));
}

