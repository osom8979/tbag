/**
 * @file   ErrPairTest.cpp
 * @brief  ErrPair class tester.
 * @author zer0
 * @date   2019-09-11
 */

#include <gtest/gtest.h>
#include <libtbag/ErrPair.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <string>
#include <sstream>

using namespace libtbag;

TEST(ErrPairTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(ErrPair<int>, obj1);
    TEST_DEFAULT_ASSIGNMENT(ErrPair<int>, obj2);
}

TEST(ErrPairTest, StringCast)
{
    ErrPair<int> obj(E_ILLARGS);
    ASSERT_STREQ("E_ILLARGS", obj.name());
}

TEST(ErrPairTest, StreamCast)
{
    ErrPair<int> obj(E_ILLARGS);
    std::stringstream ss;
    ss << obj;
    ASSERT_STREQ("E_ILLARGS", ss.str().c_str());
}

TEST(ErrPairTest, BooleanCast)
{
    ErrPair<int> obj1(E_ILLARGS);
    ASSERT_FALSE(static_cast<bool>(obj1));

    ErrPair<int> obj2(E_SUCCESS);
    ASSERT_TRUE(static_cast<bool>(obj2));
}

TEST(ErrPairTest, ErrMsg)
{
    ErrMsg err;
    ASSERT_EQ(nullptr, err.value);
}

TEST(ErrPairTest, MakeErrMsg)
{
    ErrMsgPair<int> err1 = TBAG_MAKE_ERR_MSG("test1", E_UNKNOWN, 100);
    std::cout << "err1 msg: " << err1.msg << std::endl;
    ASSERT_FALSE(err1.msg.empty());
    ASSERT_EQ(E_UNKNOWN, err1.code);
    ASSERT_EQ(100, err1.value);

    ErrMsgPair<std::string> err2 = TBAG_MAKE_ERR_MSG("test2", E_INVALID_ID, "value");
    std::cout << "err2 msg: " << err2.msg << std::endl;
    ASSERT_FALSE(err2.msg.empty());
    ASSERT_EQ(E_INVALID_ID, err2.code);
    ASSERT_STREQ("value", err2.value.c_str());

    ErrMsgPair<double> err3 = TBAG_MAKE_ERR_MSG("test3", E_SUCCESS);
    std::cout << "err3 msg: " << err3.msg << std::endl;
    ASSERT_FALSE(err3.msg.empty());
    ASSERT_EQ(E_SUCCESS, err3.code);

    ErrMsg err4 = TBAG_MAKE_ERR_MSG(E_ALREADY);
    std::cout << "err4 msg: " << err4.msg << std::endl;
    if (isReleaseMode()) {
        ASSERT_TRUE(err4.msg.empty());
    } else {
        ASSERT_FALSE(err4.msg.empty());
    }
    ASSERT_EQ(E_ALREADY, err4.code);
}

TEST(ErrPairTest, ErrFmt)
{
    auto const err = TBAG_ERR_FMT(E_UNKNOWN, "0:{}:{}", 1, "2");
    ASSERT_EQ(E_UNKNOWN, err.code);
    auto const tokens = libtbag::string::splitTokens(err.msg, " ");
    std::string format_message;
    if (isReleaseMode()) {
        ASSERT_EQ(1, tokens.size());
        format_message = tokens[0];
    } else {
        ASSERT_EQ(2, tokens.size());
        format_message = tokens[1];
    }
    ASSERT_STREQ("0:1:2", format_message.c_str());
}

TEST(ErrPairTest, CastErrMsg)
{
    ErrMsgPair<int> err1("test1", E_ILLARGS, 100);
    ASSERT_STREQ("test1", err1.msg.c_str());
    ASSERT_EQ(E_ILLARGS, err1.code);
    ASSERT_EQ(100, err1.value);

    ErrMsg err2 = err1;
    ASSERT_STREQ("test1", err2.msg.c_str());
    ASSERT_EQ(E_ILLARGS, err2.code);

    ErrPair<int> err3 = err1;
    ASSERT_EQ(E_ILLARGS, err3.code);
    ASSERT_EQ(100, err3.value);
}

