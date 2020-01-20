/**
 * @file   LuaMachineTest.cpp
 * @brief  LuaMachine class tester.
 * @author zer0
 * @date   2019-04-27
 */

#include <gtest/gtest.h>
#include <libtbag/script/LuaMachine.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::script;
using namespace libtbag::util;

TEST(LuaMachineTest, Default)
{
    libtbag::log::SeverityGuard guard;
    LuaMachine lua;
    lua.initDefault();
    ASSERT_EQ(0, lua._gettop());
    ASSERT_EQ(getTbagVersion().toString(), lua.getTbagVersion());
    ASSERT_EQ(0, lua._gettop());
}

TEST(LuaMachineTest, StringLua_format)
{
    libtbag::log::SeverityGuard guard;
    LuaMachine lua;
    lua.initDefault();
    ASSERT_TRUE(lua.isLuaFunction("tbag", "format"));
    char const * const TEST_SCRIPT = "text = tbag.format('Format: {}, {}, {}, {}', 'haha', 100, 1.9, true)";
    char const * const RESULT_TEXT = "Format: haha, 100, 1.9, true";
    ASSERT_TRUE(lua.runScript(TEST_SCRIPT));
    ASSERT_STREQ(RESULT_TEXT, lua.getGlobalString("text").c_str());
}

