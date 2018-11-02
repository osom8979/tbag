/**
 * @file   DsNodeTest.cpp
 * @brief  DsNode class tester.
 * @author zer0
 * @date   2018-11-01
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/network/distribution/DsNode.hpp>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::distribution;

TEST(DsNodeTest, Pipe)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    char const * PATH1 = "\\\\.\\pipe\\DS_NODE_TEST_DEFAULT_1";
    char const * PATH2 = "\\\\.\\pipe\\DS_NODE_TEST_DEFAULT_2";
#else
    char const * const TEST1_FILENAME = "pipe1.sock";
    char const * const TEST2_FILENAME = "pipe2.sock";
    tttDir_Automatic();
    auto const TEST1_FILE_PATH = tttDir_Get() / TEST1_FILENAME;
    auto const TEST2_FILE_PATH = tttDir_Get() / TEST2_FILENAME;
    char const * PATH1 = TEST1_FILE_PATH.c_str();
    char const * PATH2 = TEST2_FILE_PATH.c_str();
#endif

    DsNode node1;
    auto func1 = node1.updateFunctionalEvent();
    func1->connect_cb = [&](std::string const & name){
        tDLogI("Node1@connect({})", name);
    };
    func1->disconnect_cb = [&](std::string const & name){
        tDLogI("Node1@disconnect({})", name);
    };
    func1->recv_cb = [&](std::string const & name, char const * buffer, std::size_t size){
        tDLogI("Node1@recv({}): {}", name, std::string(buffer, buffer + size));
    };

    DsNode node2;
    auto func2 = node2.updateFunctionalEvent();
    func2->connect_cb = [&](std::string const & name){
        tDLogI("Node2@connect({})", name);
    };
    func2->disconnect_cb = [&](std::string const & name){
        tDLogI("Node2@disconnect({})", name);
    };
    func2->recv_cb = [&](std::string const & name, char const * buffer, std::size_t size){
        tDLogI("Node2@recv({}): {}", name, std::string(buffer, buffer + size));
    };

    std::string const NODE1_NAME = "node1";
    std::string const NODE2_NAME = "node2";
    std::string const LOCAL_HOST = "localhost";

    ASSERT_EQ(DsNode::State::S_NONE, node1.getState());
    ASSERT_EQ(DsNode::State::S_NONE, node2.getState());

    ASSERT_EQ(Err::E_SUCCESS, node1.openPipe(NODE1_NAME, PATH1, true));
    ASSERT_EQ(Err::E_SUCCESS, node2.openPipe(NODE2_NAME, PATH2, true));

//    ASSERT_TRUE(node1.busyWaitingUntilOpened());
//    ASSERT_TRUE(node2.busyWaitingUntilOpened());
//
//    ASSERT_EQ(Err::E_SUCCESS, node1.connectPipe(NODE2_NAME, PATH2));
//    ASSERT_TRUE(node1.busyWaitingUntilConnected(NODE2_NAME));
//
////    ASSERT_EQ(Err::E_SUCCESS, node2.connectPipe(NODE1_NAME, PATH1));
////    ASSERT_TRUE(node2.busyWaitingUntilConnected(NODE1_NAME));
//
//    std::string const NODE1_MESSAGE = "message1";
//    std::string const NODE2_MESSAGE = "message2";
//
//    ASSERT_EQ(Err::E_SUCCESS, node1.write(NODE2_NAME, NODE1_MESSAGE));
//    ASSERT_EQ(Err::E_SUCCESS, node2.write(NODE1_NAME, NODE2_MESSAGE));
}

