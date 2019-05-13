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
    ShowDemoWindow();
    return 0;
}

static int _GuiBegin(lua_State * L)
{
    lua_pushboolean(L, Begin(luaL_checkstring(L, 1)));
    return 1;
}

static int _GuiEnd(lua_State * L)
{
    End();
    return 0;
}

static int _GuiText(lua_State * L)
{
    Text(luaL_checkstring(L, 1));
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

