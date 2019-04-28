/**
 * @file   LuaMachineTest.cpp
 * @brief  LuaMachine class tester.
 * @author zer0
 * @date   2019-04-27
 */

#include <gtest/gtest.h>
#include <libtbag/script/LuaMachine.hpp>
#include <libtbag/util/Version.hpp>

using namespace libtbag;
using namespace libtbag::script;

TEST(LuaMachineTest, Default)
{
    LuaMachine lua;
    ASSERT_EQ(0, lua._gettop());
    using namespace libtbag::util;
    ASSERT_EQ(getTbagVersion().toString(), lua.getTbagVersion());
    ASSERT_EQ(LuaMachine::getDefaultLuaPath(), lua.getLuaPath());
    ASSERT_EQ(LuaMachine::getDefaultLuaCPath(), lua.getLuaCPath());
}

