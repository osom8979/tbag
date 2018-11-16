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

TEST(MqNodeTest, BindAndRelease)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    auto const TEST_FILE_PATH = std::string(R"(\\.\pipe\)") + test_info_->test_case_name() + "_" + test_info_->name();
    char const * PATH = TEST_FILE_PATH.c_str();
#else
    tttDir(true, true);
    auto const TEST_FILE_PATH = tttDir_Get() / "mq_node.sock";
    char const * PATH = TEST_FILE_PATH.c_str();
#endif

    MqNode node;
    ASSERT_EQ(Err::E_SUCCESS, node.bind(std::string("pipe://") + PATH));
}

TEST(MqNodeTest, BindAndConnection)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    auto const TEST_FILE_PATH = std::string(R"(\\.\pipe\)") + test_info_->test_case_name() + "_" + test_info_->name();
    char const * PATH = TEST_FILE_PATH.c_str();
#else
    tttDir(true, true);
    auto const TEST_FILE_PATH = tttDir_Get() / "mq_node.sock";
    char const * PATH = TEST_FILE_PATH.c_str();
#endif

    MqNode node1;
    ASSERT_EQ(Err::E_SUCCESS, node1.bind(std::string("pipe://") + PATH));

    MqNode node2;
    ASSERT_EQ(Err::E_SUCCESS, node2.connect(std::string("pipe://") + PATH));
}

