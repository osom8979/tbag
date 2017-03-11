/**
 * @file   ErrorCodeTest.cpp
 * @brief  ErrorCode enum class tester.
 * @author zer0
 * @date   2016-12-14
 */

#include <gtest/gtest.h>
#include <libtbag/debug/ErrorCode.hpp>

#include <limits>

using namespace libtbag;
using namespace libtbag::debug;

TEST(ErrorCodeTest, Default)
{
    libtbag::Err code = static_cast<libtbag::Err>(std::numeric_limits<libtbag::ErrType>::max());
    ASSERT_NE(nullptr, getErrorMessage(code));
    ASSERT_STREQ(getErrorMessage(libtbag::Err::SUCCESS), getErrorMessage(0));
}

