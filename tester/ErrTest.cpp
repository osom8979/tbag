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
}

