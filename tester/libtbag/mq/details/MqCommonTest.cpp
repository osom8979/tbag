/**
 * @file   MqCommonTest.cpp
 * @brief  MqCommon class tester.
 * @author zer0
 * @date   2018-11-16
 */

#include <gtest/gtest.h>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::mq;
using namespace libtbag::mq::details;

TEST(MqCommonTest, ConvertUriToParams)
{
    auto const PARAMS1 = convertUriToParams("tcp://192.168.0.1:9999?read_error=10&verify_msg=true");
    ASSERT_STREQ("192.168.0.1", PARAMS1.address.c_str());
    ASSERT_EQ(9999, PARAMS1.port);
    ASSERT_EQ(MqType::MT_TCP, PARAMS1.type);
    ASSERT_EQ(10, PARAMS1.continuous_read_error_count);
    ASSERT_TRUE(PARAMS1.verify_restore_message);

    auto const PARAMS2 = convertUriToParams("pipe:////./pipe/test?tcp_ipv6=true&send_msg=5", true);
    ASSERT_STREQ("\\\\.\\pipe\\test", PARAMS2.address.c_str());
    ASSERT_EQ(MqType::MT_PIPE, PARAMS2.type);
    ASSERT_TRUE(PARAMS2.tcp_ipv6_only);
    ASSERT_EQ(5, PARAMS2.send_msg_size);

    auto const PARAMS3 = convertUriToParams("pipe:///home/user/test.sock?tcp_ipv6=false&try_reconnect=4");
    ASSERT_STREQ("/home/user/test.sock", PARAMS3.address.c_str());
    ASSERT_EQ(MqType::MT_PIPE, PARAMS3.type);
    ASSERT_FALSE(PARAMS3.tcp_ipv6_only);
    ASSERT_EQ(4, PARAMS3.try_reconnect_count);

    log::SeverityGuard guard;
    auto const PARAMS4 = convertUriToParams("http://localhost?recv_queue=8");
    ASSERT_FALSE(PARAMS4.address.empty());
    ASSERT_STRNE("localhost", PARAMS4.address.c_str());
    ASSERT_EQ(80, PARAMS4.port);
    ASSERT_EQ(MqType::MT_TCP, PARAMS4.type);
    ASSERT_EQ(8, PARAMS4.recv_queue_size);
}

