/**
 * @file   ErrTest.cpp
 * @brief  Err enum class tester.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCodeTest -> ErrTest)
 */

#include <gtest/gtest.h>
#include <libtbag/Err.hpp>

#include <limits>

using namespace libtbag;

TEST(ErrTest, Default)
{
    Err code = static_cast<Err>(std::numeric_limits<int>::max());
    ASSERT_NE(nullptr, getErrName(code));
    ASSERT_NE(nullptr, getErrDetail(code));

    std::cout << "Error code: " << code << std::endl;
}

TEST(ErrTest, GetErr)
{
    ASSERT_EQ(E_UNKNOWN, getErr("?"));

    ASSERT_EQ(E_SUCCESS, getErr("E_SUCCESS"));
    ASSERT_EQ(E_WARNING, getErr("E_WARNING"));

    ASSERT_EQ(E_ILLARGS, getErr("ILLARGS"));
    ASSERT_EQ(E_ALREADY, getErr("ALREADY"));
}

