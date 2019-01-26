/**
 * @file   TlsTcpTest.cpp
 * @brief  TlsTcp class tester.
 * @author zer0
 * @date   2019-01-26
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/ex/TlsTcp.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::ex;

TEST(TlsTcpTest, Default)
{
    TlsTcp object;
    ASSERT_TRUE(true);
}

