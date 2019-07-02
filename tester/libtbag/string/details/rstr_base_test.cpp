/**
 * @file   rstr_base_test.cpp
 * @brief  rstr_base class tester.
 * @author zer0
 * @date   2019-07-02
 */

#include <gtest/gtest.h>
#include <libtbag/string/details/rstr_base.hpp>

using namespace libtbag;
using namespace libtbag::string;
using namespace libtbag::string::details;

TEST(rstr_base_test, Default)
{
    rstr_base object;
    ASSERT_TRUE(true);
}

