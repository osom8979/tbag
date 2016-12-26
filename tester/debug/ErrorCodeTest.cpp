/**
 * @file   ErrorCodeTest.cpp
 * @brief  ErrorCode enum class tester.
 * @author zer0
 * @date   2016-12-14
 */

#include <gtest/gtest.h>
#include <libtbag/debug/ErrorCode.hpp>

using namespace libtbag;
using namespace libtbag::debug;

TEST(ErrorCodeTest, Default)
{
    libtbag::ErrorCode code;
    ASSERT_NE(nullptr, getErrorMessage(code));
    ASSERT_STREQ(getErrorMessage(libtbag::ErrorCode::SUCCESS), getErrorMessage(0));
}

