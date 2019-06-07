/**
 * @file   RayGuiLua.cpp
 * @brief  RayGuiLua class implementation.
 * @author zer0
 * @date   2019-05-07
 */

#include <libtbag/script/lua/RayGuiLua.hpp>
#include <libtbag/script/lua/RayLua.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/ray/RayGui.hpp>
#include <libtbag/ray/RayGuiExtension.hpp>
#include <libtbag/ray/RayGuiNodes.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

static int _GuiInitRay(lua_State * L)
{
    lua_pushboolean(L, GuiInitRay()?1:0);
    return 1;
}

static int _GuiUpdateRay(lua_State * L)
{
    GuiUpdateRay();
    return 0;
}

static int _GuiShutdownRay(lua_State * L)
{
    GuiShutdownRay();
    return 0;
}

static int _GuiShowDemoWindow(lua_State * L)
{
    GuiShowDemoWindow();
    return 0;
}

static int _GuiShowAboutWindow(lua_State * L)
{
    GuiShowAboutWindow();
    return 0;
}

static int _GuiShowMetricsWindow(lua_State * L)
{
    GuiShowMetricsWindow();
    return 0;
}

static int _GuiShowStyleEditor(lua_State * L)
{
    GuiShowStyleEditor();
    return 0;
}

static int _GuiShowStyleSelector(lua_State * L)
{
    lua_pushboolean(L, GuiShowStyleSelector(luaL_checkstring(L, 1)));
    return 1;
}

static int _GuiShowFontSelector(lua_State * L)
{
    GuiShowFontSelector(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiShowUserGuide(lua_State * L)
{
    GuiShowUserGuide();
    return 0;
}

static int _GuiGetVersion(lua_State * L)
{
    lua_pushstring(L, GuiGetVersion());
    return 1;
}

static int _GuiStyleColorsDark(lua_State * L)
{
    GuiStyleColorsDark();
    return 0;
}

static int _GuiStyleColorsClassic(lua_State * L)
{
    GuiStyleColorsClassic();
    return 0;
}

static int _GuiStyleColorsLight(lua_State * L)
{
    GuiStyleColorsLight();
    return 0;
}

static int _GuiGetStyle(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _GuiSetStyle(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _GuiBegin(lua_State * L)
{
    if (lua_isboolean(L, 2)) {
        bool p_open = luaL_checkboolean(L, 2);
        lua_pushboolean(L, GuiBegin(luaL_checkstring(L, 1), &p_open, luaL_optinteger(L, 3, 0)));
        lua_pushboolean(L, p_open);
        return 2;
    }
    lua_pushboolean(L, GuiBegin(luaL_checkstring(L, 1), nullptr, luaL_optinteger(L, 2, 0)));
    return 1;
}

static int _GuiEnd(lua_State * L)
{
    GuiEnd();
    return 0;
}

static int _GuiBeginChild(lua_State * L)
{
    auto const size = luaE_optvector2(L, 2, Vector2{0, 0});
    auto const border = luaL_optboolean(L, 3, 0);
    auto const flags = luaL_optinteger(L, 4, 0);
    if (lua_isstring(L, lua_absindex(L, 1))) {
        lua_pushboolean(L, GuiBeginChild(luaL_checkstring(L, 1), size, border, flags));
    } else {
        lua_pushboolean(L, GuiBeginChild(luaL_checkinteger(L, 1), size, border, flags));
    }
    return 1;
}

static int _GuiEndChild(lua_State * L)
{
    GuiEndChild();
    return 0;
}

static int _GuiIsWindowAppearing(lua_State * L)
{
    lua_pushboolean(L, GuiIsWindowAppearing());
    return 1;
}

static int _GuiIsWindowCollapsed(lua_State * L)
{
    lua_pushboolean(L, GuiIsWindowCollapsed());
    return 1;
}

static int _GuiIsWindowFocused(lua_State * L)
{
    lua_pushboolean(L, GuiIsWindowFocused(luaL_optinteger(L, 1, 0)));
    return 1;
}

static int _GuiIsWindowHovered(lua_State * L)
{
    lua_pushboolean(L, GuiIsWindowHovered(luaL_optinteger(L, 1, 0)));
    return 1;
}

static int _GuiGetWindowPos(lua_State * L)
{
    luaE_pushvector2(L, GuiGetWindowPos());
    return 1;
}

static int _GuiGetWindowSize(lua_State * L)
{
    luaE_pushvector2(L, GuiGetWindowSize());
    return 1;
}

static int _GuiGetWindowWidth(lua_State * L)
{
    lua_pushnumber(L, GuiGetWindowWidth());
    return 1;
}

static int _GuiGetWindowHeight(lua_State * L)
{
    lua_pushnumber(L, GuiGetWindowHeight());
    return 1;
}

static int _GuiSetNextWindowPos(lua_State * L)
{
    GuiSetNextWindowPos(luaE_checkvector2(L, 1),
                        luaL_optinteger(L, 2, 0),
                        luaE_optvector2(L, 3, Vector2{0, 0}));
    return 0;
}

static int _GuiSetNextWindowSize(lua_State * L)
{
    GuiSetNextWindowSize(luaE_checkvector2(L, 1),
                         luaL_optinteger(L, 2, 0));
    return 0;
}

static int _GuiSetNextWindowContentSize(lua_State * L)
{
    GuiSetNextWindowContentSize(luaE_checkvector2(L, 1));
    return 0;
}

static int _GuiSetNextWindowCollapsed(lua_State * L)
{
    GuiSetNextWindowCollapsed(luaL_checkboolean(L, 1),
                              luaL_optinteger(L, 2, 0));
    return 0;
}

static int _GuiSetNextWindowFocus(lua_State * L)
{
    GuiSetNextWindowFocus();
    return 0;
}

static int _GuiSetNextWindowBgAlpha(lua_State * L)
{
    GuiSetNextWindowBgAlpha(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiSetWindowPos(lua_State * L)
{
    if (lua_isstring(L, lua_absindex(L, 1))) {
        GuiSetWindowPos(luaL_checkstring(L, 1), luaE_checkvector2(L, 2), luaL_optinteger(L, 3, 0));
    } else {
        GuiSetWindowPos(luaE_checkvector2(L, 1), luaL_optinteger(L, 2, 0));
    }
    return 0;
}

static int _GuiSetWindowSize(lua_State * L)
{
    if (lua_isstring(L, lua_absindex(L, 1))) {
        GuiSetWindowSize(luaL_checkstring(L, 1), luaE_checkvector2(L, 2), luaL_optinteger(L, 3, 0));
    } else {
        GuiSetWindowSize(luaE_checkvector2(L, 1), luaL_optinteger(L, 2, 0));
    }
    return 0;
}

static int _GuiSetWindowCollapsed(lua_State * L)
{
    if (lua_isstring(L, lua_absindex(L, 1))) {
        GuiSetWindowCollapsed(luaL_checkstring(L, 1), luaL_checkboolean(L, 2), luaL_optinteger(L, 3, 0));
    } else {
        GuiSetWindowCollapsed(luaL_checkboolean(L, 1), luaL_optinteger(L, 2, 0));
    }
    return 0;
}

static int _GuiSetWindowFocus(lua_State * L)
{
    if (lua_isstring(L, lua_absindex(L, 1))) {
        GuiSetWindowFocus(luaL_checkstring(L, 1));
    } else {
        GuiSetWindowFocus();
    }
    return 0;
}

static int _GuiSetWindowFontScale(lua_State * L)
{
    GuiSetWindowFontScale(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiGetContentRegionMax(lua_State * L)
{
    luaE_pushvector2(L, GuiGetContentRegionMax());
    return 1;
}

static int _GuiGetContentRegionAvail(lua_State * L)
{
    luaE_pushvector2(L, GuiGetContentRegionAvail());
    return 1;
}

static int _GuiGetWindowContentRegionMin(lua_State * L)
{
    luaE_pushvector2(L, GuiGetWindowContentRegionMin());
    return 1;
}

static int _GuiGetWindowContentRegionMax(lua_State * L)
{
    luaE_pushvector2(L, GuiGetWindowContentRegionMax());
    return 1;
}

static int _GuiGetWindowContentRegionWidth(lua_State * L)
{
    lua_pushnumber(L, GuiGetWindowContentRegionWidth());
    return 1;
}

static int _GuiGetScrollX(lua_State * L)
{
    lua_pushnumber(L, GuiGetScrollX());
    return 1;
}

static int _GuiGetScrollY(lua_State * L)
{
    lua_pushnumber(L, GuiGetScrollY());
    return 1;
}

static int _GuiGetScrollMaxX(lua_State * L)
{
    lua_pushnumber(L, GuiGetScrollMaxX());
    return 1;
}

static int _GuiGetScrollMaxY(lua_State * L)
{
    lua_pushnumber(L, GuiGetScrollMaxY());
    return 1;
}

static int _GuiSetScrollX(lua_State * L)
{
    GuiSetScrollX(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiSetScrollY(lua_State * L)
{
    GuiSetScrollY(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiSetScrollHereY(lua_State * L)
{
    GuiSetScrollHereY(luaL_optnumber(L, 1, 0.5f));
    return 0;
}

static int _GuiSetScrollFromPosY(lua_State * L)
{
    GuiSetScrollFromPosY(luaL_checknumber(L, 1), luaL_optnumber(L, 2, 0.5f));
    return 0;
}

static int _GuiPushStyleColor(lua_State * L)
{
    if (lua_isnumber(L, lua_absindex(L, 2))) {
        GuiPushStyleColor(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    } else {
        GuiPushStyleColor(luaL_checkinteger(L, 1), luaE_checkvector4(L, 2));
    }
    return 0;
}

static int _GuiPopStyleColor(lua_State * L)
{
    GuiPopStyleColor(luaL_optinteger(L, 1, 1));
    return 0;
}

static int _GuiPushStyleVar(lua_State * L)
{
    if (lua_isnumber(L, lua_absindex(L, 2))) {
        GuiPushStyleVar(luaL_checkinteger(L, 1), luaL_checknumber(L, 2));
    } else {
        GuiPushStyleVar(luaL_checkinteger(L, 1), luaE_checkvector2(L, 2));
    }
    return 0;
}

static int _GuiPopStyleVar(lua_State * L)
{
    GuiPopStyleVar(luaL_optinteger(L, 1, 1));
    return 0;
}

static int _GuiGetStyleColorVec4(lua_State * L)
{
    luaE_pushvector4(L, GuiGetStyleColorVec4(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiGetFontSize(lua_State * L)
{
    lua_pushnumber(L, GuiGetFontSize());
    return 1;
}

static int _GuiGetFontTexUvWhitePixel(lua_State * L)
{
    luaE_pushvector2(L, GuiGetFontTexUvWhitePixel());
    return 1;
}

static int _GuiGetColorU32WithAlpha(lua_State * L)
{
    lua_pushinteger(L, GuiGetColorU32(luaL_checkinteger(L, 1), luaL_optnumber(L, 2, 1.0f)));
    return 1;
}

static int _GuiGetColorU32(lua_State * L)
{
    if (lua_isnumber(L, 1)) {
        lua_pushinteger(L, GuiGetColorU32((unsigned int)luaL_checkinteger(L, 1)));
    } else {
        lua_pushinteger(L, GuiGetColorU32(luaE_checkvector4(L, 1)));
    }
    return 1;
}

static int _GuiPushItemWidth(lua_State * L)
{
    GuiPushItemWidth(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiPopItemWidth(lua_State * L)
{
    GuiPopItemWidth();
    return 0;
}

static int _GuiSetNextItemWidth(lua_State * L)
{
    GuiSetNextItemWidth(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiCalcItemWidth(lua_State * L)
{
    lua_pushnumber(L, GuiCalcItemWidth());
    return 1;
}

static int _GuiPushTextWrapPos(lua_State * L)
{
    GuiPushTextWrapPos(luaL_optnumber(L, 1, 0.0f));
    return 0;
}

static int _GuiPopTextWrapPos(lua_State * L)
{
    GuiPopTextWrapPos();
    return 0;
}

static int _GuiPushAllowKeyboardFocus(lua_State * L)
{
    GuiPushAllowKeyboardFocus(luaL_checkboolean(L, 1));
    return 0;
}

static int _GuiPopAllowKeyboardFocus(lua_State * L)
{
    GuiPopAllowKeyboardFocus();
    return 0;
}

static int _GuiPushButtonRepeat(lua_State * L)
{
    GuiPushButtonRepeat(luaL_checkboolean(L, 1));
    return 0;
}

static int _GuiPopButtonRepeat(lua_State * L)
{
    GuiPopButtonRepeat();
    return 0;
}

static int _GuiSeparator(lua_State * L)
{
    GuiSeparator();
    return 0;
}

static int _GuiSameLine(lua_State * L)
{
    GuiSameLine(luaL_optnumber(L, 1, 0.0f), luaL_optnumber(L, 2, -1.0f));
    return 0;
}

static int _GuiNewLine(lua_State * L)
{
    GuiNewLine();
    return 0;
}

static int _GuiSpacing(lua_State * L)
{
    GuiSpacing();
    return 0;
}

static int _GuiDummy(lua_State * L)
{
    GuiDummy(luaE_checkvector2(L, 1));
    return 0;
}

static int _GuiIndent(lua_State * L)
{
    GuiIndent(luaL_optnumber(L, 1, 0.0f));
    return 0;
}

static int _GuiUnindent(lua_State * L)
{
    GuiUnindent(luaL_optnumber(L, 1, 0.0f));
    return 0;
}

static int _GuiBeginGroup(lua_State * L)
{
    GuiBeginGroup();
    return 0;
}

static int _GuiEndGroup(lua_State * L)
{
    GuiEndGroup();
    return 0;
}

static int _GuiGetCursorPos(lua_State * L)
{
    luaE_pushvector2(L, GuiGetCursorPos());
    return 1;
}

static int _GuiGetCursorPosX(lua_State * L)
{
    lua_pushnumber(L, GuiGetCursorPosX());
    return 1;
}

static int _GuiGetCursorPosY(lua_State * L)
{
    lua_pushnumber(L, GuiGetCursorPosY());
    return 0;
}

static int _GuiSetCursorPos(lua_State * L)
{
    GuiSetCursorPos(luaE_checkvector2(L, 1));
    return 0;
}

static int _GuiSetCursorPosX(lua_State * L)
{
    GuiSetCursorPosX(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiSetCursorPosY(lua_State * L)
{
    GuiSetCursorPosY(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiGetCursorStartPos(lua_State * L)
{
    luaE_pushvector2(L, GuiGetCursorStartPos());
    return 1;
}

static int _GuiGetCursorScreenPos(lua_State * L)
{
    luaE_pushvector2(L, GuiGetCursorScreenPos());
    return 1;
}

static int _GuiSetCursorScreenPos(lua_State * L)
{
    GuiSetCursorScreenPos(luaE_checkvector2(L, 1));
    return 0;
}

static int _GuiAlignTextToFramePadding(lua_State * L)
{
    GuiAlignTextToFramePadding();
    return 0;
}

static int _GuiGetTextLineHeight(lua_State * L)
{
    lua_pushnumber(L, GuiGetTextLineHeight());
    return 1;
}

static int _GuiGetTextLineHeightWithSpacing(lua_State * L)
{
    lua_pushnumber(L, GuiGetTextLineHeightWithSpacing());
    return 1;
}

static int _GuiGetFrameHeight(lua_State * L)
{
    lua_pushnumber(L, GuiGetFrameHeight());
    return 1;
}

static int _GuiGetFrameHeightWithSpacing(lua_State * L)
{
    lua_pushnumber(L, GuiGetFrameHeightWithSpacing());
    return 1;
}

static int _GuiPushID(lua_State * L)
{
    if (lua_isnumber(L, lua_absindex(L, 1))) {
        GuiPushID(luaL_checkinteger(L, 1));
    } else if (lua_isstring(L, lua_absindex(L, 2))) {
        GuiPushID(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    } else {
        GuiPushID(luaL_checkstring(L, 1));
    }
    return 0;
}

static int _GuiPopID(lua_State * L)
{
    GuiPopID();
    return 0;
}

static int _GuiGetID(lua_State * L)
{
    if (lua_isstring(L, lua_absindex(L, 2))) {
        lua_pushinteger(L, GuiGetID(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    } else {
        lua_pushinteger(L, GuiGetID(luaL_checkstring(L, 1)));
    }
    return 1;
}

static int _GuiTextUnformatted(lua_State * L)
{
    GuiTextUnformatted(luaL_checkstring(L, 1), luaL_optstring(L, 2, nullptr));
    return 0;
}

static int _GuiText(lua_State * L)
{
    GuiText(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiTextColored(lua_State * L)
{
    GuiTextColored(luaE_checkvector4(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiTextDisabled(lua_State * L)
{
    GuiTextDisabled(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiTextWrapped(lua_State * L)
{
    GuiTextWrapped(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiLabelText(lua_State * L)
{
    GuiLabelText(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiBulletText(lua_State * L)
{
    GuiBulletText(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiButton(lua_State * L)
{
    lua_pushboolean(L, GuiButton(luaL_checkstring(L, 1), luaE_optvector2(L, 2, Vector2{0,0})));
    return 1;
}

static int _GuiSmallButton(lua_State * L)
{
    lua_pushboolean(L, GuiSmallButton(luaL_checkstring(L, 1)));
    return 1;
}

static int _GuiInvisibleButton(lua_State * L)
{
    lua_pushboolean(L, GuiInvisibleButton(luaL_checkstring(L, 1), luaE_checkvector2(L, 2)));
    return 1;
}

static int _GuiArrowButton(lua_State * L)
{
    lua_pushboolean(L, GuiArrowButton(luaL_checkstring(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _GuiImage(lua_State * L)
{
    GuiImage((GuiTextureID)luaL_checkinteger(L, 1),
             luaE_checkvector2(L, 2),
             luaE_optvector2(L, 3, Vector2{0,0}),
             luaE_optvector2(L, 4, Vector2{1,1}),
             luaE_optvector4(L, 5, Vector4{1,1,1,1}),
             luaE_optvector4(L, 6, Vector4{0,0,0,0}));
    return 0;
}

static int _GuiImageButton(lua_State * L)
{
    lua_pushboolean(L, GuiImageButton((GuiTextureID)luaL_checkinteger(L, 1),
                                      luaE_checkvector2(L, 2),
                                      luaE_optvector2(L, 3, Vector2{0,0}),
                                      luaE_optvector2(L, 4, Vector2{1,1}),
                                      luaL_optinteger(L, 5, -1),
                                      luaE_optvector4(L, 6, Vector4{0,0,0,0}),
                                      luaE_optvector4(L, 7, Vector4{1,1,1,1})));
    return 1;
}

static int _GuiCheckbox(lua_State * L)
{
    bool v = luaL_checkboolean(L, 2);
    lua_pushboolean(L, GuiCheckbox(luaL_checkstring(L, 1), &v));
    lua_pushboolean(L, v);
    return 2;
}

static int _GuiCheckboxFlags(lua_State * L)
{
    unsigned int flags = luaL_checkinteger(L, 2);
    lua_pushboolean(L, GuiCheckboxFlags(luaL_checkstring(L, 1),
                                        &flags,
                                        luaL_checkinteger(L, 3)));
    lua_pushinteger(L, flags);
    return 2;
}

static int _GuiRadioButton(lua_State * L)
{
    if (lua_isboolean(L, 2)) {
        lua_pushboolean(L, GuiRadioButton(luaL_checkstring(L, 1), luaL_checkboolean(L, 2)));
        return 1;
    } else {
        int v = luaL_checkboolean(L, 2);
        lua_pushboolean(L, GuiRadioButton(luaL_checkstring(L, 1),
                                          &v,
                                          luaL_checkinteger(L, 3)));
        lua_pushinteger(L, v);
        return 2;
    }
}

static int _GuiProgressBar(lua_State * L)
{
    GuiProgressBar(luaL_checknumber(L, 1),
                   luaE_optvector2(L, 2, Vector2{-1,0}),
                   luaL_optstring(L, 3, nullptr));
    return 0;
}

static int _GuiBullet(lua_State * L)
{
    GuiBullet();
    return 0;
}

static int _GuiBeginCombo(lua_State * L)
{
    lua_pushboolean(L, GuiBeginCombo(luaL_checkstring(L, 1),
                                     luaL_checkstring(L, 2),
                                     luaL_optinteger(L, 3, 0)));
    return 1;
}

static int _GuiEndCombo(lua_State * L)
{
    GuiEndCombo();
    return 0;
}

static bool _GuiCombo_items_getter(void * data, int idx, char const ** out_text)
{
    auto * L = (lua_State*)data;
    assert(L != nullptr);

    lua_pushvalue(L, 3); // Duplicate callback function.
    lua_pushnumber(L, idx);
    auto const CODE = lua_pcall(L, 1, 2, 0);

    bool result1 = false;
    char const * result2 = nullptr;
    switch (CODE) {
    case 0: // LUA_OK
        result1 = luaL_checkboolean(L, -2);
        result2 = luaL_checkstring(L, -1);
        lua_pop(L, 2);
        break;
    case LUA_ERRRUN:
        TBAG_FALLTHROUGH
    case LUA_ERRMEM:
        TBAG_FALLTHROUGH
    case LUA_ERRERR:
        TBAG_FALLTHROUGH
    default:
        char const * msg = luaL_checkstring(L, -1);
        tDLogW("_GuiCombo_items_getter() lua_pcall() error({}): {}", CODE, msg);
        lua_pop(L, 1);
        break;
    }

    if (result2) {
        *out_text = result2;
    }
    return result1;
}

static int _GuiCombo(lua_State * L)
{
    auto * label = luaL_checkstring(L, 1);
    int current_item = luaL_checkinteger(L, 2);
    if (lua_isfunction(L, 3)) {
        lua_pushboolean(L, GuiCombo(label, &current_item, &_GuiCombo_items_getter, (void*)L,
                                    luaL_checkinteger(L, 4),
                                    luaL_optinteger(L, 4, -1)));
    } else {
        auto items_strings = luaE_checkstring_array(L, 3);
        std::vector<char const *> items;
        for (auto & item : items_strings) {
            items.push_back(item.c_str());
        }
        lua_pushboolean(L, GuiCombo(label, &current_item, items.data(), items.size(), luaL_optinteger(L, 4, -1)));
    }
    lua_pushinteger(L, current_item);
    return 2;
}

static int _GuiDragFloat(lua_State * L)
{
    float v = luaL_checknumber(L, 2);
    lua_pushboolean(L, GuiDragFloat(luaL_checkstring(L, 1), &v,
                                    luaL_optnumber(L, 3, 1.0f),
                                    luaL_optnumber(L, 4, 0.0f),
                                    luaL_optnumber(L, 5, 0.0f),
                                    luaL_optstring(L, 6, "%.3f"),
                                    luaL_optnumber(L, 7, 1.0f)));
    lua_pushnumber(L, v);
    return 2;
}

static int _GuiDragFloat2(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiDragFloat2(luaL_checkstring(L, 1), v,
                                     luaL_optnumber(L, 3, 1.0f),
                                     luaL_optnumber(L, 4, 0.0f),
                                     luaL_optnumber(L, 5, 0.0f),
                                     luaL_optstring(L, 6, "%.3f"),
                                     luaL_optnumber(L, 7, 1.0f)));
    lua_Number v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushnumber_array(L, v_result, 2);
    return 2;
}

static int _GuiDragFloat3(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiDragFloat3(luaL_checkstring(L, 1), v,
                                     luaL_optnumber(L, 3, 1.0f),
                                     luaL_optnumber(L, 4, 0.0f),
                                     luaL_optnumber(L, 5, 0.0f),
                                     luaL_optstring(L, 6, "%.3f"),
                                     luaL_optnumber(L, 7, 1.0f)));
    lua_Number v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushnumber_array(L, v_result, 3);
    return 2;
}

static int _GuiDragFloat4(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiDragFloat4(luaL_checkstring(L, 1), v,
                                     luaL_optnumber(L, 3, 1.0f),
                                     luaL_optnumber(L, 4, 0.0f),
                                     luaL_optnumber(L, 5, 0.0f),
                                     luaL_optstring(L, 6, "%.3f"),
                                     luaL_optnumber(L, 7, 1.0f)));
    lua_Number v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushnumber_array(L, v_result, 4);
    return 2;
}

static int _GuiDragFloatRange2(lua_State * L)
{
    float v_current_min = luaL_checknumber(L, 2);
    float v_current_max = luaL_checknumber(L, 3);
    lua_pushboolean(L, GuiDragFloatRange2(luaL_checkstring(L, 1), &v_current_min, &v_current_max,
                                          luaL_optnumber(L, 4, 1.0f),
                                          luaL_optnumber(L, 5, 0.0f),
                                          luaL_optnumber(L, 6, 0.0f),
                                          luaL_optstring(L, 7, "%.3f"),
                                          luaL_optstring(L, 8, nullptr),
                                          luaL_optnumber(L, 9, 1.0f)));
    lua_pushnumber(L, v_current_min);
    lua_pushnumber(L, v_current_max);
    return 3;
}

static int _GuiDragInt(lua_State * L)
{
    int v = luaL_checkinteger(L, 2);
    lua_pushboolean(L, GuiDragInt(luaL_checkstring(L, 1), &v,
                                  luaL_optnumber(L, 3, 1.0f),
                                  luaL_optinteger(L, 4, 0),
                                  luaL_optinteger(L, 5, 0),
                                  luaL_optstring(L, 6, "%d")));
    lua_pushinteger(L, v);
    return 2;
}

static int _GuiDragInt2(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiDragInt2(luaL_checkstring(L, 1), v,
                                   luaL_optnumber(L, 3, 1.0f),
                                   luaL_optinteger(L, 4, 0),
                                   luaL_optinteger(L, 5, 0),
                                   luaL_optstring(L, 6, "%d")));
    lua_Integer v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushinteger_array(L, v_result, 2);
    return 2;
}

static int _GuiDragInt3(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiDragInt3(luaL_checkstring(L, 1), v,
                                   luaL_optnumber(L, 3, 1.0f),
                                   luaL_optinteger(L, 4, 0),
                                   luaL_optinteger(L, 5, 0),
                                   luaL_optstring(L, 6, "%d")));
    lua_Integer v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushinteger_array(L, v_result, 3);
    return 2;
}

static int _GuiDragInt4(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiDragInt4(luaL_checkstring(L, 1), v,
                                   luaL_optnumber(L, 3, 1.0f),
                                   luaL_optinteger(L, 4, 0),
                                   luaL_optinteger(L, 5, 0),
                                   luaL_optstring(L, 6, "%d")));
    lua_Integer v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushinteger_array(L, v_result, 4);
    return 2;
}

static int _GuiDragIntRange2(lua_State * L)
{
    int v_current_min = luaL_checkinteger(L, 2);
    int v_current_max = luaL_checkinteger(L, 3);
    lua_pushboolean(L, GuiDragIntRange2(luaL_checkstring(L, 1), &v_current_min, &v_current_max,
                                        luaL_optnumber(L, 4, 1.0f),
                                        luaL_optinteger(L, 5, 0),
                                        luaL_optinteger(L, 6, 0),
                                        luaL_optstring(L, 7, "%d"),
                                        luaL_optstring(L, 8, nullptr)));
    lua_pushinteger(L, v_current_min);
    lua_pushinteger(L, v_current_max);
    return 3;
}

static int _GuiSliderFloat(lua_State * L)
{
    float v = luaL_checknumber(L, 2);
    lua_pushboolean(L, GuiSliderFloat(luaL_checkstring(L, 1), &v,
                                      luaL_checknumber(L, 3),
                                      luaL_checknumber(L, 4),
                                      luaL_optstring(L, 5, "%.3f"),
                                      luaL_optnumber(L, 6, 1.0f)));
    lua_pushnumber(L, v);
    return 2;
}

static int _GuiSliderFloat2(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiSliderFloat2(luaL_checkstring(L, 1), v,
                                       luaL_checknumber(L, 3),
                                       luaL_checknumber(L, 4),
                                       luaL_optstring(L, 5, "%.3f"),
                                       luaL_optnumber(L, 6, 1.0f)));
    lua_Number v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushnumber_array(L, v_result, 2);
    return 2;
}

static int _GuiSliderFloat3(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiSliderFloat3(luaL_checkstring(L, 1), v,
                                       luaL_checknumber(L, 3),
                                       luaL_checknumber(L, 4),
                                       luaL_optstring(L, 5, "%.3f"),
                                       luaL_optnumber(L, 6, 1.0f)));
    lua_Number v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushnumber_array(L, v_result, 3);
    return 2;
}

static int _GuiSliderFloat4(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiSliderFloat4(luaL_checkstring(L, 1), v,
                                       luaL_checknumber(L, 3),
                                       luaL_checknumber(L, 4),
                                       luaL_optstring(L, 5, "%.3f"),
                                       luaL_optnumber(L, 6, 1.0f)));
    lua_Number v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushnumber_array(L, v_result, 4);
    return 2;
}

static int _GuiSliderAngle(lua_State * L)
{
    float v_rad = luaL_checknumber(L, 2);
    lua_pushboolean(L, GuiSliderAngle(luaL_checkstring(L, 1), &v_rad,
                                      luaL_optnumber(L, 3, -360.0f),
                                      luaL_optnumber(L, 4, +360.0f),
                                      luaL_optstring(L, 5, "%.3f deg")));
    lua_pushnumber(L, v_rad);
    return 2;
}

static int _GuiSliderInt(lua_State * L)
{
    int v = luaL_checkinteger(L, 2);
    lua_pushboolean(L, GuiSliderInt(luaL_checkstring(L, 1), &v,
                                    luaL_checkinteger(L, 3),
                                    luaL_checkinteger(L, 4),
                                    luaL_optstring(L, 5, "%d")));
    lua_pushinteger(L, v);
    return 2;
}

static int _GuiSliderInt2(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiSliderInt2(luaL_checkstring(L, 1), v,
                                     luaL_checkinteger(L, 3),
                                     luaL_checkinteger(L, 4),
                                     luaL_optstring(L, 5, "%d")));
    lua_Integer v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushinteger_array(L, v_result, 2);
    return 2;
}

static int _GuiSliderInt3(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiSliderInt3(luaL_checkstring(L, 1), v,
                                     luaL_checkinteger(L, 3),
                                     luaL_checkinteger(L, 4),
                                     luaL_optstring(L, 5, "%d")));
    lua_Integer v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushinteger_array(L, v_result, 3);
    return 2;
}

static int _GuiSliderInt4(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiSliderInt4(luaL_checkstring(L, 1), v,
                                     luaL_checkinteger(L, 3),
                                     luaL_checkinteger(L, 4),
                                     luaL_optstring(L, 5, "%d")));
    lua_Integer v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushinteger_array(L, v_result, 4);
    return 2;
}

static int _GuiVSliderFloat(lua_State * L)
{
    float v = luaL_checknumber(L, 3);
    lua_pushboolean(L, GuiVSliderFloat(luaL_checkstring(L, 1),
                                       luaE_checkvector2(L, 2),
                                       &v,
                                       luaL_checknumber(L, 4),
                                       luaL_checknumber(L, 5),
                                       luaL_optstring(L, 6, "%.3f"),
                                       luaL_optnumber(L, 7, 1.0f)));
    lua_pushnumber(L, v);
    return 2;
}

static int _GuiVSliderInt(lua_State * L)
{
    int v = luaL_checkinteger(L, 3);
    lua_pushboolean(L, GuiVSliderInt(luaL_checkstring(L, 1),
                                     luaE_checkvector2(L, 2),
                                     &v,
                                     luaL_checkinteger(L, 4),
                                     luaL_checkinteger(L, 5),
                                     luaL_optstring(L, 6, "%d")));
    lua_pushinteger(L, v);
    return 2;
}

static int _GuiInputText(lua_State * L)
{
    std::vector<char> buffer(luaL_checkinteger(L, 2) + 1, '\0');
    lua_pushboolean(L, GuiInputText(luaL_checkstring(L, 1),
                                    buffer.data(), buffer.size(),
                                    luaL_optinteger(L, 3, 0)));
    lua_pushstring(L, buffer.data());
    return 2;
}

static int _GuiInputTextMultiline(lua_State * L)
{
    std::vector<char> buffer(luaL_checkinteger(L, 2) + 1, '\0');
    lua_pushboolean(L, GuiInputTextMultiline(luaL_checkstring(L, 1),
                                             buffer.data(), buffer.size(),
                                             luaE_optvector2(L, 3, Vector2{0,0}),
                                             luaL_optinteger(L, 4, 0)));
    lua_pushstring(L, buffer.data());
    return 2;
}

static int _GuiInputTextWithHint(lua_State * L)
{
    std::vector<char> buffer(luaL_checkinteger(L, 3) + 1, '\0');
    lua_pushboolean(L, GuiInputTextWithHint(luaL_checkstring(L, 1),
                                            luaL_checkstring(L, 2),
                                            buffer.data(), buffer.size(),
                                            luaL_optinteger(L, 4, 0)));
    lua_pushstring(L, buffer.data());
    return 2;
}

static int _GuiInputFloat(lua_State * L)
{
    float v = luaL_checknumber(L, 2);
    lua_pushboolean(L, GuiInputFloat(luaL_checkstring(L, 1), &v,
                                     luaL_optnumber(L, 3, 0.0f),
                                     luaL_optnumber(L, 4, 0.0f),
                                     luaL_optstring(L, 5, "%.3f"),
                                     luaL_optinteger(L, 6, 0)));
    lua_pushnumber(L, v);
    return 2;
}

static int _GuiInputFloat2(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiInputFloat2(luaL_checkstring(L, 1), v,
                                      luaL_optstring(L, 3, "%.3f"),
                                      luaL_optinteger(L, 4, 0)));
    lua_Number v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushnumber_array(L, v_result, 2);
    return 2;
}

static int _GuiInputFloat3(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiInputFloat3(luaL_checkstring(L, 1), v,
                                      luaL_optstring(L, 3, "%.3f"),
                                      luaL_optinteger(L, 4, 0)));
    lua_Number v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushnumber_array(L, v_result, 3);
    return 2;
}

static int _GuiInputFloat4(lua_State * L)
{
    auto v_array = luaE_checknumber_array(L, 2);
    float v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiInputFloat4(luaL_checkstring(L, 1), v,
                                      luaL_optstring(L, 3, "%.3f"),
                                      luaL_optinteger(L, 4, 0)));
    lua_Number v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushnumber_array(L, v_result, 4);
    return 2;
}

static int _GuiInputInt(lua_State * L)
{
    int v = luaL_checkinteger(L, 2);
    lua_pushboolean(L, GuiInputInt(luaL_checkstring(L, 1), &v,
                                   luaL_optinteger(L, 3, 1),
                                   luaL_optinteger(L, 4, 100),
                                   luaL_optinteger(L, 5, 0)));
    lua_pushinteger(L, v);
    return 2;
}

static int _GuiInputInt2(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[2];
    v[0] = v_array[0];
    v[1] = v_array[1];
    lua_pushboolean(L, GuiInputInt2(luaL_checkstring(L, 1), v, luaL_optinteger(L, 3, 0)));
    lua_Integer v_result[2];
    v_result[0] = v[0];
    v_result[1] = v[1];
    luaE_pushinteger_array(L, v_result, 2);
    return 2;
}

static int _GuiInputInt3(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[3];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    lua_pushboolean(L, GuiInputInt3(luaL_checkstring(L, 1), v, luaL_optinteger(L, 3, 0)));
    lua_Integer v_result[3];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    luaE_pushinteger_array(L, v_result, 3);
    return 2;
}

static int _GuiInputInt4(lua_State * L)
{
    auto v_array = luaE_checkinteger_array(L, 2);
    int v[4];
    v[0] = v_array[0];
    v[1] = v_array[1];
    v[2] = v_array[2];
    v[3] = v_array[3];
    lua_pushboolean(L, GuiInputInt4(luaL_checkstring(L, 1), v, luaL_optinteger(L, 3, 0)));
    lua_Integer v_result[4];
    v_result[0] = v[0];
    v_result[1] = v[1];
    v_result[2] = v[2];
    v_result[3] = v[3];
    luaE_pushinteger_array(L, v_result, 4);
    return 2;
}

static int _GuiInputDouble(lua_State * L)
{
    double v = luaL_checknumber(L, 2);
    lua_pushboolean(L, GuiInputDouble(luaL_checkstring(L, 1), &v,
                                      luaL_optnumber(L, 3, 0.0f),
                                      luaL_optnumber(L, 4, 0.0f),
                                      luaL_optstring(L, 5, "%.6f"),
                                      luaL_optinteger(L, 6, 0)));
    lua_pushnumber(L, v);
    return 2;
}

static int _GuiColorEdit3(lua_State * L)
{
    auto col_vector3 = luaE_checkvector3(L, 2);
    float col[3] = { col_vector3.x, col_vector3.y, col_vector3.z };
    lua_pushboolean(L, GuiColorEdit3(luaL_checkstring(L, 1), col, luaL_optinteger(L, 3, 0)));
    luaE_pushvector3(L, Vector3{col[0],col[1],col[2]});
    return 2;
}

static int _GuiColorEdit4(lua_State * L)
{
    auto col_vector4 = luaE_checkvector4(L, 2);
    float col[4] = { col_vector4.x, col_vector4.y, col_vector4.z, col_vector4.w };
    lua_pushboolean(L, GuiColorEdit4(luaL_checkstring(L, 1), col, luaL_optinteger(L, 3, 0)));
    luaE_pushvector4(L, Vector4{col[0],col[1],col[2],col[3]});
    return 2;
}

static int _GuiColorPicker3(lua_State * L)
{
    auto col_vector3 = luaE_checkvector3(L, 2);
    float col[3] = { col_vector3.x, col_vector3.y, col_vector3.z };
    lua_pushboolean(L, GuiColorPicker3(luaL_checkstring(L, 1), col, luaL_optinteger(L, 3, 0)));
    luaE_pushvector3(L, Vector3{col[0],col[1],col[2]});
    return 2;
}

static int _GuiColorPicker4(lua_State * L)
{
    auto * label = luaL_checkstring(L, 1);
    auto col_vector4 = luaE_checkvector4(L, 2);
    float col[4] = { col_vector4.x, col_vector4.y, col_vector4.z, col_vector4.w };
    auto flags = luaL_optinteger(L, 3, 0);
    if (lua_istable(L, 4)) {
        auto ref_col_vector4 = luaE_checkvector4(L, 4);
        float ref_col[4] = { ref_col_vector4.x, ref_col_vector4.y, ref_col_vector4.z, ref_col_vector4.w };
        lua_pushboolean(L, GuiColorPicker4(label, col, flags, ref_col));
    } else {
        lua_pushboolean(L, GuiColorPicker4(label, col, flags, nullptr));
    }
    luaE_pushvector4(L, Vector4{col[0],col[1],col[2],col[3]});
    return 2;
}

static int _GuiColorButton(lua_State * L)
{
    lua_pushboolean(L, GuiColorButton(luaL_checkstring(L, 1),
                                      luaE_checkvector4(L, 2),
                                      luaL_optinteger(L, 3, 0),
                                      luaE_optvector2(L, 4, Vector2{0,0})));
    return 1;
}

static int _GuiSetColorEditOptions(lua_State * L)
{
    GuiSetColorEditOptions(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiTreeNode(lua_State * L)
{
    if (lua_isstring(L, 2)) {
        lua_pushboolean(L, GuiTreeNode(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    } else {
        lua_pushboolean(L, GuiTreeNode(luaL_checkstring(L, 1)));
    }
    return 1;
}

static int _GuiTreeNodeEx(lua_State * L)
{
    if (lua_isstring(L, 3)) {
        lua_pushboolean(L, GuiTreeNodeEx(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_checkstring(L, 3)));
    } else {
        lua_pushboolean(L, GuiTreeNodeEx(luaL_checkstring(L, 1), luaL_optinteger(L, 2, 0)));
    }
    return 1;
}

static int _GuiTreePush(lua_State * L)
{
    GuiTreePush(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiTreePushNull(lua_State * L)
{
    GuiTreePush();
    return 0;
}

static int _GuiTreePop(lua_State * L)
{
    GuiTreePop();
    return 0;
}

static int _GuiTreeAdvanceToLabelPos(lua_State * L)
{
    GuiTreeAdvanceToLabelPos();
    return 0;
}

static int _GuiGetTreeNodeToLabelSpacing(lua_State * L)
{
    lua_pushnumber(L, GuiGetTreeNodeToLabelSpacing());
    return 1;
}

static int _GuiSetNextTreeNodeOpen(lua_State * L)
{
    GuiSetNextTreeNodeOpen(luaL_checkboolean(L, 1), luaL_optinteger(L, 2, 0));
    return 0;
}

static int _GuiCollapsingHeader(lua_State * L)
{
    if (lua_isboolean(L, 2)) {
        bool p_open = luaL_checkboolean(L, 2);
        lua_pushboolean(L, GuiCollapsingHeader(luaL_checkstring(L, 1), &p_open, luaL_optinteger(L, 3, 0)));
        lua_pushboolean(L, p_open);
        return 2;
    } else {
        lua_pushboolean(L, GuiCollapsingHeader(luaL_checkstring(L, 1), luaL_optinteger(L, 2, 0)));
        return 1;
    }
}

static int _GuiSelectable(lua_State * L)
{
    lua_pushboolean(L, GuiSelectable(luaL_checkstring(L, 1),
                                     luaL_optboolean(L, 2, 0),
                                     luaL_optinteger(L, 3, 0),
                                     luaE_optvector2(L, 4, Vector2{0,0})));
    return 1;
}

static int _GuiSelectable2(lua_State * L)
{
    bool p_selected = luaL_checkboolean(L, 2);
    lua_pushboolean(L, GuiSelectable(luaL_checkstring(L, 1),
                                     &p_selected,
                                     luaL_optinteger(L, 3, 0),
                                     luaE_optvector2(L, 4, Vector2{0,0})));
    lua_pushboolean(L, p_selected);
    return 2;
}

static bool _GuiListBox_items_getter(void * data, int idx, char const ** out_text)
{
    auto * L = (lua_State*)data;
    assert(L != nullptr);

    lua_pushvalue(L, 2); // Duplicate callback function.
    lua_pushnumber(L, idx);
    auto const CODE = lua_pcall(L, 1, 2, 0);

    bool result1 = false;
    char const * result2 = nullptr;
    switch (CODE) {
    case 0: // LUA_OK
        result1 = luaL_checkboolean(L, -2);
        result2 = luaL_checkstring(L, -1);
        lua_pop(L, 2);
        break;
    case LUA_ERRRUN:
        TBAG_FALLTHROUGH
    case LUA_ERRMEM:
        TBAG_FALLTHROUGH
    case LUA_ERRERR:
        TBAG_FALLTHROUGH
    default:
        char const * msg = luaL_checkstring(L, -1);
        tDLogW("_GuiListBox_items_getter() lua_pcall() error({}): {}", CODE, msg);
        lua_pop(L, 1);
        break;
    }

    if (result2) {
        *out_text = result2;
    }
    return result1;
}

static int _GuiListBox(lua_State * L)
{
    char const * label = luaL_checkstring(L, 1);
    assert(label != nullptr);
    int current_item = luaL_checkinteger(L, 2);

    if (lua_isfunction(L, 2)) {
        lua_pushboolean(L, GuiListBox(label, &current_item, &_GuiListBox_items_getter, (void*)L,
                                      luaL_checkinteger(L, 3),
                                      luaL_optinteger(L, 4, -1)));
        lua_pushinteger(L, current_item);
        return 2;
    } else {
        auto items_strings = luaE_checkstring_array(L, 3);
        std::vector<char const *> items;
        for (auto & item : items_strings) {
            items.push_back(item.c_str());
        }

        lua_pushboolean(L, GuiListBox(label, &current_item, items.data(), items.size(),
                                      luaL_optinteger(L, 4, -1)));
        lua_pushinteger(L, current_item);
        return 2;
    }
}

static int _GuiListBoxHeader(lua_State * L)
{
    if (lua_isinteger(L, 2)) {
        lua_pushboolean(L, GuiListBoxHeader(luaL_checkstring(L, 1),
                                            luaL_checkinteger(L, 2),
                                            luaL_optinteger(L, 3, -1)));
    } else {
        lua_pushboolean(L, GuiListBoxHeader(luaL_checkstring(L, 1),
                                            luaE_optvector2(L, 2, Vector2{0,0})));
    }
    return 1;
}

static int _GuiListBoxFooter(lua_State * L)
{
    GuiListBoxFooter();
    return 0;
}

static float _GuiPlotLines_values_getter(void * data, int idx)
{
    auto * L = (lua_State*)data;
    assert(L != nullptr);

    lua_pushvalue(L, 2); // Duplicate callback function.
    lua_pushnumber(L, idx);
    auto const CODE = lua_pcall(L, 1, 1, 0);

    float result = 0.0f;
    switch (CODE) {
    case 0: // LUA_OK
        result = luaL_checknumber(L, -1);
        lua_pop(L, 1);
        break;
    case LUA_ERRRUN:
        TBAG_FALLTHROUGH
    case LUA_ERRMEM:
        TBAG_FALLTHROUGH
    case LUA_ERRERR:
        TBAG_FALLTHROUGH
    default:
        char const * msg = luaL_checkstring(L, -1);
        tDLogW("_GuiPlotLines_values_getter() lua_pcall() error({}): {}", CODE, msg);
        lua_pop(L, 1);
        break;
    }

    return result;
}

static int _GuiPlotLines(lua_State * L)
{
    char const * label = luaL_checkstring(L, 1);
    assert(label != nullptr);

    if (lua_isfunction(L, 2)) {
        GuiPlotLines(label, &_GuiPlotLines_values_getter, (void*)L,
                     luaL_checkinteger(L, 3),
                     luaL_optinteger(L, 4, 0),
                     luaL_optstring(L, 5, nullptr),
                     luaL_optnumber(L, 6, FLT_MAX),
                     luaL_optnumber(L, 7, FLT_MAX),
                     luaE_optvector2(L, 8, Vector2{0,0}));
    } else {
        auto values_number = luaE_checknumber_array(L, 2);
        auto values = std::vector<float>(values_number.begin(), values_number.end());

        GuiPlotLines(label, values.data(), values.size(),
                     luaL_optinteger(L, 3, 0),
                     luaL_optstring(L, 4, nullptr),
                     luaL_optnumber(L, 5, FLT_MAX),
                     luaL_optnumber(L, 6, FLT_MAX),
                     luaE_optvector2(L, 7, Vector2{0,0}),
                     luaL_optinteger(L, 8, sizeof(float)));
    }
    return 0;
}

static float _GuiPlotHistogram_values_getter(void * data, int idx)
{
    auto * L = (lua_State*)data;
    assert(L != nullptr);

    lua_pushvalue(L, 2); // Duplicate callback function.
    lua_pushnumber(L, idx);
    auto const CODE = lua_pcall(L, 1, 1, 0);

    float result = 0.0f;
    switch (CODE) {
    case 0: // LUA_OK
        result = luaL_checknumber(L, -1);
        lua_pop(L, 1);
        break;
    case LUA_ERRRUN:
        TBAG_FALLTHROUGH
    case LUA_ERRMEM:
        TBAG_FALLTHROUGH
    case LUA_ERRERR:
        TBAG_FALLTHROUGH
    default:
        char const * msg = luaL_checkstring(L, -1);
        tDLogW("_GuiPlotHistogram_values_getter() lua_pcall() error({}): {}", CODE, msg);
        lua_pop(L, 1);
        break;
    }

    return result;
}

/**
 * @code{.lua}
 *  function saw(idx)
 *      if idx % 2 == 0 then
 *          return 1.0
 *      else
 *          return -1.0
 *      end
 *  end
 *  -- ...
 *  if tbag.GuiBegin('Window') then
 *      tbag.GuiPlotHistogram('Histogram', saw, 70, 0, '', -1.0, 1.0, {0,80})
 *  end
 *  tbag.GuiEnd()
 * @endcode
 */
static int _GuiPlotHistogram(lua_State * L)
{
    char const * label = luaL_checkstring(L, 1);
    assert(label != nullptr);

    if (lua_isfunction(L, 2)) {
        GuiPlotHistogram(label, &_GuiPlotHistogram_values_getter, (void*)L,
                         luaL_checkinteger(L, 3),
                         luaL_optinteger(L, 4, 0),
                         luaL_optstring(L, 5, nullptr),
                         luaL_optnumber(L, 6, FLT_MAX),
                         luaL_optnumber(L, 7, FLT_MAX),
                         luaE_optvector2(L, 8, Vector2{0,0}));
    } else {
        auto values_number = luaE_checknumber_array(L, 2);
        auto values = std::vector<float>(values_number.begin(), values_number.end());

        GuiPlotHistogram(label, values.data(), values.size(),
                         luaL_optinteger(L, 3, 0),
                         luaL_optstring(L, 4, nullptr),
                         luaL_optnumber(L, 5, FLT_MAX),
                         luaL_optnumber(L, 6, FLT_MAX),
                         luaE_optvector2(L, 7, Vector2{0,0}),
                         luaL_optinteger(L, 8, sizeof(float)));
    }
    return 0;
}

static int _GuiValueBool(lua_State * L)
{
    GuiValue(luaL_checkstring(L, 1), luaL_checkboolean(L, 2));
    return 0;
}

static int _GuiValueInt(lua_State * L)
{
    GuiValue(luaL_checkstring(L, 1), (int)luaL_checkinteger(L, 2));
    return 0;
}

static int _GuiValueUnsigned(lua_State * L)
{
    GuiValue(luaL_checkstring(L, 1), (unsigned)luaL_checkinteger(L, 2));
    return 0;
}

static int _GuiValueFloat(lua_State * L)
{
    GuiValue(luaL_checkstring(L, 1), (float)luaL_checknumber(L, 2), luaL_optstring(L, 3, nullptr));
    return 0;
}

static int _GuiBeginMainMenuBar(lua_State * L)
{
    lua_pushboolean(L, GuiBeginMainMenuBar());
    return 1;
}

static int _GuiEndMainMenuBar(lua_State * L)
{
    GuiEndMainMenuBar();
    return 0;
}

static int _GuiBeginMenuBar(lua_State * L)
{
    lua_pushboolean(L, GuiBeginMenuBar());
    return 1;
}

static int _GuiEndMenuBar(lua_State * L)
{
    GuiEndMenuBar();
    return 0;
}

static int _GuiBeginMenu(lua_State * L)
{
    lua_pushboolean(L, GuiBeginMenu(luaL_checkstring(L, 1), luaL_optboolean(L, 2, 1)));
    return 1;
}

static int _GuiEndMenu(lua_State * L)
{
    GuiEndMenu();
    return 0;
}

static int _GuiMenuItem(lua_State * L)
{
    lua_pushboolean(L, GuiMenuItem(luaL_checkstring(L, 1),
                                   luaL_optstring(L, 2, nullptr),
                                   luaL_optboolean(L, 3, 0),
                                   luaL_optboolean(L, 4, 1)));
    return 1;
}

static int _GuiMenuItem2(lua_State * L)
{
    bool p_selected = luaL_checkboolean(L, 3);
    lua_pushboolean(L, GuiMenuItem(luaL_checkstring(L, 1),
                                   luaL_checkstring(L, 2),
                                   &p_selected,
                                   luaL_optboolean(L, 4, 1)));
    lua_pushboolean(L, p_selected);
    return 2;
}

static int _GuiBeginTooltip(lua_State * L)
{
    GuiBeginTooltip();
    return 0;
}

static int _GuiEndTooltip(lua_State * L)
{
    GuiEndTooltip();
    return 0;
}

static int _GuiSetTooltip(lua_State * L)
{
    GuiSetTooltip(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiOpenPopup(lua_State * L)
{
    GuiOpenPopup(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiBeginPopup(lua_State * L)
{
    lua_pushboolean(L, GuiBeginPopup(luaL_checkstring(L, 1), luaL_optinteger(L, 2, 0)));
    return 0;
}

static int _GuiBeginPopupContextItem(lua_State * L)
{
    lua_pushboolean(L, GuiBeginPopupContextItem(luaL_optstring(L, 1, nullptr), luaL_optinteger(L, 2, 1)));
    return 1;
}

static int _GuiBeginPopupContextWindow(lua_State * L)
{
    lua_pushboolean(L, GuiBeginPopupContextWindow(luaL_optstring(L, 1, nullptr),
                                                  luaL_optinteger(L, 2, 1),
                                                  luaL_optboolean(L, 3, 1)));
    return 1;
}

static int _GuiBeginPopupContextVoid(lua_State * L)
{
    lua_pushboolean(L, GuiBeginPopupContextVoid(luaL_optstring(L, 1, nullptr), luaL_optinteger(L, 2, 1)));
    return 1;
}

static int _GuiBeginPopupModal(lua_State * L)
{
    if (lua_isboolean(L, 2)) {
        bool p_open = luaL_checkboolean(L, 2);
        lua_pushboolean(L, GuiBeginPopupModal(luaL_checkstring(L, 1), &p_open, luaL_optinteger(L, 3, 0)));
        lua_pushboolean(L, p_open);
        return 2;
    }

    lua_pushboolean(L, GuiBeginPopupModal(luaL_checkstring(L, 1), nullptr, luaL_optinteger(L, 2, 0)));
    return 1;
}

static int _GuiEndPopup(lua_State * L)
{
    GuiEndPopup();
    return 0;
}

static int _GuiOpenPopupOnItemClick(lua_State * L)
{
    lua_pushboolean(L, GuiOpenPopupOnItemClick(luaL_optstring(L, 1, nullptr),
                                               luaL_optinteger(L, 2, 1)));
    return 1;
}

static int _GuiIsPopupOpen(lua_State * L)
{
    lua_pushboolean(L, GuiIsPopupOpen(luaL_checkstring(L, 1)));
    return 1;
}

static int _GuiCloseCurrentPopup(lua_State * L)
{
    GuiCloseCurrentPopup();
    return 0;
}

static int _GuiColumns(lua_State * L)
{
    GuiColumns(luaL_optinteger(L, 1, 1),
               luaL_optstring(L, 2, nullptr),
               luaL_optboolean(L, 3, 1));
    return 0;
}

static int _GuiNextColumn(lua_State * L)
{
    GuiNextColumn();
    return 0;
}

static int _GuiGetColumnIndex(lua_State * L)
{
    lua_pushinteger(L, GuiGetColumnIndex());
    return 1;
}

static int _GuiGetColumnWidth(lua_State * L)
{
    lua_pushnumber(L, GuiGetColumnWidth(luaL_optinteger(L, 1, -1)));
    return 1;
}

static int _GuiSetColumnWidth(lua_State * L)
{
    GuiSetColumnWidth(luaL_checkinteger(L, 1),
                      luaL_checknumber(L, 2));
    return 0;
}

static int _GuiGetColumnOffset(lua_State * L)
{
    lua_pushnumber(L, GuiGetColumnOffset(luaL_optinteger(L, 1, -1)));
    return 1;
}

static int _GuiSetColumnOffset(lua_State * L)
{
    GuiSetColumnOffset(luaL_checkinteger(L, 1),
                       luaL_checknumber(L, 2));
    return 0;
}

static int _GuiGetColumnsCount(lua_State * L)
{
    lua_pushinteger(L, GuiGetColumnsCount());
    return 1;
}

static int _GuiBeginTabBar(lua_State * L)
{
    lua_pushboolean(L, GuiBeginTabBar(luaL_checkstring(L, 1), luaL_optinteger(L, 2, 0)));
    return 1;
}

static int _GuiEndTabBar(lua_State * L)
{
    GuiEndTabBar();
    return 0;
}

static int _GuiBeginTabItem(lua_State * L)
{
    if (lua_isboolean(L, 2)) {
        bool p_open = luaL_checkboolean(L, 2);
        lua_pushboolean(L, GuiBeginTabItem(luaL_checkstring(L, 1), &p_open, luaL_optinteger(L, 3, 0)));
        lua_pushboolean(L, p_open);
        return 2;
    }

    lua_pushboolean(L, GuiBeginTabItem(luaL_checkstring(L, 1), nullptr, luaL_optinteger(L, 2, 0)));
    return 1;
}

static int _GuiEndTabItem(lua_State * L)
{
    GuiEndTabItem();
    return 0;
}

static int _GuiSetTabItemClosed(lua_State * L)
{
    GuiSetTabItemClosed(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiLogToTTY(lua_State * L)
{
    GuiLogToTTY(luaL_optinteger(L, 1, -1));
    return 0;
}

static int _GuiLogToFile(lua_State * L)
{
    GuiLogToFile(luaL_optinteger(L, 1, -1), luaL_optstring(L, 2, nullptr));
    return 0;
}

static int _GuiLogToClipboard(lua_State * L)
{
    GuiLogToClipboard(luaL_optinteger(L, 1, -1));
    return 0;
}

static int _GuiLogFinish(lua_State * L)
{
    GuiLogFinish();
    return 0;
}

static int _GuiLogButtons(lua_State * L)
{
    GuiLogButtons();
    return 0;
}

static int _GuiLogText(lua_State * L)
{
    GuiLogText(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiBeginDragDropSource(lua_State * L)
{
    lua_pushboolean(L, GuiBeginDragDropSource(luaL_optinteger(L, 1, 0)));
    return 1;
}

static int _GuiSetDragDropPayload(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _GuiEndDragDropSource(lua_State * L)
{
    GuiEndDragDropSource();
    return 0;
}

static int _GuiBeginDragDropTarget(lua_State * L)
{
    lua_pushboolean(L, GuiBeginDragDropTarget());
    return 0;
}

static int _GuiAcceptDragDropPayload(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _GuiEndDragDropTarget(lua_State * L)
{
    GuiEndDragDropTarget();
    return 0;
}

static int _GuiGetDragDropPayload(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _GuiPushClipRect(lua_State * L)
{
    GuiPushClipRect(luaE_checkvector2(L, 1),
                    luaE_checkvector2(L, 2),
                    luaL_checkboolean(L, 3));
    return 0;
}

static int _GuiPopClipRect(lua_State * L)
{
    GuiPopClipRect();
    return 0;
}

static int _GuiSetItemDefaultFocus(lua_State * L)
{
    GuiSetItemDefaultFocus();
    return 0;
}

static int _GuiSetKeyboardFocusHere(lua_State * L)
{
    GuiSetKeyboardFocusHere(luaL_optinteger(L, 1, 0));
    return 0;
}

static int _GuiIsItemHovered(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemHovered(luaL_optinteger(L, 1, 0)));
    return 1;
}

static int _GuiIsItemActive(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemActive());
    return 1;
}

static int _GuiIsItemFocused(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemFocused());
    return 1;
}

static int _GuiIsItemClicked(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemClicked(luaL_optinteger(L, 1, 0)));
    return 1;
}

static int _GuiIsItemVisible(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemVisible());
    return 1;
}

static int _GuiIsItemEdited(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemEdited());
    return 1;
}

static int _GuiIsItemActivated(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemActivated());
    return 1;
}

static int _GuiIsItemDeactivated(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemDeactivated());
    return 1;
}

static int _GuiIsItemDeactivatedAfterEdit(lua_State * L)
{
    lua_pushboolean(L, GuiIsItemDeactivatedAfterEdit());
    return 1;
}

static int _GuiIsAnyItemHovered(lua_State * L)
{
    lua_pushboolean(L, GuiIsAnyItemHovered());
    return 1;
}

static int _GuiIsAnyItemActive(lua_State * L)
{
    lua_pushboolean(L, GuiIsAnyItemActive());
    return 1;
}

static int _GuiIsAnyItemFocused(lua_State * L)
{
    lua_pushboolean(L, GuiIsAnyItemFocused());
    return 1;
}

static int _GuiGetItemRectMin(lua_State * L)
{
    luaE_pushvector2(L, GuiGetItemRectMin());
    return 1;
}

static int _GuiGetItemRectMax(lua_State * L)
{
    luaE_pushvector2(L, GuiGetItemRectMax());
    return 1;
}

static int _GuiGetItemRectSize(lua_State * L)
{
    luaE_pushvector2(L, GuiGetItemRectSize());
    return 1;
}

static int _GuiSetItemAllowOverlap(lua_State * L)
{
    GuiSetItemAllowOverlap();
    return 0;
}

static int _GuiIsRectVisible(lua_State * L)
{
    if (lua_istable(L, 2)) {
        lua_pushboolean(L, GuiIsRectVisible(luaE_checkvector2(L, 1), luaE_checkvector2(L, 2)));
    } else {
        lua_pushboolean(L, GuiIsRectVisible(luaE_checkvector2(L, 1)));
    }
    return 1;
}

static int _GuiGetTime(lua_State * L)
{
    lua_pushnumber(L, GuiGetTime());
    return 1;
}

static int _GuiGetFrameCount(lua_State * L)
{
    lua_pushinteger(L, GuiGetFrameCount());
    return 1;
}

static int _GuiGetStyleColorName(lua_State * L)
{
    lua_pushstring(L, GuiGetStyleColorName(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiCalcTextSize(lua_State * L)
{
    luaE_pushvector2(L, GuiCalcTextSize(luaL_checkstring(L, 1),
                                        luaL_optstring(L, 2, nullptr),
                                        luaL_optboolean(L, 3, 0),
                                        luaL_optnumber(L, 4, -1.0f)));
    return 1;
}

static int _GuiCalcListClipping(lua_State * L)
{
    int out_items_display_start = 0;
    int out_items_display_end = 0;
    GuiCalcListClipping(luaL_checkinteger(L, 1),
                        luaL_checknumber(L, 2),
                        &out_items_display_start,
                        &out_items_display_end);
    lua_pushinteger(L, out_items_display_start);
    lua_pushinteger(L, out_items_display_end);
    return 2;
}

static int _GuiBeginChildFrame(lua_State * L)
{
    lua_pushboolean(L, GuiBeginChildFrame(luaL_checkinteger(L, 1),
                                          luaE_checkvector2(L, 2),
                                          luaL_optinteger(L, 3, 0)));
    return 1;
}

static int _GuiEndChildFrame(lua_State * L)
{
    GuiEndChildFrame();
    return 0;
}

static int _GuiColorConvertU32ToFloat4(lua_State * L)
{
    luaE_pushvector4(L, GuiColorConvertU32ToFloat4(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiColorConvertFloat4ToU32(lua_State * L)
{
    lua_pushinteger(L, GuiColorConvertFloat4ToU32(luaE_checkvector4(L, 1)));
    return 1;
}

static int _GuiColorConvertRGBtoHSV(lua_State * L)
{
    float out_h, out_s, out_v;
    GuiColorConvertRGBtoHSV(luaL_checknumber(L, 1),
                            luaL_checknumber(L, 2),
                            luaL_checknumber(L, 3),
                            out_h, out_s, out_v);
    lua_pushnumber(L, out_h);
    lua_pushnumber(L, out_s);
    lua_pushnumber(L, out_v);
    return 3;
}

static int _GuiColorConvertHSVtoRGB(lua_State * L)
{
    float out_r, out_g, out_b;
    GuiColorConvertHSVtoRGB(luaL_checknumber(L, 1),
                            luaL_checknumber(L, 2),
                            luaL_checknumber(L, 3),
                            out_r, out_g, out_b);
    lua_pushnumber(L, out_r);
    lua_pushnumber(L, out_g);
    lua_pushnumber(L, out_b);
    return 3;
}

static int _GuiGetKeyIndex(lua_State * L)
{
    lua_pushinteger(L, GuiGetKeyIndex(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiIsKeyDown(lua_State * L)
{
    lua_pushboolean(L, GuiIsKeyDown(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiIsKeyPressed(lua_State * L)
{
    lua_pushboolean(L, GuiIsKeyPressed(luaL_checkinteger(L, 1),
                                       luaL_optboolean(L, 2, 1)));
    return 1;
}

static int _GuiIsKeyReleased(lua_State * L)
{
    lua_pushboolean(L, GuiIsKeyReleased(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiGetKeyPressedAmount(lua_State * L)
{
    lua_pushinteger(L, GuiGetKeyPressedAmount(luaL_checkinteger(L, 1),
                                              luaL_checknumber(L, 2),
                                              luaL_checknumber(L, 3)));
    return 1;
}

static int _GuiIsMouseDown(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseDown(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiIsAnyMouseDown(lua_State * L)
{
    lua_pushboolean(L, GuiIsAnyMouseDown());
    return 1;
}

static int _GuiIsMouseClicked(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseClicked(luaL_checkinteger(L, 1), luaL_optboolean(L, 2, 0)));
    return 1;
}

static int _GuiIsMouseDoubleClicked(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseDoubleClicked(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiIsMouseReleased(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseReleased(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GuiIsMouseDragging(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseDragging(luaL_optinteger(L, 1, 0),
                                          luaL_optnumber(L, 2, -1.0f)));
    return 1;
}

static int _GuiIsMouseHoveringRect(lua_State * L)
{
    lua_pushboolean(L, GuiIsMouseHoveringRect(luaE_checkvector2(L, 1),
                                              luaE_checkvector2(L, 2),
                                              luaL_optboolean(L, 3, 1)));
    return 1;
}

static int _GuiIsMousePosValid(lua_State * L)
{
    if (lua_istable(L, 1)) {
        auto const MOUSE_POS = luaE_checkvector2(L, 1);
        lua_pushboolean(L, GuiIsMousePosValid(&MOUSE_POS));
    } else {
        lua_pushboolean(L, GuiIsMousePosValid());
    }
    return 1;
}

static int _GuiGetMousePos(lua_State * L)
{
    luaE_pushvector2(L, GuiGetMousePos());
    return 1;
}

static int _GuiGetMousePosOnOpeningCurrentPopup(lua_State * L)
{
    luaE_pushvector2(L, GuiGetMousePosOnOpeningCurrentPopup());
    return 1;
}

static int _GuiGetMouseDragDelta(lua_State * L)
{
    luaE_pushvector2(L, GuiGetMouseDragDelta(luaL_optinteger(L, 1, 0),
                                             luaL_optnumber(L, 2, -1.0f)));
    return 1;
}

static int _GuiResetMouseDragDelta(lua_State * L)
{
    GuiResetMouseDragDelta(luaL_optinteger(L, 1, 0));
    return 0;
}

static int _GuiGetMouseCursor(lua_State * L)
{
    lua_pushinteger(L, GuiGetMouseCursor());
    return 1;
}

static int _GuiSetMouseCursor(lua_State * L)
{
    GuiSetMouseCursor(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiCaptureKeyboardFromApp(lua_State * L)
{
    GuiCaptureKeyboardFromApp(luaL_optboolean(L, 1, 1));
    return 0;
}

static int _GuiCaptureMouseFromApp(lua_State * L)
{
    GuiCaptureMouseFromApp(luaL_optboolean(L, 1, 1));
    return 0;
}

static int _GuiGetClipboardText(lua_State * L)
{
    lua_pushstring(L, GetClipboardText());
    return 1;
}

static int _GuiSetClipboardText(lua_State * L)
{
    GuiSetClipboardText(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiLoadIniSettingsFromDisk(lua_State * L)
{
    GuiLoadIniSettingsFromDisk(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiLoadIniSettingsFromMemory(lua_State * L)
{
    GuiLoadIniSettingsFromMemory(luaL_checkstring(L, 1), luaL_optinteger(L, 2, 0));
    return 0;
}

static int _GuiSaveIniSettingsToDisk(lua_State * L)
{
    GuiSaveIniSettingsToDisk(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiSaveIniSettingsToMemory(lua_State * L)
{
    size_t out_init_size = 0;
    lua_pushstring(L, GuiSaveIniSettingsToMemory(&out_init_size));
    lua_pushinteger(L, out_init_size);
    return 2;
}

#ifndef RAYGUI_REGISTER
#define RAYGUI_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_laygui[] = {
        // Raylib-ImGui.
        RAYGUI_REGISTER(GuiInitRay),
        RAYGUI_REGISTER(GuiUpdateRay),
        RAYGUI_REGISTER(GuiShutdownRay),

        // Demo, Debug, Information
        RAYGUI_REGISTER(GuiShowDemoWindow),
        RAYGUI_REGISTER(GuiShowAboutWindow),
        RAYGUI_REGISTER(GuiShowMetricsWindow),
        RAYGUI_REGISTER(GuiShowStyleEditor),
        RAYGUI_REGISTER(GuiShowStyleSelector),
        RAYGUI_REGISTER(GuiShowFontSelector),
        RAYGUI_REGISTER(GuiShowUserGuide),
        RAYGUI_REGISTER(GuiGetVersion),

        // Styles
        RAYGUI_REGISTER(GuiStyleColorsDark),
        RAYGUI_REGISTER(GuiStyleColorsClassic),
        RAYGUI_REGISTER(GuiStyleColorsLight),
        RAYGUI_REGISTER(GuiGetStyle),
        RAYGUI_REGISTER(GuiSetStyle),

        // Windows
        RAYGUI_REGISTER(GuiBegin),
        RAYGUI_REGISTER(GuiEnd),

        // Child Windows
        RAYGUI_REGISTER(GuiBeginChild),
        RAYGUI_REGISTER(GuiEndChild),

        // Windows Utilities
        RAYGUI_REGISTER(GuiIsWindowAppearing),
        RAYGUI_REGISTER(GuiIsWindowCollapsed),
        RAYGUI_REGISTER(GuiIsWindowFocused),
        RAYGUI_REGISTER(GuiIsWindowHovered),
        RAYGUI_REGISTER(GuiGetWindowPos),
        RAYGUI_REGISTER(GuiGetWindowSize),
        RAYGUI_REGISTER(GuiGetWindowWidth),
        RAYGUI_REGISTER(GuiGetWindowHeight),

        // SetNextXXX and SetXXX
        RAYGUI_REGISTER(GuiSetNextWindowPos),
        RAYGUI_REGISTER(GuiSetNextWindowSize),
        RAYGUI_REGISTER(GuiSetNextWindowContentSize),
        RAYGUI_REGISTER(GuiSetNextWindowCollapsed),
        RAYGUI_REGISTER(GuiSetNextWindowFocus),
        RAYGUI_REGISTER(GuiSetNextWindowBgAlpha),
        RAYGUI_REGISTER(GuiSetWindowPos),
        RAYGUI_REGISTER(GuiSetWindowSize),
        RAYGUI_REGISTER(GuiSetWindowCollapsed),
        RAYGUI_REGISTER(GuiSetWindowFocus),
        RAYGUI_REGISTER(GuiSetWindowFontScale),

        // Content region
        RAYGUI_REGISTER(GuiGetContentRegionMax),
        RAYGUI_REGISTER(GuiGetContentRegionAvail),
        RAYGUI_REGISTER(GuiGetWindowContentRegionMin),
        RAYGUI_REGISTER(GuiGetWindowContentRegionMax),
        RAYGUI_REGISTER(GuiGetWindowContentRegionWidth),

        // Windows Scrolling
        RAYGUI_REGISTER(GuiGetScrollX),
        RAYGUI_REGISTER(GuiGetScrollY),
        RAYGUI_REGISTER(GuiGetScrollMaxX),
        RAYGUI_REGISTER(GuiGetScrollMaxY),
        RAYGUI_REGISTER(GuiSetScrollX),
        RAYGUI_REGISTER(GuiSetScrollY),
        RAYGUI_REGISTER(GuiSetScrollHereY),
        RAYGUI_REGISTER(GuiSetScrollFromPosY),

        // Parameters stacks
        RAYGUI_REGISTER(GuiPushStyleColor),
        RAYGUI_REGISTER(GuiPopStyleColor),
        RAYGUI_REGISTER(GuiPushStyleVar),
        RAYGUI_REGISTER(GuiPopStyleVar),
        RAYGUI_REGISTER(GuiGetStyleColorVec4),
        RAYGUI_REGISTER(GuiGetFontSize),
        RAYGUI_REGISTER(GuiGetFontTexUvWhitePixel),
        RAYGUI_REGISTER(GuiGetColorU32WithAlpha),
        RAYGUI_REGISTER(GuiGetColorU32),

        // Parameters stacks
        RAYGUI_REGISTER(GuiPushItemWidth),
        RAYGUI_REGISTER(GuiPopItemWidth),
        RAYGUI_REGISTER(GuiSetNextItemWidth),
        RAYGUI_REGISTER(GuiCalcItemWidth),
        RAYGUI_REGISTER(GuiPushTextWrapPos),
        RAYGUI_REGISTER(GuiPopTextWrapPos),
        RAYGUI_REGISTER(GuiPushAllowKeyboardFocus),
        RAYGUI_REGISTER(GuiPopAllowKeyboardFocus),
        RAYGUI_REGISTER(GuiPushButtonRepeat),
        RAYGUI_REGISTER(GuiPopButtonRepeat),

        // Cursor / Layout
        RAYGUI_REGISTER(GuiSeparator),
        RAYGUI_REGISTER(GuiSameLine),
        RAYGUI_REGISTER(GuiNewLine),
        RAYGUI_REGISTER(GuiSpacing),
        RAYGUI_REGISTER(GuiDummy),
        RAYGUI_REGISTER(GuiIndent),
        RAYGUI_REGISTER(GuiUnindent),
        RAYGUI_REGISTER(GuiBeginGroup),
        RAYGUI_REGISTER(GuiEndGroup),
        RAYGUI_REGISTER(GuiGetCursorPos),
        RAYGUI_REGISTER(GuiGetCursorPosX),
        RAYGUI_REGISTER(GuiGetCursorPosY),
        RAYGUI_REGISTER(GuiSetCursorPos),
        RAYGUI_REGISTER(GuiSetCursorPosX),
        RAYGUI_REGISTER(GuiSetCursorPosY),
        RAYGUI_REGISTER(GuiGetCursorStartPos),
        RAYGUI_REGISTER(GuiGetCursorScreenPos),
        RAYGUI_REGISTER(GuiSetCursorScreenPos),
        RAYGUI_REGISTER(GuiAlignTextToFramePadding),
        RAYGUI_REGISTER(GuiGetTextLineHeight),
        RAYGUI_REGISTER(GuiGetTextLineHeightWithSpacing),
        RAYGUI_REGISTER(GuiGetFrameHeight),
        RAYGUI_REGISTER(GuiGetFrameHeightWithSpacing),

        // ID stack/scopes
        RAYGUI_REGISTER(GuiPushID),
        RAYGUI_REGISTER(GuiPopID),
        RAYGUI_REGISTER(GuiGetID),

        // Widgets: Text
        RAYGUI_REGISTER(GuiTextUnformatted),
        RAYGUI_REGISTER(GuiText),
        RAYGUI_REGISTER(GuiTextColored),
        RAYGUI_REGISTER(GuiTextDisabled),
        RAYGUI_REGISTER(GuiTextWrapped),
        RAYGUI_REGISTER(GuiLabelText),
        RAYGUI_REGISTER(GuiBulletText),

        // Widgets: Main
        RAYGUI_REGISTER(GuiButton),
        RAYGUI_REGISTER(GuiSmallButton),
        RAYGUI_REGISTER(GuiInvisibleButton),
        RAYGUI_REGISTER(GuiArrowButton),
        RAYGUI_REGISTER(GuiImage),
        RAYGUI_REGISTER(GuiImageButton),
        RAYGUI_REGISTER(GuiCheckbox),
        RAYGUI_REGISTER(GuiCheckboxFlags),
        RAYGUI_REGISTER(GuiRadioButton),
        RAYGUI_REGISTER(GuiProgressBar),
        RAYGUI_REGISTER(GuiBullet),

        // Widgets: Combo Box
        RAYGUI_REGISTER(GuiBeginCombo),
        RAYGUI_REGISTER(GuiEndCombo),
        RAYGUI_REGISTER(GuiCombo),

        // Widgets: Drags
        RAYGUI_REGISTER(GuiDragFloat),
        RAYGUI_REGISTER(GuiDragFloat2),
        RAYGUI_REGISTER(GuiDragFloat3),
        RAYGUI_REGISTER(GuiDragFloat4),
        RAYGUI_REGISTER(GuiDragFloatRange2),
        RAYGUI_REGISTER(GuiDragInt),
        RAYGUI_REGISTER(GuiDragInt2),
        RAYGUI_REGISTER(GuiDragInt3),
        RAYGUI_REGISTER(GuiDragInt4),
        RAYGUI_REGISTER(GuiDragIntRange2),

        // Widgets: Sliders
        RAYGUI_REGISTER(GuiSliderFloat),
        RAYGUI_REGISTER(GuiSliderFloat2),
        RAYGUI_REGISTER(GuiSliderFloat3),
        RAYGUI_REGISTER(GuiSliderFloat4),
        RAYGUI_REGISTER(GuiSliderAngle),
        RAYGUI_REGISTER(GuiSliderInt),
        RAYGUI_REGISTER(GuiSliderInt2),
        RAYGUI_REGISTER(GuiSliderInt3),
        RAYGUI_REGISTER(GuiSliderInt4),
        RAYGUI_REGISTER(GuiVSliderFloat),
        RAYGUI_REGISTER(GuiVSliderInt),

        // Widgets: Input with Keyboard
        RAYGUI_REGISTER(GuiInputText),
        RAYGUI_REGISTER(GuiInputTextMultiline),
        RAYGUI_REGISTER(GuiInputTextWithHint),
        RAYGUI_REGISTER(GuiInputFloat),
        RAYGUI_REGISTER(GuiInputFloat2),
        RAYGUI_REGISTER(GuiInputFloat3),
        RAYGUI_REGISTER(GuiInputFloat4),
        RAYGUI_REGISTER(GuiInputInt),
        RAYGUI_REGISTER(GuiInputInt2),
        RAYGUI_REGISTER(GuiInputInt3),
        RAYGUI_REGISTER(GuiInputInt4),
        RAYGUI_REGISTER(GuiInputDouble),

        // Widgets: Color Editor/Picker
        RAYGUI_REGISTER(GuiColorEdit3),
        RAYGUI_REGISTER(GuiColorEdit4),
        RAYGUI_REGISTER(GuiColorPicker3),
        RAYGUI_REGISTER(GuiColorPicker4),
        RAYGUI_REGISTER(GuiColorButton),
        RAYGUI_REGISTER(GuiSetColorEditOptions),

        // Widgets: Trees
        RAYGUI_REGISTER(GuiTreeNode),
        RAYGUI_REGISTER(GuiTreeNodeEx),
        RAYGUI_REGISTER(GuiTreePush),
        RAYGUI_REGISTER(GuiTreePushNull),
        RAYGUI_REGISTER(GuiTreePop),
        RAYGUI_REGISTER(GuiTreeAdvanceToLabelPos),
        RAYGUI_REGISTER(GuiGetTreeNodeToLabelSpacing),
        RAYGUI_REGISTER(GuiSetNextTreeNodeOpen),
        RAYGUI_REGISTER(GuiCollapsingHeader),

        // Widgets: Selectables
        RAYGUI_REGISTER(GuiSelectable),
        RAYGUI_REGISTER(GuiSelectable2),

        // Widgets: List Boxes
        RAYGUI_REGISTER(GuiListBox),
        RAYGUI_REGISTER(GuiListBoxHeader),
        RAYGUI_REGISTER(GuiListBoxFooter),

        // Widgets: Data Plotting
        RAYGUI_REGISTER(GuiPlotLines),
        RAYGUI_REGISTER(GuiPlotHistogram),

        // Widgets: Value),
        RAYGUI_REGISTER(GuiValueBool),
        RAYGUI_REGISTER(GuiValueInt),
        RAYGUI_REGISTER(GuiValueUnsigned),
        RAYGUI_REGISTER(GuiValueFloat),

        // Widgets: Menus
        RAYGUI_REGISTER(GuiBeginMainMenuBar),
        RAYGUI_REGISTER(GuiEndMainMenuBar),
        RAYGUI_REGISTER(GuiBeginMenuBar),
        RAYGUI_REGISTER(GuiEndMenuBar),
        RAYGUI_REGISTER(GuiBeginMenu),
        RAYGUI_REGISTER(GuiEndMenu),
        RAYGUI_REGISTER(GuiMenuItem),
        RAYGUI_REGISTER(GuiMenuItem2),

        // Tooltips
        RAYGUI_REGISTER(GuiBeginTooltip),
        RAYGUI_REGISTER(GuiEndTooltip),
        RAYGUI_REGISTER(GuiSetTooltip),

        // Popups, Modals
        RAYGUI_REGISTER(GuiOpenPopup),
        RAYGUI_REGISTER(GuiBeginPopup),
        RAYGUI_REGISTER(GuiBeginPopupContextItem),
        RAYGUI_REGISTER(GuiBeginPopupContextWindow),
        RAYGUI_REGISTER(GuiBeginPopupContextVoid),
        RAYGUI_REGISTER(GuiBeginPopupModal),
        RAYGUI_REGISTER(GuiEndPopup),
        RAYGUI_REGISTER(GuiOpenPopupOnItemClick),
        RAYGUI_REGISTER(GuiIsPopupOpen),
        RAYGUI_REGISTER(GuiCloseCurrentPopup),

        // Columns
        RAYGUI_REGISTER(GuiColumns),
        RAYGUI_REGISTER(GuiNextColumn),
        RAYGUI_REGISTER(GuiGetColumnIndex),
        RAYGUI_REGISTER(GuiGetColumnWidth),
        RAYGUI_REGISTER(GuiSetColumnWidth),
        RAYGUI_REGISTER(GuiGetColumnOffset),
        RAYGUI_REGISTER(GuiSetColumnOffset),
        RAYGUI_REGISTER(GuiGetColumnsCount),

        // Tab Bars, Tabs
        RAYGUI_REGISTER(GuiBeginTabBar),
        RAYGUI_REGISTER(GuiEndTabBar),
        RAYGUI_REGISTER(GuiBeginTabItem),
        RAYGUI_REGISTER(GuiEndTabItem),
        RAYGUI_REGISTER(GuiSetTabItemClosed),

        // Logging/Capture
        RAYGUI_REGISTER(GuiLogToTTY),
        RAYGUI_REGISTER(GuiLogToFile),
        RAYGUI_REGISTER(GuiLogToClipboard),
        RAYGUI_REGISTER(GuiLogFinish),
        RAYGUI_REGISTER(GuiLogButtons),
        RAYGUI_REGISTER(GuiLogText),

        // Drag and Drop
        RAYGUI_REGISTER(GuiBeginDragDropSource),
        RAYGUI_REGISTER(GuiSetDragDropPayload),
        RAYGUI_REGISTER(GuiEndDragDropSource),
        RAYGUI_REGISTER(GuiBeginDragDropTarget),
        RAYGUI_REGISTER(GuiAcceptDragDropPayload),
        RAYGUI_REGISTER(GuiEndDragDropTarget),
        RAYGUI_REGISTER(GuiGetDragDropPayload),

        // Clipping
        RAYGUI_REGISTER(GuiPushClipRect),
        RAYGUI_REGISTER(GuiPopClipRect),

        // Focus, Activation
        RAYGUI_REGISTER(GuiSetItemDefaultFocus),
        RAYGUI_REGISTER(GuiSetKeyboardFocusHere),

        // Item/Widgets Utilities
        RAYGUI_REGISTER(GuiIsItemHovered),
        RAYGUI_REGISTER(GuiIsItemActive),
        RAYGUI_REGISTER(GuiIsItemFocused),
        RAYGUI_REGISTER(GuiIsItemClicked),
        RAYGUI_REGISTER(GuiIsItemVisible),
        RAYGUI_REGISTER(GuiIsItemEdited),
        RAYGUI_REGISTER(GuiIsItemActivated),
        RAYGUI_REGISTER(GuiIsItemDeactivated),
        RAYGUI_REGISTER(GuiIsItemDeactivatedAfterEdit),
        RAYGUI_REGISTER(GuiIsAnyItemHovered),
        RAYGUI_REGISTER(GuiIsAnyItemActive),
        RAYGUI_REGISTER(GuiIsAnyItemFocused),
        RAYGUI_REGISTER(GuiGetItemRectMin),
        RAYGUI_REGISTER(GuiGetItemRectMax),
        RAYGUI_REGISTER(GuiGetItemRectSize),
        RAYGUI_REGISTER(GuiSetItemAllowOverlap),

        // Miscellaneous Utilities
        RAYGUI_REGISTER(GuiIsRectVisible),
        RAYGUI_REGISTER(GuiGetTime),
        RAYGUI_REGISTER(GuiGetFrameCount),
        RAYGUI_REGISTER(GuiGetStyleColorName),
        RAYGUI_REGISTER(GuiCalcTextSize),
        RAYGUI_REGISTER(GuiCalcListClipping),
        RAYGUI_REGISTER(GuiBeginChildFrame),
        RAYGUI_REGISTER(GuiEndChildFrame),

        // Color Utilities
        RAYGUI_REGISTER(GuiColorConvertU32ToFloat4),
        RAYGUI_REGISTER(GuiColorConvertFloat4ToU32),
        RAYGUI_REGISTER(GuiColorConvertRGBtoHSV),
        RAYGUI_REGISTER(GuiColorConvertHSVtoRGB),

        // Inputs Utilities
        RAYGUI_REGISTER(GuiGetKeyIndex),
        RAYGUI_REGISTER(GuiIsKeyDown),
        RAYGUI_REGISTER(GuiIsKeyPressed),
        RAYGUI_REGISTER(GuiIsKeyReleased),
        RAYGUI_REGISTER(GuiGetKeyPressedAmount),
        RAYGUI_REGISTER(GuiIsMouseDown),
        RAYGUI_REGISTER(GuiIsAnyMouseDown),
        RAYGUI_REGISTER(GuiIsMouseClicked),
        RAYGUI_REGISTER(GuiIsMouseDoubleClicked),
        RAYGUI_REGISTER(GuiIsMouseReleased),
        RAYGUI_REGISTER(GuiIsMouseDragging),
        RAYGUI_REGISTER(GuiIsMouseHoveringRect),
        RAYGUI_REGISTER(GuiIsMousePosValid),
        RAYGUI_REGISTER(GuiGetMousePos),
        RAYGUI_REGISTER(GuiGetMousePosOnOpeningCurrentPopup),
        RAYGUI_REGISTER(GuiGetMouseDragDelta),
        RAYGUI_REGISTER(GuiResetMouseDragDelta),
        RAYGUI_REGISTER(GuiGetMouseCursor),
        RAYGUI_REGISTER(GuiSetMouseCursor),
        RAYGUI_REGISTER(GuiCaptureKeyboardFromApp),
        RAYGUI_REGISTER(GuiCaptureMouseFromApp),

        // Clipboard Utilities
        RAYGUI_REGISTER(GuiGetClipboardText),
        RAYGUI_REGISTER(GuiSetClipboardText),

        // Settings/.Ini Utilities
        RAYGUI_REGISTER(GuiLoadIniSettingsFromDisk),
        RAYGUI_REGISTER(GuiLoadIniSettingsFromMemory),
        RAYGUI_REGISTER(GuiSaveIniSettingsToDisk),
        RAYGUI_REGISTER(GuiSaveIniSettingsToMemory),

        { nullptr, nullptr }
};

#ifndef RAYGUI_INT_SYMBOL
#define RAYGUI_INT_SYMBOL(L, s)     \
    do {                            \
        lua_pushinteger(L, s);      \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

static void luaL_push_raygui_symbols(lua_State * L)
{
    RAYGUI_INT_SYMBOL(L, GuiCol_Text);
    RAYGUI_INT_SYMBOL(L, GuiCol_TextDisabled);
    RAYGUI_INT_SYMBOL(L, GuiCol_WindowBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_ChildBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_PopupBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_Border);
    RAYGUI_INT_SYMBOL(L, GuiCol_BorderShadow);
    RAYGUI_INT_SYMBOL(L, GuiCol_FrameBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_FrameBgHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_FrameBgActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_TitleBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_TitleBgActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_TitleBgCollapsed);
    RAYGUI_INT_SYMBOL(L, GuiCol_MenuBarBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_ScrollbarBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_ScrollbarGrab);
    RAYGUI_INT_SYMBOL(L, GuiCol_ScrollbarGrabHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_ScrollbarGrabActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_CheckMark);
    RAYGUI_INT_SYMBOL(L, GuiCol_SliderGrab);
    RAYGUI_INT_SYMBOL(L, GuiCol_SliderGrabActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_Button);
    RAYGUI_INT_SYMBOL(L, GuiCol_ButtonHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_ButtonActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_Header);
    RAYGUI_INT_SYMBOL(L, GuiCol_HeaderHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_HeaderActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_Separator);
    RAYGUI_INT_SYMBOL(L, GuiCol_SeparatorHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_SeparatorActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_ResizeGrip);
    RAYGUI_INT_SYMBOL(L, GuiCol_ResizeGripHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_ResizeGripActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_Tab);
    RAYGUI_INT_SYMBOL(L, GuiCol_TabHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_TabActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_TabUnfocused);
    RAYGUI_INT_SYMBOL(L, GuiCol_TabUnfocusedActive);
    RAYGUI_INT_SYMBOL(L, GuiCol_PlotLines);
    RAYGUI_INT_SYMBOL(L, GuiCol_PlotLinesHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_PlotHistogram);
    RAYGUI_INT_SYMBOL(L, GuiCol_PlotHistogramHovered);
    RAYGUI_INT_SYMBOL(L, GuiCol_TextSelectedBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_DragDropTarget);
    RAYGUI_INT_SYMBOL(L, GuiCol_NavHighlight);
    RAYGUI_INT_SYMBOL(L, GuiCol_NavWindowingHighlight);
    RAYGUI_INT_SYMBOL(L, GuiCol_NavWindowingDimBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_ModalWindowDimBg);
    RAYGUI_INT_SYMBOL(L, GuiCol_COUNT);

    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoTitleBar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoResize);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoMove);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoScrollbar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoScrollWithMouse);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoCollapse);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_AlwaysAutoResize);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoBackground);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoSavedSettings);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoMouseInputs);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_MenuBar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_HorizontalScrollbar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoFocusOnAppearing);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoBringToFrontOnFocus);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_AlwaysVerticalScrollbar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_AlwaysHorizontalScrollbar);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_AlwaysUseWindowPadding);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoNavInputs);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoNavFocus);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_UnsavedDocument);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoNav);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoDecoration);
    RAYGUI_INT_SYMBOL(L, GuiWindowFlags_NoInputs);

    RAYGUI_INT_SYMBOL(L, GuiFocusedFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiFocusedFlags_ChildWindows);
    RAYGUI_INT_SYMBOL(L, GuiFocusedFlags_RootWindow);
    RAYGUI_INT_SYMBOL(L, GuiFocusedFlags_AnyWindow);
    RAYGUI_INT_SYMBOL(L, GuiFocusedFlags_RootAndChildWindows);

    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_ChildWindows);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_RootWindow);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AnyWindow);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AllowWhenBlockedByPopup);
    //RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AllowWhenBlockedByModal);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AllowWhenBlockedByActiveItem);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AllowWhenOverlapped);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_AllowWhenDisabled);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_RectOnly);
    RAYGUI_INT_SYMBOL(L, GuiHoveredFlags_RootAndChildWindows);

    RAYGUI_INT_SYMBOL(L, GuiCond_Always);
    RAYGUI_INT_SYMBOL(L, GuiCond_Once);
    RAYGUI_INT_SYMBOL(L, GuiCond_FirstUseEver);
    RAYGUI_INT_SYMBOL(L, GuiCond_Appearing);

    RAYGUI_INT_SYMBOL(L, GuiStyleVar_Alpha);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_WindowPadding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_WindowRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_WindowBorderSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_WindowMinSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_WindowTitleAlign);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ChildRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ChildBorderSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_PopupRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_PopupBorderSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_FramePadding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_FrameRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_FrameBorderSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ItemSpacing);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ItemInnerSpacing);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_IndentSpacing);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ScrollbarSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ScrollbarRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_GrabMinSize);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_GrabRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_TabRounding);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_ButtonTextAlign);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_SelectableTextAlign);
    RAYGUI_INT_SYMBOL(L, GuiStyleVar_COUNT);

    RAYGUI_INT_SYMBOL(L, GuiDir_None);
    RAYGUI_INT_SYMBOL(L, GuiDir_Left);
    RAYGUI_INT_SYMBOL(L, GuiDir_Right);
    RAYGUI_INT_SYMBOL(L, GuiDir_Up);
    RAYGUI_INT_SYMBOL(L, GuiDir_Down);
    RAYGUI_INT_SYMBOL(L, GuiDir_COUNT);

    RAYGUI_INT_SYMBOL(L, GuiComboFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_PopupAlignLeft);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_HeightSmall);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_HeightRegular);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_HeightLarge);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_HeightLargest);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_NoArrowButton);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_NoPreview);
    RAYGUI_INT_SYMBOL(L, GuiComboFlags_HeightMask_);

    RAYGUI_INT_SYMBOL(L, GuiDataType_S8);
    RAYGUI_INT_SYMBOL(L, GuiDataType_U8);
    RAYGUI_INT_SYMBOL(L, GuiDataType_S16);
    RAYGUI_INT_SYMBOL(L, GuiDataType_U16);
    RAYGUI_INT_SYMBOL(L, GuiDataType_S32);
    RAYGUI_INT_SYMBOL(L, GuiDataType_U32);
    RAYGUI_INT_SYMBOL(L, GuiDataType_S64);
    RAYGUI_INT_SYMBOL(L, GuiDataType_U64);
    RAYGUI_INT_SYMBOL(L, GuiDataType_Float);
    RAYGUI_INT_SYMBOL(L, GuiDataType_Double);
    RAYGUI_INT_SYMBOL(L, GuiDataType_COUNT);

    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoAlpha);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoPicker);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoOptions);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoSmallPreview);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoInputs);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoTooltip);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoLabel);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoSidePreview);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_NoDragDrop);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_AlphaBar);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_AlphaPreview);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_AlphaPreviewHalf);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_HDR);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_DisplayRGB);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_DisplayHSV);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_DisplayHex);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_Uint8);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_Float);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_PickerHueBar);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_PickerHueWheel);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_InputRGB);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags_InputHSV);
    RAYGUI_INT_SYMBOL(L, GuiColorEditFlags__OptionsDefault);

    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_Selected);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_Framed);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_AllowItemOverlap);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_NoTreePushOnOpen);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_NoAutoOpenOnLog);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_DefaultOpen);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_OpenOnDoubleClick);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_OpenOnArrow);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_Leaf);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_Bullet);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_FramePadding);
    //RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_SpanAllAvailWidth);
    //RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_NoScrollOnOpen);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_NavLeftJumpsBackHere);
    RAYGUI_INT_SYMBOL(L, GuiTreeNodeFlags_CollapsingHeader);

    RAYGUI_INT_SYMBOL(L, GuiSelectableFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiSelectableFlags_DontClosePopups);
    RAYGUI_INT_SYMBOL(L, GuiSelectableFlags_SpanAllColumns);
    RAYGUI_INT_SYMBOL(L, GuiSelectableFlags_AllowDoubleClick);
    RAYGUI_INT_SYMBOL(L, GuiSelectableFlags_Disabled);

    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_Reorderable);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_AutoSelectNewTabs);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_TabListPopupButton);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_NoCloseWithMiddleMouseButton);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_NoTabListScrollingButtons);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_NoTooltip);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_FittingPolicyResizeDown);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_FittingPolicyScroll);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_FittingPolicyMask_);
    RAYGUI_INT_SYMBOL(L, GuiTabBarFlags_FittingPolicyDefault_);

    RAYGUI_INT_SYMBOL(L, GuiTabItemFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiTabItemFlags_UnsavedDocument);
    RAYGUI_INT_SYMBOL(L, GuiTabItemFlags_SetSelected);
    RAYGUI_INT_SYMBOL(L, GuiTabItemFlags_NoCloseWithMiddleMouseButton);
    RAYGUI_INT_SYMBOL(L, GuiTabItemFlags_NoPushId);

    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_None);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceNoPreviewTooltip);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceNoDisableHover);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceNoHoldToOpenOthers);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceAllowNullID);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceExtern);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_SourceAutoExpirePayload);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_AcceptBeforeDelivery);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_AcceptNoDrawDefaultRect);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_AcceptNoPreviewTooltip);
    RAYGUI_INT_SYMBOL(L, GuiDragDropFlags_AcceptPeekOnly);

    RAYGUI_INT_SYMBOL(L, GuiKey_Tab);
    RAYGUI_INT_SYMBOL(L, GuiKey_LeftArrow);
    RAYGUI_INT_SYMBOL(L, GuiKey_RightArrow);
    RAYGUI_INT_SYMBOL(L, GuiKey_UpArrow);
    RAYGUI_INT_SYMBOL(L, GuiKey_DownArrow);
    RAYGUI_INT_SYMBOL(L, GuiKey_PageUp);
    RAYGUI_INT_SYMBOL(L, GuiKey_PageDown);
    RAYGUI_INT_SYMBOL(L, GuiKey_Home);
    RAYGUI_INT_SYMBOL(L, GuiKey_End);
    RAYGUI_INT_SYMBOL(L, GuiKey_Insert);
    RAYGUI_INT_SYMBOL(L, GuiKey_Delete);
    RAYGUI_INT_SYMBOL(L, GuiKey_Backspace);
    RAYGUI_INT_SYMBOL(L, GuiKey_Space);
    RAYGUI_INT_SYMBOL(L, GuiKey_Enter);
    RAYGUI_INT_SYMBOL(L, GuiKey_Escape);
    RAYGUI_INT_SYMBOL(L, GuiKey_A);
    RAYGUI_INT_SYMBOL(L, GuiKey_C);
    RAYGUI_INT_SYMBOL(L, GuiKey_V);
    RAYGUI_INT_SYMBOL(L, GuiKey_X);
    RAYGUI_INT_SYMBOL(L, GuiKey_Y);
    RAYGUI_INT_SYMBOL(L, GuiKey_Z);
    RAYGUI_INT_SYMBOL(L, GuiKey_COUNT);

    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_None);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_Arrow);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_TextInput);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_ResizeAll);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_ResizeNS);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_ResizeEW);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_ResizeNESW);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_ResizeNWSE);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_Hand);
    RAYGUI_INT_SYMBOL(L, GuiMouseCursor_COUNT);
}

bool luaE_open_raygui(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_laygui);
    {
        luaL_push_raygui_symbols(L);
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

