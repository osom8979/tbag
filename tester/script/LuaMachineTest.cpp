/**
 * @file   LuaMachineTest.cpp
 * @brief  LuaMachine class tester.
 * @author zer0
 * @date   2016-04-26
 */

#include <gtest/gtest.h>
#include <libtbag/script/LuaMachine.hpp>

using namespace libtbag;
using namespace libtbag::script;

TEST(LuaMachineTest, Default)
{
    LuaMachine machine;

    ASSERT_TRUE(machine.initialize());

    machine.release();
}

