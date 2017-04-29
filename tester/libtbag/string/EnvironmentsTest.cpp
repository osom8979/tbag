/**
 * @file   EnvironmentsTest.cpp
 * @brief  Environments class tester.
 * @author zer0
 * @date   2017-04-29
 */

#include <gtest/gtest.h>
#include <libtbag/string/Environments.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(EnvironmentsTest, Default)
{
    Flags flags("value --arg1 --arg2=conv");
    Environments envs;

    envs.assign(flags);
    ASSERT_STREQ("conv,,conv", envs.convert("${arg2},${arg1},${arg2}").c_str());
}

