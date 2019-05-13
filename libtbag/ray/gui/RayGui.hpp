/**
 * @file   RayGui.hpp
 * @brief  RayGui class prototype.
 * @author zer0
 * @date   2019-05-06
 * @date   2019-05-13 (Move: libtbag/ray -> libtbag/ray/gui)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_GUI_RAYGUI_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_GUI_RAYGUI_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayBypass.hpp>

#include <cstdlib>
#include <cstring>
#include <cfloat>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {
namespace gui {

// ----------------
// clang-format off
// ----------------

#ifndef ImTextureID
typedef void * TextureID;
#endif
typedef unsigned int ID;
typedef unsigned short Wchar;
typedef int Col;
typedef int Cond;
typedef int DataType;
typedef int Dir;
typedef int Key;
typedef int NavInput;
typedef int MouseCursor;
typedef int StyleVar;
typedef int DrawCornerFlags;
typedef int DrawListFlags;
typedef int FontAtlasFlags;
typedef int BackendFlags;
typedef int ColorEditFlags;
typedef int ColumnsFlags;
typedef int ConfigFlags;
typedef int ComboFlags;
typedef int DragDropFlags;
typedef int FocusedFlags;
typedef int HoveredFlags;
typedef int InputTextFlags;
typedef int SelectableFlags;
typedef int TabBarFlags;
typedef int TabItemFlags;
typedef int TreeNodeFlags;
typedef int WindowFlags;

enum Col_
{
    Col_Text,
    Col_TextDisabled,
    Col_WindowBg,
    Col_ChildBg,
    Col_PopupBg,
    Col_Border,
    Col_BorderShadow,
    Col_FrameBg,
    Col_FrameBgHovered,
    Col_FrameBgActive,
    Col_TitleBg,
    Col_TitleBgActive,
    Col_TitleBgCollapsed,
    Col_MenuBarBg,
    Col_ScrollbarBg,
    Col_ScrollbarGrab,
    Col_ScrollbarGrabHovered,
    Col_ScrollbarGrabActive,
    Col_CheckMark,
    Col_SliderGrab,
    Col_SliderGrabActive,
    Col_Button,
    Col_ButtonHovered,
    Col_ButtonActive,
    Col_Header,
    Col_HeaderHovered,
    Col_HeaderActive,
    Col_Separator,
    Col_SeparatorHovered,
    Col_SeparatorActive,
    Col_ResizeGrip,
    Col_ResizeGripHovered,
    Col_ResizeGripActive,
    Col_Tab,
    Col_TabHovered,
    Col_TabActive,
    Col_TabUnfocused,
    Col_TabUnfocusedActive,
    Col_PlotLines,
    Col_PlotLinesHovered,
    Col_PlotHistogram,
    Col_PlotHistogramHovered,
    Col_TextSelectedBg,
    Col_DragDropTarget,
    Col_NavHighlight,
    Col_NavWindowingHighlight,
    Col_NavWindowingDimBg,
    Col_ModalWindowDimBg,
    Col_COUNT
};

enum WindowFlags_
{
    WindowFlags_None                      = 0,
    WindowFlags_NoTitleBar                = 1 << 0,
    WindowFlags_NoResize                  = 1 << 1,
    WindowFlags_NoMove                    = 1 << 2,
    WindowFlags_NoScrollbar               = 1 << 3,
    WindowFlags_NoScrollWithMouse         = 1 << 4,
    WindowFlags_NoCollapse                = 1 << 5,
    WindowFlags_AlwaysAutoResize          = 1 << 6,
    WindowFlags_NoBackground              = 1 << 7,
    WindowFlags_NoSavedSettings           = 1 << 8,
    WindowFlags_NoMouseInputs             = 1 << 9,
    WindowFlags_MenuBar                   = 1 << 10,
    WindowFlags_HorizontalScrollbar       = 1 << 11,
    WindowFlags_NoFocusOnAppearing        = 1 << 12,
    WindowFlags_NoBringToFrontOnFocus     = 1 << 13,
    WindowFlags_AlwaysVerticalScrollbar   = 1 << 14,
    WindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
    WindowFlags_AlwaysUseWindowPadding    = 1 << 16,
    WindowFlags_NoNavInputs               = 1 << 18,
    WindowFlags_NoNavFocus                = 1 << 19,
    WindowFlags_UnsavedDocument           = 1 << 20,

    WindowFlags_NoNav = WindowFlags_NoNavInputs |
                        WindowFlags_NoNavFocus,
    WindowFlags_NoDecoration = WindowFlags_NoTitleBar |
                               WindowFlags_NoResize |
                               WindowFlags_NoScrollbar |
                               WindowFlags_NoCollapse,
    WindowFlags_NoInputs = WindowFlags_NoMouseInputs |
                           WindowFlags_NoNavInputs |
                           WindowFlags_NoNavFocus,
};

enum FocusedFlags_
{
    FocusedFlags_None         = 0,
    FocusedFlags_ChildWindows = 1 << 0,
    FocusedFlags_RootWindow   = 1 << 1,
    FocusedFlags_AnyWindow    = 1 << 2,

    FocusedFlags_RootAndChildWindows = FocusedFlags_RootWindow |
                                       FocusedFlags_ChildWindows
};

enum HoveredFlags_
{
    HoveredFlags_None                          = 0,
    HoveredFlags_ChildWindows                  = 1 << 0,
    HoveredFlags_RootWindow                    = 1 << 1,
    HoveredFlags_AnyWindow                     = 1 << 2,
    HoveredFlags_AllowWhenBlockedByPopup       = 1 << 3,
    //HoveredFlags_AllowWhenBlockedByModal     = 1 << 4,
            HoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 5,
    HoveredFlags_AllowWhenOverlapped           = 1 << 6,
    HoveredFlags_AllowWhenDisabled             = 1 << 7,

    HoveredFlags_RectOnly = HoveredFlags_AllowWhenBlockedByPopup |
                            HoveredFlags_AllowWhenBlockedByActiveItem |
                            HoveredFlags_AllowWhenOverlapped,
    HoveredFlags_RootAndChildWindows = HoveredFlags_RootWindow |
                                       HoveredFlags_ChildWindows,
};

enum Cond_
{
    Cond_Always       = 1 << 0,
    Cond_Once         = 1 << 1,
    Cond_FirstUseEver = 1 << 2,
    Cond_Appearing    = 1 << 3
};

enum StyleVar_
{
    StyleVar_Alpha,
    StyleVar_WindowPadding,
    StyleVar_WindowRounding,
    StyleVar_WindowBorderSize,
    StyleVar_WindowMinSize,
    StyleVar_WindowTitleAlign,
    StyleVar_ChildRounding,
    StyleVar_ChildBorderSize,
    StyleVar_PopupRounding,
    StyleVar_PopupBorderSize,
    StyleVar_FramePadding,
    StyleVar_FrameRounding,
    StyleVar_FrameBorderSize,
    StyleVar_ItemSpacing,
    StyleVar_ItemInnerSpacing,
    StyleVar_IndentSpacing,
    StyleVar_ScrollbarSize,
    StyleVar_ScrollbarRounding,
    StyleVar_GrabMinSize,
    StyleVar_GrabRounding,
    StyleVar_TabRounding,
    StyleVar_ButtonTextAlign,
    StyleVar_SelectableTextAlign,
    StyleVar_COUNT
};

enum Dir_
{
    Dir_None  = -1,
    Dir_Left  = 0,
    Dir_Right = 1,
    Dir_Up    = 2,
    Dir_Down  = 3,
    Dir_COUNT
};

enum ComboFlags_
{
    ComboFlags_None           = 0,
    ComboFlags_PopupAlignLeft = 1 << 0,
    ComboFlags_HeightSmall    = 1 << 1,
    ComboFlags_HeightRegular  = 1 << 2,
    ComboFlags_HeightLarge    = 1 << 3,
    ComboFlags_HeightLargest  = 1 << 4,
    ComboFlags_NoArrowButton  = 1 << 5,
    ComboFlags_NoPreview      = 1 << 6,

    ComboFlags_HeightMask_ = ComboFlags_HeightSmall |
                             ComboFlags_HeightRegular |
                             ComboFlags_HeightLarge |
                             ComboFlags_HeightLargest
};

enum DataType_
{
    DataType_S8,
    DataType_U8,
    DataType_S16,
    DataType_U16,
    DataType_S32,
    DataType_U32,
    DataType_S64,
    DataType_U64,
    DataType_Float,
    DataType_Double,
    DataType_COUNT
};

enum ColorEditFlags_
{
    ColorEditFlags_None             = 0,
    ColorEditFlags_NoAlpha          = 1 << 1,
    ColorEditFlags_NoPicker         = 1 << 2,
    ColorEditFlags_NoOptions        = 1 << 3,
    ColorEditFlags_NoSmallPreview   = 1 << 4,
    ColorEditFlags_NoInputs         = 1 << 5,
    ColorEditFlags_NoTooltip        = 1 << 6,
    ColorEditFlags_NoLabel          = 1 << 7,
    ColorEditFlags_NoSidePreview    = 1 << 8,
    ColorEditFlags_NoDragDrop       = 1 << 9,
    ColorEditFlags_AlphaBar         = 1 << 16,
    ColorEditFlags_AlphaPreview     = 1 << 17,
    ColorEditFlags_AlphaPreviewHalf = 1 << 18,
    ColorEditFlags_HDR              = 1 << 19,
    ColorEditFlags_DisplayRGB       = 1 << 20,
    ColorEditFlags_DisplayHSV       = 1 << 21,
    ColorEditFlags_DisplayHex       = 1 << 22,
    ColorEditFlags_Uint8            = 1 << 23,
    ColorEditFlags_Float            = 1 << 24,
    ColorEditFlags_PickerHueBar     = 1 << 25,
    ColorEditFlags_PickerHueWheel   = 1 << 26,
    ColorEditFlags_InputRGB         = 1 << 27,
    ColorEditFlags_InputHSV         = 1 << 28,

    ColorEditFlags__OptionsDefault = ColorEditFlags_Uint8 |
                                     ColorEditFlags_DisplayRGB |
                                     ColorEditFlags_InputRGB |
                                     ColorEditFlags_PickerHueBar,
};

enum TreeNodeFlags_
{
    TreeNodeFlags_None                 = 0,
    TreeNodeFlags_Selected             = 1 << 0,
    TreeNodeFlags_Framed               = 1 << 1,
    TreeNodeFlags_AllowItemOverlap     = 1 << 2,
    TreeNodeFlags_NoTreePushOnOpen     = 1 << 3,
    TreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,
    TreeNodeFlags_DefaultOpen          = 1 << 5,
    TreeNodeFlags_OpenOnDoubleClick    = 1 << 6,
    TreeNodeFlags_OpenOnArrow          = 1 << 7,
    TreeNodeFlags_Leaf                 = 1 << 8,
    TreeNodeFlags_Bullet               = 1 << 9,
    TreeNodeFlags_FramePadding         = 1 << 10,
    //TreeNodeFlags_SpanAllAvailWidth  = 1 << 11,
    //TreeNodeFlags_NoScrollOnOpen     = 1 << 12,
            TreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,

    TreeNodeFlags_CollapsingHeader = TreeNodeFlags_Framed |
                                     TreeNodeFlags_NoTreePushOnOpen |
                                     TreeNodeFlags_NoAutoOpenOnLog
};

enum SelectableFlags_
{
    SelectableFlags_None             = 0,
    SelectableFlags_DontClosePopups  = 1 << 0,
    SelectableFlags_SpanAllColumns   = 1 << 1,
    SelectableFlags_AllowDoubleClick = 1 << 2,
    SelectableFlags_Disabled         = 1 << 3
};

enum TabBarFlags_
{
    TabBarFlags_None                           = 0,
    TabBarFlags_Reorderable                    = 1 << 0,
    TabBarFlags_AutoSelectNewTabs              = 1 << 1,
    TabBarFlags_TabListPopupButton             = 1 << 2,
    TabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,
    TabBarFlags_NoTabListScrollingButtons      = 1 << 4,
    TabBarFlags_NoTooltip                      = 1 << 5,
    TabBarFlags_FittingPolicyResizeDown        = 1 << 6,
    TabBarFlags_FittingPolicyScroll            = 1 << 7,

    TabBarFlags_FittingPolicyMask_ = TabBarFlags_FittingPolicyResizeDown | TabBarFlags_FittingPolicyScroll,
    TabBarFlags_FittingPolicyDefault_ = TabBarFlags_FittingPolicyResizeDown
};

enum TabItemFlags_
{
    TabItemFlags_None                          = 0,
    TabItemFlags_UnsavedDocument               = 1 << 0,
    TabItemFlags_SetSelected                   = 1 << 1,
    TabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,
    TabItemFlags_NoPushId                      = 1 << 3
};

enum DragDropFlags_
{
    DragDropFlags_None = 0,

    // BeginDragDropSource() flags
            DragDropFlags_SourceNoPreviewTooltip       = 1 << 0,
    DragDropFlags_SourceNoDisableHover         = 1 << 1,
    DragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,
    DragDropFlags_SourceAllowNullID            = 1 << 3,
    DragDropFlags_SourceExtern                 = 1 << 4,
    DragDropFlags_SourceAutoExpirePayload      = 1 << 5,

    // AcceptDragDropPayload() flags
            DragDropFlags_AcceptBeforeDelivery         = 1 << 10,
    DragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,
    DragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,
    DragDropFlags_AcceptPeekOnly = DragDropFlags_AcceptBeforeDelivery | DragDropFlags_AcceptNoDrawDefaultRect
};

enum Key_
{
    Key_Tab,
    Key_LeftArrow,
    Key_RightArrow,
    Key_UpArrow,
    Key_DownArrow,
    Key_PageUp,
    Key_PageDown,
    Key_Home,
    Key_End,
    Key_Insert,
    Key_Delete,
    Key_Backspace,
    Key_Space,
    Key_Enter,
    Key_Escape,
    Key_A,
    Key_C,
    Key_V,
    Key_X,
    Key_Y,
    Key_Z,
    Key_COUNT
};

enum MouseCursor_
{
    MouseCursor_None = -1,
    MouseCursor_Arrow = 0,
    MouseCursor_TextInput,
    MouseCursor_ResizeAll,
    MouseCursor_ResizeNS,
    MouseCursor_ResizeEW,
    MouseCursor_ResizeNESW,
    MouseCursor_ResizeNWSE,
    MouseCursor_Hand,
    MouseCursor_COUNT
};

struct Style
{
    float       Alpha;
    Vector2     WindowPadding;
    float       WindowRounding;
    float       WindowBorderSize;
    Vector2     WindowMinSize;
    Vector2     WindowTitleAlign;
    float       ChildRounding;
    float       ChildBorderSize;
    float       PopupRounding;
    float       PopupBorderSize;
    Vector2     FramePadding;
    float       FrameRounding;
    float       FrameBorderSize;
    Vector2     ItemSpacing;
    Vector2     ItemInnerSpacing;
    Vector2     TouchExtraPadding;
    float       IndentSpacing;
    float       ColumnsMinSpacing;
    float       ScrollbarSize;
    float       ScrollbarRounding;
    float       GrabMinSize;
    float       GrabRounding;
    float       TabRounding;
    float       TabBorderSize;
    Vector2     ButtonTextAlign;
    Vector2     SelectableTextAlign;
    Vector2     DisplayWindowPadding;
    Vector2     DisplaySafeAreaPadding;
    float       MouseCursorScale;
    bool        AntiAliasedLines;
    bool        AntiAliasedFill;
    float       CurveTessellationTol;
    Vector4     Colors[Col_COUNT];
};

struct Payload
{
    void * Data;
    int DataSize;

    ID SourceId;
    ID SourceParentId;
    int DataFrameCount;
    char DataType[32+1];
    bool Preview;
    bool Delivery;

    bool IsDataType(char const * type) const
    { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }

    bool IsPreview() const
    { return Preview; }

    bool IsDelivery() const
    { return Delivery; }
};

// ---------------
// clang-format on
// ---------------

TBAG_API bool InitRayGui();
TBAG_API void UpdateRayGui();
TBAG_API void ShutdownRayGui();

// Demo, Debug, Information
TBAG_API void ShowDemoWindow(bool * p_open = nullptr);
TBAG_API void ShowAboutWindow(bool * p_open = nullptr);
TBAG_API void ShowMetricsWindow(bool * p_open = nullptr);
TBAG_API void ShowStyleEditor();
TBAG_API bool ShowStyleSelector(const char * label);
TBAG_API void ShowFontSelector(const char * label);
TBAG_API void ShowUserGuide();

TBAG_API const char * GetVersion();

// Styles
TBAG_API void StyleColorsDark();
TBAG_API void StyleColorsClassic();
TBAG_API void StyleColorsLight();

TBAG_API Style GetStyle();
TBAG_API void SetStyle(Style const & style);

// Windows
TBAG_API bool Begin(char const * text, bool * p_open = nullptr, WindowFlags flags = 0);
TBAG_API void End();

// Child Windows
TBAG_API bool BeginChild(char const * str_id, Vector2 const & size = Vector2{0,0},
                         bool border = false, WindowFlags flags = 0);
TBAG_API bool BeginChild(ID id, Vector2 const & size = Vector2{0,0},
                         bool border = false, WindowFlags flags = 0);
TBAG_API void EndChild();

// Windows Utilities
TBAG_API bool IsWindowAppearing();
TBAG_API bool IsWindowCollapsed();
TBAG_API bool IsWindowFocused(FocusedFlags flags = 0);
TBAG_API bool IsWindowHovered(HoveredFlags flags = 0);
TBAG_API Vector2 GetWindowPos();
TBAG_API Vector2 GetWindowSize();
TBAG_API float GetWindowWidth();
TBAG_API float GetWindowHeight();

// Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
TBAG_API void SetNextWindowPos(Vector2 const & pos, Cond cond = 0, Vector2 const & pivot = Vector2{0,0});
TBAG_API void SetNextWindowSize(Vector2 const & size, Cond cond = 0);
TBAG_API void SetNextWindowContentSize(Vector2 const & size);
TBAG_API void SetNextWindowCollapsed(bool collapsed, Cond cond = 0);
TBAG_API void SetNextWindowFocus();
TBAG_API void SetNextWindowBgAlpha(float alpha);
TBAG_API void SetWindowPos(Vector2 const & pos, Cond cond = 0);
TBAG_API void SetWindowSize(Vector2 const & size, Cond cond = 0);
TBAG_API void SetWindowCollapsed(bool collapsed, Cond cond = 0);
TBAG_API void SetWindowFocus();
TBAG_API void SetWindowFontScale(float scale);
TBAG_API void SetWindowPos(char const * name, Vector2 const & pos, Cond cond = 0);
TBAG_API void SetWindowSize(char const * name, Vector2 const & size, Cond cond = 0);
TBAG_API void SetWindowCollapsed(char const * name, bool collapsed, Cond cond = 0);
TBAG_API void SetWindowFocus(char const * name);

// Content region
TBAG_API Vector2 GetContentRegionMax();
TBAG_API Vector2 GetContentRegionAvail();
TBAG_API Vector2 GetWindowContentRegionMin();
TBAG_API Vector2 GetWindowContentRegionMax();
TBAG_API float GetWindowContentRegionWidth();

// Windows Scrolling
TBAG_API float GetScrollX();
TBAG_API float GetScrollY();
TBAG_API float GetScrollMaxX();
TBAG_API float GetScrollMaxY();
TBAG_API void SetScrollX(float scroll_x);
TBAG_API void SetScrollY(float scroll_y);
TBAG_API void SetScrollHereY(float center_y_ratio = 0.5f);
TBAG_API void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);

// Parameters stacks (shared)
TBAG_API void PushStyleColor(Col idx, unsigned int col);
TBAG_API void PushStyleColor(Col idx, Vector4 const & col);
TBAG_API void PopStyleColor(int count = 1);
TBAG_API void PushStyleVar(StyleVar idx, float val);
TBAG_API void PushStyleVar(StyleVar idx, Vector2 const & val);
TBAG_API void PopStyleVar(int count = 1);
TBAG_API Vector4 GetStyleColorVec4(Col idx);
TBAG_API float GetFontSize();
TBAG_API Vector2 GetFontTexUvWhitePixel();
TBAG_API unsigned int GetColorU32(Col idx, float alpha_mul = 1.0f);
TBAG_API unsigned int GetColorU32(Vector4 const & col);
TBAG_API unsigned int GetColorU32(unsigned int col);

// Parameters stacks (current window)
TBAG_API void PushItemWidth(float item_width);
TBAG_API void PopItemWidth();
TBAG_API void SetNextItemWidth(float item_width);
TBAG_API float CalcItemWidth();
TBAG_API void PushTextWrapPos(float wrap_local_pos_x = 0.0f);
TBAG_API void PopTextWrapPos();
TBAG_API void PushAllowKeyboardFocus(bool allow_keyboard_focus);
TBAG_API void PopAllowKeyboardFocus();
TBAG_API void PushButtonRepeat(bool repeat);
TBAG_API void PopButtonRepeat();

// Cursor / Layout
TBAG_API void Separator();
TBAG_API void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
TBAG_API void NewLine();
TBAG_API void Spacing();
TBAG_API void Dummy(Vector2 const & size);
TBAG_API void Indent(float indent_w = 0.0f);
TBAG_API void Unindent(float indent_w = 0.0f);
TBAG_API void BeginGroup();
TBAG_API void EndGroup();
TBAG_API Vector2 GetCursorPos();
TBAG_API float GetCursorPosX();
TBAG_API float GetCursorPosY();
TBAG_API void SetCursorPos(Vector2 const & local_pos);
TBAG_API void SetCursorPosX(float local_x);
TBAG_API void SetCursorPosY(float local_y);
TBAG_API Vector2 GetCursorStartPos();
TBAG_API Vector2 GetCursorScreenPos();
TBAG_API void SetCursorScreenPos(Vector2 const & pos);
TBAG_API void AlignTextToFramePadding();
TBAG_API float GetTextLineHeight();
TBAG_API float GetTextLineHeightWithSpacing();
TBAG_API float GetFrameHeight();
TBAG_API float GetFrameHeightWithSpacing();

// ID stack/scopes
TBAG_API void PushID(char const * str_id);
TBAG_API void PushID(char const * str_id_begin, char const * str_id_end);
TBAG_API void PushID(void const * ptr_id);
TBAG_API void PushID(int int_id);
TBAG_API void PopID();
TBAG_API ID GetID(char const * str_id);
TBAG_API ID GetID(char const * str_id_begin, char const * str_id_end);
TBAG_API ID GetID(void const * ptr_id);

// Widgets: Text
TBAG_API void TextUnformatted(char const * text, char const * text_end = nullptr);
TBAG_API void Text(char const * text);
TBAG_API void TextColored(Vector4 const & col, char const * text);
TBAG_API void TextDisabled(char const * text);
TBAG_API void TextWrapped(char const * text);
TBAG_API void LabelText(char const * label, char const * text);
TBAG_API void BulletText(char const * text);

// Widgets: Main
TBAG_API bool Button(char const * label, Vector2 const & size = Vector2{0,0});
TBAG_API bool SmallButton(char const * label);
TBAG_API bool InvisibleButton(char const * str_id, Vector2 const & size);
TBAG_API bool ArrowButton(char const * str_id, Dir dir);
TBAG_API void Image(TextureID user_texture_id,
                    Vector2 const & size,
                    Vector2 const & uv0 = Vector2{0,0},
                    Vector2 const & uv1 = Vector2{1,1},
                    Vector4 const & tint_col = Vector4{1,1,1,1},
                    Vector4 const & border_col = Vector4{0,0,0,0});
TBAG_API bool ImageButton(TextureID user_texture_id,
                          Vector2 const & size,
                          Vector2 const & uv0 = Vector2{0,0},
                          Vector2 const & uv1 = Vector2{1,1},
                          int frame_padding = -1,
                          Vector4 const & bg_col = Vector4{0,0,0,0},
                          Vector4 const & tint_col = Vector4{1,1,1,1});
TBAG_API bool Checkbox(char const * label, bool * v);
TBAG_API bool CheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value);
TBAG_API bool RadioButton(char const * label, bool active);
TBAG_API bool RadioButton(char const * label, int * v, int v_button);
TBAG_API void ProgressBar(float fraction, Vector2 const & size_arg = Vector2{-1,0},
                          char const * overlay = nullptr);
TBAG_API void Bullet();

// Widgets: Combo Box
TBAG_API bool BeginCombo(char const * label, char const * preview_value, ComboFlags flags = 0);
TBAG_API void EndCombo();
TBAG_API bool Combo(char const * label, int * current_item, char const * const items[], int items_count,
                    int popup_max_height_in_items = -1);
TBAG_API bool Combo(char const * label, int * current_item, char const * items_separated_by_zeros,
                    int popup_max_height_in_items = -1);
TBAG_API bool Combo(char const * label, int * current_item,
                    bool(*items_getter)(void * data, int idx, char const ** out_text),
                    void * data, int items_count, int popup_max_height_in_items = -1);

// Widgets: Drags
TBAG_API bool DragFloat(char const * label, float * v, float v_speed = 1.0f, float v_min = 0.0f,
                        float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool DragFloat2(char const * label, float v[2], float v_speed = 1.0f, float v_min = 0.0f,
                         float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool DragFloat3(char const * label, float v[3], float v_speed = 1.0f, float v_min = 0.0f,
                         float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool DragFloat4(char const * label, float v[4], float v_speed = 1.0f, float v_min = 0.0f,
                         float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool DragFloatRange2(char const * label, float * v_current_min, float * v_current_max,
                              float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f,
                              char const * format = "%.3f", char const * format_max = nullptr,
                              float power = 1.0f);
TBAG_API bool DragInt(char const * label, int * v, float v_speed = 1.0f,
                      int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool DragInt2(char const * label, int v[2], float v_speed = 1.0f,
                       int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool DragInt3(char const * label, int v[3], float v_speed = 1.0f,
                       int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool DragInt4(char const * label, int v[4], float v_speed = 1.0f,
                       int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool DragIntRange2(char const * label, int * v_current_min, int * v_current_max,
                            float v_speed = 1.0f, int v_min = 0, int v_max = 0,
                            char const * format = "%d", char const * format_max = nullptr);
TBAG_API bool DragScalar(char const * label, DataType data_type, void * v, float v_speed,
                         void const * v_min = nullptr, void const * v_max = nullptr,
                         char const * format = nullptr, float power = 1.0f);
TBAG_API bool DragScalarN(char const * label, DataType data_type, void * v, int components,
                          float v_speed, void const * v_min = nullptr, void const * v_max = nullptr,
                          char const * format = nullptr, float power = 1.0f);

// Widgets: Sliders
TBAG_API bool SliderFloat(char const * label, float * v, float v_min, float v_max,
                          char const * format = "%.3f", float power = 1.0f);
TBAG_API bool SliderFloat2(char const * label, float v[2], float v_min, float v_max,
                           char const * format = "%.3f", float power = 1.0f);
TBAG_API bool SliderFloat3(char const * label, float v[3], float v_min, float v_max,
                           char const * format = "%.3f", float power = 1.0f);
TBAG_API bool SliderFloat4(char const * label, float v[4], float v_min, float v_max,
                           char const * format = "%.3f", float power = 1.0f);
TBAG_API bool SliderAngle(char const * label, float * v_rad, float v_degrees_min = -360.0f,
                          float v_degrees_max = +360.0f, char const * format = "%.0f deg");
TBAG_API bool SliderInt(char const * label, int * v, int v_min, int v_max, char const * format = "%d");
TBAG_API bool SliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format = "%d");
TBAG_API bool SliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format = "%d");
TBAG_API bool SliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format = "%d");
TBAG_API bool SliderScalar(char const * label, DataType data_type, void * v, void const * v_min,
                           void const * v_max, char const * format = nullptr, float power = 1.0f);
TBAG_API bool SliderScalarN(char const * label, DataType data_type, void * v, int components,
                            void const * v_min, void const * v_max, char const * format = nullptr,
                            float power = 1.0f);
TBAG_API bool VSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max,
                           char const * format = "%.3f", float power = 1.0f);
TBAG_API bool VSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max,
                         char const * format = "%d");
TBAG_API bool VSliderScalar(char const * label, Vector2 const & size, DataType data_type, void * v,
                            void const * v_min, void const * v_max, char const * format = nullptr,
                            float power = 1.0f);

// Widgets: Input with Keyboard
TBAG_API bool InputText(char const * label, char * buf, size_t buf_size, InputTextFlags flags = 0);
TBAG_API bool InputTextMultiline(char const * label, char * buf, size_t buf_size,
                                 Vector2 const & size = Vector2{0,0}, InputTextFlags flags = 0);
TBAG_API bool InputTextWithHint(char const * label, char const * hint, char * buf,
                                size_t buf_size, InputTextFlags flags = 0);
TBAG_API bool InputFloat(char const * label, float * v, float step = 0.0f, float step_fast = 0.0f,
                         char const * format = "%.3f", InputTextFlags flags = 0);
TBAG_API bool InputFloat2(char const * label, float v[2], char const * format = "%.3f", InputTextFlags flags = 0);
TBAG_API bool InputFloat3(char const * label, float v[3], char const * format = "%.3f", InputTextFlags flags = 0);
TBAG_API bool InputFloat4(char const * label, float v[4], char const * format = "%.3f", InputTextFlags flags = 0);
TBAG_API bool InputInt(char const * label, int * v, int step = 1, int step_fast = 100, InputTextFlags flags = 0);
TBAG_API bool InputInt2(char const * label, int v[2], InputTextFlags flags = 0);
TBAG_API bool InputInt3(char const * label, int v[3], InputTextFlags flags = 0);
TBAG_API bool InputInt4(char const * label, int v[4], InputTextFlags flags = 0);
TBAG_API bool InputDouble(char const * label, double * v, double step = 0.0, double step_fast = 0.0,
                          char const * format = "%.6f", InputTextFlags flags = 0);
TBAG_API bool InputScalar(char const * label, DataType data_type, void * v, void const * step = nullptr,
                          void const * step_fast = nullptr, char const * format = nullptr, InputTextFlags flags = 0);
TBAG_API bool InputScalarN(char const * label, DataType data_type, void * v, int components,
                           void const * step = nullptr, void const * step_fast = nullptr,
                           char const * format = nullptr, InputTextFlags flags = 0);

// Widgets: Color Editor/Picker
TBAG_API bool ColorEdit3(char const * label, float col[3], ColorEditFlags flags = 0);
TBAG_API bool ColorEdit4(char const * label, float col[4], ColorEditFlags flags = 0);
TBAG_API bool ColorPicker3(char const * label, float col[3], ColorEditFlags flags = 0);
TBAG_API bool ColorPicker4(char const * label, float col[4], ColorEditFlags flags = 0,
                           float const * ref_col = nullptr);
TBAG_API bool ColorButton(char const * desc_id, Vector4 const & col,
                          ColorEditFlags flags = 0, Vector2 size = Vector2{0,0});
TBAG_API void SetColorEditOptions(ColorEditFlags flags);

// Widgets: Trees
TBAG_API bool TreeNode(char const * label);
TBAG_API bool TreeNode(char const * str_id, char const * text);
TBAG_API bool TreeNodeEx(char const * label, TreeNodeFlags flags = 0);
TBAG_API bool TreeNodeEx(char const * str_id, TreeNodeFlags flags, char const * text);
TBAG_API void TreePush(char const * str_id);
TBAG_API void TreePush(void const * ptr_id = nullptr);
TBAG_API void TreePop();
TBAG_API void TreeAdvanceToLabelPos();
TBAG_API float GetTreeNodeToLabelSpacing();
TBAG_API void SetNextTreeNodeOpen(bool is_open, Cond cond = 0);
TBAG_API bool CollapsingHeader(char const * label, TreeNodeFlags flags = 0);
TBAG_API bool CollapsingHeader(char const * label, bool * p_open, TreeNodeFlags flags = 0);

// Widgets: Selectables
TBAG_API bool Selectable(char const * label, bool selected = false, SelectableFlags flags = 0,
                         Vector2 const & size = Vector2{0,0});
TBAG_API bool Selectable(char const * label, bool * p_selected, SelectableFlags flags = 0,
                         Vector2 const & size = Vector2{0,0});

// Widgets: List Boxes
TBAG_API bool ListBox(char const * label, int * current_item, char const * const items[],
                      int items_count, int height_in_items = -1);
TBAG_API bool ListBox(char const * label, int * current_item,
                      bool (*items_getter)(void * data, int idx, char const ** out_text),
                      void * data, int items_count, int height_in_items = -1);
TBAG_API bool ListBoxHeader(char const * label, Vector2 const & size = Vector2{0,0});
TBAG_API bool ListBoxHeader(char const * label, int items_count, int height_in_items = -1);
TBAG_API void ListBoxFooter();

// Widgets: Data Plotting
TBAG_API void PlotLines(char const * label, float const * values, int values_count, int values_offset = 0,
                        char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                        Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
TBAG_API void PlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data,
                        int values_count, int values_offset = 0, char const * overlay_text = nullptr,
                        float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
TBAG_API void PlotHistogram(char const * label, float const * values, int values_count, int values_offset = 0,
                            char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                            Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
TBAG_API void PlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data,
                            int values_count, int values_offset = 0, char const * overlay_text = nullptr,
                            float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});

// Widgets: Value() Helpers.
TBAG_API void Value(char const * prefix, bool b);
TBAG_API void Value(char const * prefix, int v);
TBAG_API void Value(char const * prefix, unsigned int v);
TBAG_API void Value(char const * prefix, float v, char const * float_format = nullptr);

// Widgets: Menus
TBAG_API bool BeginMainMenuBar();
TBAG_API void EndMainMenuBar();
TBAG_API bool BeginMenuBar();
TBAG_API void EndMenuBar();
TBAG_API bool BeginMenu(char const * label, bool enabled = true);
TBAG_API void EndMenu();
TBAG_API bool MenuItem(char const * label, char const * shortcut = nullptr, bool selected = false, bool enabled = true);
TBAG_API bool MenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled = true);

// Tooltips
TBAG_API void BeginTooltip();
TBAG_API void EndTooltip();
TBAG_API void SetTooltip(char const * text);

// Popups, Modals
TBAG_API void OpenPopup(char const * str_id);
TBAG_API bool BeginPopup(char const * str_id, WindowFlags flags = 0);
TBAG_API bool BeginPopupContextItem(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool BeginPopupContextWindow(char const * str_id = nullptr, int mouse_button = 1, bool also_over_items = true);
TBAG_API bool BeginPopupContextVoid(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool BeginPopupModal(char const * name, bool * p_open = nullptr, WindowFlags flags = 0);
TBAG_API void EndPopup();
TBAG_API bool OpenPopupOnItemClick(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool IsPopupOpen(char const * str_id);
TBAG_API void CloseCurrentPopup();

// Columns
TBAG_API void Columns(int count = 1, char const * id = nullptr, bool border = true);
TBAG_API void NextColumn();
TBAG_API int GetColumnIndex();
TBAG_API float GetColumnWidth(int column_index = -1);
TBAG_API void SetColumnWidth(int column_index, float width);
TBAG_API float GetColumnOffset(int column_index = -1);
TBAG_API void SetColumnOffset(int column_index, float offset_x);
TBAG_API int GetColumnsCount();

// Tab Bars, Tabs
TBAG_API bool BeginTabBar(char const * str_id, TabBarFlags flags = 0);
TBAG_API void EndTabBar();
TBAG_API bool BeginTabItem(char const * label, bool * p_open = nullptr, TabItemFlags flags = 0);
TBAG_API void EndTabItem();
TBAG_API void SetTabItemClosed(char const * tab_or_docked_window_label);

// Logging/Capture
TBAG_API void LogToTTY(int auto_open_depth = -1);
TBAG_API void LogToFile(int auto_open_depth = -1, char const * filename = nullptr);
TBAG_API void LogToClipboard(int auto_open_depth = -1);
TBAG_API void LogFinish();
TBAG_API void LogButtons();
TBAG_API void LogText(char const * text);

// Drag and Drop
TBAG_API bool BeginDragDropSource(DragDropFlags flags = 0);
TBAG_API bool SetDragDropPayload(char const * type, void const * data, size_t sz, Cond cond = 0);
TBAG_API void EndDragDropSource();
TBAG_API bool BeginDragDropTarget();
TBAG_API Payload AcceptDragDropPayload(char const * type, DragDropFlags flags = 0);
TBAG_API void EndDragDropTarget();
TBAG_API Payload GetDragDropPayload();

// Clipping
TBAG_API void PushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max, bool intersect_with_current_clip_rect);
TBAG_API void PopClipRect();

// Focus, Activation
TBAG_API void SetItemDefaultFocus();
TBAG_API void SetKeyboardFocusHere(int offset = 0);

// Item/Widgets Utilities
TBAG_API bool IsItemHovered(HoveredFlags flags = 0);
TBAG_API bool IsItemActive();
TBAG_API bool IsItemFocused();
TBAG_API bool IsItemClicked(int mouse_button = 0);
TBAG_API bool IsItemVisible();
TBAG_API bool IsItemEdited();
TBAG_API bool IsItemActivated();
TBAG_API bool IsItemDeactivated();
TBAG_API bool IsItemDeactivatedAfterEdit();
TBAG_API bool IsAnyItemHovered();
TBAG_API bool IsAnyItemActive();
TBAG_API bool IsAnyItemFocused();
TBAG_API Vector2 GetItemRectMin();
TBAG_API Vector2 GetItemRectMax();
TBAG_API Vector2 GetItemRectSize();
TBAG_API void SetItemAllowOverlap();

// Miscellaneous Utilities
TBAG_API bool IsRectVisible(Vector2 const & size);
TBAG_API bool IsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max);
TBAG_API double GetTime();
TBAG_API int GetFrameCount();
TBAG_API char const * GetStyleColorName(Col idx);
TBAG_API Vector2 CalcTextSize(char const * text, char const * text_end = nullptr,
                              bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
TBAG_API void CalcListClipping(int items_count, float items_height, int * out_items_display_start,
                               int * out_items_display_end);
TBAG_API bool BeginChildFrame(ID id, Vector2 const & size, WindowFlags flags = 0);
TBAG_API void EndChildFrame();

// Color Utilities
TBAG_API Vector4 ColorConvertU32ToFloat4(unsigned int in);
TBAG_API unsigned int ColorConvertFloat4ToU32(Vector4 const & in);
TBAG_API void ColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v);
TBAG_API void ColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b);

// Inputs Utilities
TBAG_API int GetKeyIndex(Key imgui_key);
TBAG_API bool IsKeyDown(int user_key_index);
TBAG_API bool IsKeyPressed(int user_key_index, bool repeat = true);
TBAG_API bool IsKeyReleased(int user_key_index);
TBAG_API int GetKeyPressedAmount(int key_index, float repeat_delay, float rate);
TBAG_API bool IsMouseDown(int button);
TBAG_API bool IsAnyMouseDown();
TBAG_API bool IsMouseClicked(int button, bool repeat = false);
TBAG_API bool IsMouseDoubleClicked(int button);
TBAG_API bool IsMouseReleased(int button);
TBAG_API bool IsMouseDragging(int button = 0, float lock_threshold = -1.0f);
TBAG_API bool IsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip = true);
TBAG_API bool IsMousePosValid(Vector2 const * mouse_pos = nullptr);
TBAG_API Vector2 GetMousePos();
TBAG_API Vector2 GetMousePosOnOpeningCurrentPopup();
TBAG_API Vector2 GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);
TBAG_API void ResetMouseDragDelta(int button = 0);
TBAG_API MouseCursor GetMouseCursor();
TBAG_API void SetMouseCursor(MouseCursor type);
TBAG_API void CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);
TBAG_API void CaptureMouseFromApp(bool want_capture_mouse_value = true);

// Clipboard Utilities
TBAG_API char const * GetClipboardText();
TBAG_API void SetClipboardText(char const * text);

// Settings/.Ini Utilities
TBAG_API void LoadIniSettingsFromDisk(char const * ini_filename);
TBAG_API void LoadIniSettingsFromMemory(char const * ini_data, size_t ini_size = 0);
TBAG_API void SaveIniSettingsToDisk(char const * ini_filename);
TBAG_API char const * SaveIniSettingsToMemory(size_t * out_ini_size = nullptr);

} // namespace gui
} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_GUI_RAYGUI_HPP__

