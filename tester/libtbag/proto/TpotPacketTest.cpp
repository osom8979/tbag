/**
 * @file   TpotPacketTest.cpp
 * @brief  TpotPacket class tester.
 * @author zer0
 * @date   2017-09-06
 */

#include <gtest/gtest.h>
#include <libtbag/proto/TpotPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;
using namespace libtbag::proto::fbs;
using namespace libtbag::proto::fbs::tpot;

TEST(TpotPacketTest, Default)
{
    std::string const TEST_FILE  = "temp";
    std::string const TEST_ARG1  = "arg1";
    std::string const TEST_ARG2  = "arg2";
    std::string const TEST_ENV1  = "env1";
    std::string const TEST_ENV2  = "env2";
    std::string const TEST_ENV3  = "env3";
    std::string const TEST_CWD   = "cwd";
    std::string const TEST_INPUT = "input";

    std::vector<char> buffer;
    FunctionalTpotPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.buildExecRequest(TEST_FILE,
                                                      {TEST_ARG1, TEST_ARG2},
                                                      {TEST_ENV1, TEST_ENV2, TEST_ENV3},
                                                      TEST_CWD,
                                                      TEST_INPUT));
    buffer.assign(packet.point(), packet.point() + packet.size());
    ASSERT_LT(0, buffer.size());

    uint64_t header_id = 0;
    int header_code;

    std::string file;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    std::string cwd;
    std::string input;

    packet.setOnExecRequest([&](Header const & header, ExecRequest const & packet, void * arg){
        header_id   = header.id();
        header_code = static_cast<int>(header.code());
        file  = packet.file()->str();
        cwd   = packet.cwd()->str();
        input = packet.input()->str();
        for (auto args_itr = packet.args()->begin(); args_itr != packet.args()->end(); ++args_itr) {
            args.push_back(args_itr->str());
        }
        for (auto envs_itr = packet.envs()->begin(); envs_itr != packet.envs()->end(); ++envs_itr) {
            envs.push_back(envs_itr->str());
        }
    });
    ASSERT_EQ(Err::E_SUCCESS, packet.parse(buffer.data(), buffer.size()));

    ASSERT_LT(0, header_id);
    ASSERT_EQ(static_cast<int>(ResultCode_SUCCESS), header_code);
    ASSERT_EQ(TEST_FILE, file);
    ASSERT_EQ(TEST_CWD, cwd);
    ASSERT_EQ(TEST_INPUT, input);
    ASSERT_EQ(2, args.size());
    ASSERT_EQ(TEST_ARG1, args[0]);
    ASSERT_EQ(TEST_ARG2, args[1]);
    ASSERT_EQ(3, envs.size());
    ASSERT_EQ(TEST_ENV1, envs[0]);
    ASSERT_EQ(TEST_ENV2, envs[1]);
    ASSERT_EQ(TEST_ENV3, envs[2]);
}

