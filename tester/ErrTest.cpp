/**
 * @file   ErrTest.cpp
 * @brief  Err enum class tester.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCodeTest -> ErrTest)
 */

#include <gtest/gtest.h>
#include <libtbag/Err.hpp>
#include <libtbag/ErrPair.hpp>

#include <limits>

using namespace libtbag;

TEST(ErrTest, Default)
{
    Err code = static_cast<Err>(std::numeric_limits<int>::max());
    ASSERT_NE(nullptr, getErrName(code));
    ASSERT_NE(nullptr, getErrDetail(code));

    std::cout << "Error code: " << code << std::endl;
}

TEST(ErrTest, ErrMsg)
{
    ErrMsg err;
    ASSERT_EQ(nullptr, err.value);
}

TEST(ErrTest, MakeErrMsg)
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
    ASSERT_FALSE(err4.msg.empty());
    ASSERT_EQ(E_ALREADY, err4.code);
}

