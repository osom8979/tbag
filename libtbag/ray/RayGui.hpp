/**
 * @file   RayGui.hpp
 * @brief  RayGui class prototype.
 * @author zer0
 * @date   2019-05-06
 * @date   2019-05-13 (Move: libtbag/ray -> libtbag/ray/gui)
 * @date   2019-05-14 (Move: libtbag/ray/gui -> libtbag/ray)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__

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

// ----------------
// clang-format off
// ----------------

typedef void * GuiTextureID;
typedef unsigned int GuiID;
typedef unsigned short GuiWchar;
typedef int GuiCol;
typedef int GuiCond;
typedef int GuiDataType;
typedef int GuiDir;
typedef int GuiKey;
typedef int GuiNavInput;
typedef int GuiMouseCursor;
typedef int GuiStyleVar;
typedef int GuiDrawCornerFlags;
typedef int GuiDrawListFlags;
typedef int GuiFontAtlasFlags;
typedef int GuiBackendFlags;
typedef int GuiColorEditFlags;
typedef int GuiColumnsFlags;
typedef int GuiConfigFlags;
typedef int GuiComboFlags;
typedef int GuiDragDropFlags;
typedef int GuiFocusedFlags;
typedef int GuiHoveredFlags;
typedef int GuiInputTextFlags;
typedef int GuiSelectableFlags;
typedef int GuiTabBarFlags;
typedef int GuiTabItemFlags;
typedef int GuiTreeNodeFlags;
typedef int GuiWindowFlags;

enum GuiCol_
{
    GuiCol_Text,
    GuiCol_TextDisabled,
    GuiCol_WindowBg,
    GuiCol_ChildBg,
    GuiCol_PopupBg,
    GuiCol_Border,
    GuiCol_BorderShadow,
    GuiCol_FrameBg,
    GuiCol_FrameBgHovered,
    GuiCol_FrameBgActive,
    GuiCol_TitleBg,
    GuiCol_TitleBgActive,
    GuiCol_TitleBgCollapsed,
    GuiCol_MenuBarBg,
    GuiCol_ScrollbarBg,
    GuiCol_ScrollbarGrab,
    GuiCol_ScrollbarGrabHovered,
    GuiCol_ScrollbarGrabActive,
    GuiCol_CheckMark,
    GuiCol_SliderGrab,
    GuiCol_SliderGrabActive,
    GuiCol_Button,
    GuiCol_ButtonHovered,
    GuiCol_ButtonActive,
    GuiCol_Header,
    GuiCol_HeaderHovered,
    GuiCol_HeaderActive,
    GuiCol_Separator,
    GuiCol_SeparatorHovered,
    GuiCol_SeparatorActive,
    GuiCol_ResizeGrip,
    GuiCol_ResizeGripHovered,
    GuiCol_ResizeGripActive,
    GuiCol_Tab,
    GuiCol_TabHovered,
    GuiCol_TabActive,
    GuiCol_TabUnfocused,
    GuiCol_TabUnfocusedActive,
    GuiCol_PlotLines,
    GuiCol_PlotLinesHovered,
    GuiCol_PlotHistogram,
    GuiCol_PlotHistogramHovered,
    GuiCol_TextSelectedBg,
    GuiCol_DragDropTarget,
    GuiCol_NavHighlight,
    GuiCol_NavWindowingHighlight,
    GuiCol_NavWindowingDimBg,
    GuiCol_ModalWindowDimBg,
    GuiCol_COUNT
};

enum GuiWindowFlags_
{
    GuiWindowFlags_None                      = 0,
    GuiWindowFlags_NoTitleBar                = 1 << 0,
    GuiWindowFlags_NoResize                  = 1 << 1,
    GuiWindowFlags_NoMove                    = 1 << 2,
    GuiWindowFlags_NoScrollbar               = 1 << 3,
    GuiWindowFlags_NoScrollWithMouse         = 1 << 4,
    GuiWindowFlags_NoCollapse                = 1 << 5,
    GuiWindowFlags_AlwaysAutoResize          = 1 << 6,
    GuiWindowFlags_NoBackground              = 1 << 7,
    GuiWindowFlags_NoSavedSettings           = 1 << 8,
    GuiWindowFlags_NoMouseInputs             = 1 << 9,
    GuiWindowFlags_MenuBar                   = 1 << 10,
    GuiWindowFlags_HorizontalScrollbar       = 1 << 11,
    GuiWindowFlags_NoFocusOnAppearing        = 1 << 12,
    GuiWindowFlags_NoBringToFrontOnFocus     = 1 << 13,
    GuiWindowFlags_AlwaysVerticalScrollbar   = 1 << 14,
    GuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
    GuiWindowFlags_AlwaysUseWindowPadding    = 1 << 16,
    GuiWindowFlags_NoNavInputs               = 1 << 18,
    GuiWindowFlags_NoNavFocus                = 1 << 19,
    GuiWindowFlags_UnsavedDocument           = 1 << 20,

    GuiWindowFlags_NoNav = GuiWindowFlags_NoNavInputs |
                           GuiWindowFlags_NoNavFocus,
    GuiWindowFlags_NoDecoration = GuiWindowFlags_NoTitleBar  |
                                  GuiWindowFlags_NoResize    |
                                  GuiWindowFlags_NoScrollbar |
                                  GuiWindowFlags_NoCollapse,
    GuiWindowFlags_NoInputs = GuiWindowFlags_NoMouseInputs |
                              GuiWindowFlags_NoNavInputs   |
                              GuiWindowFlags_NoNavFocus,
};

enum GuiFocusedFlags_
{
    GuiFocusedFlags_None         = 0,
    GuiFocusedFlags_ChildWindows = 1 << 0,
    GuiFocusedFlags_RootWindow   = 1 << 1,
    GuiFocusedFlags_AnyWindow    = 1 << 2,

    GuiFocusedFlags_RootAndChildWindows = GuiFocusedFlags_RootWindow |
                                          GuiFocusedFlags_ChildWindows
};

enum GuiHoveredFlags_
{
    GuiHoveredFlags_None                          = 0,
    GuiHoveredFlags_ChildWindows                  = 1 << 0,
    GuiHoveredFlags_RootWindow                    = 1 << 1,
    GuiHoveredFlags_AnyWindow                     = 1 << 2,
    GuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 3,
    //GuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 4,
    GuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 5,
    GuiHoveredFlags_AllowWhenOverlapped           = 1 << 6,
    GuiHoveredFlags_AllowWhenDisabled             = 1 << 7,

    GuiHoveredFlags_RectOnly = GuiHoveredFlags_AllowWhenBlockedByPopup      |
                               GuiHoveredFlags_AllowWhenBlockedByActiveItem |
                               GuiHoveredFlags_AllowWhenOverlapped,
    GuiHoveredFlags_RootAndChildWindows = GuiHoveredFlags_RootWindow |
                                          GuiHoveredFlags_ChildWindows,
};

enum GuiCond_
{
    GuiCond_Always       = 1 << 0,
    GuiCond_Once         = 1 << 1,
    GuiCond_FirstUseEver = 1 << 2,
    GuiCond_Appearing    = 1 << 3
};

enum GuiStyleVar_
{
    GuiStyleVar_Alpha,
    GuiStyleVar_WindowPadding,
    GuiStyleVar_WindowRounding,
    GuiStyleVar_WindowBorderSize,
    GuiStyleVar_WindowMinSize,
    GuiStyleVar_WindowTitleAlign,
    GuiStyleVar_ChildRounding,
    GuiStyleVar_ChildBorderSize,
    GuiStyleVar_PopupRounding,
    GuiStyleVar_PopupBorderSize,
    GuiStyleVar_FramePadding,
    GuiStyleVar_FrameRounding,
    GuiStyleVar_FrameBorderSize,
    GuiStyleVar_ItemSpacing,
    GuiStyleVar_ItemInnerSpacing,
    GuiStyleVar_IndentSpacing,
    GuiStyleVar_ScrollbarSize,
    GuiStyleVar_ScrollbarRounding,
    GuiStyleVar_GrabMinSize,
    GuiStyleVar_GrabRounding,
    GuiStyleVar_TabRounding,
    GuiStyleVar_ButtonTextAlign,
    GuiStyleVar_SelectableTextAlign,
    GuiStyleVar_COUNT
};

enum GuiDir_
{
    GuiDir_None  = -1,
    GuiDir_Left  = 0,
    GuiDir_Right = 1,
    GuiDir_Up    = 2,
    GuiDir_Down  = 3,
    GuiDir_COUNT
};

enum GuiComboFlags_
{
    GuiComboFlags_None           = 0,
    GuiComboFlags_PopupAlignLeft = 1 << 0,
    GuiComboFlags_HeightSmall    = 1 << 1,
    GuiComboFlags_HeightRegular  = 1 << 2,
    GuiComboFlags_HeightLarge    = 1 << 3,
    GuiComboFlags_HeightLargest  = 1 << 4,
    GuiComboFlags_NoArrowButton  = 1 << 5,
    GuiComboFlags_NoPreview      = 1 << 6,

    GuiComboFlags_HeightMask_ = GuiComboFlags_HeightSmall   |
                                GuiComboFlags_HeightRegular |
                                GuiComboFlags_HeightLarge   |
                                GuiComboFlags_HeightLargest
};

enum GuiDataType_
{
    GuiDataType_S8,
    GuiDataType_U8,
    GuiDataType_S16,
    GuiDataType_U16,
    GuiDataType_S32,
    GuiDataType_U32,
    GuiDataType_S64,
    GuiDataType_U64,
    GuiDataType_Float,
    GuiDataType_Double,
    GuiDataType_COUNT
};

enum GuiColorEditFlags_
{
    GuiColorEditFlags_None             = 0,
    GuiColorEditFlags_NoAlpha          = 1 << 1,
    GuiColorEditFlags_NoPicker         = 1 << 2,
    GuiColorEditFlags_NoOptions        = 1 << 3,
    GuiColorEditFlags_NoSmallPreview   = 1 << 4,
    GuiColorEditFlags_NoInputs         = 1 << 5,
    GuiColorEditFlags_NoTooltip        = 1 << 6,
    GuiColorEditFlags_NoLabel          = 1 << 7,
    GuiColorEditFlags_NoSidePreview    = 1 << 8,
    GuiColorEditFlags_NoDragDrop       = 1 << 9,
    GuiColorEditFlags_AlphaBar         = 1 << 16,
    GuiColorEditFlags_AlphaPreview     = 1 << 17,
    GuiColorEditFlags_AlphaPreviewHalf = 1 << 18,
    GuiColorEditFlags_HDR              = 1 << 19,
    GuiColorEditFlags_DisplayRGB       = 1 << 20,
    GuiColorEditFlags_DisplayHSV       = 1 << 21,
    GuiColorEditFlags_DisplayHex       = 1 << 22,
    GuiColorEditFlags_Uint8            = 1 << 23,
    GuiColorEditFlags_Float            = 1 << 24,
    GuiColorEditFlags_PickerHueBar     = 1 << 25,
    GuiColorEditFlags_PickerHueWheel   = 1 << 26,
    GuiColorEditFlags_InputRGB         = 1 << 27,
    GuiColorEditFlags_InputHSV         = 1 << 28,

    GuiColorEditFlags__OptionsDefault = GuiColorEditFlags_Uint8      |
                                        GuiColorEditFlags_DisplayRGB |
                                        GuiColorEditFlags_InputRGB   |
                                        GuiColorEditFlags_PickerHueBar,
};

enum GuiTreeNodeFlags_
{
    GuiTreeNodeFlags_None                 = 0,
    GuiTreeNodeFlags_Selected             = 1 << 0,
    GuiTreeNodeFlags_Framed               = 1 << 1,
    GuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,
    GuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,
    GuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,
    GuiTreeNodeFlags_DefaultOpen          = 1 << 5,
    GuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,
    GuiTreeNodeFlags_OpenOnArrow          = 1 << 7,
    GuiTreeNodeFlags_Leaf                 = 1 << 8,
    GuiTreeNodeFlags_Bullet               = 1 << 9,
    GuiTreeNodeFlags_FramePadding         = 1 << 10,
    //GuiTreeNodeFlags_SpanAllAvailWidth  = 1 << 11,
    //GuiTreeNodeFlags_NoScrollOnOpen     = 1 << 12,
    GuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,

    GuiTreeNodeFlags_CollapsingHeader = GuiTreeNodeFlags_Framed           |
                                        GuiTreeNodeFlags_NoTreePushOnOpen |
                                        GuiTreeNodeFlags_NoAutoOpenOnLog
};

enum GuiSelectableFlags_
{
    GuiSelectableFlags_None             = 0,
    GuiSelectableFlags_DontClosePopups  = 1 << 0,
    GuiSelectableFlags_SpanAllColumns   = 1 << 1,
    GuiSelectableFlags_AllowDoubleClick = 1 << 2,
    GuiSelectableFlags_Disabled         = 1 << 3
};

enum GuiTabBarFlags_
{
    GuiTabBarFlags_None                           = 0,
    GuiTabBarFlags_Reorderable                    = 1 << 0,
    GuiTabBarFlags_AutoSelectNewTabs              = 1 << 1,
    GuiTabBarFlags_TabListPopupButton             = 1 << 2,
    GuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,
    GuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4,
    GuiTabBarFlags_NoTooltip                      = 1 << 5,
    GuiTabBarFlags_FittingPolicyResizeDown        = 1 << 6,
    GuiTabBarFlags_FittingPolicyScroll            = 1 << 7,

    GuiTabBarFlags_FittingPolicyMask_ = GuiTabBarFlags_FittingPolicyResizeDown |
                                        GuiTabBarFlags_FittingPolicyScroll,
    GuiTabBarFlags_FittingPolicyDefault_ = GuiTabBarFlags_FittingPolicyResizeDown
};

enum GuiTabItemFlags_
{
    GuiTabItemFlags_None                          = 0,
    GuiTabItemFlags_UnsavedDocument               = 1 << 0,
    GuiTabItemFlags_SetSelected                   = 1 << 1,
    GuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,
    GuiTabItemFlags_NoPushId                      = 1 << 3
};

enum GuiDragDropFlags_
{
    GuiDragDropFlags_None = 0,

    // GuiBeginDragDropSource() flags
    GuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,
    GuiDragDropFlags_SourceNoDisableHover         = 1 << 1,
    GuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,
    GuiDragDropFlags_SourceAllowNullID            = 1 << 3,
    GuiDragDropFlags_SourceExtern                 = 1 << 4,
    GuiDragDropFlags_SourceAutoExpirePayload      = 1 << 5,

    // GuiAcceptDragDropPayload() flags
    GuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,
    GuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,
    GuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,
    GuiDragDropFlags_AcceptPeekOnly = GuiDragDropFlags_AcceptBeforeDelivery |
                                      GuiDragDropFlags_AcceptNoDrawDefaultRect
};

enum GuiKey_
{
    GuiKey_Tab,
    GuiKey_LeftArrow,
    GuiKey_RightArrow,
    GuiKey_UpArrow,
    GuiKey_DownArrow,
    GuiKey_PageUp,
    GuiKey_PageDown,
    GuiKey_Home,
    GuiKey_End,
    GuiKey_Insert,
    GuiKey_Delete,
    GuiKey_Backspace,
    GuiKey_Space,
    GuiKey_Enter,
    GuiKey_Escape,
    GuiKey_A,
    GuiKey_C,
    GuiKey_V,
    GuiKey_X,
    GuiKey_Y,
    GuiKey_Z,
    GuiKey_COUNT
};

enum GuiMouseCursor_
{
    GuiMouseCursor_None = -1,
    GuiMouseCursor_Arrow = 0,
    GuiMouseCursor_TextInput,
    GuiMouseCursor_ResizeAll,
    GuiMouseCursor_ResizeNS,
    GuiMouseCursor_ResizeEW,
    GuiMouseCursor_ResizeNESW,
    GuiMouseCursor_ResizeNWSE,
    GuiMouseCursor_Hand,
    GuiMouseCursor_COUNT
};

struct GuiStyle
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
    Vector4     Colors[GuiCol_COUNT];
};

struct GuiPayload
{
    void * Data;
    int    DataSize;

    GuiID  SourceId;
    GuiID  SourceParentId;
    int    DataFrameCount;
    char   DataType[32+1];
    bool   Preview;
    bool   Delivery;

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

TBAG_API bool GuiInitRay();
TBAG_API void GuiUpdateRay();
TBAG_API void GuiShutdownRay();

// Demo, Debug, Information
TBAG_API void GuiShowDemoWindow(bool * p_open = nullptr);
TBAG_API void GuiShowAboutWindow(bool * p_open = nullptr);
TBAG_API void GuiShowMetricsWindow(bool * p_open = nullptr);
TBAG_API void GuiShowStyleEditor();
TBAG_API bool GuiShowStyleSelector(char const * label);
TBAG_API void GuiShowFontSelector(char const * label);
TBAG_API void GuiShowUserGuide();

TBAG_API char const * GuiGetVersion();

// Styles
TBAG_API void GuiStyleColorsDark();
TBAG_API void GuiStyleColorsClassic();
TBAG_API void GuiStyleColorsLight();

TBAG_API GuiStyle GuiGetStyle();
TBAG_API void     GuiSetStyle(GuiStyle const & style);

// Windows
TBAG_API bool GuiBegin(char const * text, bool * p_open = nullptr, GuiWindowFlags flags = 0);
TBAG_API void GuiEnd();

// Child Windows
TBAG_API bool GuiBeginChild(char const * str_id,
                            Vector2 const & size = Vector2{0,0},
                            bool border = false,
                            GuiWindowFlags flags = 0);
TBAG_API bool GuiBeginChild(GuiID id, Vector2 const & size = Vector2{0,0},
                            bool border = false, GuiWindowFlags flags = 0);
TBAG_API void GuiEndChild();

// Windows Utilities
TBAG_API bool    GuiIsWindowAppearing();
TBAG_API bool    GuiIsWindowCollapsed();
TBAG_API bool    GuiIsWindowFocused(GuiFocusedFlags flags = 0);
TBAG_API bool    GuiIsWindowHovered(GuiHoveredFlags flags = 0);
TBAG_API Vector2 GuiGetWindowPos();
TBAG_API Vector2 GuiGetWindowSize();
TBAG_API float   GuiGetWindowWidth();
TBAG_API float   GuiGetWindowHeight();

// Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
TBAG_API void GuiSetNextWindowPos(Vector2 const & pos, GuiCond cond = 0, Vector2 const & pivot = Vector2{0,0});
TBAG_API void GuiSetNextWindowSize(Vector2 const & size, GuiCond cond = 0);
TBAG_API void GuiSetNextWindowContentSize(Vector2 const & size);
TBAG_API void GuiSetNextWindowCollapsed(bool collapsed, GuiCond cond = 0);
TBAG_API void GuiSetNextWindowFocus();
TBAG_API void GuiSetNextWindowBgAlpha(float alpha);
TBAG_API void GuiSetWindowPos(Vector2 const & pos, GuiCond cond = 0);
TBAG_API void GuiSetWindowSize(Vector2 const & size, GuiCond cond = 0);
TBAG_API void GuiSetWindowCollapsed(bool collapsed, GuiCond cond = 0);
TBAG_API void GuiSetWindowFocus();
TBAG_API void GuiSetWindowFontScale(float scale);
TBAG_API void GuiSetWindowPos(char const * name, Vector2 const & pos, GuiCond cond = 0);
TBAG_API void GuiSetWindowSize(char const * name, Vector2 const & size, GuiCond cond = 0);
TBAG_API void GuiSetWindowCollapsed(char const * name, bool collapsed, GuiCond cond = 0);
TBAG_API void GuiSetWindowFocus(char const * name);

// Content region
TBAG_API Vector2 GuiGetContentRegionMax();
TBAG_API Vector2 GuiGetContentRegionAvail();
TBAG_API Vector2 GuiGetWindowContentRegionMin();
TBAG_API Vector2 GuiGetWindowContentRegionMax();
TBAG_API float   GuiGetWindowContentRegionWidth();

// Windows Scrolling
TBAG_API float GuiGetScrollX();
TBAG_API float GuiGetScrollY();
TBAG_API float GuiGetScrollMaxX();
TBAG_API float GuiGetScrollMaxY();
TBAG_API void  GuiSetScrollX(float scroll_x);
TBAG_API void  GuiSetScrollY(float scroll_y);
TBAG_API void  GuiSetScrollHereY(float center_y_ratio = 0.5f);
TBAG_API void  GuiSetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);

// Parameters stacks (shared)
TBAG_API void         GuiPushStyleColor(GuiCol idx, unsigned int col);
TBAG_API void         GuiPushStyleColor(GuiCol idx, Vector4 const & col);
TBAG_API void         GuiPopStyleColor(int count = 1);
TBAG_API void         GuiPushStyleVar(GuiStyleVar idx, float val);
TBAG_API void         GuiPushStyleVar(GuiStyleVar idx, Vector2 const & val);
TBAG_API void         GuiPopStyleVar(int count = 1);
TBAG_API Vector4      GuiGetStyleColorVec4(GuiCol idx);
TBAG_API float        GuiGetFontSize();
TBAG_API Vector2      GuiGetFontTexUvWhitePixel();
TBAG_API unsigned int GuiGetColorU32(GuiCol idx, float alpha_mul = 1.0f);
TBAG_API unsigned int GuiGetColorU32(Vector4 const & col);
TBAG_API unsigned int GuiGetColorU32(unsigned int col);

// Parameters stacks (current window)
TBAG_API void  GuiPushItemWidth(float item_width);
TBAG_API void  GuiPopItemWidth();
TBAG_API void  GuiSetNextItemWidth(float item_width);
TBAG_API float GuiCalcItemWidth();
TBAG_API void  GuiPushTextWrapPos(float wrap_local_pos_x = 0.0f);
TBAG_API void  GuiPopTextWrapPos();
TBAG_API void  GuiPushAllowKeyboardFocus(bool allow_keyboard_focus);
TBAG_API void  GuiPopAllowKeyboardFocus();
TBAG_API void  GuiPushButtonRepeat(bool repeat);
TBAG_API void  GuiPopButtonRepeat();

// Cursor / Layout
TBAG_API void    GuiSeparator();
TBAG_API void    GuiSameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
TBAG_API void    GuiNewLine();
TBAG_API void    GuiSpacing();
TBAG_API void    GuiDummy(Vector2 const & size);
TBAG_API void    GuiIndent(float indent_w = 0.0f);
TBAG_API void    GuiUnindent(float indent_w = 0.0f);
TBAG_API void    GuiBeginGroup();
TBAG_API void    GuiEndGroup();
TBAG_API Vector2 GuiGetCursorPos();
TBAG_API float   GuiGetCursorPosX();
TBAG_API float   GuiGetCursorPosY();
TBAG_API void    GuiSetCursorPos(Vector2 const & local_pos);
TBAG_API void    GuiSetCursorPosX(float local_x);
TBAG_API void    GuiSetCursorPosY(float local_y);
TBAG_API Vector2 GuiGetCursorStartPos();
TBAG_API Vector2 GuiGetCursorScreenPos();
TBAG_API void    GuiSetCursorScreenPos(Vector2 const & pos);
TBAG_API void    GuiAlignTextToFramePadding();
TBAG_API float   GuiGetTextLineHeight();
TBAG_API float   GuiGetTextLineHeightWithSpacing();
TBAG_API float   GuiGetFrameHeight();
TBAG_API float   GuiGetFrameHeightWithSpacing();

// GuiID stack/scopes
TBAG_API void  GuiPushID(char const * str_id);
TBAG_API void  GuiPushID(char const * str_id_begin, char const * str_id_end);
TBAG_API void  GuiPushID(void const * ptr_id);
TBAG_API void  GuiPushID(int int_id);
TBAG_API void  GuiPopID();
TBAG_API GuiID GuiGetID(char const * str_id);
TBAG_API GuiID GuiGetID(char const * str_id_begin, char const * str_id_end);
TBAG_API GuiID GuiGetID(void const * ptr_id);

// Widgets: Text
TBAG_API void GuiTextUnformatted(char const * text, char const * text_end = nullptr);
TBAG_API void GuiText(char const * text);
TBAG_API void GuiTextColored(Vector4 const & col, char const * text);
TBAG_API void GuiTextDisabled(char const * text);
TBAG_API void GuiTextWrapped(char const * text);
TBAG_API void GuiLabelText(char const * label, char const * text);
TBAG_API void GuiBulletText(char const * text);

// Widgets: Main
TBAG_API bool GuiButton(char const * label, Vector2 const & size = Vector2{0,0});
TBAG_API bool GuiSmallButton(char const * label);
TBAG_API bool GuiInvisibleButton(char const * str_id, Vector2 const & size);
TBAG_API bool GuiArrowButton(char const * str_id, GuiDir dir);
TBAG_API void GuiImage(GuiTextureID user_texture_id,
                       Vector2 const & size,
                       Vector2 const & uv0 = Vector2{0,0},
                       Vector2 const & uv1 = Vector2{1,1},
                       Vector4 const & tint_col = Vector4{1,1,1,1},
                       Vector4 const & border_col = Vector4{0,0,0,0});
TBAG_API bool GuiImageButton(GuiTextureID user_texture_id,
                             Vector2 const & size,
                             Vector2 const & uv0 = Vector2{0,0},
                             Vector2 const & uv1 = Vector2{1,1},
                             int frame_padding = -1,
                             Vector4 const & bg_col = Vector4{0,0,0,0},
                             Vector4 const & tint_col = Vector4{1,1,1,1});
TBAG_API bool GuiCheckbox(char const * label, bool * v);
TBAG_API bool GuiCheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value);
TBAG_API bool GuiRadioButton(char const * label, bool active);
TBAG_API bool GuiRadioButton(char const * label, int * v, int v_button);
TBAG_API void GuiProgressBar(float fraction, Vector2 const & size_arg = Vector2{-1,0},
                             char const * overlay = nullptr);
TBAG_API void GuiBullet();

// Widgets: Combo Box
TBAG_API bool GuiBeginCombo(char const * label, char const * preview_value, GuiComboFlags flags = 0);
TBAG_API void GuiEndCombo();
TBAG_API bool GuiCombo(char const * label, int * current_item, char const * const items[], int items_count,
                       int popup_max_height_in_items = -1);
TBAG_API bool GuiCombo(char const * label, int * current_item, char const * items_separated_by_zeros,
                       int popup_max_height_in_items = -1);
TBAG_API bool GuiCombo(char const * label, int * current_item,
                       bool(*items_getter)(void * data, int idx, char const ** out_text),
                       void * data, int items_count, int popup_max_height_in_items = -1);

// Widgets: Drags
TBAG_API bool GuiDragFloat(char const * label, float * v, float v_speed = 1.0f, float v_min = 0.0f,
                           float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiDragFloat2(char const * label, float v[2], float v_speed = 1.0f, float v_min = 0.0f,
                            float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiDragFloat3(char const * label, float v[3], float v_speed = 1.0f, float v_min = 0.0f,
                            float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiDragFloat4(char const * label, float v[4], float v_speed = 1.0f, float v_min = 0.0f,
                            float v_max = 0.0f, char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiDragFloatRange2(char const * label, float * v_current_min, float * v_current_max,
                                 float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f,
                                 char const * format = "%.3f", char const * format_max = nullptr,
                                 float power = 1.0f);
TBAG_API bool GuiDragInt(char const * label, int * v, float v_speed = 1.0f,
                         int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool GuiDragInt2(char const * label, int v[2], float v_speed = 1.0f,
                          int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool GuiDragInt3(char const * label, int v[3], float v_speed = 1.0f,
                          int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool GuiDragInt4(char const * label, int v[4], float v_speed = 1.0f,
                          int v_min = 0, int v_max = 0, char const * format = "%d");
TBAG_API bool GuiDragIntRange2(char const * label, int * v_current_min, int * v_current_max,
                               float v_speed = 1.0f, int v_min = 0, int v_max = 0,
                               char const * format = "%d", char const * format_max = nullptr);
TBAG_API bool GuiDragScalar(char const * label, GuiDataType data_type, void * v, float v_speed,
                            void const * v_min = nullptr, void const * v_max = nullptr,
                            char const * format = nullptr, float power = 1.0f);
TBAG_API bool GuiDragScalarN(char const * label, GuiDataType data_type, void * v, int components,
                             float v_speed, void const * v_min = nullptr, void const * v_max = nullptr,
                             char const * format = nullptr, float power = 1.0f);

// Widgets: Sliders
TBAG_API bool GuiSliderFloat(char const * label, float * v, float v_min, float v_max,
                             char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiSliderFloat2(char const * label, float v[2], float v_min, float v_max,
                              char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiSliderFloat3(char const * label, float v[3], float v_min, float v_max,
                              char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiSliderFloat4(char const * label, float v[4], float v_min, float v_max,
                              char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiSliderAngle(char const * label, float * v_rad, float v_degrees_min = -360.0f,
                             float v_degrees_max = +360.0f, char const * format = "%.0f deg");
TBAG_API bool GuiSliderInt(char const * label, int * v, int v_min, int v_max, char const * format = "%d");
TBAG_API bool GuiSliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format = "%d");
TBAG_API bool GuiSliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format = "%d");
TBAG_API bool GuiSliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format = "%d");
TBAG_API bool GuiSliderScalar(char const * label, GuiDataType data_type, void * v, void const * v_min,
                              void const * v_max, char const * format = nullptr, float power = 1.0f);
TBAG_API bool GuiSliderScalarN(char const * label, GuiDataType data_type, void * v, int components,
                               void const * v_min, void const * v_max, char const * format = nullptr,
                               float power = 1.0f);
TBAG_API bool GuiVSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max,
                              char const * format = "%.3f", float power = 1.0f);
TBAG_API bool GuiVSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max,
                            char const * format = "%d");
TBAG_API bool GuiVSliderScalar(char const * label, Vector2 const & size, GuiDataType data_type, void * v,
                               void const * v_min, void const * v_max, char const * format = nullptr,
                               float power = 1.0f);

// Widgets: Input with Keyboard
TBAG_API bool GuiInputText(char const * label, char * buf, size_t buf_size, GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputTextMultiline(char const * label, char * buf, size_t buf_size,
                                    Vector2 const & size = Vector2{0,0}, GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputTextWithHint(char const * label, char const * hint, char * buf,
                                   size_t buf_size, GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputFloat(char const * label, float * v, float step = 0.0f, float step_fast = 0.0f,
                            char const * format = "%.3f", GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputFloat2(char const * label, float v[2], char const * format = "%.3f", GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputFloat3(char const * label, float v[3], char const * format = "%.3f", GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputFloat4(char const * label, float v[4], char const * format = "%.3f", GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputInt(char const * label, int * v, int step = 1, int step_fast = 100, GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputInt2(char const * label, int v[2], GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputInt3(char const * label, int v[3], GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputInt4(char const * label, int v[4], GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputDouble(char const * label, double * v, double step = 0.0, double step_fast = 0.0,
                             char const * format = "%.6f", GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputScalar(char const * label, GuiDataType data_type, void * v, void const * step = nullptr,
                             void const * step_fast = nullptr, char const * format = nullptr, GuiInputTextFlags flags = 0);
TBAG_API bool GuiInputScalarN(char const * label, GuiDataType data_type, void * v, int components,
                              void const * step = nullptr, void const * step_fast = nullptr,
                              char const * format = nullptr, GuiInputTextFlags flags = 0);

// Widgets: Color Editor/Picker
TBAG_API bool GuiColorEdit3(char const * label, float col[3], GuiColorEditFlags flags = 0);
TBAG_API bool GuiColorEdit4(char const * label, float col[4], GuiColorEditFlags flags = 0);
TBAG_API bool GuiColorPicker3(char const * label, float col[3], GuiColorEditFlags flags = 0);
TBAG_API bool GuiColorPicker4(char const * label, float col[4], GuiColorEditFlags flags = 0,
                              float const * ref_col = nullptr);
TBAG_API bool GuiColorButton(char const * desc_id, Vector4 const & col,
                             GuiColorEditFlags flags = 0, Vector2 size = Vector2{0,0});
TBAG_API void GuiSetColorEditOptions(GuiColorEditFlags flags);

// Widgets: Trees
TBAG_API bool  GuiTreeNode(char const * label);
TBAG_API bool  GuiTreeNode(char const * str_id, char const * text);
TBAG_API bool  GuiTreeNodeEx(char const * label, GuiTreeNodeFlags flags = 0);
TBAG_API bool  GuiTreeNodeEx(char const * str_id, GuiTreeNodeFlags flags, char const * text);
TBAG_API void  GuiTreePush(char const * str_id);
TBAG_API void  GuiTreePush(void const * ptr_id = nullptr);
TBAG_API void  GuiTreePop();
TBAG_API void  GuiTreeAdvanceToLabelPos();
TBAG_API float GuiGetTreeNodeToLabelSpacing();
TBAG_API void  GuiSetNextTreeNodeOpen(bool is_open, GuiCond cond = 0);
TBAG_API bool  GuiCollapsingHeader(char const * label, GuiTreeNodeFlags flags = 0);
TBAG_API bool  GuiCollapsingHeader(char const * label, bool * p_open, GuiTreeNodeFlags flags = 0);

// Widgets: Selectables
TBAG_API bool GuiSelectable(char const * label, bool selected = false, GuiSelectableFlags flags = 0,
                            Vector2 const & size = Vector2{0,0});
TBAG_API bool GuiSelectable(char const * label, bool * p_selected, GuiSelectableFlags flags = 0,
                            Vector2 const & size = Vector2{0,0});

// Widgets: List Boxes
TBAG_API bool GuiListBox(char const * label, int * current_item, char const * const items[],
                         int items_count, int height_in_items = -1);
TBAG_API bool GuiListBox(char const * label, int * current_item,
                         bool (*items_getter)(void * data, int idx, char const ** out_text),
                         void * data, int items_count, int height_in_items = -1);
TBAG_API bool GuiListBoxHeader(char const * label, Vector2 const & size = Vector2{0,0});
TBAG_API bool GuiListBoxHeader(char const * label, int items_count, int height_in_items = -1);
TBAG_API void GuiListBoxFooter();

// Widgets: Data Plotting
TBAG_API void GuiPlotLines(char const * label, float const * values, int values_count, int values_offset = 0,
                           char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                           Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
TBAG_API void GuiPlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data,
                           int values_count, int values_offset = 0, char const * overlay_text = nullptr,
                           float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});
TBAG_API void GuiPlotHistogram(char const * label, float const * values, int values_count, int values_offset = 0,
                               char const * overlay_text = nullptr, float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                               Vector2 graph_size = Vector2{0,0}, int stride = sizeof(float));
TBAG_API void GuiPlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data,
                               int values_count, int values_offset = 0, char const * overlay_text = nullptr,
                               float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2{0,0});

// Widgets: Value() Helpers.
TBAG_API void GuiValue(char const * prefix, bool b);
TBAG_API void GuiValue(char const * prefix, int v);
TBAG_API void GuiValue(char const * prefix, unsigned int v);
TBAG_API void GuiValue(char const * prefix, float v, char const * float_format = nullptr);

// Widgets: Menus
TBAG_API bool GuiBeginMainMenuBar();
TBAG_API void GuiEndMainMenuBar();
TBAG_API bool GuiBeginMenuBar();
TBAG_API void GuiEndMenuBar();
TBAG_API bool GuiBeginMenu(char const * label, bool enabled = true);
TBAG_API void GuiEndMenu();
TBAG_API bool GuiMenuItem(char const * label, char const * shortcut = nullptr, bool selected = false, bool enabled = true);
TBAG_API bool GuiMenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled = true);

// Tooltips
TBAG_API void GuiBeginTooltip();
TBAG_API void GuiEndTooltip();
TBAG_API void GuiSetTooltip(char const * text);

// Popups, Modals
TBAG_API void GuiOpenPopup(char const * str_id);
TBAG_API bool GuiBeginPopup(char const * str_id, GuiWindowFlags flags = 0);
TBAG_API bool GuiBeginPopupContextItem(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool GuiBeginPopupContextWindow(char const * str_id = nullptr, int mouse_button = 1, bool also_over_items = true);
TBAG_API bool GuiBeginPopupContextVoid(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool GuiBeginPopupModal(char const * name, bool * p_open = nullptr, GuiWindowFlags flags = 0);
TBAG_API void GuiEndPopup();
TBAG_API bool GuiOpenPopupOnItemClick(char const * str_id = nullptr, int mouse_button = 1);
TBAG_API bool GuiIsPopupOpen(char const * str_id);
TBAG_API void GuiCloseCurrentPopup();

// Columns
TBAG_API void  GuiColumns(int count = 1, char const * id = nullptr, bool border = true);
TBAG_API void  GuiNextColumn();
TBAG_API int   GuiGetColumnIndex();
TBAG_API float GuiGetColumnWidth(int column_index = -1);
TBAG_API void  GuiSetColumnWidth(int column_index, float width);
TBAG_API float GuiGetColumnOffset(int column_index = -1);
TBAG_API void  GuiSetColumnOffset(int column_index, float offset_x);
TBAG_API int   GuiGetColumnsCount();

// Tab Bars, Tabs
TBAG_API bool GuiBeginTabBar(char const * str_id, GuiTabBarFlags flags = 0);
TBAG_API void GuiEndTabBar();
TBAG_API bool GuiBeginTabItem(char const * label, bool * p_open = nullptr, GuiTabItemFlags flags = 0);
TBAG_API void GuiEndTabItem();
TBAG_API void GuiSetTabItemClosed(char const * tab_or_docked_window_label);

// Logging/Capture
TBAG_API void GuiLogToTTY(int auto_open_depth = -1);
TBAG_API void GuiLogToFile(int auto_open_depth = -1, char const * filename = nullptr);
TBAG_API void GuiLogToClipboard(int auto_open_depth = -1);
TBAG_API void GuiLogFinish();
TBAG_API void GuiLogButtons();
TBAG_API void GuiLogText(char const * text);

// Drag and Drop
TBAG_API bool       GuiBeginDragDropSource(GuiDragDropFlags flags = 0);
TBAG_API bool       GuiSetDragDropPayload(char const * type, void const * data, size_t sz, GuiCond cond = 0);
TBAG_API void       GuiEndDragDropSource();
TBAG_API bool       GuiBeginDragDropTarget();
TBAG_API GuiPayload GuiAcceptDragDropPayload(char const * type, GuiDragDropFlags flags = 0);
TBAG_API void       GuiEndDragDropTarget();
TBAG_API GuiPayload GuiGetDragDropPayload();

// Clipping
TBAG_API void GuiPushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max,
                              bool intersect_with_current_clip_rect);
TBAG_API void GuiPopClipRect();

// Focus, Activation
TBAG_API void GuiSetItemDefaultFocus();
TBAG_API void GuiSetKeyboardFocusHere(int offset = 0);

// Item/Widgets Utilities
TBAG_API bool    GuiIsItemHovered(GuiHoveredFlags flags = 0);
TBAG_API bool    GuiIsItemActive();
TBAG_API bool    GuiIsItemFocused();
TBAG_API bool    GuiIsItemClicked(int mouse_button = 0);
TBAG_API bool    GuiIsItemVisible();
TBAG_API bool    GuiIsItemEdited();
TBAG_API bool    GuiIsItemActivated();
TBAG_API bool    GuiIsItemDeactivated();
TBAG_API bool    GuiIsItemDeactivatedAfterEdit();
TBAG_API bool    GuiIsAnyItemHovered();
TBAG_API bool    GuiIsAnyItemActive();
TBAG_API bool    GuiIsAnyItemFocused();
TBAG_API Vector2 GuiGetItemRectMin();
TBAG_API Vector2 GuiGetItemRectMax();
TBAG_API Vector2 GuiGetItemRectSize();
TBAG_API void    GuiSetItemAllowOverlap();

// Miscellaneous Utilities
TBAG_API bool         GuiIsRectVisible(Vector2 const & size);
TBAG_API bool         GuiIsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max);
TBAG_API double       GuiGetTime();
TBAG_API int          GuiGetFrameCount();
TBAG_API char const * GuiGetStyleColorName(GuiCol idx);
TBAG_API Vector2      GuiCalcTextSize(char const * text, char const * text_end = nullptr,
                                      bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
TBAG_API void         GuiCalcListClipping(int items_count, float items_height, int * out_items_display_start,
                                          int * out_items_display_end);
TBAG_API bool         GuiBeginChildFrame(GuiID id, Vector2 const & size, GuiWindowFlags flags = 0);
TBAG_API void         GuiEndChildFrame();

// Color Utilities
TBAG_API Vector4      GuiColorConvertU32ToFloat4(unsigned int in);
TBAG_API unsigned int GuiColorConvertFloat4ToU32(Vector4 const & in);
TBAG_API void         GuiColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v);
TBAG_API void         GuiColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b);

// Inputs Utilities
TBAG_API int            GuiGetKeyIndex(GuiKey imgui_key);
TBAG_API bool           GuiIsKeyDown(int user_key_index);
TBAG_API bool           GuiIsKeyPressed(int user_key_index, bool repeat = true);
TBAG_API bool           GuiIsKeyReleased(int user_key_index);
TBAG_API int            GuiGetKeyPressedAmount(int key_index, float repeat_delay, float rate);
TBAG_API bool           GuiIsMouseDown(int button);
TBAG_API bool           GuiIsAnyMouseDown();
TBAG_API bool           GuiIsMouseClicked(int button, bool repeat = false);
TBAG_API bool           GuiIsMouseDoubleClicked(int button);
TBAG_API bool           GuiIsMouseReleased(int button);
TBAG_API bool           GuiIsMouseDragging(int button = 0, float lock_threshold = -1.0f);
TBAG_API bool           GuiIsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip = true);
TBAG_API bool           GuiIsMousePosValid(Vector2 const * mouse_pos = nullptr);
TBAG_API Vector2        GuiGetMousePos();
TBAG_API Vector2        GuiGetMousePosOnOpeningCurrentPopup();
TBAG_API Vector2        GuiGetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);
TBAG_API void           GuiResetMouseDragDelta(int button = 0);
TBAG_API GuiMouseCursor GuiGetMouseCursor();
TBAG_API void           GuiSetMouseCursor(GuiMouseCursor type);
TBAG_API void           GuiCaptureKeyboardFromApp(bool want_capture_keyboard_value = true);
TBAG_API void           GuiCaptureMouseFromApp(bool want_capture_mouse_value = true);

// Clipboard Utilities
TBAG_API char const * GuiGetClipboardText();
TBAG_API void         GuiSetClipboardText(char const * text);

// Settings/.Ini Utilities
TBAG_API void         GuiLoadIniSettingsFromDisk(char const * ini_filename);
TBAG_API void         GuiLoadIniSettingsFromMemory(char const * ini_data, size_t ini_size = 0);
TBAG_API void         GuiSaveIniSettingsToDisk(char const * ini_filename);
TBAG_API char const * GuiSaveIniSettingsToMemory(size_t * out_ini_size = nullptr);

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYGUI_HPP__

