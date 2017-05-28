/**
 * @file   TpotProtocolTest.cpp
 * @brief  TpotProtocol class tester.
 * @author zer0
 * @date   2017-05-28
 */

#include <gtest/gtest.h>
#include <libtbag/tpot/structure/TpotProtocol.hpp>

using namespace libtbag;
using namespace libtbag::tpot;
using namespace libtbag::tpot::structure;

TEST(TpotProtocolTest, ExecRequest)
{
    char const * const TEST_REQUEST_JSON = R"({
"file": "/bin/ls",
"cwd" : "/",
"args": ["-l", "-a"],
"envs": ["PATH=/bin", "CLICOLOR=1"],
"uid" : 1000,
"gid" : 1000
})";

    Exec obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromRequestJsonString(TEST_REQUEST_JSON));
    ASSERT_STREQ("/bin/ls", obj.request.file.c_str());
    ASSERT_STREQ("/", obj.request.cwd.c_str());
    ASSERT_EQ(2, obj.request.args.size());
    ASSERT_STREQ("-l", obj.request.args[0].c_str());
    ASSERT_STREQ("-a", obj.request.args[1].c_str());
    ASSERT_EQ(2, obj.request.envs.size());
    ASSERT_STREQ("PATH=/bin", obj.request.envs[0].c_str());
    ASSERT_STREQ("CLICOLOR=1", obj.request.envs[1].c_str());
    ASSERT_EQ(1000, obj.request.uid);
    ASSERT_EQ(1000, obj.request.gid);

    std::string json;
    ASSERT_EQ(Err::E_SUCCESS, obj.toRequestJsonString(json));

    Exec obj2;
    ASSERT_EQ(Err::E_SUCCESS, obj2.fromRequestJsonString(json));
    ASSERT_STREQ(obj.request.file.c_str(), obj.request.file.c_str());
    ASSERT_STREQ(obj.request.cwd.c_str(), obj.request.cwd.c_str());
    ASSERT_EQ(obj2.request.args.size(), obj.request.args.size());
    ASSERT_STREQ(obj2.request.args[0].c_str(), obj.request.args[0].c_str());
    ASSERT_STREQ(obj2.request.args[1].c_str(), obj.request.args[1].c_str());
    ASSERT_EQ(obj2.request.envs.size(), obj.request.envs.size());
    ASSERT_STREQ(obj2.request.envs[0].c_str(), obj.request.envs[0].c_str());
    ASSERT_STREQ(obj2.request.envs[1].c_str(), obj.request.envs[1].c_str());
    ASSERT_EQ(obj2.request.uid, obj.request.uid);
    ASSERT_EQ(obj2.request.gid, obj.request.gid);
}

TEST(TpotProtocolTest, ExecResponse)
{
    char const * const TEST_RESPONSE_JSON = R"({
"id": 1234567890
})";

    Exec obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromResponseJsonString(TEST_RESPONSE_JSON));
    ASSERT_EQ(1234567890, obj.response.id);

    std::string json;
    ASSERT_EQ(Err::E_SUCCESS, obj.toResponseJsonString(json));

    Exec obj2;
    ASSERT_EQ(Err::E_SUCCESS, obj2.fromResponseJsonString(json));
    ASSERT_EQ(obj2.response.id, obj.response.id);
}

TEST(TpotProtocolTest, Heartbit)
{
    std::string json;
    Heartbit obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromRequestJsonString (json));
    ASSERT_EQ(Err::E_SUCCESS, obj.fromResponseJsonString(json));
    ASSERT_EQ(Err::E_SUCCESS, obj.toRequestJsonString   (json));
    ASSERT_EQ(Err::E_SUCCESS, obj.toResponseJsonString  (json));
}

TEST(TpotProtocolTest, ListRequest)
{
    std::string json;
    List obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromRequestJsonString(json));
    ASSERT_EQ(Err::E_SUCCESS, obj.toRequestJsonString  (json));
}

TEST(TpotProtocolTest, ListResponse)
{
    char const * const TEST_RESPONSE_JSON = R"({
"ids": [1, 2, 3, 9]
})";

    List obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromResponseJsonString(TEST_RESPONSE_JSON));
    ASSERT_EQ(4, obj.response.ids.size());
    ASSERT_EQ(1, obj.response.ids[0]);
    ASSERT_EQ(2, obj.response.ids[1]);
    ASSERT_EQ(3, obj.response.ids[2]);
    ASSERT_EQ(9, obj.response.ids[3]);

    std::string json;
    ASSERT_EQ(Err::E_SUCCESS, obj.toResponseJsonString(json));

    List obj2;
    ASSERT_EQ(Err::E_SUCCESS, obj2.fromResponseJsonString(json));
    ASSERT_EQ(obj2.response.ids.size(), obj.response.ids.size());
    ASSERT_EQ(obj2.response.ids[0], obj.response.ids[0]);
    ASSERT_EQ(obj2.response.ids[1], obj.response.ids[1]);
    ASSERT_EQ(obj2.response.ids[2], obj.response.ids[2]);
    ASSERT_EQ(obj2.response.ids[3], obj.response.ids[3]);
}

TEST(TpotProtocolTest, KillRequest)
{
    char const * const TEST_REQUEST_JSON = R"({
"id": 1234567890
})";

    Kill obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromRequestJsonString(TEST_REQUEST_JSON));
    ASSERT_EQ(1234567890, obj.request.id);

    std::string json;
    ASSERT_EQ(Err::E_SUCCESS, obj.toRequestJsonString(json));

    Kill obj2;
    ASSERT_EQ(Err::E_SUCCESS, obj2.fromRequestJsonString(json));
    ASSERT_EQ(obj2.request.id, obj.request.id);
}

TEST(TpotProtocolTest, KillResponse)
{
    std::string json;
    Kill obj;
    ASSERT_EQ(Err::E_SUCCESS, obj.fromResponseJsonString(json));
    ASSERT_EQ(Err::E_SUCCESS, obj.toResponseJsonString  (json));
}

