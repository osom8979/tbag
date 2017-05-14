/**
 * @file   TpotBuilderTest.cpp
 * @brief  TpotBuilder class tester.
 * @author zer0
 * @date   2017-05-14
 */

#include <gtest/gtest.h>
#include <libtbag/tpot/TpotBuilder.hpp>

using namespace libtbag;
using namespace libtbag::tpot;

TEST(TpotBuilderTest, Unemployed)
{
    uint64_t const TEST_REQUEST = 99;

    TpotBuilder::Header header;
    header.request = TEST_REQUEST;

    TpotBuilder builder;
    TpotBuilder::Buffer buffer;
    ASSERT_TRUE(builder.build(header, buffer) == TpotBuilder::Code::SUCCESS);
    ASSERT_LT(0, buffer.size());

    TpotBuilder::Header read_header;
    TpotBuilder::Type   read_type;
    ASSERT_TRUE(builder.parse(buffer, read_header, read_type) == TpotBuilder::Code::SUCCESS);
    ASSERT_TRUE(read_type == TpotBuilder::Type::UNEMPLOYED);
    ASSERT_EQ(TEST_REQUEST, read_header.request);
}

TEST(TpotBuilderTest, Version)
{
    TpotBuilder builder;
    TpotBuilder::Buffer  buffer;
    TpotBuilder::Header  header;
    TpotBuilder::Version version;
    header.request = 0;
    version.major  = 1;
    version.minor  = 2;
    version.patch  = 3;
    ASSERT_TRUE(builder.build(header, version, buffer) == TpotBuilder::Code::SUCCESS);
    ASSERT_LT(0, buffer.size());

    TpotBuilder::Version read_version;
    ASSERT_TRUE(builder.parse(buffer, read_version) == TpotBuilder::Code::SUCCESS);
    ASSERT_EQ(1, read_version.major);
    ASSERT_EQ(2, read_version.minor);
    ASSERT_EQ(3, read_version.patch);
}

TEST(TpotBuilderTest, Command)
{
    char const * EXEC_TEST = "/bin/ls";
    char const * CWD_TEST = "/home/public";
    char const * ARGS_TEST_01 = "-c";
    char const * ARGS_TEST_02 = "output";
    char const * ENV_TEST_01_KEY = "env1";
    char const * ENV_TEST_01_VAL = "01";
    char const * ENV_TEST_02_KEY = "env2";
    char const * ENV_TEST_02_VAL = "01";

    TpotBuilder builder;
    TpotBuilder::Buffer  buffer;
    TpotBuilder::Header  header;
    TpotBuilder::Command command;
    header.request = 0;
    command.exec = EXEC_TEST;
    command.cwd = CWD_TEST;
    command.args.push_back(std::string(ARGS_TEST_01));
    command.args.push_back(std::string(ARGS_TEST_02));
    command.envs.push_back(TpotBuilder::Env{ENV_TEST_01_KEY, ENV_TEST_01_VAL});
    command.envs.push_back(TpotBuilder::Env{ENV_TEST_02_KEY, ENV_TEST_02_VAL});
    ASSERT_TRUE(builder.build(header, command, buffer) == TpotBuilder::Code::SUCCESS);
    ASSERT_LT(0, buffer.size());

    TpotBuilder::Command read_command;
    ASSERT_TRUE(builder.parse(buffer, read_command) == TpotBuilder::Code::SUCCESS);
    ASSERT_STREQ(EXEC_TEST, read_command.exec.c_str());
    ASSERT_STREQ(CWD_TEST, read_command.cwd.c_str());
    ASSERT_EQ(2, read_command.args.size());
    ASSERT_STREQ(ARGS_TEST_01, read_command.args.at(0).c_str());
    ASSERT_STREQ(ARGS_TEST_02, read_command.args.at(1).c_str());
    ASSERT_EQ(2, read_command.envs.size());
    ASSERT_STREQ(ENV_TEST_01_KEY, read_command.envs.at(0).key.c_str());
    ASSERT_STREQ(ENV_TEST_01_VAL, read_command.envs.at(0).value.c_str());
    ASSERT_STREQ(ENV_TEST_02_KEY, read_command.envs.at(1).key.c_str());
    ASSERT_STREQ(ENV_TEST_02_VAL, read_command.envs.at(1).value.c_str());
}

TEST(TpotBuilderTest, Script)
{
    char const * MIME_TEST = "application/tbag";
    char const * BODY_TEST = "test string;";

    TpotBuilder builder;
    TpotBuilder::Buffer buffer;
    TpotBuilder::Header header;
    TpotBuilder::Script script;
    header.request = 0;
    script.mime = MIME_TEST;
    script.body = BODY_TEST;
    ASSERT_TRUE(builder.build(header, script, buffer) == TpotBuilder::Code::SUCCESS);
    ASSERT_LT(0, buffer.size());

    TpotBuilder::Script read_script;
    ASSERT_TRUE(builder.parse(buffer, read_script) == TpotBuilder::Code::SUCCESS);
    ASSERT_STREQ(MIME_TEST, read_script.mime.c_str());
    ASSERT_STREQ(BODY_TEST, read_script.body.c_str());
}

TEST(TpotBuilderTest, Error)
{
    uint32_t const ID_TEST = 99;
    char const * ERROR_TEST = "test error string;";

    TpotBuilder builder;
    TpotBuilder::Buffer buffer;
    TpotBuilder::Header header;
    TpotBuilder::Error error;
    header.request = 0;
    error.id  = ID_TEST;
    error.msg = ERROR_TEST;
    ASSERT_TRUE(builder.build(header, error, buffer) == TpotBuilder::Code::SUCCESS);
    ASSERT_LT(0, buffer.size());

    TpotBuilder::Error read_error;
    ASSERT_TRUE(builder.parse(buffer, read_error) == TpotBuilder::Code::SUCCESS);
    ASSERT_EQ(ID_TEST, read_error.id);
    ASSERT_STREQ(ERROR_TEST, read_error.msg.c_str());
}

