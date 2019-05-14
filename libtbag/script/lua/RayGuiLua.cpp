/**
 * @file   RayGuiLua.cpp
 * @brief  RayGuiLua class implementation.
 * @author zer0
 * @date   2019-05-07
 */

#include <libtbag/script/lua/RayGuiLua.hpp>
#include <libtbag/script/lua/RayLua.hpp>

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
    auto const border = luaL_optinteger(L, 3, 0);
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
    GuiSetNextWindowCollapsed(luaL_checkinteger(L, 1),
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
        GuiSetWindowCollapsed(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_optinteger(L, 3, 0));
    } else {
        GuiSetWindowCollapsed(luaL_checkinteger(L, 1), luaL_optinteger(L, 2, 0));
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
    GuiPushAllowKeyboardFocus(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiPopAllowKeyboardFocus(lua_State * L)
{
    GuiPopAllowKeyboardFocus();
    return 0;
}

static int _GuiPushButtonRepeat(lua_State * L)
{
    GuiPushButtonRepeat(luaL_checkinteger(L, 1));
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
    //lua_pushboolean(L, GuiCheckbox(luaL_checkstring(L, 1), bool * v));
    return 1;
}

static int _GuiCheckboxFlags(lua_State * L)
{
    //bool GuiCheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value);
    return 0;
}

static int _GuiRadioButton(lua_State * L)
{
    //bool GuiRadioButton(char const * label, bool active);
    //bool GuiRadioButton(char const * label, int * v, int v_button);
    return 0;
}

static int _GuiProgressBar(lua_State * L)
{
    //void ProgressBar(float fraction, Vector2 const & size_arg = Vector2{-1,0}, char const * overlay = nullptr);
    return 0;
}

static int _GuiBullet(lua_State * L)
{
    //void Bullet();
    return 0;
}

static int _GuiBeginCombo(lua_State * L)
{
    //bool BeginCombo(char const * label, char const * preview_value, ComboFlags flags = 0);
    return 0;
}

static int _GuiEndCombo(lua_State * L)
{
    //void EndCombo();
    return 0;
}

static int _GuiCombo(lua_State * L)
{
    //bool Combo(char const * label, int * current_item, char const * const items[], int items_count, int popup_max_height_in_items = -1);
    //bool Combo(char const * label, int * current_item, char const * items_separated_by_zeros, int popup_max_height_in_items = -1);
    //bool Combo(char const * label, int * current_item, bool(*items_getter)(void * data, int idx, char const ** out_text), void * data, int items_count, int popup_max_height_in_items = -1);
    return 0;
}

static int _GuiDragFloat(lua_State * L)
{
    //bool DragFloat(char const * label, float * v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiDragFloat2(lua_State * L)
{
    //bool DragFloat2(char const * label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiDragFloat3(lua_State * L)
{
    //bool DragFloat3(char const * label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiDragFloat4(lua_State * L)
{
    //bool DragFloat4(char const * label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiDragFloatRange2(lua_State * L)
{
    //bool DragFloatRange2(char const * label, float * v_current_min, float * v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", char const * format_max = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiDragInt(lua_State * L)
{
    //bool DragInt(char const * label, int * v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _GuiDragInt2(lua_State * L)
{
    //bool DragInt2(char const * label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _GuiDragInt3(lua_State * L)
{
    //bool DragInt3(char const * label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _GuiDragInt4(lua_State * L)
{
    //bool DragInt4(char const * label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _GuiDragIntRange2(lua_State * L)
{
    //bool DragIntRange2(char const * label, int * v_current_min, int * v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d", char const * format_max = nullptr);
    return 0;
}

static int _GuiDragScalar(lua_State * L)
{
    //bool DragScalar(char const * label, DataType data_type, void * v, float v_speed, void const * v_min = nullptr, void const * v_max = nullptr, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiDragScalarN(lua_State * L)
{
    //bool DragScalarN(char const * label, DataType data_type, void * v, int components, float v_speed, void const * v_min = nullptr, void const * v_max = nullptr, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiSliderFloat(lua_State * L)
{
    //bool SliderFloat(char const * label, float * v, float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiSliderFloat2(lua_State * L)
{
    //bool SliderFloat2(char const * label, float v[2], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiSliderFloat3(lua_State * L)
{
    //bool SliderFloat3(char const * label, float v[3], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiSliderFloat4(lua_State * L)
{
    //bool SliderFloat4(char const * label, float v[4], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiSliderAngle(lua_State * L)
{
    //bool SliderAngle(char const * label, float * v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, char const * format = "%.0f deg");
    return 0;
}

static int _GuiSliderInt(lua_State * L)
{
    //bool SliderInt(char const * label, int * v, int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _GuiSliderInt2(lua_State * L)
{
    //bool SliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _GuiSliderInt3(lua_State * L)
{
    //bool SliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _GuiSliderInt4(lua_State * L)
{
    //bool SliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _GuiSliderScalar(lua_State * L)
{
    //bool SliderScalar(char const * label, DataType data_type, void * v, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiSliderScalarN(lua_State * L)
{
    //bool SliderScalarN(char const * label, DataType data_type, void * v, int components, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiVSliderFloat(lua_State * L)
{
    //bool VSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _GuiVSliderInt(lua_State * L)
{
    //bool VSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _GuiVSliderScalar(lua_State * L)
{
    //bool VSliderScalar(char const * label, Vector2 const & size, DataType data_type, void * v, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _GuiInputText(lua_State * L)
{
    //bool InputText(char const * label, char * buf, size_t buf_size, InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputTextMultiline(lua_State * L)
{
    //bool InputTextMultiline(char const * label, char * buf, size_t buf_size, Vector2 const & size = Vector2{0,0}, InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputTextWithHint(lua_State * L)
{
    //bool InputTextWithHint(char const * label, char const * hint, char * buf, size_t buf_size, InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputFloat(lua_State * L)
{
    //bool InputFloat(char const * label, float * v, float step = 0.0f, float step_fast = 0.0f, char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputFloat2(lua_State * L)
{
    //bool InputFloat2(char const * label, float v[2], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputFloat3(lua_State * L)
{
    //bool InputFloat3(char const * label, float v[3], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputFloat4(lua_State * L)
{
    //bool InputFloat4(char const * label, float v[4], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputInt(lua_State * L)
{
    //bool InputInt(char const * label, int * v, int step = 1, int step_fast = 100, InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputInt2(lua_State * L)
{
    //bool InputInt2(char const * label, int v[2], InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputInt3(lua_State * L)
{
    //bool InputInt3(char const * label, int v[3], InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputInt4(lua_State * L)
{
    //bool InputInt4(char const * label, int v[4], InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputDouble(lua_State * L)
{
    //bool InputDouble(char const * label, double * v, double step = 0.0, double step_fast = 0.0, char const * format = "%.6f", InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputScalar(lua_State * L)
{
    //bool InputScalar(char const * label, DataType data_type, void * v, void const * step = nullptr, void const * step_fast = nullptr, char const * format = nullptr, InputTextFlags flags = 0);
    return 0;
}

static int _GuiInputScalarN(lua_State * L)
{
    //bool InputScalarN(char const * label, DataType data_type, void * v, int components, void const * step = nullptr, void const * step_fast = nullptr, char const * format = nullptr, InputTextFlags flags = 0);
    return 0;
}

static int _GuiColorEdit3(lua_State * L)
{
    //bool ColorEdit3(char const * label, float col[3], ColorEditFlags flags = 0);
    return 0;
}

static int _GuiColorEdit4(lua_State * L)
{
    //bool ColorEdit4(char const * label, float col[4], ColorEditFlags flags = 0);
    return 0;
}

static int _GuiColorPicker3(lua_State * L)
{
    //bool ColorPicker3(char const * label, float col[3], ColorEditFlags flags = 0);
    return 0;
}

static int _GuiColorPicker4(lua_State * L)
{
    //bool ColorPicker4(char const * label, float col[4], ColorEditFlags flags = 0, float const * ref_col = nullptr);
    return 0;
}

static int _GuiColorButton(lua_State * L)
{
    //bool ColorButton(char const * desc_id, Vector4 const & col, ColorEditFlags flags = 0, Vector2 size = Vector2{0,0});
    return 0;
}

static int _GuiSetColorEditOptions(lua_State * L)
{
    //void SetColorEditOptions(ColorEditFlags flags);
    return 0;
}

static int _GuiTreeNode(lua_State * L)
{
    //bool TreeNode(char const * label);
    //bool TreeNode(char const * str_id, char const * text);
    return 0;
}

static int _GuiTreeNodeEx(lua_State * L)
{
    //bool TreeNodeEx(char const * label, TreeNodeFlags flags = 0);
    //bool TreeNodeEx(char const * str_id, TreeNodeFlags flags, char const * text);
    return 0;
}

static int _GuiTreePush(lua_State * L)
{
    //void TreePush(char const * str_id);
    //void TreePush(void const * ptr_id = nullptr);
    return 0;
}

static int _GuiTreePop(lua_State * L)
{
    //void TreePop();
    return 0;
}

static int _GuiTreeAdvanceToLabelPos(lua_State * L)
{
    //void TreeAdvanceToLabelPos();
    return 0;
}

static int _GuiGetTreeNodeToLabelSpacing(lua_State * L)
{
    //float GetTreeNodeToLabelSpacing();
    return 0;
}

static int _GuiSetNextTreeNodeOpen(lua_State * L)
{
    //void SetNextTreeNodeOpen(bool is_open, Cond cond = 0);
    return 0;
}

static int _GuiCollapsingHeader(lua_State * L)
{
    //bool CollapsingHeader(char const * label, TreeNodeFlags flags = 0);
    //bool CollapsingHeader(char const * label, bool * p_open, TreeNodeFlags flags = 0);
    return 0;
}

static int _GuiSelectable(lua_State * L)
{
    //bool Selectable(char const * label, bool selected = false, SelectableFlags flags = 0, Vector2 const & size = Vector2{0,0});
    //bool Selectable(char const * label, bool * p_selected, SelectableFlags flags = 0, Vector2 const & size = Vector2{0,0});
    return 0;
}

static int _GuiListBox(lua_State * L)
{
    //bool ListBox(char const * label, int * current_item, char const * const items[], int items_count, int height_in_items = -1);
    //bool ListBox(char const * label, int * current_item, bool (*items_getter)(void * data, int idx, char const ** out_text), void * data, int items_count, int height_in_items = -1);
    return 0;
}

static int _GuiListBoxHeader(lua_State * L)
{
    //bool ListBoxHeader(char const * label, Vector2 const & size = Vector2{0,0});
    //bool ListBoxHeader(char const * label, int items_count, int height_in_items = -1);
    return 0;
}

static int _GuiListBoxFooter(lua_State * L)
{
    //void ListBoxFooter();
    return 0;
}

static int _GuiPlotLines(lua_State * L)
{
    //void PlotLines(char const * label, float const * values, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
    //void PlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
    return 0;
}

static int _GuiPlotHistogram(lua_State * L)
{
    //void PlotHistogram(char const * label, float const * values, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
    //void PlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
    return 0;
}

static int _GuiValue(lua_State * L)
{
    //void Value(char const * prefix, bool b);
    //void Value(char const * prefix, int v);
    //void Value(char const * prefix, unsigned int v);
    //void Value(char const * prefix, float v, char const * float_format = nullptr);
    return 0;
}

static int _GuiBeginMainMenuBar(lua_State * L)
{
    //bool BeginMainMenuBar();
    return 0;
}

static int _GuiEndMainMenuBar(lua_State * L)
{
    //void EndMainMenuBar();
    return 0;
}

static int _GuiBeginMenuBar(lua_State * L)
{
    //bool BeginMenuBar();
    return 0;
}

static int _GuiEndMenuBar(lua_State * L)
{
    //void EndMenuBar();
    return 0;
}

static int _GuiBeginMenu(lua_State * L)
{
    //bool BeginMenu(char const * label, bool enabled = true);
    return 0;
}

static int _GuiEndMenu(lua_State * L)
{
    //void EndMenu();
    return 0;
}

static int _GuiMenuItem(lua_State * L)
{
    //bool MenuItem(char const * label, char const * shortcut = nullptr, bool selected = false, bool enabled = true);
    //bool MenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled = true);
    return 0;
}

static int _GuiBeginTooltip(lua_State * L)
{
    //void BeginTooltip();
    return 0;
}

static int _GuiEndTooltip(lua_State * L)
{
    //void EndTooltip();
    return 0;
}

static int _GuiSetTooltip(lua_State * L)
{
    //void SetTooltip(char const * text);
    return 0;
}

static int _GuiOpenPopup(lua_State * L)
{
    //void OpenPopup(char const * str_id);
    return 0;
}

static int _GuiBeginPopup(lua_State * L)
{
    //bool BeginPopup(char const * str_id, WindowFlags flags = 0);
    return 0;
}

static int _GuiBeginPopupContextItem(lua_State * L)
{
    //bool BeginPopupContextItem(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _GuiBeginPopupContextWindow(lua_State * L)
{
    //bool BeginPopupContextWindow(char const * str_id = nullptr, int mouse_button = 1, bool also_over_items = true);
    return 0;
}

static int _GuiBeginPopupContextVoid(lua_State * L)
{
    //bool BeginPopupContextVoid(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _GuiBeginPopupModal(lua_State * L)
{
    //bool BeginPopupModal(char const * name, bool * p_open = nullptr, WindowFlags flags = 0);
    return 0;
}

static int _GuiEndPopup(lua_State * L)
{
    //void EndPopup();
    return 0;
}

static int _GuiOpenPopupOnItemClick(lua_State * L)
{
    //bool OpenPopupOnItemClick(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _GuiIsPopupOpen(lua_State * L)
{
    //bool IsPopupOpen(char const * str_id);
    return 0;
}

static int _GuiCloseCurrentPopup(lua_State * L)
{
    //void CloseCurrentPopup();
    return 0;
}

static int _GuiColumns(lua_State * L)
{
    //void Columns(int count = 1, char const * id = nullptr, bool border = true);
    return 0;
}

static int _GuiNextColumn(lua_State * L)
{
    //void NextColumn();
    return 0;
}

static int _GuiGetColumnIndex(lua_State * L)
{
    //int GetColumnIndex();
    return 0;
}

static int _GuiGetColumnWidth(lua_State * L)
{
    //float GetColumnWidth(int column_index = -1);
    return 0;
}

static int _GuiSetColumnWidth(lua_State * L)
{
    //void SetColumnWidth(int column_index, float width);
    return 0;
}

static int _GuiGetColumnOffset(lua_State * L)
{
    //float GetColumnOffset(int column_index = -1);
    return 0;
}

static int _GuiSetColumnOffset(lua_State * L)
{
    //void SetColumnOffset(int column_index, float offset_x);
    return 0;
}

static int _GuiGetColumnsCount(lua_State * L)
{
    //int GetColumnsCount();
    return 0;
}

static int _GuiBeginTabBar(lua_State * L)
{
    //bool BeginTabBar(char const * str_id, TabBarFlags flags = 0);
    return 0;
}

static int _GuiEndTabBar(lua_State * L)
{
    //void EndTabBar();
    return 0;
}

static int _GuiBeginTabItem(lua_State * L)
{
    //bool BeginTabItem(char const * label, bool * p_open = nullptr, TabItemFlags flags = 0);
    return 0;
}

static int _GuiEndTabItem(lua_State * L)
{
    //void EndTabItem();
    return 0;
}

static int _GuiSetTabItemClosed(lua_State * L)
{
    //void SetTabItemClosed(char const * tab_or_docked_window_label);
    return 0;
}

static int _GuiLogToTTY(lua_State * L)
{
    //void LogToTTY(int auto_open_depth = -1);
    return 0;
}

static int _GuiLogToFile(lua_State * L)
{
    //void LogToFile(int auto_open_depth = -1, char const * filename = nullptr);
    return 0;
}

static int _GuiLogToClipboard(lua_State * L)
{
    //void LogToClipboard(int auto_open_depth = -1);
    return 0;
}

static int _GuiLogFinish(lua_State * L)
{
    //void LogFinish();
    return 0;
}

static int _GuiLogButtons(lua_State * L)
{
    //void LogButtons();
    return 0;
}

static int _GuiLogText(lua_State * L)
{
    //void LogText(char const * text);
    return 0;
}

static int _GuiBeginDragDropSource(lua_State * L)
{
    //bool BeginDragDropSource(DragDropFlags flags = 0);
    return 0;
}

static int _GuiSetDragDropPayload(lua_State * L)
{
    //bool SetDragDropPayload(char const * type, void const * data, size_t sz, Cond cond = 0);
    return 0;
}

static int _GuiEndDragDropSource(lua_State * L)
{
    //void EndDragDropSource();
    return 0;
}

static int _GuiBeginDragDropTarget(lua_State * L)
{
    //bool BeginDragDropTarget();
    return 0;
}

static int _GuiAcceptDragDropPayload(lua_State * L)
{
    //Payload AcceptDragDropPayload(char const * type, DragDropFlags flags = 0);
    return 0;
}

static int _GuiEndDragDropTarget(lua_State * L)
{
    //void EndDragDropTarget();
    return 0;
}

static int _GuiGetDragDropPayload(lua_State * L)
{
    //Payload GetDragDropPayload();
    return 0;
}

static int _GuiPushClipRect(lua_State * L)
{
    //void PushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max, bool intersect_with_current_clip_rect);
    return 0;
}

static int _GuiPopClipRect(lua_State * L)
{
    //void PopClipRect();
    return 0;
}

static int _GuiSetItemDefaultFocus(lua_State * L)
{
    //void SetItemDefaultFocus();
    return 0;
}

static int _GuiSetKeyboardFocusHere(lua_State * L)
{
    //void SetKeyboardFocusHere(int offset = 0);
    return 0;
}

static int _GuiIsItemHovered(lua_State * L)
{
    //bool IsItemHovered(HoveredFlags flags = 0);
    return 0;
}

static int _GuiIsItemActive(lua_State * L)
{
    //bool IsItemActive();
    return 0;
}

static int _GuiIsItemFocused(lua_State * L)
{
    //bool IsItemFocused();
    return 0;
}

static int _GuiIsItemClicked(lua_State * L)
{
    //bool IsItemClicked(int mouse_button = 0);
    return 0;
}

static int _GuiIsItemVisible(lua_State * L)
{
    //bool IsItemVisible();
    return 0;
}

static int _GuiIsItemEdited(lua_State * L)
{
    //bool IsItemEdited();
    return 0;
}

static int _GuiIsItemActivated(lua_State * L)
{
    //bool IsItemActivated();
    return 0;
}

static int _GuiIsItemDeactivated(lua_State * L)
{
    //bool IsItemDeactivated();
    return 0;
}

static int _GuiIsItemDeactivatedAfterEdit(lua_State * L)
{
    //bool IsItemDeactivatedAfterEdit();
    return 0;
}

static int _GuiIsAnyItemHovered(lua_State * L)
{
    //bool IsAnyItemHovered();
    return 0;
}

static int _GuiIsAnyItemActive(lua_State * L)
{
    //bool IsAnyItemActive();
    return 0;
}

static int _GuiIsAnyItemFocused(lua_State * L)
{
    //bool IsAnyItemFocused();
    return 0;
}

static int _GuiGetItemRectMin(lua_State * L)
{
    //Vector2 GetItemRectMin();
    return 0;
}

static int _GuiGetItemRectMax(lua_State * L)
{
    //Vector2 GetItemRectMax();
    return 0;
}

static int _GuiGetItemRectSize(lua_State * L)
{
    //Vector2 GetItemRectSize();
    return 0;
}

static int _GuiSetItemAllowOverlap(lua_State * L)
{
    //void SetItemAllowOverlap();
    return 0;
}

static int _GuiIsRectVisible(lua_State * L)
{
    //bool IsRectVisible(Vector2 const & size);
    //bool IsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max);
    return 0;
}

static int _GuiGetTime(lua_State * L)
{
    //double GetTime();
    return 0;
}

static int _GuiGetFrameCount(lua_State * L)
{
    //int GetFrameCount();
    return 0;
}

static int _GuiGetStyleColorName(lua_State * L)
{
    //char const * GetStyleColorName(Col idx);
    return 0;
}

static int _GuiCalcTextSize(lua_State * L)
{
    //Vector2 CalcTextSize(char const * text, char const * text_end = nullptr, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
    return 0;
}

static int _GuiCalcListClipping(lua_State * L)
{
    //void CalcListClipping(int items_count, float items_height, int * out_items_display_start, int * out_items_display_end);
    return 0;
}

static int _GuiBeginChildFrame(lua_State * L)
{
    //bool BeginChildFrame(ID id, Vector2 const & size, WindowFlags flags = 0);
    return 0;
}

static int _GuiEndChildFrame(lua_State * L)
{
    //void EndChildFrame();
    return 0;
}

static int _GuiColorConvertU32ToFloat4(lua_State * L)
{
    //Vector4 ColorConvertU32ToFloat4(unsigned int in);
    return 0;
}

static int _GuiColorConvertFloat4ToU32(lua_State * L)
{
    //unsigned int ColorConvertFloat4ToU32(Vector4 const & in);
    return 0;
}

static int _GuiColorConvertRGBtoHSV(lua_State * L)
{
    //void ColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v);
    return 0;
}

static int _GuiColorConvertHSVtoRGB(lua_State * L)
{
    //void ColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b);
    return 0;
}

static int _GuiGetKeyIndex(lua_State * L)
{
    //int GetKeyIndex(Key imgui_key);
    return 0;
}

static int _GuiIsKeyDown(lua_State * L)
{
    //bool IsKeyDown(int user_key_index);
    return 0;
}

static int _GuiIsKeyPressed(lua_State * L)
{
    //bool IsKeyPressed(int user_key_index, bool repeat = true);
    return 0;
}

static int _GuiIsKeyReleased(lua_State * L)
{
    //bool IsKeyReleased(int user_key_index);
    return 0;
}

static int _GuiGetKeyPressedAmount(lua_State * L)
{
    //int GetKeyPressedAmount(int key_index, float repeat_delay, float rate);
    return 0;
}

static int _GuiIsMouseDown(lua_State * L)
{
    //bool IsMouseDown(int button);
    return 0;
}

static int _GuiIsAnyMouseDown(lua_State * L)
{
    //bool IsAnyMouseDown();
    return 0;
}

static int _GuiIsMouseClicked(lua_State * L)
{
    //bool IsMouseClicked(int button, bool repeat = false);
    return 0;
}

static int _GuiIsMouseDoubleClicked(lua_State * L)
{
    //bool IsMouseDoubleClicked(int button);
    return 0;
}

static int _GuiIsMouseReleased(lua_State * L)
{
    //bool IsMouseReleased(int button);
    return 0;
}

static int _GuiIsMouseDragging(lua_State * L)
{
    //bool IsMouseDragging(int button = 0, float lock_threshold = -1.0f);
    return 0;
}

static int _GuiIsMouseHoveringRect(lua_State * L)
{
    //bool IsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip = true);
    return 0;
}

static int _GuiIsMousePosValid(lua_State * L)
{
    //bool IsMousePosValid(Vector2 const * mouse_pos = nullptr);
    return 0;
}

static int _GuiGetMousePos(lua_State * L)
{
    //Vector2 GetMousePos();
    return 0;
}

static int _GuiGetMousePosOnOpeningCurrentPopup(lua_State * L)
{
    //Vector2 GetMousePosOnOpeningCurrentPopup();
    return 0;
}

static int _GuiGetMouseDragDelta(lua_State * L)
{
    //Vector2 GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);
    return 0;
}

static int _GuiResetMouseDragDelta(lua_State * L)
{
    //void ResetMouseDragDelta(int button = 0);
    return 0;
}

static int _GuiGetMouseCursor(lua_State * L)
{
    //MouseCursor GetMouseCursor();
    return 0;
}

static int _GuiSetMouseCursor(lua_State * L)
{
    //void SetMouseCursor(MouseCursor type);
    return 0;
}

static int _GuiCaptureKeyboardFromApp(lua_State * L)
{
    //void CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);
    return 0;
}

static int _GuiCaptureMouseFromApp(lua_State * L)
{
    //void CaptureMouseFromApp(bool want_capture_mouse_value = true);
    return 0;
}

static int _GuiGetClipboardText(lua_State * L)
{
    //char const * GetClipboardText();
    return 0;
}

static int _GuiSetClipboardText(lua_State * L)
{
    //void SetClipboardText(char const * text);
    return 0;
}

static int _GuiLoadIniSettingsFromDisk(lua_State * L)
{
    //void LoadIniSettingsFromDisk(char const * ini_filename);
    return 0;
}

static int _GuiLoadIniSettingsFromMemory(lua_State * L)
{
    //void LoadIniSettingsFromMemory(char const * ini_data, size_t ini_size = 0);
    return 0;
}

static int _GuiSaveIniSettingsToDisk(lua_State * L)
{
    //void SaveIniSettingsToDisk(char const * ini_filename);
    return 0;
}

static int _GuiSaveIniSettingsToMemory(lua_State * L)
{
    // char const * SaveIniSettingsToMemory(size_t * out_ini_size = nullptr);
    return 0;
}

#ifndef RAYGUI_REGISTER
#define RAYGUI_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_lay_gui[] = {
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
        RAYGUI_REGISTER(GuiDragScalar),
        RAYGUI_REGISTER(GuiDragScalarN),

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
        RAYGUI_REGISTER(GuiSliderScalar),
        RAYGUI_REGISTER(GuiSliderScalarN),
        RAYGUI_REGISTER(GuiVSliderFloat),
        RAYGUI_REGISTER(GuiVSliderInt),
        RAYGUI_REGISTER(GuiVSliderScalar),

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
        RAYGUI_REGISTER(GuiInputScalar),
        RAYGUI_REGISTER(GuiInputScalarN),

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
        RAYGUI_REGISTER(GuiTreePop),
        RAYGUI_REGISTER(GuiTreeAdvanceToLabelPos),
        RAYGUI_REGISTER(GuiGetTreeNodeToLabelSpacing),
        RAYGUI_REGISTER(GuiSetNextTreeNodeOpen),
        RAYGUI_REGISTER(GuiCollapsingHeader),

        // Widgets: Selectables
        RAYGUI_REGISTER(GuiSelectable),

        // Widgets: List Boxes
        RAYGUI_REGISTER(GuiListBox),
        RAYGUI_REGISTER(GuiListBoxHeader),
        RAYGUI_REGISTER(GuiListBoxFooter),

        // Widgets: Data Plotting
        RAYGUI_REGISTER(GuiPlotLines),
        RAYGUI_REGISTER(GuiPlotHistogram),

        // Widgets: Value),
        RAYGUI_REGISTER(GuiValue),

        // Widgets: Menus
        RAYGUI_REGISTER(GuiBeginMainMenuBar),
        RAYGUI_REGISTER(GuiEndMainMenuBar),
        RAYGUI_REGISTER(GuiBeginMenuBar),
        RAYGUI_REGISTER(GuiEndMenuBar),
        RAYGUI_REGISTER(GuiBeginMenu),
        RAYGUI_REGISTER(GuiEndMenu),
        RAYGUI_REGISTER(GuiMenuItem),

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

bool luaopen_raygui(lua_State * L)
{
    luaL_register(L, lua_raygui_name(), __lua_lay_gui);
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

