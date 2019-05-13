/**
 * @file   RayGuiLua.cpp
 * @brief  RayGuiLua class implementation.
 * @author zer0
 * @date   2019-05-07
 */

#include <libtbag/script/lua/RayGuiLua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray::gui;

static int luaL_unsupport(lua_State * L)
{
    return luaL_error(L, "Unsupported operation error");
}

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

static int _ShowDemoWindow(lua_State * L)
{
    ShowDemoWindow();
    return 0;
}

static int _ShowAboutWindow(lua_State * L)
{
    ShowAboutWindow();
    return 0;
}

static int _ShowMetricsWindow(lua_State * L)
{
    ShowMetricsWindow();
    return 0;
}

static int _ShowStyleEditor(lua_State * L)
{
    ShowStyleEditor();
    return 0;
}

static int _ShowStyleSelector(lua_State * L)
{
    lua_pushboolean(L, ShowStyleSelector(luaL_checkstring(L, 1)));
    return 1;
}

static int _ShowFontSelector(lua_State * L)
{
    ShowFontSelector(luaL_checkstring(L, 1));
    return 0;
}

static int _ShowUserGuide(lua_State * L)
{
    ShowUserGuide();
    return 0;
}

static int _GetVersion(lua_State * L)
{
    lua_pushstring(L, GetVersion());
    return 1;
}

static int _StyleColorsDark(lua_State * L)
{
    StyleColorsDark();
    return 0;
}

static int _StyleColorsClassic(lua_State * L)
{
    StyleColorsClassic();
    return 0;
}

static int _StyleColorsLight(lua_State * L)
{
    StyleColorsLight();
    return 0;
}

static int _GetStyle(lua_State * L)
{
    luaL_unsupport(L);
    return 0;
}

static int _SetStyle(lua_State * L)
{
    luaL_unsupport(L);
    return 0;
}

static int _Begin(lua_State * L)
{
    lua_pushboolean(L, Begin(luaL_checkstring(L, 1), nullptr, luaL_checkinteger(L, 2)));
    return 1;
}

static int _End(lua_State * L)
{
    End();
    return 0;
}

static int _BeginChild(lua_State * L)
{
    //bool BeginChild(char const * str_id, Vector2 const & size = Vector2{0,0}, bool border = false, WindowFlags flags = 0);
    //bool BeginChild(ID id, Vector2 const & size = Vector2{0,0}, bool border = false, WindowFlags flags = 0);
    return 0;
}

static int _EndChild(lua_State * L)
{
    //void EndChild();
    return 0;
}

static int _IsWindowAppearing(lua_State * L)
{
    //bool IsWindowAppearing();
    return 0;
}

static int _IsWindowCollapsed(lua_State * L)
{
    //bool IsWindowCollapsed();
    return 0;
}

static int _IsWindowFocused(lua_State * L)
{
    //bool IsWindowFocused(FocusedFlags flags = 0);
    return 0;
}

static int _IsWindowHovered(lua_State * L)
{
    //bool IsWindowHovered(HoveredFlags flags = 0);
    return 0;
}

static int _GetWindowPos(lua_State * L)
{
    //Vector2 GetWindowPos();
    return 0;
}

static int _GetWindowSize(lua_State * L)
{
    //Vector2 GetWindowSize();
    return 0;
}

static int _GetWindowWidth(lua_State * L)
{
    //float GetWindowWidth();
    return 0;
}

static int _GetWindowHeight(lua_State * L)
{
    //float GetWindowHeight();
    return 0;
}

static int _SetNextWindowPos(lua_State * L)
{
    //void SetNextWindowPos(Vector2 const & pos, Cond cond = 0, Vector2 const & pivot = Vector2{0,0});
    return 0;
}

static int _SetNextWindowSize(lua_State * L)
{
    //void SetNextWindowSize(Vector2 const & size, Cond cond = 0);
    return 0;
}

static int _SetNextWindowContentSize(lua_State * L)
{
    //void SetNextWindowContentSize(Vector2 const & size);
    return 0;
}

static int _SetNextWindowCollapsed(lua_State * L)
{
    //void SetNextWindowCollapsed(bool collapsed, Cond cond = 0);
    return 0;
}

static int _SetNextWindowFocus(lua_State * L)
{
    //void SetNextWindowFocus();
    return 0;
}

static int _SetNextWindowBgAlpha(lua_State * L)
{
    //void SetNextWindowBgAlpha(float alpha);
    return 0;
}

static int _SetWindowPos(lua_State * L)
{
    //void SetWindowPos(Vector2 const & pos, Cond cond = 0);
    //void SetWindowPos(char const * name, Vector2 const & pos, Cond cond = 0);
    return 0;
}

static int _SetWindowSize(lua_State * L)
{
    //void SetWindowSize(Vector2 const & size, Cond cond = 0);
    //void SetWindowSize(char const * name, Vector2 const & size, Cond cond = 0);
    return 0;
}

static int _SetWindowCollapsed(lua_State * L)
{
    //void SetWindowCollapsed(bool collapsed, Cond cond = 0);
    //void SetWindowCollapsed(char const * name, bool collapsed, Cond cond = 0);
    return 0;
}

static int _SetWindowFocus(lua_State * L)
{
    //void SetWindowFocus();
    //void SetWindowFocus(char const * name);
    return 0;
}

static int _SetWindowFontScale(lua_State * L)
{
    //void SetWindowFontScale(float scale);
    return 0;
}

static int _GetContentRegionMax(lua_State * L)
{
    //Vector2 GetContentRegionMax();
    return 0;
}

static int _GetContentRegionAvail(lua_State * L)
{
    //Vector2 GetContentRegionAvail();
    return 0;
}

static int _GetWindowContentRegionMin(lua_State * L)
{
    //Vector2 GetWindowContentRegionMin();
    return 0;
}

static int _GetWindowContentRegionMax(lua_State * L)
{
    //Vector2 GetWindowContentRegionMax();
    return 0;
}

static int _GetWindowContentRegionWidth(lua_State * L)
{
    //float GetWindowContentRegionWidth();
    return 0;
}

static int _GetScrollX(lua_State * L)
{
    //float GetScrollX();
    return 0;
}

static int _GetScrollY(lua_State * L)
{
    //float GetScrollY();
    return 0;
}

static int _GetScrollMaxX(lua_State * L)
{
    //float GetScrollMaxX();
    return 0;
}

static int _GetScrollMaxY(lua_State * L)
{
    //float GetScrollMaxY();
    return 0;
}

static int _SetScrollX(lua_State * L)
{
    //void SetScrollX(float scroll_x);
    return 0;
}

static int _SetScrollY(lua_State * L)
{
    //void SetScrollY(float scroll_y);
    return 0;
}

static int _SetScrollHereY(lua_State * L)
{
    //void SetScrollHereY(float center_y_ratio = 0.5f);
    return 0;
}

static int _SetScrollFromPosY(lua_State * L)
{
    //void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);
    return 0;
}

static int _PushStyleColor(lua_State * L)
{
    //void PushStyleColor(Col idx, unsigned int col);
    //void PushStyleColor(Col idx, Vector4 const & col);
    return 0;
}

static int _PopStyleColor(lua_State * L)
{
    //void PopStyleColor(int count = 1);
    return 0;
}

static int _PushStyleVar(lua_State * L)
{
    //void PushStyleVar(StyleVar idx, float val);
    //void PushStyleVar(StyleVar idx, Vector2 const & val);
    return 0;
}

static int _PopStyleVar(lua_State * L)
{
    //void PopStyleVar(int count = 1);
    return 0;
}

static int _GetStyleColorVec4(lua_State * L)
{
    //Vector4 GetStyleColorVec4(Col idx);
    return 0;
}

static int _GetFontSize(lua_State * L)
{
    //float GetFontSize();
    return 0;
}

static int _GetFontTexUvWhitePixel(lua_State * L)
{
    //Vector2 GetFontTexUvWhitePixel();
    return 0;
}

static int _GetColorU32(lua_State * L)
{
    //unsigned int GetColorU32(Col idx, float alpha_mul = 1.0f);
    //unsigned int GetColorU32(Vector4 const & col);
    //unsigned int GetColorU32(unsigned int col);
    return 0;
}

static int _PushItemWidth(lua_State * L)
{
    //void PushItemWidth(float item_width);
    return 0;
}

static int _PopItemWidth(lua_State * L)
{
    //void PopItemWidth();
    return 0;
}

static int _SetNextItemWidth(lua_State * L)
{
    //void SetNextItemWidth(float item_width);
    return 0;
}

static int _CalcItemWidth(lua_State * L)
{
    //float CalcItemWidth();
    return 0;
}

static int _PushTextWrapPos(lua_State * L)
{
    //void PushTextWrapPos(float wrap_local_pos_x = 0.0f);
    return 0;
}

static int _PopTextWrapPos(lua_State * L)
{
    //void PopTextWrapPos();
    return 0;
}

static int _PushAllowKeyboardFocus(lua_State * L)
{
    //void PushAllowKeyboardFocus(bool allow_keyboard_focus);
    return 0;
}

static int _PopAllowKeyboardFocus(lua_State * L)
{
    //void PopAllowKeyboardFocus();
    return 0;
}

static int _PushButtonRepeat(lua_State * L)
{
    //void PushButtonRepeat(bool repeat);
    return 0;
}

static int _PopButtonRepeat(lua_State * L)
{
    //void PopButtonRepeat();
    return 0;
}

static int _Separator(lua_State * L)
{
    //void Separator();
    return 0;
}

static int _SameLine(lua_State * L)
{
    //void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
    return 0;
}

static int _NewLine(lua_State * L)
{
    //void NewLine();
    return 0;
}

static int _Spacing(lua_State * L)
{
    //void Spacing();
    return 0;
}

static int _Dummy(lua_State * L)
{
    //void Dummy(Vector2 const & size);
    return 0;
}

static int _Indent(lua_State * L)
{
    //void Indent(float indent_w = 0.0f);
    return 0;
}

static int _Unindent(lua_State * L)
{
    //void Unindent(float indent_w = 0.0f);
    return 0;
}

static int _BeginGroup(lua_State * L)
{
    //void BeginGroup();
    return 0;
}

static int _EndGroup(lua_State * L)
{
    //void EndGroup();
    return 0;
}

static int _GetCursorPos(lua_State * L)
{
    //Vector2 GetCursorPos();
    return 0;
}

static int _GetCursorPosX(lua_State * L)
{
    //float GetCursorPosX();
    return 0;
}

static int _GetCursorPosY(lua_State * L)
{
    //float GetCursorPosY();
    return 0;
}

static int _SetCursorPos(lua_State * L)
{
    //void SetCursorPos(Vector2 const & local_pos);
    return 0;
}

static int _SetCursorPosX(lua_State * L)
{
    //void SetCursorPosX(float local_x);
    return 0;
}

static int _SetCursorPosY(lua_State * L)
{
    //void SetCursorPosY(float local_y);
    return 0;
}

static int _GetCursorStartPos(lua_State * L)
{
    //Vector2 GetCursorStartPos();
    return 0;
}

static int _GetCursorScreenPos(lua_State * L)
{
    //Vector2 GetCursorScreenPos();
    return 0;
}

static int _SetCursorScreenPos(lua_State * L)
{
    //void SetCursorScreenPos(Vector2 const & pos);
    return 0;
}

static int _AlignTextToFramePadding(lua_State * L)
{
    //void AlignTextToFramePadding();
    return 0;
}

static int _GetTextLineHeight(lua_State * L)
{
    //float GetTextLineHeight();
    return 0;
}

static int _GetTextLineHeightWithSpacing(lua_State * L)
{
    //float GetTextLineHeightWithSpacing();
    return 0;
}

static int _GetFrameHeight(lua_State * L)
{
    //float GetFrameHeight();
    return 0;
}

static int _GetFrameHeightWithSpacing(lua_State * L)
{
    //float GetFrameHeightWithSpacing();
    return 0;
}

static int _PushID(lua_State * L)
{
    //void PushID(char const * str_id);
    //void PushID(char const * str_id_begin, char const * str_id_end);
    //void PushID(void const * ptr_id);
    //void PushID(int int_id);
    return 0;
}

static int _PopID(lua_State * L)
{
    //void PopID();
    return 0;
}

static int _GetID(lua_State * L)
{
    //ID GetID(char const * str_id);
    //ID GetID(char const * str_id_begin, char const * str_id_end);
    //ID GetID(void const * ptr_id);
    return 0;
}

static int _TextUnformatted(lua_State * L)
{
    TextUnformatted(luaL_checkstring(L, 1), luaL_optstring(L, 2, nullptr));
    return 0;
}

static int _Text(lua_State * L)
{
    Text(luaL_checkstring(L, 1));
    return 0;
}

static int _TextColored(lua_State * L)
{
    //void TextColored(Vector4 const & col, char const * text);
    return 0;
}

static int _TextDisabled(lua_State * L)
{
    //void TextDisabled(char const * text);
    return 0;
}

static int _TextWrapped(lua_State * L)
{
    //void TextWrapped(char const * text);
    return 0;
}

static int _LabelText(lua_State * L)
{
    //void LabelText(char const * label, char const * text);
    return 0;
}

static int _BulletText(lua_State * L)
{
    //void BulletText(char const * text);
    return 0;
}

static int _Button(lua_State * L)
{
    //bool Button(char const * label, Vector2 const & size = Vector2{0,0});
    return 0;
}

static int _SmallButton(lua_State * L)
{
    //bool SmallButton(char const * label);
    return 0;
}

static int _InvisibleButton(lua_State * L)
{
    //bool InvisibleButton(char const * str_id, Vector2 const & size);
    return 0;
}

static int _ArrowButton(lua_State * L)
{
    //bool ArrowButton(char const * str_id, Dir dir);
    return 0;
}

static int _Image(lua_State * L)
{
    //void Image(TextureID user_texture_id, Vector2 const & size, Vector2 const & uv0 = Vector2{0,0}, Vector2 const & uv1 = Vector2{1,1}, Vector4 const & tint_col = Vector4{1,1,1,1}, Vector4 const & border_col = Vector4{0,0,0,0});
    return 0;
}

static int _ImageButton(lua_State * L)
{
    //bool ImageButton(TextureID user_texture_id, Vector2 const & size, Vector2 const & uv0 = Vector2{0,0}, Vector2 const & uv1 = Vector2{1,1}, int frame_padding = -1, Vector4 const & bg_col = Vector4{0,0,0,0}, Vector4 const & tint_col = Vector4{1,1,1,1});
    return 0;
}

static int _Checkbox(lua_State * L)
{
    //bool Checkbox(char const * label, bool * v);
    return 0;
}

static int _CheckboxFlags(lua_State * L)
{
    //bool CheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value);
    return 0;
}

static int _RadioButton(lua_State * L)
{
    //bool RadioButton(char const * label, bool active);
    //bool RadioButton(char const * label, int * v, int v_button);
    return 0;
}

static int _ProgressBar(lua_State * L)
{
    //void ProgressBar(float fraction, Vector2 const & size_arg = Vector2{-1,0}, char const * overlay = nullptr);
    return 0;
}

static int _Bullet(lua_State * L)
{
    //void Bullet();
    return 0;
}

static int _BeginCombo(lua_State * L)
{
    //bool BeginCombo(char const * label, char const * preview_value, ComboFlags flags = 0);
    return 0;
}

static int _EndCombo(lua_State * L)
{
    //void EndCombo();
    return 0;
}

static int _Combo(lua_State * L)
{
    //bool Combo(char const * label, int * current_item, char const * const items[], int items_count, int popup_max_height_in_items = -1);
    //bool Combo(char const * label, int * current_item, char const * items_separated_by_zeros, int popup_max_height_in_items = -1);
    //bool Combo(char const * label, int * current_item, bool(*items_getter)(void * data, int idx, char const ** out_text), void * data, int items_count, int popup_max_height_in_items = -1);
    return 0;
}

static int _DragFloat(lua_State * L)
{
    //bool DragFloat(char const * label, float * v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _DragFloat2(lua_State * L)
{
    //bool DragFloat2(char const * label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _DragFloat3(lua_State * L)
{
    //bool DragFloat3(char const * label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _DragFloat4(lua_State * L)
{
    //bool DragFloat4(char const * label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _DragFloatRange2(lua_State * L)
{
    //bool DragFloatRange2(char const * label, float * v_current_min, float * v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, char const * format = "%.3f", char const * format_max = nullptr, float power = 1.0f);
    return 0;
}

static int _DragInt(lua_State * L)
{
    //bool DragInt(char const * label, int * v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _DragInt2(lua_State * L)
{
    //bool DragInt2(char const * label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _DragInt3(lua_State * L)
{
    //bool DragInt3(char const * label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _DragInt4(lua_State * L)
{
    //bool DragInt4(char const * label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d");
    return 0;
}

static int _DragIntRange2(lua_State * L)
{
    //bool DragIntRange2(char const * label, int * v_current_min, int * v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, char const * format = "%d", char const * format_max = nullptr);
    return 0;
}

static int _DragScalar(lua_State * L)
{
    //bool DragScalar(char const * label, DataType data_type, void * v, float v_speed, void const * v_min = nullptr, void const * v_max = nullptr, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _DragScalarN(lua_State * L)
{
    //bool DragScalarN(char const * label, DataType data_type, void * v, int components, float v_speed, void const * v_min = nullptr, void const * v_max = nullptr, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _SliderFloat(lua_State * L)
{
    //bool SliderFloat(char const * label, float * v, float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _SliderFloat2(lua_State * L)
{
    //bool SliderFloat2(char const * label, float v[2], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _SliderFloat3(lua_State * L)
{
    //bool SliderFloat3(char const * label, float v[3], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _SliderFloat4(lua_State * L)
{
    //bool SliderFloat4(char const * label, float v[4], float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _SliderAngle(lua_State * L)
{
    //bool SliderAngle(char const * label, float * v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, char const * format = "%.0f deg");
    return 0;
}

static int _SliderInt(lua_State * L)
{
    //bool SliderInt(char const * label, int * v, int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _SliderInt2(lua_State * L)
{
    //bool SliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _SliderInt3(lua_State * L)
{
    //bool SliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _SliderInt4(lua_State * L)
{
    //bool SliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _SliderScalar(lua_State * L)
{
    //bool SliderScalar(char const * label, DataType data_type, void * v, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _SliderScalarN(lua_State * L)
{
    //bool SliderScalarN(char const * label, DataType data_type, void * v, int components, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _VSliderFloat(lua_State * L)
{
    //bool VSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max, char const * format = "%.3f", float power = 1.0f);
    return 0;
}

static int _VSliderInt(lua_State * L)
{
    //bool VSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max, char const * format = "%d");
    return 0;
}

static int _VSliderScalar(lua_State * L)
{
    //bool VSliderScalar(char const * label, Vector2 const & size, DataType data_type, void * v, void const * v_min, void const * v_max, char const * format = nullptr, float power = 1.0f);
    return 0;
}

static int _InputText(lua_State * L)
{
    //bool InputText(char const * label, char * buf, size_t buf_size, InputTextFlags flags = 0);
    return 0;
}

static int _InputTextMultiline(lua_State * L)
{
    //bool InputTextMultiline(char const * label, char * buf, size_t buf_size, Vector2 const & size = Vector2{0,0}, InputTextFlags flags = 0);
    return 0;
}

static int _InputTextWithHint(lua_State * L)
{
    //bool InputTextWithHint(char const * label, char const * hint, char * buf, size_t buf_size, InputTextFlags flags = 0);
    return 0;
}

static int _InputFloat(lua_State * L)
{
    //bool InputFloat(char const * label, float * v, float step = 0.0f, float step_fast = 0.0f, char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _InputFloat2(lua_State * L)
{
    //bool InputFloat2(char const * label, float v[2], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _InputFloat3(lua_State * L)
{
    //bool InputFloat3(char const * label, float v[3], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _InputFloat4(lua_State * L)
{
    //bool InputFloat4(char const * label, float v[4], char const * format = "%.3f", InputTextFlags flags = 0);
    return 0;
}

static int _InputInt(lua_State * L)
{
    //bool InputInt(char const * label, int * v, int step = 1, int step_fast = 100, InputTextFlags flags = 0);
    return 0;
}

static int _InputInt2(lua_State * L)
{
    //bool InputInt2(char const * label, int v[2], InputTextFlags flags = 0);
    return 0;
}

static int _InputInt3(lua_State * L)
{
    //bool InputInt3(char const * label, int v[3], InputTextFlags flags = 0);
    return 0;
}

static int _InputInt4(lua_State * L)
{
    //bool InputInt4(char const * label, int v[4], InputTextFlags flags = 0);
    return 0;
}

static int _InputDouble(lua_State * L)
{
    //bool InputDouble(char const * label, double * v, double step = 0.0, double step_fast = 0.0, char const * format = "%.6f", InputTextFlags flags = 0);
    return 0;
}

static int _InputScalar(lua_State * L)
{
    //bool InputScalar(char const * label, DataType data_type, void * v, void const * step = nullptr, void const * step_fast = nullptr, char const * format = nullptr, InputTextFlags flags = 0);
    return 0;
}

static int _InputScalarN(lua_State * L)
{
    //bool InputScalarN(char const * label, DataType data_type, void * v, int components, void const * step = nullptr, void const * step_fast = nullptr, char const * format = nullptr, InputTextFlags flags = 0);
    return 0;
}

static int _ColorEdit3(lua_State * L)
{
    //bool ColorEdit3(char const * label, float col[3], ColorEditFlags flags = 0);
    return 0;
}

static int _ColorEdit4(lua_State * L)
{
    //bool ColorEdit4(char const * label, float col[4], ColorEditFlags flags = 0);
    return 0;
}

static int _ColorPicker3(lua_State * L)
{
    //bool ColorPicker3(char const * label, float col[3], ColorEditFlags flags = 0);
    return 0;
}

static int _ColorPicker4(lua_State * L)
{
    //bool ColorPicker4(char const * label, float col[4], ColorEditFlags flags = 0, float const * ref_col = nullptr);
    return 0;
}

static int _ColorButton(lua_State * L)
{
    //bool ColorButton(char const * desc_id, Vector4 const & col, ColorEditFlags flags = 0, Vector2 size = Vector2{0,0});
    return 0;
}

static int _SetColorEditOptions(lua_State * L)
{
    //void SetColorEditOptions(ColorEditFlags flags);
    return 0;
}

static int _TreeNode(lua_State * L)
{
    //bool TreeNode(char const * label);
    //bool TreeNode(char const * str_id, char const * text);
    return 0;
}

static int _TreeNodeEx(lua_State * L)
{
    //bool TreeNodeEx(char const * label, TreeNodeFlags flags = 0);
    //bool TreeNodeEx(char const * str_id, TreeNodeFlags flags, char const * text);
    return 0;
}

static int _TreePush(lua_State * L)
{
    //void TreePush(char const * str_id);
    //void TreePush(void const * ptr_id = nullptr);
    return 0;
}

static int _TreePop(lua_State * L)
{
    //void TreePop();
    return 0;
}

static int _TreeAdvanceToLabelPos(lua_State * L)
{
    //void TreeAdvanceToLabelPos();
    return 0;
}

static int _GetTreeNodeToLabelSpacing(lua_State * L)
{
    //float GetTreeNodeToLabelSpacing();
    return 0;
}

static int _SetNextTreeNodeOpen(lua_State * L)
{
    //void SetNextTreeNodeOpen(bool is_open, Cond cond = 0);
    return 0;
}

static int _CollapsingHeader(lua_State * L)
{
    //bool CollapsingHeader(char const * label, TreeNodeFlags flags = 0);
    //bool CollapsingHeader(char const * label, bool * p_open, TreeNodeFlags flags = 0);
    return 0;
}

static int _Selectable(lua_State * L)
{
    //bool Selectable(char const * label, bool selected = false, SelectableFlags flags = 0, Vector2 const & size = Vector2{0,0});
    //bool Selectable(char const * label, bool * p_selected, SelectableFlags flags = 0, Vector2 const & size = Vector2{0,0});
    return 0;
}

static int _ListBox(lua_State * L)
{
    //bool ListBox(char const * label, int * current_item, char const * const items[], int items_count, int height_in_items = -1);
    //bool ListBox(char const * label, int * current_item, bool (*items_getter)(void * data, int idx, char const ** out_text), void * data, int items_count, int height_in_items = -1);
    return 0;
}

static int _ListBoxHeader(lua_State * L)
{
    //bool ListBoxHeader(char const * label, Vector2 const & size = Vector2{0,0});
    //bool ListBoxHeader(char const * label, int items_count, int height_in_items = -1);
    return 0;
}

static int _ListBoxFooter(lua_State * L)
{
    //void ListBoxFooter();
    return 0;
}

static int _PlotLines(lua_State * L)
{
    //void PlotLines(char const * label, float const * values, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
    //void PlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
    return 0;
}

static int _PlotHistogram(lua_State * L)
{
    //void PlotHistogram(char const * label, float const * values, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
    //void PlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data, int values_count, int values_offset = 0, char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
    return 0;
}

static int _Value(lua_State * L)
{
    //void Value(char const * prefix, bool b);
    //void Value(char const * prefix, int v);
    //void Value(char const * prefix, unsigned int v);
    //void Value(char const * prefix, float v, char const * float_format = nullptr);
    return 0;
}

static int _BeginMainMenuBar(lua_State * L)
{
    //bool BeginMainMenuBar();
    return 0;
}

static int _EndMainMenuBar(lua_State * L)
{
    //void EndMainMenuBar();
    return 0;
}

static int _BeginMenuBar(lua_State * L)
{
    //bool BeginMenuBar();
    return 0;
}

static int _EndMenuBar(lua_State * L)
{
    //void EndMenuBar();
    return 0;
}

static int _BeginMenu(lua_State * L)
{
    //bool BeginMenu(char const * label, bool enabled = true);
    return 0;
}

static int _EndMenu(lua_State * L)
{
    //void EndMenu();
    return 0;
}

static int _MenuItem(lua_State * L)
{
    //bool MenuItem(char const * label, char const * shortcut = nullptr, bool selected = false, bool enabled = true);
    //bool MenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled = true);
    return 0;
}

static int _BeginTooltip(lua_State * L)
{
    //void BeginTooltip();
    return 0;
}

static int _EndTooltip(lua_State * L)
{
    //void EndTooltip();
    return 0;
}

static int _SetTooltip(lua_State * L)
{
    //void SetTooltip(char const * text);
    return 0;
}

static int _OpenPopup(lua_State * L)
{
    //void OpenPopup(char const * str_id);
    return 0;
}

static int _BeginPopup(lua_State * L)
{
    //bool BeginPopup(char const * str_id, WindowFlags flags = 0);
    return 0;
}

static int _BeginPopupContextItem(lua_State * L)
{
    //bool BeginPopupContextItem(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _BeginPopupContextWindow(lua_State * L)
{
    //bool BeginPopupContextWindow(char const * str_id = nullptr, int mouse_button = 1, bool also_over_items = true);
    return 0;
}

static int _BeginPopupContextVoid(lua_State * L)
{
    //bool BeginPopupContextVoid(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _BeginPopupModal(lua_State * L)
{
    //bool BeginPopupModal(char const * name, bool * p_open = nullptr, WindowFlags flags = 0);
    return 0;
}

static int _EndPopup(lua_State * L)
{
    //void EndPopup();
    return 0;
}

static int _OpenPopupOnItemClick(lua_State * L)
{
    //bool OpenPopupOnItemClick(char const * str_id = nullptr, int mouse_button = 1);
    return 0;
}

static int _IsPopupOpen(lua_State * L)
{
    //bool IsPopupOpen(char const * str_id);
    return 0;
}

static int _CloseCurrentPopup(lua_State * L)
{
    //void CloseCurrentPopup();
    return 0;
}

static int _Columns(lua_State * L)
{
    //void Columns(int count = 1, char const * id = nullptr, bool border = true);
    return 0;
}

static int _NextColumn(lua_State * L)
{
    //void NextColumn();
    return 0;
}

static int _GetColumnIndex(lua_State * L)
{
    //int GetColumnIndex();
    return 0;
}

static int _GetColumnWidth(lua_State * L)
{
    //float GetColumnWidth(int column_index = -1);
    return 0;
}

static int _SetColumnWidth(lua_State * L)
{
    //void SetColumnWidth(int column_index, float width);
    return 0;
}

static int _GetColumnOffset(lua_State * L)
{
    //float GetColumnOffset(int column_index = -1);
    return 0;
}

static int _SetColumnOffset(lua_State * L)
{
    //void SetColumnOffset(int column_index, float offset_x);
    return 0;
}

static int _GetColumnsCount(lua_State * L)
{
    //int GetColumnsCount();
    return 0;
}

static int _BeginTabBar(lua_State * L)
{
    //bool BeginTabBar(char const * str_id, TabBarFlags flags = 0);
    return 0;
}

static int _EndTabBar(lua_State * L)
{
    //void EndTabBar();
    return 0;
}

static int _BeginTabItem(lua_State * L)
{
    //bool BeginTabItem(char const * label, bool * p_open = nullptr, TabItemFlags flags = 0);
    return 0;
}

static int _EndTabItem(lua_State * L)
{
    //void EndTabItem();
    return 0;
}

static int _SetTabItemClosed(lua_State * L)
{
    //void SetTabItemClosed(char const * tab_or_docked_window_label);
    return 0;
}

static int _LogToTTY(lua_State * L)
{
    //void LogToTTY(int auto_open_depth = -1);
    return 0;
}

static int _LogToFile(lua_State * L)
{
    //void LogToFile(int auto_open_depth = -1, char const * filename = nullptr);
    return 0;
}

static int _LogToClipboard(lua_State * L)
{
    //void LogToClipboard(int auto_open_depth = -1);
    return 0;
}

static int _LogFinish(lua_State * L)
{
    //void LogFinish();
    return 0;
}

static int _LogButtons(lua_State * L)
{
    //void LogButtons();
    return 0;
}

static int _LogText(lua_State * L)
{
    //void LogText(char const * text);
    return 0;
}

static int _BeginDragDropSource(lua_State * L)
{
    //bool BeginDragDropSource(DragDropFlags flags = 0);
    return 0;
}

static int _SetDragDropPayload(lua_State * L)
{
    //bool SetDragDropPayload(char const * type, void const * data, size_t sz, Cond cond = 0);
    return 0;
}

static int _EndDragDropSource(lua_State * L)
{
    //void EndDragDropSource();
    return 0;
}

static int _BeginDragDropTarget(lua_State * L)
{
    //bool BeginDragDropTarget();
    return 0;
}

static int _AcceptDragDropPayload(lua_State * L)
{
    //Payload AcceptDragDropPayload(char const * type, DragDropFlags flags = 0);
    return 0;
}

static int _EndDragDropTarget(lua_State * L)
{
    //void EndDragDropTarget();
    return 0;
}

static int _GetDragDropPayload(lua_State * L)
{
    //Payload GetDragDropPayload();
    return 0;
}

static int _PushClipRect(lua_State * L)
{
    //void PushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max, bool intersect_with_current_clip_rect);
    return 0;
}

static int _PopClipRect(lua_State * L)
{
    //void PopClipRect();
    return 0;
}

static int _SetItemDefaultFocus(lua_State * L)
{
    //void SetItemDefaultFocus();
    return 0;
}

static int _SetKeyboardFocusHere(lua_State * L)
{
    //void SetKeyboardFocusHere(int offset = 0);
    return 0;
}

static int _IsItemHovered(lua_State * L)
{
    //bool IsItemHovered(HoveredFlags flags = 0);
    return 0;
}

static int _IsItemActive(lua_State * L)
{
    //bool IsItemActive();
    return 0;
}

static int _IsItemFocused(lua_State * L)
{
    //bool IsItemFocused();
    return 0;
}

static int _IsItemClicked(lua_State * L)
{
    //bool IsItemClicked(int mouse_button = 0);
    return 0;
}

static int _IsItemVisible(lua_State * L)
{
    //bool IsItemVisible();
    return 0;
}

static int _IsItemEdited(lua_State * L)
{
    //bool IsItemEdited();
    return 0;
}

static int _IsItemActivated(lua_State * L)
{
    //bool IsItemActivated();
    return 0;
}

static int _IsItemDeactivated(lua_State * L)
{
    //bool IsItemDeactivated();
    return 0;
}

static int _IsItemDeactivatedAfterEdit(lua_State * L)
{
    //bool IsItemDeactivatedAfterEdit();
    return 0;
}

static int _IsAnyItemHovered(lua_State * L)
{
    //bool IsAnyItemHovered();
    return 0;
}

static int _IsAnyItemActive(lua_State * L)
{
    //bool IsAnyItemActive();
    return 0;
}

static int _IsAnyItemFocused(lua_State * L)
{
    //bool IsAnyItemFocused();
    return 0;
}

static int _GetItemRectMin(lua_State * L)
{
    //Vector2 GetItemRectMin();
    return 0;
}

static int _GetItemRectMax(lua_State * L)
{
    //Vector2 GetItemRectMax();
    return 0;
}

static int _GetItemRectSize(lua_State * L)
{
    //Vector2 GetItemRectSize();
    return 0;
}

static int _SetItemAllowOverlap(lua_State * L)
{
    //void SetItemAllowOverlap();
    return 0;
}

static int _IsRectVisible(lua_State * L)
{
    //bool IsRectVisible(Vector2 const & size);
    //bool IsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max);
    return 0;
}

static int _GetTime(lua_State * L)
{
    //double GetTime();
    return 0;
}

static int _GetFrameCount(lua_State * L)
{
    //int GetFrameCount();
    return 0;
}

static int _GetStyleColorName(lua_State * L)
{
    //char const * GetStyleColorName(Col idx);
    return 0;
}

static int _CalcTextSize(lua_State * L)
{
    //Vector2 CalcTextSize(char const * text, char const * text_end = nullptr, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
    return 0;
}

static int _CalcListClipping(lua_State * L)
{
    //void CalcListClipping(int items_count, float items_height, int * out_items_display_start, int * out_items_display_end);
    return 0;
}

static int _BeginChildFrame(lua_State * L)
{
    //bool BeginChildFrame(ID id, Vector2 const & size, WindowFlags flags = 0);
    return 0;
}

static int _EndChildFrame(lua_State * L)
{
    //void EndChildFrame();
    return 0;
}

static int _ColorConvertU32ToFloat4(lua_State * L)
{
    //Vector4 ColorConvertU32ToFloat4(unsigned int in);
    return 0;
}

static int _ColorConvertFloat4ToU32(lua_State * L)
{
    //unsigned int ColorConvertFloat4ToU32(Vector4 const & in);
    return 0;
}

static int _ColorConvertRGBtoHSV(lua_State * L)
{
    //void ColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v);
    return 0;
}

static int _ColorConvertHSVtoRGB(lua_State * L)
{
    //void ColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b);
    return 0;
}

static int _GetKeyIndex(lua_State * L)
{
    //int GetKeyIndex(Key imgui_key);
    return 0;
}

static int _IsKeyDown(lua_State * L)
{
    //bool IsKeyDown(int user_key_index);
    return 0;
}

static int _IsKeyPressed(lua_State * L)
{
    //bool IsKeyPressed(int user_key_index, bool repeat = true);
    return 0;
}

static int _IsKeyReleased(lua_State * L)
{
    //bool IsKeyReleased(int user_key_index);
    return 0;
}

static int _GetKeyPressedAmount(lua_State * L)
{
    //int GetKeyPressedAmount(int key_index, float repeat_delay, float rate);
    return 0;
}

static int _IsMouseDown(lua_State * L)
{
    //bool IsMouseDown(int button);
    return 0;
}

static int _IsAnyMouseDown(lua_State * L)
{
    //bool IsAnyMouseDown();
    return 0;
}

static int _IsMouseClicked(lua_State * L)
{
    //bool IsMouseClicked(int button, bool repeat = false);
    return 0;
}

static int _IsMouseDoubleClicked(lua_State * L)
{
    //bool IsMouseDoubleClicked(int button);
    return 0;
}

static int _IsMouseReleased(lua_State * L)
{
    //bool IsMouseReleased(int button);
    return 0;
}

static int _IsMouseDragging(lua_State * L)
{
    //bool IsMouseDragging(int button = 0, float lock_threshold = -1.0f);
    return 0;
}

static int _IsMouseHoveringRect(lua_State * L)
{
    //bool IsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip = true);
    return 0;
}

static int _IsMousePosValid(lua_State * L)
{
    //bool IsMousePosValid(Vector2 const * mouse_pos = nullptr);
    return 0;
}

static int _GetMousePos(lua_State * L)
{
    //Vector2 GetMousePos();
    return 0;
}

static int _GetMousePosOnOpeningCurrentPopup(lua_State * L)
{
    //Vector2 GetMousePosOnOpeningCurrentPopup();
    return 0;
}

static int _GetMouseDragDelta(lua_State * L)
{
    //Vector2 GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);
    return 0;
}

static int _ResetMouseDragDelta(lua_State * L)
{
    //void ResetMouseDragDelta(int button = 0);
    return 0;
}

static int _GetMouseCursor(lua_State * L)
{
    //MouseCursor GetMouseCursor();
    return 0;
}

static int _SetMouseCursor(lua_State * L)
{
    //void SetMouseCursor(MouseCursor type);
    return 0;
}

static int _CaptureKeyboardFromApp(lua_State * L)
{
    //void CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);
    return 0;
}

static int _CaptureMouseFromApp(lua_State * L)
{
    //void CaptureMouseFromApp(bool want_capture_mouse_value = true);
    return 0;
}

static int _GetClipboardText(lua_State * L)
{
    //char const * GetClipboardText();
    return 0;
}

static int _SetClipboardText(lua_State * L)
{
    //void SetClipboardText(char const * text);
    return 0;
}

static int _LoadIniSettingsFromDisk(lua_State * L)
{
    //void LoadIniSettingsFromDisk(char const * ini_filename);
    return 0;
}

static int _LoadIniSettingsFromMemory(lua_State * L)
{
    //void LoadIniSettingsFromMemory(char const * ini_data, size_t ini_size = 0);
    return 0;
}

static int _SaveIniSettingsToDisk(lua_State * L)
{
    //void SaveIniSettingsToDisk(char const * ini_filename);
    return 0;
}

static int _SaveIniSettingsToMemory(lua_State * L)
{
    // char const * SaveIniSettingsToMemory(size_t * out_ini_size = nullptr);
    return 0;
}

#ifndef RAYGUI_REGISTER
#define RAYGUI_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_lay_gui[] = {
        // Raylib-ImGui.
        RAYGUI_REGISTER(InitRayGui),
        RAYGUI_REGISTER(UpdateRayGui),
        RAYGUI_REGISTER(ShutdownRayGui),

        // Demo, Debug, Information
        RAYGUI_REGISTER(ShowDemoWindow),
        RAYGUI_REGISTER(ShowAboutWindow),
        RAYGUI_REGISTER(ShowMetricsWindow),
        RAYGUI_REGISTER(ShowStyleEditor),
        RAYGUI_REGISTER(ShowStyleSelector),
        RAYGUI_REGISTER(ShowFontSelector),
        RAYGUI_REGISTER(ShowUserGuide),
        RAYGUI_REGISTER(GetVersion),

        // Styles
        RAYGUI_REGISTER(StyleColorsDark),
        RAYGUI_REGISTER(StyleColorsClassic),
        RAYGUI_REGISTER(StyleColorsLight),
        RAYGUI_REGISTER(GetStyle),
        RAYGUI_REGISTER(SetStyle),

        // Windows
        RAYGUI_REGISTER(Begin),
        RAYGUI_REGISTER(End),

        // Child Windows
        RAYGUI_REGISTER(BeginChild),
        RAYGUI_REGISTER(EndChild),

        // Windows Utilities
        RAYGUI_REGISTER(IsWindowAppearing),
        RAYGUI_REGISTER(IsWindowCollapsed),
        RAYGUI_REGISTER(IsWindowFocused),
        RAYGUI_REGISTER(IsWindowHovered),
        RAYGUI_REGISTER(GetWindowPos),
        RAYGUI_REGISTER(GetWindowSize),
        RAYGUI_REGISTER(GetWindowWidth),
        RAYGUI_REGISTER(GetWindowHeight),

        // SetNextXXX and SetXXX
        RAYGUI_REGISTER(SetNextWindowPos),
        RAYGUI_REGISTER(SetNextWindowSize),
        RAYGUI_REGISTER(SetNextWindowContentSize),
        RAYGUI_REGISTER(SetNextWindowCollapsed),
        RAYGUI_REGISTER(SetNextWindowFocus),
        RAYGUI_REGISTER(SetNextWindowBgAlpha),
        RAYGUI_REGISTER(SetWindowPos),
        RAYGUI_REGISTER(SetWindowSize),
        RAYGUI_REGISTER(SetWindowCollapsed),
        RAYGUI_REGISTER(SetWindowFocus),
        RAYGUI_REGISTER(SetWindowFontScale),

        // Content region
        RAYGUI_REGISTER(GetContentRegionMax),
        RAYGUI_REGISTER(GetContentRegionAvail),
        RAYGUI_REGISTER(GetWindowContentRegionMin),
        RAYGUI_REGISTER(GetWindowContentRegionMax),
        RAYGUI_REGISTER(GetWindowContentRegionWidth),

        // Windows Scrolling
        RAYGUI_REGISTER(GetScrollX),
        RAYGUI_REGISTER(GetScrollY),
        RAYGUI_REGISTER(GetScrollMaxX),
        RAYGUI_REGISTER(GetScrollMaxY),
        RAYGUI_REGISTER(SetScrollX),
        RAYGUI_REGISTER(SetScrollY),
        RAYGUI_REGISTER(SetScrollHereY),
        RAYGUI_REGISTER(SetScrollFromPosY),

        // Parameters stacks
        RAYGUI_REGISTER(PushStyleColor),
        RAYGUI_REGISTER(PopStyleColor),
        RAYGUI_REGISTER(PushStyleVar),
        RAYGUI_REGISTER(PopStyleVar),
        RAYGUI_REGISTER(GetStyleColorVec4),
        RAYGUI_REGISTER(GetFontSize),
        RAYGUI_REGISTER(GetFontTexUvWhitePixel),
        RAYGUI_REGISTER(GetColorU32),

        // Parameters stacks
        RAYGUI_REGISTER(PushItemWidth),
        RAYGUI_REGISTER(PopItemWidth),
        RAYGUI_REGISTER(SetNextItemWidth),
        RAYGUI_REGISTER(CalcItemWidth),
        RAYGUI_REGISTER(PushTextWrapPos),
        RAYGUI_REGISTER(PopTextWrapPos),
        RAYGUI_REGISTER(PushAllowKeyboardFocus),
        RAYGUI_REGISTER(PopAllowKeyboardFocus),
        RAYGUI_REGISTER(PushButtonRepeat),
        RAYGUI_REGISTER(PopButtonRepeat),

        // Cursor / Layout
        RAYGUI_REGISTER(Separator),
        RAYGUI_REGISTER(SameLine),
        RAYGUI_REGISTER(NewLine),
        RAYGUI_REGISTER(Spacing),
        RAYGUI_REGISTER(Dummy),
        RAYGUI_REGISTER(Indent),
        RAYGUI_REGISTER(Unindent),
        RAYGUI_REGISTER(BeginGroup),
        RAYGUI_REGISTER(EndGroup),
        RAYGUI_REGISTER(GetCursorPos),
        RAYGUI_REGISTER(GetCursorPosX),
        RAYGUI_REGISTER(GetCursorPosY),
        RAYGUI_REGISTER(SetCursorPos),
        RAYGUI_REGISTER(SetCursorPosX),
        RAYGUI_REGISTER(SetCursorPosY),
        RAYGUI_REGISTER(GetCursorStartPos),
        RAYGUI_REGISTER(GetCursorScreenPos),
        RAYGUI_REGISTER(SetCursorScreenPos),
        RAYGUI_REGISTER(AlignTextToFramePadding),
        RAYGUI_REGISTER(GetTextLineHeight),
        RAYGUI_REGISTER(GetTextLineHeightWithSpacing),
        RAYGUI_REGISTER(GetFrameHeight),
        RAYGUI_REGISTER(GetFrameHeightWithSpacing),

        // ID stack/scopes
        RAYGUI_REGISTER(PushID),
        RAYGUI_REGISTER(PopID),
        RAYGUI_REGISTER(GetID),

        // Widgets: Text
        RAYGUI_REGISTER(TextUnformatted),
        RAYGUI_REGISTER(Text),
        RAYGUI_REGISTER(TextColored),
        RAYGUI_REGISTER(TextDisabled),
        RAYGUI_REGISTER(TextWrapped),
        RAYGUI_REGISTER(LabelText),
        RAYGUI_REGISTER(BulletText),

        // Widgets: Main
        RAYGUI_REGISTER(Button),
        RAYGUI_REGISTER(SmallButton),
        RAYGUI_REGISTER(InvisibleButton),
        RAYGUI_REGISTER(ArrowButton),
        RAYGUI_REGISTER(Image),
        RAYGUI_REGISTER(ImageButton),
        RAYGUI_REGISTER(Checkbox),
        RAYGUI_REGISTER(CheckboxFlags),
        RAYGUI_REGISTER(RadioButton),
        RAYGUI_REGISTER(ProgressBar),
        RAYGUI_REGISTER(Bullet),

        // Widgets: Combo Box
        RAYGUI_REGISTER(BeginCombo),
        RAYGUI_REGISTER(EndCombo),
        RAYGUI_REGISTER(Combo),

        // Widgets: Drags
        RAYGUI_REGISTER(DragFloat),
        RAYGUI_REGISTER(DragFloat2),
        RAYGUI_REGISTER(DragFloat3),
        RAYGUI_REGISTER(DragFloat4),
        RAYGUI_REGISTER(DragFloatRange2),
        RAYGUI_REGISTER(DragInt),
        RAYGUI_REGISTER(DragInt2),
        RAYGUI_REGISTER(DragInt3),
        RAYGUI_REGISTER(DragInt4),
        RAYGUI_REGISTER(DragIntRange2),
        RAYGUI_REGISTER(DragScalar),
        RAYGUI_REGISTER(DragScalarN),

        // Widgets: Sliders
        RAYGUI_REGISTER(SliderFloat),
        RAYGUI_REGISTER(SliderFloat2),
        RAYGUI_REGISTER(SliderFloat3),
        RAYGUI_REGISTER(SliderFloat4),
        RAYGUI_REGISTER(SliderAngle),
        RAYGUI_REGISTER(SliderInt),
        RAYGUI_REGISTER(SliderInt2),
        RAYGUI_REGISTER(SliderInt3),
        RAYGUI_REGISTER(SliderInt4),
        RAYGUI_REGISTER(SliderScalar),
        RAYGUI_REGISTER(SliderScalarN),
        RAYGUI_REGISTER(VSliderFloat),
        RAYGUI_REGISTER(VSliderInt),
        RAYGUI_REGISTER(VSliderScalar),

        // Widgets: Input with Keyboard
        RAYGUI_REGISTER(InputText),
        RAYGUI_REGISTER(InputTextMultiline),
        RAYGUI_REGISTER(InputTextWithHint),
        RAYGUI_REGISTER(InputFloat),
        RAYGUI_REGISTER(InputFloat2),
        RAYGUI_REGISTER(InputFloat3),
        RAYGUI_REGISTER(InputFloat4),
        RAYGUI_REGISTER(InputInt),
        RAYGUI_REGISTER(InputInt2),
        RAYGUI_REGISTER(InputInt3),
        RAYGUI_REGISTER(InputInt4),
        RAYGUI_REGISTER(InputDouble),
        RAYGUI_REGISTER(InputScalar),
        RAYGUI_REGISTER(InputScalarN),

        // Widgets: Color Editor/Picker
        RAYGUI_REGISTER(ColorEdit3),
        RAYGUI_REGISTER(ColorEdit4),
        RAYGUI_REGISTER(ColorPicker3),
        RAYGUI_REGISTER(ColorPicker4),
        RAYGUI_REGISTER(ColorButton),
        RAYGUI_REGISTER(SetColorEditOptions),

        // Widgets: Trees
        RAYGUI_REGISTER(TreeNode),
        RAYGUI_REGISTER(TreeNodeEx),
        RAYGUI_REGISTER(TreePush),
        RAYGUI_REGISTER(TreePop),
        RAYGUI_REGISTER(TreeAdvanceToLabelPos),
        RAYGUI_REGISTER(GetTreeNodeToLabelSpacing),
        RAYGUI_REGISTER(SetNextTreeNodeOpen),
        RAYGUI_REGISTER(CollapsingHeader),

        // Widgets: Selectables
        RAYGUI_REGISTER(Selectable),

        // Widgets: List Boxes
        RAYGUI_REGISTER(ListBox),
        RAYGUI_REGISTER(ListBoxHeader),
        RAYGUI_REGISTER(ListBoxFooter),

        // Widgets: Data Plotting
        RAYGUI_REGISTER(PlotLines),
        RAYGUI_REGISTER(PlotHistogram),

        // Widgets: Value),
        RAYGUI_REGISTER(Value),

        // Widgets: Menus
        RAYGUI_REGISTER(BeginMainMenuBar),
        RAYGUI_REGISTER(EndMainMenuBar),
        RAYGUI_REGISTER(BeginMenuBar),
        RAYGUI_REGISTER(EndMenuBar),
        RAYGUI_REGISTER(BeginMenu),
        RAYGUI_REGISTER(EndMenu),
        RAYGUI_REGISTER(MenuItem),

        // Tooltips
        RAYGUI_REGISTER(BeginTooltip),
        RAYGUI_REGISTER(EndTooltip),
        RAYGUI_REGISTER(SetTooltip),

        // Popups, Modals
        RAYGUI_REGISTER(OpenPopup),
        RAYGUI_REGISTER(BeginPopup),
        RAYGUI_REGISTER(BeginPopupContextItem),
        RAYGUI_REGISTER(BeginPopupContextWindow),
        RAYGUI_REGISTER(BeginPopupContextVoid),
        RAYGUI_REGISTER(BeginPopupModal),
        RAYGUI_REGISTER(EndPopup),
        RAYGUI_REGISTER(OpenPopupOnItemClick),
        RAYGUI_REGISTER(IsPopupOpen),
        RAYGUI_REGISTER(CloseCurrentPopup),

        // Columns
        RAYGUI_REGISTER(Columns),
        RAYGUI_REGISTER(NextColumn),
        RAYGUI_REGISTER(GetColumnIndex),
        RAYGUI_REGISTER(GetColumnWidth),
        RAYGUI_REGISTER(SetColumnWidth),
        RAYGUI_REGISTER(GetColumnOffset),
        RAYGUI_REGISTER(SetColumnOffset),
        RAYGUI_REGISTER(GetColumnsCount),

        // Tab Bars, Tabs
        RAYGUI_REGISTER(BeginTabBar),
        RAYGUI_REGISTER(EndTabBar),
        RAYGUI_REGISTER(BeginTabItem),
        RAYGUI_REGISTER(EndTabItem),
        RAYGUI_REGISTER(SetTabItemClosed),

        // Logging/Capture
        RAYGUI_REGISTER(LogToTTY),
        RAYGUI_REGISTER(LogToFile),
        RAYGUI_REGISTER(LogToClipboard),
        RAYGUI_REGISTER(LogFinish),
        RAYGUI_REGISTER(LogButtons),
        RAYGUI_REGISTER(LogText),

        // Drag and Drop
        RAYGUI_REGISTER(BeginDragDropSource),
        RAYGUI_REGISTER(SetDragDropPayload),
        RAYGUI_REGISTER(EndDragDropSource),
        RAYGUI_REGISTER(BeginDragDropTarget),
        RAYGUI_REGISTER(AcceptDragDropPayload),
        RAYGUI_REGISTER(EndDragDropTarget),
        RAYGUI_REGISTER(GetDragDropPayload),

        // Clipping
        RAYGUI_REGISTER(PushClipRect),
        RAYGUI_REGISTER(PopClipRect),

        // Focus, Activation
        RAYGUI_REGISTER(SetItemDefaultFocus),
        RAYGUI_REGISTER(SetKeyboardFocusHere),

        // Item/Widgets Utilities
        RAYGUI_REGISTER(IsItemHovered),
        RAYGUI_REGISTER(IsItemActive),
        RAYGUI_REGISTER(IsItemFocused),
        RAYGUI_REGISTER(IsItemClicked),
        RAYGUI_REGISTER(IsItemVisible),
        RAYGUI_REGISTER(IsItemEdited),
        RAYGUI_REGISTER(IsItemActivated),
        RAYGUI_REGISTER(IsItemDeactivated),
        RAYGUI_REGISTER(IsItemDeactivatedAfterEdit),
        RAYGUI_REGISTER(IsAnyItemHovered),
        RAYGUI_REGISTER(IsAnyItemActive),
        RAYGUI_REGISTER(IsAnyItemFocused),
        RAYGUI_REGISTER(GetItemRectMin),
        RAYGUI_REGISTER(GetItemRectMax),
        RAYGUI_REGISTER(GetItemRectSize),
        RAYGUI_REGISTER(SetItemAllowOverlap),

        // Miscellaneous Utilities
        RAYGUI_REGISTER(IsRectVisible),
        RAYGUI_REGISTER(GetTime),
        RAYGUI_REGISTER(GetFrameCount),
        RAYGUI_REGISTER(GetStyleColorName),
        RAYGUI_REGISTER(CalcTextSize),
        RAYGUI_REGISTER(CalcListClipping),
        RAYGUI_REGISTER(BeginChildFrame),
        RAYGUI_REGISTER(EndChildFrame),

        // Color Utilities
        RAYGUI_REGISTER(ColorConvertU32ToFloat4),
        RAYGUI_REGISTER(ColorConvertFloat4ToU32),
        RAYGUI_REGISTER(ColorConvertRGBtoHSV),
        RAYGUI_REGISTER(ColorConvertHSVtoRGB),

        // Inputs Utilities
        RAYGUI_REGISTER(GetKeyIndex),
        RAYGUI_REGISTER(IsKeyDown),
        RAYGUI_REGISTER(IsKeyPressed),
        RAYGUI_REGISTER(IsKeyReleased),
        RAYGUI_REGISTER(GetKeyPressedAmount),
        RAYGUI_REGISTER(IsMouseDown),
        RAYGUI_REGISTER(IsAnyMouseDown),
        RAYGUI_REGISTER(IsMouseClicked),
        RAYGUI_REGISTER(IsMouseDoubleClicked),
        RAYGUI_REGISTER(IsMouseReleased),
        RAYGUI_REGISTER(IsMouseDragging),
        RAYGUI_REGISTER(IsMouseHoveringRect),
        RAYGUI_REGISTER(IsMousePosValid),
        RAYGUI_REGISTER(GetMousePos),
        RAYGUI_REGISTER(GetMousePosOnOpeningCurrentPopup),
        RAYGUI_REGISTER(GetMouseDragDelta),
        RAYGUI_REGISTER(ResetMouseDragDelta),
        RAYGUI_REGISTER(GetMouseCursor),
        RAYGUI_REGISTER(SetMouseCursor),
        RAYGUI_REGISTER(CaptureKeyboardFromApp),
        RAYGUI_REGISTER(CaptureMouseFromApp),

        // Clipboard Utilities
        RAYGUI_REGISTER(GetClipboardText),
        RAYGUI_REGISTER(SetClipboardText),

        // Settings/.Ini Utilities
        RAYGUI_REGISTER(LoadIniSettingsFromDisk),
        RAYGUI_REGISTER(LoadIniSettingsFromMemory),
        RAYGUI_REGISTER(SaveIniSettingsToDisk),
        RAYGUI_REGISTER(SaveIniSettingsToMemory),

        { nullptr, nullptr }
};

#ifndef RAYGUI_INT_SYMBOL
#define RAYGUI_INT_SYMBOL(L, s)        \
    do {                            \
        lua_pushinteger(L, s);      \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

static void luaL_push_raygui_symbols(lua_State * L)
{
    RAYGUI_INT_SYMBOL(L, Col_Text);
    RAYGUI_INT_SYMBOL(L, Col_TextDisabled);
    RAYGUI_INT_SYMBOL(L, Col_WindowBg);
    RAYGUI_INT_SYMBOL(L, Col_ChildBg);
    RAYGUI_INT_SYMBOL(L, Col_PopupBg);
    RAYGUI_INT_SYMBOL(L, Col_Border);
    RAYGUI_INT_SYMBOL(L, Col_BorderShadow);
    RAYGUI_INT_SYMBOL(L, Col_FrameBg);
    RAYGUI_INT_SYMBOL(L, Col_FrameBgHovered);
    RAYGUI_INT_SYMBOL(L, Col_FrameBgActive);
    RAYGUI_INT_SYMBOL(L, Col_TitleBg);
    RAYGUI_INT_SYMBOL(L, Col_TitleBgActive);
    RAYGUI_INT_SYMBOL(L, Col_TitleBgCollapsed);
    RAYGUI_INT_SYMBOL(L, Col_MenuBarBg);
    RAYGUI_INT_SYMBOL(L, Col_ScrollbarBg);
    RAYGUI_INT_SYMBOL(L, Col_ScrollbarGrab);
    RAYGUI_INT_SYMBOL(L, Col_ScrollbarGrabHovered);
    RAYGUI_INT_SYMBOL(L, Col_ScrollbarGrabActive);
    RAYGUI_INT_SYMBOL(L, Col_CheckMark);
    RAYGUI_INT_SYMBOL(L, Col_SliderGrab);
    RAYGUI_INT_SYMBOL(L, Col_SliderGrabActive);
    RAYGUI_INT_SYMBOL(L, Col_Button);
    RAYGUI_INT_SYMBOL(L, Col_ButtonHovered);
    RAYGUI_INT_SYMBOL(L, Col_ButtonActive);
    RAYGUI_INT_SYMBOL(L, Col_Header);
    RAYGUI_INT_SYMBOL(L, Col_HeaderHovered);
    RAYGUI_INT_SYMBOL(L, Col_HeaderActive);
    RAYGUI_INT_SYMBOL(L, Col_Separator);
    RAYGUI_INT_SYMBOL(L, Col_SeparatorHovered);
    RAYGUI_INT_SYMBOL(L, Col_SeparatorActive);
    RAYGUI_INT_SYMBOL(L, Col_ResizeGrip);
    RAYGUI_INT_SYMBOL(L, Col_ResizeGripHovered);
    RAYGUI_INT_SYMBOL(L, Col_ResizeGripActive);
    RAYGUI_INT_SYMBOL(L, Col_Tab);
    RAYGUI_INT_SYMBOL(L, Col_TabHovered);
    RAYGUI_INT_SYMBOL(L, Col_TabActive);
    RAYGUI_INT_SYMBOL(L, Col_TabUnfocused);
    RAYGUI_INT_SYMBOL(L, Col_TabUnfocusedActive);
    RAYGUI_INT_SYMBOL(L, Col_PlotLines);
    RAYGUI_INT_SYMBOL(L, Col_PlotLinesHovered);
    RAYGUI_INT_SYMBOL(L, Col_PlotHistogram);
    RAYGUI_INT_SYMBOL(L, Col_PlotHistogramHovered);
    RAYGUI_INT_SYMBOL(L, Col_TextSelectedBg);
    RAYGUI_INT_SYMBOL(L, Col_DragDropTarget);
    RAYGUI_INT_SYMBOL(L, Col_NavHighlight);
    RAYGUI_INT_SYMBOL(L, Col_NavWindowingHighlight);
    RAYGUI_INT_SYMBOL(L, Col_NavWindowingDimBg);
    RAYGUI_INT_SYMBOL(L, Col_ModalWindowDimBg);
    RAYGUI_INT_SYMBOL(L, Col_COUNT);

    RAYGUI_INT_SYMBOL(L, WindowFlags_None);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoTitleBar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoResize);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoMove);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoScrollbar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoScrollWithMouse);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoCollapse);
    RAYGUI_INT_SYMBOL(L, WindowFlags_AlwaysAutoResize);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoBackground);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoSavedSettings);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoMouseInputs);
    RAYGUI_INT_SYMBOL(L, WindowFlags_MenuBar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_HorizontalScrollbar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoFocusOnAppearing);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoBringToFrontOnFocus);
    RAYGUI_INT_SYMBOL(L, WindowFlags_AlwaysVerticalScrollbar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_AlwaysHorizontalScrollbar);
    RAYGUI_INT_SYMBOL(L, WindowFlags_AlwaysUseWindowPadding);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoNavInputs);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoNavFocus);
    RAYGUI_INT_SYMBOL(L, WindowFlags_UnsavedDocument);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoNav);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoDecoration);
    RAYGUI_INT_SYMBOL(L, WindowFlags_NoInputs);

    RAYGUI_INT_SYMBOL(L, FocusedFlags_None);
    RAYGUI_INT_SYMBOL(L, FocusedFlags_ChildWindows);
    RAYGUI_INT_SYMBOL(L, FocusedFlags_RootWindow);
    RAYGUI_INT_SYMBOL(L, FocusedFlags_AnyWindow);
    RAYGUI_INT_SYMBOL(L, FocusedFlags_RootAndChildWindows);

    RAYGUI_INT_SYMBOL(L, HoveredFlags_None);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_ChildWindows);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_RootWindow);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_AnyWindow);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_AllowWhenBlockedByPopup);
    //RAYGUI_INT_SYMBOL(L, HoveredFlags_AllowWhenBlockedByModal);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_AllowWhenBlockedByActiveItem);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_AllowWhenOverlapped);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_AllowWhenDisabled);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_RectOnly);
    RAYGUI_INT_SYMBOL(L, HoveredFlags_RootAndChildWindows);

    RAYGUI_INT_SYMBOL(L, Cond_Always);
    RAYGUI_INT_SYMBOL(L, Cond_Once);
    RAYGUI_INT_SYMBOL(L, Cond_FirstUseEver);
    RAYGUI_INT_SYMBOL(L, Cond_Appearing);

    RAYGUI_INT_SYMBOL(L, StyleVar_Alpha);
    RAYGUI_INT_SYMBOL(L, StyleVar_WindowPadding);
    RAYGUI_INT_SYMBOL(L, StyleVar_WindowRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_WindowBorderSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_WindowMinSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_WindowTitleAlign);
    RAYGUI_INT_SYMBOL(L, StyleVar_ChildRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_ChildBorderSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_PopupRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_PopupBorderSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_FramePadding);
    RAYGUI_INT_SYMBOL(L, StyleVar_FrameRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_FrameBorderSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_ItemSpacing);
    RAYGUI_INT_SYMBOL(L, StyleVar_ItemInnerSpacing);
    RAYGUI_INT_SYMBOL(L, StyleVar_IndentSpacing);
    RAYGUI_INT_SYMBOL(L, StyleVar_ScrollbarSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_ScrollbarRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_GrabMinSize);
    RAYGUI_INT_SYMBOL(L, StyleVar_GrabRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_TabRounding);
    RAYGUI_INT_SYMBOL(L, StyleVar_ButtonTextAlign);
    RAYGUI_INT_SYMBOL(L, StyleVar_SelectableTextAlign);
    RAYGUI_INT_SYMBOL(L, StyleVar_COUNT);

    RAYGUI_INT_SYMBOL(L, Dir_None);
    RAYGUI_INT_SYMBOL(L, Dir_Left);
    RAYGUI_INT_SYMBOL(L, Dir_Right);
    RAYGUI_INT_SYMBOL(L, Dir_Up);
    RAYGUI_INT_SYMBOL(L, Dir_Down);
    RAYGUI_INT_SYMBOL(L, Dir_COUNT);

    RAYGUI_INT_SYMBOL(L, ComboFlags_None);
    RAYGUI_INT_SYMBOL(L, ComboFlags_PopupAlignLeft);
    RAYGUI_INT_SYMBOL(L, ComboFlags_HeightSmall);
    RAYGUI_INT_SYMBOL(L, ComboFlags_HeightRegular);
    RAYGUI_INT_SYMBOL(L, ComboFlags_HeightLarge);
    RAYGUI_INT_SYMBOL(L, ComboFlags_HeightLargest);
    RAYGUI_INT_SYMBOL(L, ComboFlags_NoArrowButton);
    RAYGUI_INT_SYMBOL(L, ComboFlags_NoPreview);
    RAYGUI_INT_SYMBOL(L, ComboFlags_HeightMask_);

    RAYGUI_INT_SYMBOL(L, DataType_S8);
    RAYGUI_INT_SYMBOL(L, DataType_U8);
    RAYGUI_INT_SYMBOL(L, DataType_S16);
    RAYGUI_INT_SYMBOL(L, DataType_U16);
    RAYGUI_INT_SYMBOL(L, DataType_S32);
    RAYGUI_INT_SYMBOL(L, DataType_U32);
    RAYGUI_INT_SYMBOL(L, DataType_S64);
    RAYGUI_INT_SYMBOL(L, DataType_U64);
    RAYGUI_INT_SYMBOL(L, DataType_Float);
    RAYGUI_INT_SYMBOL(L, DataType_Double);
    RAYGUI_INT_SYMBOL(L, DataType_COUNT);

    RAYGUI_INT_SYMBOL(L, ColorEditFlags_None);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoAlpha);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoPicker);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoOptions);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoSmallPreview);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoInputs);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoTooltip);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoLabel);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoSidePreview);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_NoDragDrop);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_AlphaBar);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_AlphaPreview);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_AlphaPreviewHalf);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_HDR);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_DisplayRGB);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_DisplayHSV);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_DisplayHex);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_Uint8);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_Float);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_PickerHueBar);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_PickerHueWheel);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_InputRGB);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags_InputHSV);
    RAYGUI_INT_SYMBOL(L, ColorEditFlags__OptionsDefault);

    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_None);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_Selected);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_Framed);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_AllowItemOverlap);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_NoTreePushOnOpen);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_NoAutoOpenOnLog);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_DefaultOpen);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_OpenOnDoubleClick);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_OpenOnArrow);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_Leaf);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_Bullet);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_FramePadding);
    //RAYGUI_INT_SYMBOL(L, TreeNodeFlags_SpanAllAvailWidth);
    //RAYGUI_INT_SYMBOL(L, TreeNodeFlags_NoScrollOnOpen);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_NavLeftJumpsBackHere);
    RAYGUI_INT_SYMBOL(L, TreeNodeFlags_CollapsingHeader);

    RAYGUI_INT_SYMBOL(L, SelectableFlags_None);
    RAYGUI_INT_SYMBOL(L, SelectableFlags_DontClosePopups);
    RAYGUI_INT_SYMBOL(L, SelectableFlags_SpanAllColumns);
    RAYGUI_INT_SYMBOL(L, SelectableFlags_AllowDoubleClick);
    RAYGUI_INT_SYMBOL(L, SelectableFlags_Disabled);

    RAYGUI_INT_SYMBOL(L, TabBarFlags_None);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_Reorderable);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_AutoSelectNewTabs);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_TabListPopupButton);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_NoCloseWithMiddleMouseButton);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_NoTabListScrollingButtons);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_NoTooltip);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_FittingPolicyResizeDown);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_FittingPolicyScroll);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_FittingPolicyMask_);
    RAYGUI_INT_SYMBOL(L, TabBarFlags_FittingPolicyDefault_);

    RAYGUI_INT_SYMBOL(L, TabItemFlags_None);
    RAYGUI_INT_SYMBOL(L, TabItemFlags_UnsavedDocument);
    RAYGUI_INT_SYMBOL(L, TabItemFlags_SetSelected);
    RAYGUI_INT_SYMBOL(L, TabItemFlags_NoCloseWithMiddleMouseButton);
    RAYGUI_INT_SYMBOL(L, TabItemFlags_NoPushId);

    RAYGUI_INT_SYMBOL(L, DragDropFlags_None);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceNoPreviewTooltip);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceNoDisableHover);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceNoHoldToOpenOthers);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceAllowNullID);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceExtern);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_SourceAutoExpirePayload);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_AcceptBeforeDelivery);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_AcceptNoDrawDefaultRect);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_AcceptNoPreviewTooltip);
    RAYGUI_INT_SYMBOL(L, DragDropFlags_AcceptPeekOnly);

    RAYGUI_INT_SYMBOL(L, Key_Tab);
    RAYGUI_INT_SYMBOL(L, Key_LeftArrow);
    RAYGUI_INT_SYMBOL(L, Key_RightArrow);
    RAYGUI_INT_SYMBOL(L, Key_UpArrow);
    RAYGUI_INT_SYMBOL(L, Key_DownArrow);
    RAYGUI_INT_SYMBOL(L, Key_PageUp);
    RAYGUI_INT_SYMBOL(L, Key_PageDown);
    RAYGUI_INT_SYMBOL(L, Key_Home);
    RAYGUI_INT_SYMBOL(L, Key_End);
    RAYGUI_INT_SYMBOL(L, Key_Insert);
    RAYGUI_INT_SYMBOL(L, Key_Delete);
    RAYGUI_INT_SYMBOL(L, Key_Backspace);
    RAYGUI_INT_SYMBOL(L, Key_Space);
    RAYGUI_INT_SYMBOL(L, Key_Enter);
    RAYGUI_INT_SYMBOL(L, Key_Escape);
    RAYGUI_INT_SYMBOL(L, Key_A);
    RAYGUI_INT_SYMBOL(L, Key_C);
    RAYGUI_INT_SYMBOL(L, Key_V);
    RAYGUI_INT_SYMBOL(L, Key_X);
    RAYGUI_INT_SYMBOL(L, Key_Y);
    RAYGUI_INT_SYMBOL(L, Key_Z);
    RAYGUI_INT_SYMBOL(L, Key_COUNT);

    RAYGUI_INT_SYMBOL(L, MouseCursor_None);
    RAYGUI_INT_SYMBOL(L, MouseCursor_Arrow);
    RAYGUI_INT_SYMBOL(L, MouseCursor_TextInput);
    RAYGUI_INT_SYMBOL(L, MouseCursor_ResizeAll);
    RAYGUI_INT_SYMBOL(L, MouseCursor_ResizeNS);
    RAYGUI_INT_SYMBOL(L, MouseCursor_ResizeEW);
    RAYGUI_INT_SYMBOL(L, MouseCursor_ResizeNESW);
    RAYGUI_INT_SYMBOL(L, MouseCursor_ResizeNWSE);
    RAYGUI_INT_SYMBOL(L, MouseCursor_Hand);
    RAYGUI_INT_SYMBOL(L, MouseCursor_COUNT);
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

