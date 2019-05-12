/**
 * @file   RayGuiLua.cpp
 * @brief  RayGuiLua class implementation.
 * @author zer0
 * @date   2019-05-07
 */

#include <libtbag/script/lua/RayGuiLua.hpp>
#include <libtbag/ray/RayGui.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

static int _InitRayGui(lua_State * L)
{
    lua_pushboolean(L, InitRayGui()?1:0);
    return 1;
}

static int _UpdateRayGui(lua_State * L)
{
    UpdateRayGui();
    return 0;
}

static int _ShutdownRayGui(lua_State * L)
{
    ShutdownRayGui();
    return 0;
}

static int _GuiShowDemoWindow(lua_State * L)
{
    GuiShowDemoWindow();
    return 0;
}

static int _GuiBegin(lua_State * L)
{
    lua_pushboolean(L, GuiBegin(luaL_checkstring(L, 1)));
    return 1;
}

static int _GuiEnd(lua_State * L)
{
    GuiEnd();
    return 0;
}

static int _GuiText(lua_State * L)
{
    GuiText(luaL_checkstring(L, 1));
    return 0;
}

#ifndef RAY_REGISTER
#define RAY_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_lay_gui[] = {
        // [GUI] GUI core.
        RAY_REGISTER(InitRayGui),
        RAY_REGISTER(UpdateRayGui),
        RAY_REGISTER(ShutdownRayGui),

        // [GUI] GUI Demo widgets.
        RAY_REGISTER(GuiShowDemoWindow),

        // [GUI] GUI widgets.
        RAY_REGISTER(GuiBegin),
        RAY_REGISTER(GuiEnd),
        RAY_REGISTER(GuiText),

        { nullptr, nullptr }
};

bool luaopen_raygui(lua_State * L)
{
    luaL_register(L, lua_ray_name(), __lua_lay_gui);
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

