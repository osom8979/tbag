/**
 * @file   DefaultAppTest.cpp
 * @brief  DefaultApp class tester.
 * @author zer0
 * @date   2018-11-02
 */

#include <gtest/gtest.h>
#include <libtbag/app/ex/DefaultApp.hpp>

using namespace libtbag;
using namespace libtbag::app;
using namespace libtbag::app::ex;

TEST(DefaultAppTest, Default)
{
    DefaultApp object;
    ASSERT_TRUE(true);
}

