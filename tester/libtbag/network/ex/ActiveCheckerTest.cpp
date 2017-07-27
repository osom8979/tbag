/**
 * @file   ActiveCheckerTest.cpp
 * @brief  ActiveChecker class tester.
 * @author zer0
 * @date   2017-07-27
 */

#include <gtest/gtest.h>
#include <libtbag/network/ex/ActiveChecker.hpp>
#include <libtbag/util/Version.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::ex;

using ActiveCheckerRequest  = ActiveChecker::Request;
using ActiveCheckerResponse = ActiveChecker::Response;

TEST(ActiveCheckerTest, Request)
{
    ActiveCheckerRequest request1;
    request1.version   = util::getTbagPacketVersion();
    request1.recv_ip   = "0.0.0.0";
    request1.recv_port = 1000;

    std::string const JSON_REQUEST = request1.toJsonString();
    ASSERT_FALSE(JSON_REQUEST.empty());

    ActiveCheckerRequest request2;
    ASSERT_TRUE(request2.fromJsonString(JSON_REQUEST));
    ASSERT_EQ(request1.version  , request2.version  );
    ASSERT_EQ(request1.recv_ip  , request2.recv_ip  );
    ASSERT_EQ(request1.recv_port, request2.recv_port);
}

TEST(ActiveCheckerTest, Response)
{
    ActiveCheckerResponse response1;
    response1.version     = util::getTbagPacketVersion();
    response1.server_type = ActiveChecker::EndPointType::EPT_ROASTER;
    response1.server_ip   = "0.0.0.0";
    response1.server_port = 1000;

    std::string const JSON_REQUEST = response1.toJsonString();
    ASSERT_FALSE(JSON_REQUEST.empty());

    ActiveCheckerResponse response2;
    ASSERT_TRUE(response2.fromJsonString(JSON_REQUEST));
    ASSERT_EQ(response1.version    , response2.version    );
    ASSERT_EQ(response1.server_type, response2.server_type);
    ASSERT_EQ(response1.server_ip  , response2.server_ip  );
    ASSERT_EQ(response1.server_port, response2.server_port);
}

