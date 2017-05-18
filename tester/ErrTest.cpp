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

TEST(ErrorCodeTest, Default)
{
    libtbag::Err code = static_cast<libtbag::Err>(std::numeric_limits<int>::max());
    ASSERT_NE(nullptr, getErrorMessage(code));
    ASSERT_STREQ(getErrorMessage(libtbag::Err::E_SUCCESS), getErrorMessage(0));
}

