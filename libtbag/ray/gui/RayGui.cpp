/**
 * @file   RayGui.cpp
 * @brief  RayGui class implementation.
 * @author zer0
 * @date   2019-05-06
 * @date   2019-05-13 (Move: libtbag/ray -> libtbag/ray/gui)
 */

#include <libtbag/ray/gui/RayGui.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Type.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <cstdlib>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {
namespace gui {

struct RayGui final : private Noncopyable
{
    Texture font_texture;

    RayGui()
    {
        // EMPTY.
    }

    ~RayGui()
    {
        // EMPTY.
    }
};

static void __render_ray_gui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool InitRayGui()
{
    ImGuiContext * context = ImGui::CreateContext();
    assert(context != nullptr);

    // User data configuration.
    auto * user = new RayGui();
    assert(user != nullptr);

    ImGuiIO & io = ImGui::GetIO();
    io.UserData = (void*)user;

    // Keyboard input configuration.
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = KEY_HOME;
    io.KeyMap[ImGuiKey_End] = KEY_END;
    io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = KEY_A;
    io.KeyMap[ImGuiKey_C] = KEY_C;
    io.KeyMap[ImGuiKey_V] = KEY_V;
    io.KeyMap[ImGuiKey_X] = KEY_X;
    io.KeyMap[ImGuiKey_Y] = KEY_Y;
    io.KeyMap[ImGuiKey_Z] = KEY_Z;

    // Rendering configuration.
    io.DisplaySize.x = (float)GetScreenWidth();
    io.DisplaySize.y = (float)GetScreenHeight();

    {
        io.Fonts->AddFontDefault();

        unsigned char * pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        auto font_image = LoadImagePro(pixels, width, height, UNCOMPRESSED_R8G8B8A8);
        user->font_texture = LoadTextureFromImage(font_image);
        io.Fonts->TexID = reinterpret_cast<ImTextureID>(user->font_texture.id);
        UnloadImage(font_image);
    }

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    ImGui_ImplOpenGL3_Init();
    SetEndDrawingCallback(&__render_ray_gui);

    return true;
}

void UpdateRayGui()
{
    ImGuiIO & io = ImGui::GetIO();

    io.DeltaTime = 1.0f/60.0f;              // set the time elapsed since the previous frame (in seconds)
    io.DisplaySize.x = 1920.0f;             // set the current display width
    io.DisplaySize.y = 1280.0f;             // set the current display height here

    io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    io.MouseWheel = (float)GetMouseWheelMove();

    // clang-format off
    io.KeysDown[KEY_APOSTROPHE    ] = IsKeyDown(KEY_APOSTROPHE);
    io.KeysDown[KEY_COMMA         ] = IsKeyDown(KEY_COMMA);
    io.KeysDown[KEY_MINUS         ] = IsKeyDown(KEY_MINUS);
    io.KeysDown[KEY_PERIOD        ] = IsKeyDown(KEY_PERIOD);
    io.KeysDown[KEY_SLASH         ] = IsKeyDown(KEY_SLASH);
    io.KeysDown[KEY_ZERO          ] = IsKeyDown(KEY_ZERO);
    io.KeysDown[KEY_ONE           ] = IsKeyDown(KEY_ONE);
    io.KeysDown[KEY_TWO           ] = IsKeyDown(KEY_TWO);
    io.KeysDown[KEY_THREE         ] = IsKeyDown(KEY_THREE);
    io.KeysDown[KEY_FOUR          ] = IsKeyDown(KEY_FOUR);
    io.KeysDown[KEY_FIVE          ] = IsKeyDown(KEY_FIVE);
    io.KeysDown[KEY_SIX           ] = IsKeyDown(KEY_SIX);
    io.KeysDown[KEY_SEVEN         ] = IsKeyDown(KEY_SEVEN);
    io.KeysDown[KEY_EIGHT         ] = IsKeyDown(KEY_EIGHT);
    io.KeysDown[KEY_NINE          ] = IsKeyDown(KEY_NINE);
    io.KeysDown[KEY_SEMICOLON     ] = IsKeyDown(KEY_SEMICOLON);
    io.KeysDown[KEY_EQUAL         ] = IsKeyDown(KEY_EQUAL);
    io.KeysDown[KEY_A             ] = IsKeyDown(KEY_A);
    io.KeysDown[KEY_B             ] = IsKeyDown(KEY_B);
    io.KeysDown[KEY_C             ] = IsKeyDown(KEY_C);
    io.KeysDown[KEY_D             ] = IsKeyDown(KEY_D);
    io.KeysDown[KEY_E             ] = IsKeyDown(KEY_E);
    io.KeysDown[KEY_F             ] = IsKeyDown(KEY_F);
    io.KeysDown[KEY_G             ] = IsKeyDown(KEY_G);
    io.KeysDown[KEY_H             ] = IsKeyDown(KEY_H);
    io.KeysDown[KEY_I             ] = IsKeyDown(KEY_I);
    io.KeysDown[KEY_J             ] = IsKeyDown(KEY_J);
    io.KeysDown[KEY_K             ] = IsKeyDown(KEY_K);
    io.KeysDown[KEY_L             ] = IsKeyDown(KEY_L);
    io.KeysDown[KEY_M             ] = IsKeyDown(KEY_M);
    io.KeysDown[KEY_N             ] = IsKeyDown(KEY_N);
    io.KeysDown[KEY_O             ] = IsKeyDown(KEY_O);
    io.KeysDown[KEY_P             ] = IsKeyDown(KEY_P);
    io.KeysDown[KEY_Q             ] = IsKeyDown(KEY_Q);
    io.KeysDown[KEY_R             ] = IsKeyDown(KEY_R);
    io.KeysDown[KEY_S             ] = IsKeyDown(KEY_S);
    io.KeysDown[KEY_T             ] = IsKeyDown(KEY_T);
    io.KeysDown[KEY_U             ] = IsKeyDown(KEY_U);
    io.KeysDown[KEY_V             ] = IsKeyDown(KEY_V);
    io.KeysDown[KEY_W             ] = IsKeyDown(KEY_W);
    io.KeysDown[KEY_X             ] = IsKeyDown(KEY_X);
    io.KeysDown[KEY_Y             ] = IsKeyDown(KEY_Y);
    io.KeysDown[KEY_Z             ] = IsKeyDown(KEY_Z);
    io.KeysDown[KEY_SPACE         ] = IsKeyDown(KEY_SPACE);
    io.KeysDown[KEY_ESCAPE        ] = IsKeyDown(KEY_ESCAPE);
    io.KeysDown[KEY_ENTER         ] = IsKeyDown(KEY_ENTER);
    io.KeysDown[KEY_TAB           ] = IsKeyDown(KEY_TAB);
    io.KeysDown[KEY_BACKSPACE     ] = IsKeyDown(KEY_BACKSPACE);
    io.KeysDown[KEY_INSERT        ] = IsKeyDown(KEY_INSERT);
    io.KeysDown[KEY_DELETE        ] = IsKeyDown(KEY_DELETE);
    io.KeysDown[KEY_RIGHT         ] = IsKeyDown(KEY_RIGHT);
    io.KeysDown[KEY_LEFT          ] = IsKeyDown(KEY_LEFT);
    io.KeysDown[KEY_DOWN          ] = IsKeyDown(KEY_DOWN);
    io.KeysDown[KEY_UP            ] = IsKeyDown(KEY_UP);
    io.KeysDown[KEY_PAGE_UP       ] = IsKeyDown(KEY_PAGE_UP);
    io.KeysDown[KEY_PAGE_DOWN     ] = IsKeyDown(KEY_PAGE_DOWN);
    io.KeysDown[KEY_HOME          ] = IsKeyDown(KEY_HOME);
    io.KeysDown[KEY_END           ] = IsKeyDown(KEY_END);
    io.KeysDown[KEY_CAPS_LOCK     ] = IsKeyDown(KEY_CAPS_LOCK);
    io.KeysDown[KEY_SCROLL_LOCK   ] = IsKeyDown(KEY_SCROLL_LOCK);
    io.KeysDown[KEY_NUM_LOCK      ] = IsKeyDown(KEY_NUM_LOCK);
    io.KeysDown[KEY_PRINT_SCREEN  ] = IsKeyDown(KEY_PRINT_SCREEN);
    io.KeysDown[KEY_PAUSE         ] = IsKeyDown(KEY_PAUSE);
    io.KeysDown[KEY_F1            ] = IsKeyDown(KEY_F1);
    io.KeysDown[KEY_F2            ] = IsKeyDown(KEY_F2);
    io.KeysDown[KEY_F3            ] = IsKeyDown(KEY_F3);
    io.KeysDown[KEY_F4            ] = IsKeyDown(KEY_F4);
    io.KeysDown[KEY_F5            ] = IsKeyDown(KEY_F5);
    io.KeysDown[KEY_F6            ] = IsKeyDown(KEY_F6);
    io.KeysDown[KEY_F7            ] = IsKeyDown(KEY_F7);
    io.KeysDown[KEY_F8            ] = IsKeyDown(KEY_F8);
    io.KeysDown[KEY_F9            ] = IsKeyDown(KEY_F9);
    io.KeysDown[KEY_F10           ] = IsKeyDown(KEY_F10);
    io.KeysDown[KEY_F11           ] = IsKeyDown(KEY_F11);
    io.KeysDown[KEY_F12           ] = IsKeyDown(KEY_F12);
    io.KeysDown[KEY_LEFT_SHIFT    ] = IsKeyDown(KEY_LEFT_SHIFT);
    io.KeysDown[KEY_LEFT_CONTROL  ] = IsKeyDown(KEY_LEFT_CONTROL);
    io.KeysDown[KEY_LEFT_ALT      ] = IsKeyDown(KEY_LEFT_ALT);
    io.KeysDown[KEY_LEFT_SUPER    ] = IsKeyDown(KEY_LEFT_SUPER);
    io.KeysDown[KEY_RIGHT_SHIFT   ] = IsKeyDown(KEY_RIGHT_SHIFT);
    io.KeysDown[KEY_RIGHT_CONTROL ] = IsKeyDown(KEY_RIGHT_CONTROL);
    io.KeysDown[KEY_RIGHT_ALT     ] = IsKeyDown(KEY_RIGHT_ALT);
    io.KeysDown[KEY_RIGHT_SUPER   ] = IsKeyDown(KEY_RIGHT_SUPER);
    io.KeysDown[KEY_KB_MENU       ] = IsKeyDown(KEY_KB_MENU);
    io.KeysDown[KEY_LEFT_BRACKET  ] = IsKeyDown(KEY_LEFT_BRACKET);
    io.KeysDown[KEY_BACKSLASH     ] = IsKeyDown(KEY_BACKSLASH);
    io.KeysDown[KEY_RIGHT_BRACKET ] = IsKeyDown(KEY_RIGHT_BRACKET);
    io.KeysDown[KEY_GRAVE         ] = IsKeyDown(KEY_GRAVE);
    io.KeysDown[KEY_KP_0          ] = IsKeyDown(KEY_KP_0);
    io.KeysDown[KEY_KP_1          ] = IsKeyDown(KEY_KP_1);
    io.KeysDown[KEY_KP_2          ] = IsKeyDown(KEY_KP_2);
    io.KeysDown[KEY_KP_3          ] = IsKeyDown(KEY_KP_3);
    io.KeysDown[KEY_KP_4          ] = IsKeyDown(KEY_KP_4);
    io.KeysDown[KEY_KP_5          ] = IsKeyDown(KEY_KP_5);
    io.KeysDown[KEY_KP_6          ] = IsKeyDown(KEY_KP_6);
    io.KeysDown[KEY_KP_7          ] = IsKeyDown(KEY_KP_7);
    io.KeysDown[KEY_KP_8          ] = IsKeyDown(KEY_KP_8);
    io.KeysDown[KEY_KP_9          ] = IsKeyDown(KEY_KP_9);
    io.KeysDown[KEY_KP_DECIMAL    ] = IsKeyDown(KEY_KP_DECIMAL);
    io.KeysDown[KEY_KP_DIVIDE     ] = IsKeyDown(KEY_KP_DIVIDE);
    io.KeysDown[KEY_KP_MULTIPLY   ] = IsKeyDown(KEY_KP_MULTIPLY);
    io.KeysDown[KEY_KP_SUBTRACT   ] = IsKeyDown(KEY_KP_SUBTRACT);
    io.KeysDown[KEY_KP_ADD        ] = IsKeyDown(KEY_KP_ADD);
    io.KeysDown[KEY_KP_ENTER      ] = IsKeyDown(KEY_KP_ENTER);
    io.KeysDown[KEY_KP_EQUAL      ] = IsKeyDown(KEY_KP_EQUAL);
    io.KeyShift = (io.KeysDown[KEY_LEFT_SHIFT  ] || io.KeysDown[KEY_RIGHT_SHIFT  ]);
    io.KeyCtrl  = (io.KeysDown[KEY_LEFT_CONTROL] || io.KeysDown[KEY_RIGHT_CONTROL]);
    io.KeyAlt   = (io.KeysDown[KEY_LEFT_ALT    ] || io.KeysDown[KEY_RIGHT_ALT    ]);
    io.KeySuper = (io.KeysDown[KEY_LEFT_SUPER  ] || io.KeysDown[KEY_RIGHT_SUPER  ]);
    // clang-format on

    int key = GetKeyPressed();
    if ((key >= 32) && (key <= 125)) {
        io.AddInputCharacter(static_cast<ImWchar>(key));
    }

    io.DeltaTime = GetFrameTime();
    io.DisplaySize.x = (float)GetScreenWidth();
    io.DisplaySize.y = (float)GetScreenHeight();

    if (io.WantSetMousePos) {
        SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
    } else {
        io.MousePos = GetMousePosition();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void ShutdownRayGui()
{
    ImGuiIO & io = ImGui::GetIO();

    auto * user = (RayGui*)io.UserData;
    assert(user != nullptr);
    UnloadTexture(user->font_texture);
    delete user;

    io.UserData = nullptr;
    io.Fonts->TexID = nullptr;

    SetEndDrawingCallback(nullptr);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

#ifndef STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI
#define STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(x) \
    STATIC_ASSERT_INTEGER_EQUAL(x, ImGui##x)
#endif

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Text);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TextDisabled);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_WindowBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ChildBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_PopupBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Border);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_BorderShadow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_FrameBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_FrameBgHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_FrameBgActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TitleBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TitleBgActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TitleBgCollapsed);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_MenuBarBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ScrollbarBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ScrollbarGrab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ScrollbarGrabHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ScrollbarGrabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_CheckMark);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_SliderGrab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_SliderGrabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Button);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ButtonHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ButtonActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Header);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_HeaderHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_HeaderActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Separator);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_SeparatorHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_SeparatorActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ResizeGrip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ResizeGripHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ResizeGripActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_Tab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TabHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TabUnfocused);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TabUnfocusedActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_PlotLines);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_PlotLinesHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_PlotHistogram);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_PlotHistogramHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_TextSelectedBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_DragDropTarget);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_NavHighlight);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_NavWindowingHighlight);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_NavWindowingDimBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_ModalWindowDimBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Col_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoTitleBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoResize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoMove);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoScrollWithMouse);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoCollapse);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_AlwaysAutoResize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoBackground);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoSavedSettings);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoMouseInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_MenuBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_HorizontalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoFocusOnAppearing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoBringToFrontOnFocus);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_AlwaysVerticalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_AlwaysHorizontalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_AlwaysUseWindowPadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoNavInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoNavFocus);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_UnsavedDocument);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoNav);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoDecoration);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(WindowFlags_NoInputs);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(FocusedFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(FocusedFlags_ChildWindows);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(FocusedFlags_RootWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(FocusedFlags_AnyWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(FocusedFlags_RootAndChildWindows);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_ChildWindows);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_RootWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AnyWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AllowWhenBlockedByPopup);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AllowWhenBlockedByModal);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AllowWhenBlockedByActiveItem);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AllowWhenOverlapped);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_AllowWhenDisabled);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_RectOnly);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(HoveredFlags_RootAndChildWindows);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Cond_Always);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Cond_Once);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Cond_FirstUseEver);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Cond_Appearing);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_Alpha);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_WindowPadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_WindowRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_WindowBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_WindowMinSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_WindowTitleAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ChildRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ChildBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_PopupRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_PopupBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_FramePadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_FrameRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_FrameBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ItemSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ItemInnerSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_IndentSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ScrollbarSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ScrollbarRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_GrabMinSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_GrabRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_TabRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_ButtonTextAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_SelectableTextAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(StyleVar_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_Left);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_Right);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_Up);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_Down);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Dir_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_PopupAlignLeft);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_HeightSmall);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_HeightRegular);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_HeightLarge);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_HeightLargest);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_NoArrowButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_NoPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ComboFlags_HeightMask_);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_S8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_U8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_S16);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_U16);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_S32);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_U32);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_S64);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_U64);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_Float);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_Double);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DataType_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoAlpha);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoPicker);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoOptions);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoSmallPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoLabel);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoSidePreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_NoDragDrop);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_AlphaBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_AlphaPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_AlphaPreviewHalf);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_HDR);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_DisplayRGB);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_DisplayHSV);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_DisplayHex);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_Uint8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_Float);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_PickerHueBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_PickerHueWheel);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_InputRGB);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags_InputHSV);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(ColorEditFlags__OptionsDefault);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_Selected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_Framed);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_AllowItemOverlap);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_NoTreePushOnOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_NoAutoOpenOnLog);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_DefaultOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_OpenOnDoubleClick);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_OpenOnArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_Leaf);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_Bullet);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_FramePadding);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_SpanAllAvailWidth);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_NoScrollOnOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_NavLeftJumpsBackHere);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TreeNodeFlags_CollapsingHeader);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(SelectableFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(SelectableFlags_DontClosePopups);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(SelectableFlags_SpanAllColumns);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(SelectableFlags_AllowDoubleClick);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(SelectableFlags_Disabled);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_Reorderable);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_AutoSelectNewTabs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_TabListPopupButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_NoCloseWithMiddleMouseButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_NoTabListScrollingButtons);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_NoTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_FittingPolicyResizeDown);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_FittingPolicyScroll);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_FittingPolicyMask_);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabBarFlags_FittingPolicyDefault_);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabItemFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabItemFlags_UnsavedDocument);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabItemFlags_SetSelected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabItemFlags_NoCloseWithMiddleMouseButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(TabItemFlags_NoPushId);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceNoPreviewTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceNoDisableHover);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceNoHoldToOpenOthers);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceAllowNullID);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceExtern);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_SourceAutoExpirePayload);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_AcceptBeforeDelivery);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_AcceptNoDrawDefaultRect);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_AcceptNoPreviewTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(DragDropFlags_AcceptPeekOnly);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Tab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_LeftArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_RightArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_UpArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_DownArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_PageUp);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_PageDown);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Home);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_End);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Insert);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Delete);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Backspace);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Space);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Enter);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Escape);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_A);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_C);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_V);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_X);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Y);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_Z);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(Key_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_Arrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_TextInput);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_ResizeAll);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_ResizeNS);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_ResizeEW);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_ResizeNESW);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_ResizeNWSE);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_Hand);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(MouseCursor_COUNT);

template <typename T1, typename T2>
static void __imgui_style_copy(T1 & dest, T2 const & src)
{
    dest.Alpha                  = src.Alpha;
    dest.WindowPadding          = src.WindowPadding;
    dest.WindowRounding         = src.WindowRounding;
    dest.WindowBorderSize       = src.WindowBorderSize;
    dest.WindowMinSize          = src.WindowMinSize;
    dest.WindowTitleAlign       = src.WindowTitleAlign;
    dest.ChildRounding          = src.ChildRounding;
    dest.ChildBorderSize        = src.ChildBorderSize;
    dest.PopupRounding          = src.PopupRounding;
    dest.PopupBorderSize        = src.PopupBorderSize;
    dest.FramePadding           = src.FramePadding;
    dest.FrameRounding          = src.FrameRounding;
    dest.FrameBorderSize        = src.FrameBorderSize;
    dest.ItemSpacing            = src.ItemSpacing;
    dest.ItemInnerSpacing       = src.ItemInnerSpacing;
    dest.TouchExtraPadding      = src.TouchExtraPadding;
    dest.IndentSpacing          = src.IndentSpacing;
    dest.ColumnsMinSpacing      = src.ColumnsMinSpacing;
    dest.ScrollbarSize          = src.ScrollbarSize;
    dest.ScrollbarRounding      = src.ScrollbarRounding;
    dest.GrabMinSize            = src.GrabMinSize;
    dest.GrabRounding           = src.GrabRounding;
    dest.TabRounding            = src.TabRounding;
    dest.TabBorderSize          = src.TabBorderSize;
    dest.ButtonTextAlign        = src.ButtonTextAlign;
    dest.SelectableTextAlign    = src.SelectableTextAlign;
    dest.DisplayWindowPadding   = src.DisplayWindowPadding;
    dest.DisplaySafeAreaPadding = src.DisplaySafeAreaPadding;
    dest.MouseCursorScale       = src.MouseCursorScale;
    dest.AntiAliasedLines       = src.AntiAliasedLines;
    dest.AntiAliasedFill        = src.AntiAliasedFill;
    dest.CurveTessellationTol   = src.CurveTessellationTol;
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        dest.Colors[i] = src.Colors[i];
    }
}

// clang-format off
static void __style_copy(Style & dest, ImGuiStyle const & src) { __imgui_style_copy(dest, src); }
static void __style_copy(ImGuiStyle & dest, Style const & src) { __imgui_style_copy(dest, src); }
// clang-format on

template <typename T1, typename T2>
static void __imgui_payload_copy(T1 & dest, T2 const & src)
{
    dest.Data           = src.Data;
    dest.DataSize       = src.DataSize;
    dest.SourceId       = src.SourceId;
    dest.SourceParentId = src.SourceParentId;
    dest.DataFrameCount = src.DataFrameCount;
    for (int i = 0; i < 32+1; ++i) {
        dest.DataType[i] = src.DataType[i];
    }
    dest.Preview  = src.Preview;
    dest.Delivery = src.Delivery;
}

// clang-format off
static void __payload_copy(Payload & dest, ImGuiPayload const & src) { __imgui_payload_copy(dest, src); }
static void __payload_copy(ImGuiPayload & dest, Payload const & src) { __imgui_payload_copy(dest, src); }
// clang-format on

void ShowDemoWindow(bool * p_open)
{
    ImGui::ShowDemoWindow(p_open);
}

void ShowAboutWindow(bool * p_open)
{
    ImGui::ShowAboutWindow(p_open);
}

void ShowMetricsWindow(bool * p_open)
{
    ImGui::ShowMetricsWindow(p_open);
}

void ShowStyleEditor()
{
    ImGui::ShowStyleEditor();
}

bool ShowStyleSelector(const char * label)
{
    return ImGui::ShowStyleSelector(label);
}

void ShowFontSelector(const char * label)
{
    ImGui::ShowFontSelector(label);
}

void ShowUserGuide()
{
    ImGui::ShowUserGuide();
}

const char * GetVersion()
{
    return ImGui::GetVersion();
}

void StyleColorsDark()
{
    ImGui::StyleColorsDark();
}

void StyleColorsClassic()
{
    ImGui::StyleColorsClassic();
}

void StyleColorsLight()
{
    ImGui::StyleColorsLight();
}

Style GetStyle()
{
    Style result = {0,};
    __style_copy(result, ImGui::GetStyle());
    return result;
}

void SetStyle(Style const & style)
{
    __style_copy(ImGui::GetStyle(), style);
}

bool Begin(char const * text, bool * p_open, int flags)
{
    return ImGui::Begin(text, p_open, flags);
}

void End()
{
    ImGui::End();
}

bool BeginChild(char const * str_id, Vector2 const & size, bool border, WindowFlags flags)
{
    return ImGui::BeginChild(str_id, size, border, flags);
}

bool BeginChild(ID id, Vector2 const & size, bool border, WindowFlags flags)
{
    return ImGui::BeginChild(id, size, border, flags);
}

void EndChild()
{
    ImGui::EndChild();
}

bool IsWindowAppearing()
{
    return ImGui::IsWindowAppearing();
}

bool IsWindowCollapsed()
{
    return ImGui::IsWindowCollapsed();
}

bool IsWindowFocused(FocusedFlags flags)
{
    return ImGui::IsWindowFocused(flags);
}

bool IsWindowHovered(HoveredFlags flags)
{
    return ImGui::IsWindowHovered(flags);
}

Vector2 GetWindowPos()
{
    return ImGui::GetWindowPos();
}

Vector2 GetWindowSize()
{
    return ImGui::GetWindowSize();
}

float GetWindowWidth()
{
    return ImGui::GetWindowWidth();
}

float GetWindowHeight()
{
    return ImGui::GetWindowHeight();
}

void SetNextWindowPos(Vector2 const & pos, Cond cond, Vector2 const & pivot)
{
    ImGui::SetNextWindowPos(pos, cond, pivot);
}

void SetNextWindowSize(Vector2 const & size, Cond cond)
{
    ImGui::SetNextWindowSize(size, cond);
}

void SetNextWindowContentSize(Vector2 const & size)
{
    ImGui::SetNextWindowContentSize(size);
}

void SetNextWindowCollapsed(bool collapsed, Cond cond)
{
    ImGui::SetNextWindowCollapsed(collapsed, cond);
}

void SetNextWindowFocus()
{
    ImGui::SetNextWindowFocus();
}

void SetNextWindowBgAlpha(float alpha)
{
    ImGui::SetNextWindowBgAlpha(alpha);
}

void SetWindowPos(Vector2 const & pos, Cond cond)
{
    ImGui::SetWindowPos(pos, cond);
}

void SetWindowSize(Vector2 const & size, Cond cond)
{
    ImGui::SetWindowSize(size, cond);
}

void SetWindowCollapsed(bool collapsed, Cond cond)
{
    ImGui::SetWindowCollapsed(collapsed, cond);
}

void SetWindowFocus()
{
    ImGui::SetWindowFocus();
}

void SetWindowFontScale(float scale)
{
    ImGui::SetWindowFontScale(scale);
}

void SetWindowPos(char const * name, Vector2 const & pos, Cond cond)
{
    ImGui::SetWindowPos(name, pos, cond);
}

void SetWindowSize(char const * name, Vector2 const & size, Cond cond)
{
    ImGui::SetWindowSize(name, size, cond);
}

void SetWindowCollapsed(char const * name, bool collapsed, Cond cond)
{
    ImGui::SetWindowCollapsed(name, collapsed, cond);
}

void SetWindowFocus(char const * name)
{
    ImGui::SetWindowFocus();
}

Vector2 GetContentRegionMax()
{
    return ImGui::GetContentRegionMax();
}

Vector2 GetContentRegionAvail()
{
    return ImGui::GetContentRegionAvail();
}

Vector2 GetWindowContentRegionMin()
{
    return ImGui::GetWindowContentRegionMin();
}

Vector2 GetWindowContentRegionMax()
{
    return ImGui::GetWindowContentRegionMax();
}

float GetWindowContentRegionWidth()
{
    return ImGui::GetWindowContentRegionWidth();
}

float GetScrollX()
{
    return ImGui::GetScrollX();
}

float GetScrollY()
{
    return ImGui::GetScrollY();
}

float GetScrollMaxX()
{
    return ImGui::GetScrollMaxX();
}

float GetScrollMaxY()
{
    return ImGui::GetScrollMaxY();
}

void SetScrollX(float scroll_x)
{
    ImGui::SetScrollX(scroll_x);
}

void SetScrollY(float scroll_y)
{
    ImGui::SetScrollY(scroll_y);
}

void SetScrollHereY(float center_y_ratio)
{
    ImGui::SetScrollHereY(center_y_ratio);
}

void SetScrollFromPosY(float local_y, float center_y_ratio)
{
    ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

void PushStyleColor(Col idx, unsigned int col)
{
    ImGui::PushStyleColor(idx, col);
}

void PushStyleColor(Col idx, Vector4 const & col)
{
    ImGui::PushStyleColor(idx, col);
}

void PopStyleColor(int count)
{
    ImGui::PopStyleColor(count);
}

void PushStyleVar(StyleVar idx, float val)
{
    ImGui::PushStyleVar(idx, val);
}

void PushStyleVar(StyleVar idx, Vector2 const & val)
{
    ImGui::PushStyleVar(idx, val);
}

void PopStyleVar(int count)
{
    ImGui::PopStyleVar(count);
}

Vector4 GetStyleColorVec4(Col idx)
{
    return ImGui::GetStyleColorVec4(idx);
}

float GetFontSize()
{
    return ImGui::GetFontSize();
}

Vector2 GetFontTexUvWhitePixel()
{
    return ImGui::GetFontTexUvWhitePixel();
}

unsigned int GetColorU32(Col idx, float alpha_mul)
{
    return ImGui::GetColorU32(idx, alpha_mul);
}

unsigned int GetColorU32(Vector4 const & col)
{
    return ImGui::GetColorU32(col);
}

unsigned int GetColorU32(unsigned int col)
{
    return ImGui::GetColorU32(col);
}

void PushItemWidth(float item_width)
{
    ImGui::PushItemWidth(item_width);
}

void PopItemWidth()
{
    ImGui::PopItemWidth();
}

void SetNextItemWidth(float item_width)
{
    ImGui::SetNextItemWidth(item_width);
}

float CalcItemWidth()
{
    return ImGui::CalcItemWidth();
}

void PushTextWrapPos(float wrap_local_pos_x)
{
    ImGui::PushTextWrapPos(wrap_local_pos_x);
}

void PopTextWrapPos()
{
    ImGui::PopTextWrapPos();
}

void PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
    ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
}

void PopAllowKeyboardFocus()
{
    ImGui::PopAllowKeyboardFocus();
}

void PushButtonRepeat(bool repeat)
{
    ImGui::PushButtonRepeat(repeat);
}

void PopButtonRepeat()
{
    ImGui::PopButtonRepeat();
}

void Separator()
{
    ImGui::Separator();
}

void SameLine(float offset_from_start_x, float spacing)
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

void NewLine()
{
    ImGui::NewLine();
}

void Spacing()
{
    ImGui::Spacing();
}

void Dummy(Vector2 const & size)
{
    ImGui::Dummy(size);
}

void Indent(float indent_w)
{
    ImGui::Indent(indent_w);
}

void Unindent(float indent_w)
{
    ImGui::Unindent(indent_w);
}

void BeginGroup()
{
    ImGui::BeginGroup();
}

void EndGroup()
{
    ImGui::EndGroup();
}

Vector2 GetCursorPos()
{
    return ImGui::GetCursorPos();
}

float GetCursorPosX()
{
    return ImGui::GetCursorPosX();
}

float GetCursorPosY()
{
    return ImGui::GetCursorPosY();
}

void SetCursorPos(Vector2 const & local_pos)
{
    ImGui::SetCursorPos(local_pos);
}

void SetCursorPosX(float local_x)
{
    ImGui::SetCursorPosX(local_x);
}

void SetCursorPosY(float local_y)
{
    ImGui::SetCursorPosY(local_y);
}

Vector2 GetCursorStartPos()
{
    return ImGui::GetCursorStartPos();
}

Vector2 GetCursorScreenPos()
{
    return ImGui::GetCursorScreenPos();
}

void SetCursorScreenPos(Vector2 const & pos)
{
    ImGui::SetCursorScreenPos(pos);
}

void AlignTextToFramePadding()
{
    ImGui::AlignTextToFramePadding();
}

float GetTextLineHeight()
{
    return ImGui::GetTextLineHeight();
}

float GetTextLineHeightWithSpacing()
{
    return ImGui::GetTextLineHeightWithSpacing();
}

float GetFrameHeight()
{
    return ImGui::GetFrameHeight();
}

float GetFrameHeightWithSpacing()
{
    return ImGui::GetFrameHeightWithSpacing();
}

void PushID(char const * str_id)
{
    ImGui::PushID(str_id);
}

void PushID(char const * str_id_begin, char const * str_id_end)
{
    ImGui::PushID(str_id_begin, str_id_end);
}

void PushID(void const * ptr_id)
{
    ImGui::PushID(ptr_id);
}

void PushID(int int_id)
{
    ImGui::PushID(int_id);
}

void PopID()
{
    ImGui::PopID();
}

ID GetID(char const * str_id)
{
    return ImGui::GetID(str_id);
}

ID GetID(char const * str_id_begin, char const * str_id_end)
{
    return ImGui::GetID(str_id_begin, str_id_end);
}

ID GetID(void const * ptr_id)
{
    return ImGui::GetID(ptr_id);
}

void TextUnformatted(char const * text, char const * text_end)
{
    ImGui::TextUnformatted(text, text_end);
}

void Text(char const * text)
{
    ImGui::Text("%s", text);
}

void TextColored(Vector4 const & col, char const * text)
{
    ImGui::TextColored(col, "%s", text);
}

void TextDisabled(char const * text)
{
    return ImGui::TextDisabled("%s", text);
}

void TextWrapped(char const * text)
{
    ImGui::TextWrapped("%s", text);
}

void LabelText(char const * label, char const * text)
{
    ImGui::LabelText(label, "%s", text);
}

void BulletText(char const * text)
{
    ImGui::BulletText("%s", text);
}

bool Button(char const * label, Vector2 const & size)
{
    return ImGui::Button(label, size);
}

bool SmallButton(char const * label)
{
    return ImGui::SmallButton(label);
}

bool InvisibleButton(char const * str_id, Vector2 const & size)
{
    return ImGui::InvisibleButton(str_id, size);
}

bool ArrowButton(char const * str_id, Dir dir)
{
    return ImGui::ArrowButton(str_id, dir);
}

void Image(TextureID user_texture_id, Vector2 const & size,
           Vector2 const & uv0, Vector2 const & uv1,
           Vector4 const & tint_col, Vector4 const & border_col)
{
    ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
}

bool ImageButton(TextureID user_texture_id, Vector2 const & size,
                 Vector2 const & uv0, Vector2 const & uv1, int frame_padding,
                 Vector4 const & bg_col, Vector4 const & tint_col)
{
    return ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding, bg_col, tint_col);
}

bool Checkbox(char const * label, bool * v)
{
    return ImGui::Checkbox(label, v);
}

bool CheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value)
{
    return ImGui::CheckboxFlags(label, flags, flags_value);
}

bool RadioButton(char const * label, bool active)
{
    return ImGui::RadioButton(label, active);
}

bool RadioButton(char const * label, int * v, int v_button)
{
    return ImGui::RadioButton(label, v, v_button);
}

void ProgressBar(float fraction, Vector2 const & size_arg, char const * overlay)
{
    ImGui::ProgressBar(fraction, size_arg, overlay);
}

void Bullet()
{
    ImGui::Bullet();
}

bool BeginCombo(char const * label, char const * preview_value, ComboFlags flags)
{
    return ImGui::BeginCombo(label, preview_value, flags);
}

void EndCombo()
{
    ImGui::EndCombo();
}

bool Combo(char const * label, int * current_item, char const * const items[],
           int items_count, int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items);
}

bool Combo(char const * label, int * current_item, char const * items_separated_by_zeros,
           int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}

bool Combo(char const * label, int * current_item,
           bool(*items_getter)(void * data, int idx, char const ** out_text),
           void * data, int items_count, int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items_getter, data, items_count, popup_max_height_in_items);
}

bool DragFloat(char const * label, float * v, float v_speed, float v_min, float v_max,
               char const * format, float power)
{
    return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, power);
}

bool DragFloat2(char const * label, float v[2], float v_speed, float v_min, float v_max,
                char const * format, float power)
{
    return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, power);
}

bool DragFloat3(char const * label, float v[3], float v_speed, float v_min, float v_max,
                char const * format, float power)
{
    return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, power);
}

bool DragFloat4(char const * label, float v[4], float v_speed, float v_min, float v_max,
                char const * format, float power)
{
    return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, power);
}

bool DragFloatRange2(char const * label, float * v_current_min, float * v_current_max, float v_speed,
                     float v_min, float v_max, char const * format, char const * format_max, float power)
{
    return ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed,
                                  v_min, v_max, format, format_max, power);
}

bool DragInt(char const * label, int * v, float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt(label, v, v_speed, v_min, v_max, format);
}

bool DragInt2(char const * label, int v[2], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format);
}

bool DragInt3(char const * label, int v[3], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format);
}

bool DragInt4(char const * label, int v[4], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format);
}

bool DragIntRange2(char const * label, int * v_current_min, int * v_current_max,
                   float v_speed, int v_min, int v_max, char const * format, char const * format_max)
{
    return ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max);
}

bool DragScalar(char const * label, DataType data_type, void * v, float v_speed, const void * v_min,
                const void * v_max, char const * format, float power)
{
    return ImGui::DragScalar(label, data_type, v, v_speed, v_min, v_max, format, power);
}

bool DragScalarN(char const * label, DataType data_type, void * v, int components, float v_speed,
                 const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::DragScalarN(label, data_type, v, components, v_speed, v_min, v_max, format, power);
}

bool SliderFloat(char const * label, float * v, float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat(label, v, v_min, v_max, format, power);
}

bool SliderFloat2(char const * label, float v[2], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat2(label, v, v_min, v_max, format, power);
}

bool SliderFloat3(char const * label, float v[3], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, power);
}

bool SliderFloat4(char const * label, float v[4], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat4(label, v, v_min, v_max, format, power);
}

bool SliderAngle(char const * label, float * v_rad, float v_degrees_min, float v_degrees_max, char const * format)
{
    return ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format);
}

bool SliderInt(char const * label, int * v, int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt(label, v, v_min, v_max, format);
}

bool SliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt2(label, v, v_min, v_max, format);
}

bool SliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt3(label, v, v_min, v_max, format);
}

bool SliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt4(label, v, v_min, v_max, format);
}

bool SliderScalar(char const * label, DataType data_type, void * v, void const * v_min,
                  void const * v_max, char const * format, float power)
{
    return ImGui::SliderScalar(label, data_type, v, v_min, v_max, format, power);
}

bool SliderScalarN(char const * label, DataType data_type, void * v, int components,
                   const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::SliderScalarN(label, data_type, v, components, v_min, v_max, format, power);
}

bool VSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max,
                  char const * format, float power)
{
    return ImGui::VSliderFloat(label, size, v, v_min, v_max, format, power);
}

bool VSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max, char const * format)
{
    return ImGui::VSliderInt(label, size, v, v_min, v_max, format);
}

bool VSliderScalar(char const * label, Vector2 const & size, DataType data_type, void * v,
                   const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::VSliderScalar(label, size, data_type, v, v_min, v_max, format, power);
}

bool InputText(char const * label, char * buf, size_t buf_size, InputTextFlags flags)
{
    return ImGui::InputText(label, buf, buf_size, flags);
}

bool InputTextMultiline(char const * label, char * buf, size_t buf_size, Vector2 const & size, InputTextFlags flags)
{
    return ImGui::InputTextMultiline(label, buf, buf_size, size, flags);
}

bool InputTextWithHint(char const * label, char const * hint, char * buf, size_t buf_size, InputTextFlags flags)
{
    return ImGui::InputTextWithHint(label, hint, buf, buf_size, flags);
}

bool InputFloat(char const * label, float * v, float step, float step_fast, char const * format, InputTextFlags flags)
{
    return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}

bool InputFloat2(char const * label, float v[2], char const * format, InputTextFlags flags)
{
    return ImGui::InputFloat2(label, v, format, flags);
}

bool InputFloat3(char const * label, float v[3], char const * format, InputTextFlags flags)
{
    return ImGui::InputFloat3(label, v, format, flags);
}

bool InputFloat4(char const * label, float v[4], char const * format, InputTextFlags flags)
{
    return ImGui::InputFloat4(label, v, format, flags);
}

bool InputInt(char const * label, int * v, int step, int step_fast, InputTextFlags flags)
{
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

bool InputInt2(char const * label, int v[2], InputTextFlags flags)
{
    return ImGui::InputInt2(label, v, flags);
}

bool InputInt3(char const * label, int v[3], InputTextFlags flags)
{
    return ImGui::InputInt3(label, v, flags);
}

bool InputInt4(char const * label, int v[4], InputTextFlags flags)
{
    return ImGui::InputInt4(label, v, flags);
}

bool InputDouble(char const * label, double * v, double step, double step_fast,
                 char const * format, InputTextFlags flags)
{
    return ImGui::InputDouble(label, v, step, step_fast, format, flags);
}

bool InputScalar(char const * label, DataType data_type, void * v, void const * step,
                 void const * step_fast, char const * format, InputTextFlags flags)
{
    return ImGui::InputScalar(label, data_type, v, step, step_fast, format, flags);
}

bool InputScalarN(char const * label, DataType data_type, void * v, int components, void const * step,
                  void const * step_fast, char const * format, InputTextFlags flags)
{
    return ImGui::InputScalarN(label, data_type, v, components, step, step_fast, format, flags);
}

bool ColorEdit3(char const * label, float col[3], ColorEditFlags flags)
{
    return ImGui::ColorEdit3(label, col, flags);
}

bool ColorEdit4(char const * label, float col[4], ColorEditFlags flags)
{
    return ImGui::ColorEdit4(label, col, flags);
}

bool ColorPicker3(char const * label, float col[3], ColorEditFlags flags)
{
    return ImGui::ColorPicker3(label, col, flags);
}

bool ColorPicker4(char const * label, float col[4], ColorEditFlags flags, float const * ref_col)
{
    return ImGui::ColorPicker4(label, col, flags, ref_col);
}

bool ColorButton(char const * desc_id, Vector4 const & col, ColorEditFlags flags, Vector2 size)
{
    return ImGui::ColorButton(desc_id, col, flags, size);
}

void SetColorEditOptions(ColorEditFlags flags)
{
    ImGui::SetColorEditOptions(flags);
}

bool TreeNode(char const * label)
{
    return ImGui::TreeNode(label);
}

bool TreeNode(char const * str_id, char const * text)
{
    return ImGui::TreeNode(str_id, "%s", text);
}

bool TreeNodeEx(char const * label, TreeNodeFlags flags)
{
    return ImGui::TreeNodeEx(label, flags);
}

bool TreeNodeEx(char const * str_id, TreeNodeFlags flags, char const * text)
{
    return ImGui::TreeNodeEx(str_id, flags, "%s", text);
}

void TreePush(char const * str_id)
{
    ImGui::TreePush(str_id);
}

void TreePush(void const * ptr_id)
{
    ImGui::TreePush(ptr_id);
}

void TreePop()
{
    ImGui::TreePop();
}

void TreeAdvanceToLabelPos()
{
    ImGui::TreeAdvanceToLabelPos();
}

float GetTreeNodeToLabelSpacing()
{
    return ImGui::GetTreeNodeToLabelSpacing();
}

void SetNextTreeNodeOpen(bool is_open, Cond cond)
{
    ImGui::SetNextTreeNodeOpen(is_open, cond);
}

bool CollapsingHeader(char const * label, TreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, flags);
}

bool CollapsingHeader(char const * label, bool * p_open, TreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, p_open, flags);
}

bool Selectable(char const * label, bool selected, SelectableFlags flags, Vector2 const & size)
{
    return ImGui::Selectable(label, selected, flags, size);
}

bool Selectable(char const * label, bool * p_selected, SelectableFlags flags, Vector2 const & size)
{
    return ImGui::Selectable(label, p_selected, flags, size);
}

bool ListBox(char const * label, int * current_item, char const * const items[], int items_count, int height_in_items)
{
    return ImGui::ListBox(label, current_item, items, items_count, height_in_items);
}

bool ListBox(char const * label, int * current_item,
             bool (*items_getter)(void * data, int idx, char const ** out_text),
             void * data, int items_count, int height_in_items)
{
    return ImGui::ListBox(label, current_item, items_getter, data, items_count, height_in_items);
}

bool ListBoxHeader(char const * label, Vector2 const & size)
{
    return ImGui::ListBoxHeader(label, size);
}

bool ListBoxHeader(char const * label, int items_count, int height_in_items)
{
    return ImGui::ListBoxHeader(label, items_count, height_in_items);
}

void ListBoxFooter()
{
    ImGui::ListBoxFooter();
}

void PlotLines(char const * label, float const * values, int values_count, int values_offset,
               char const * overlay_text, float scale_min, float scale_max, Vector2 graph_size, int stride)
{
    ImGui::PlotLines(label, values, values_count, values_offset, overlay_text,
                     scale_min, scale_max, graph_size, stride);
}

void PlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data,
               int values_count, int values_offset, char const * overlay_text,
               float scale_min, float scale_max, Vector2 graph_size)
{
    ImGui::PlotLines(label, values_getter, data, values_count, values_offset,
                     overlay_text, scale_min, scale_max, graph_size);
}

void PlotHistogram(char const * label, float const * values, int values_count, int values_offset,
                   char const * overlay_text, float scale_min, float scale_max,
                   Vector2 graph_size, int stride)
{
    ImGui::PlotHistogram(label, values, values_count, values_offset, overlay_text,
                         scale_min, scale_max, graph_size, stride);
}

void PlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data,
                   int values_count, int values_offset, char const * overlay_text,
                   float scale_min, float scale_max, Vector2 graph_size)
{
    ImGui::PlotHistogram(label, values_getter, data, values_count, values_offset,
                         overlay_text, scale_min, scale_max, graph_size);
}

void Value(char const * prefix, bool b)
{
    ImGui::Value(prefix, b);
}

void Value(char const * prefix, int v)
{
    ImGui::Value(prefix, v);
}

void Value(char const * prefix, unsigned int v)
{
    ImGui::Value(prefix, v);
}

void Value(char const * prefix, float v, char const * float_format)
{
    ImGui::Value(prefix, v, float_format);
}

bool BeginMainMenuBar()
{
    return ImGui::BeginMainMenuBar();
}

void EndMainMenuBar()
{
    ImGui::EndMainMenuBar();
}

bool BeginMenuBar()
{
    return ImGui::BeginMenuBar();
}

void EndMenuBar()
{
    ImGui::EndMenuBar();
}

bool BeginMenu(char const * label, bool enabled)
{
    return ImGui::BeginMenu(label, enabled);
}

void EndMenu()
{
    ImGui::EndMenu();
}

bool MenuItem(char const * label, char const * shortcut, bool selected, bool enabled)
{
    return ImGui::MenuItem(label, shortcut, selected, enabled);
}

bool MenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled)
{
    return ImGui::MenuItem(label, shortcut, p_selected, enabled);
}

void BeginTooltip()
{
    ImGui::BeginTooltip();
}

void EndTooltip()
{
    ImGui::EndTooltip();
}

void SetTooltip(char const * text)
{
    ImGui::SetTooltip("%s", text);
}

void OpenPopup(char const * str_id)
{
    ImGui::OpenPopup(str_id);
}

bool BeginPopup(char const * str_id, WindowFlags flags)
{
    return ImGui::BeginPopup(str_id, flags);
}

bool BeginPopupContextItem(char const * str_idg, int mouse_button)
{
    return ImGui::BeginPopupContextItem(str_idg, mouse_button);
}

bool BeginPopupContextWindow(char const * str_idg, int mouse_button, bool also_over_items)
{
    return ImGui::BeginPopupContextWindow(str_idg, mouse_button, also_over_items);
}

bool BeginPopupContextVoid(char const * str_idg, int mouse_button)
{
    return ImGui::BeginPopupContextVoid(str_idg, mouse_button);
}

bool BeginPopupModal(char const * name, bool * p_openg, WindowFlags flags)
{
    return ImGui::BeginPopupModal(name, p_openg, flags);
}

void EndPopup()
{
    ImGui::EndPopup();
}

bool OpenPopupOnItemClick(char const * str_idg, int mouse_button)
{
    return ImGui::OpenPopupOnItemClick(str_idg, mouse_button);
}

bool IsPopupOpen(char const * str_id)
{
    return ImGui::IsPopupOpen(str_id);
}

void CloseCurrentPopup()
{
    ImGui::CloseCurrentPopup();
}

void Columns(int count, char const * idg, bool border)
{
    ImGui::Columns(count, idg, border);
}

void NextColumn()
{
    ImGui::NextColumn();
}

int GetColumnIndex()
{
    return ImGui::GetColumnIndex();
}

float GetColumnWidth(int column_index)
{
    return ImGui::GetColumnWidth(column_index);
}

void SetColumnWidth(int column_index, float width)
{
    ImGui::SetColumnWidth(column_index, width);
}

float GetColumnOffset(int column_index)
{
    return ImGui::GetColumnOffset(column_index);
}

void SetColumnOffset(int column_index, float offset_x)
{
    ImGui::SetColumnOffset(column_index, offset_x);
}

int GetColumnsCount()
{
    return ImGui::GetColumnsCount();
}

bool BeginTabBar(char const * str_id, TabBarFlags flags)
{
    return ImGui::BeginTabBar(str_id, flags);
}

void EndTabBar()
{
    ImGui::EndTabBar();
}

bool BeginTabItem(char const * label, bool * p_open, TabItemFlags flags)
{
    return ImGui::BeginTabItem(label, p_open, flags);
}

void EndTabItem()
{
    ImGui::EndTabItem();
}

void SetTabItemClosed(char const * tab_or_docked_window_label)
{
    ImGui::SetTabItemClosed(tab_or_docked_window_label);
}

void LogToTTY(int auto_open_depth)
{
    ImGui::LogToTTY(auto_open_depth);
}

void LogToFile(int auto_open_depth, char const * filename)
{
    ImGui::LogToFile(auto_open_depth, filename);
}

void LogToClipboard(int auto_open_depth)
{
    ImGui::LogToClipboard(auto_open_depth);
}

void LogFinish()
{
    ImGui::LogFinish();
}

void LogButtons()
{
    ImGui::LogButtons();
}

void LogText(char const * text)
{
    ImGui::LogText("%s", text);
}

bool BeginDragDropSource(DragDropFlags flags)
{
    return ImGui::BeginDragDropSource(flags);
}

bool SetDragDropPayload(char const * type, void const * data, size_t sz, Cond cond)
{
    return ImGui::SetDragDropPayload(type, data, sz, cond);
}

void EndDragDropSource()
{
    ImGui::EndDragDropSource();
}

bool BeginDragDropTarget()
{
    return ImGui::BeginDragDropTarget();
}

Payload AcceptDragDropPayload(char const * type, DragDropFlags flags)
{
    Payload result = {0,};
    __payload_copy(result, *ImGui::AcceptDragDropPayload(type, flags));
    return result;
}

void EndDragDropTarget()
{
    ImGui::EndDragDropTarget();
}

Payload GetDragDropPayload()
{
    Payload result = {0,};
    __payload_copy(result, *ImGui::GetDragDropPayload());
    return result;
}

void PushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max, bool intersect_with_current_clip_rect)
{
    ImGui::PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
}

void PopClipRect()
{
    ImGui::PopClipRect();
}

void SetItemDefaultFocus()
{
    ImGui::SetItemDefaultFocus();
}

void SetKeyboardFocusHere(int offset)
{
    ImGui::SetKeyboardFocusHere(offset);
}

bool IsItemHovered(HoveredFlags flags)
{
    return ImGui::IsItemHovered(flags);
}

bool IsItemActive()
{
    return ImGui::IsItemActive();
}

bool IsItemFocused()
{
    return ImGui::IsItemFocused();
}

bool IsItemClicked(int mouse_button)
{
    return ImGui::IsItemClicked(mouse_button);
}

bool IsItemVisible()
{
    return ImGui::IsItemVisible();
}

bool IsItemEdited()
{
    return ImGui::IsItemEdited();
}

bool IsItemActivated()
{
    return ImGui::IsItemActivated();
}

bool IsItemDeactivated()
{
    return ImGui::IsItemDeactivated();
}

bool IsItemDeactivatedAfterEdit()
{
    return ImGui::IsItemDeactivatedAfterEdit();
}

bool IsAnyItemHovered()
{
    return ImGui::IsAnyItemHovered();
}

bool IsAnyItemActive()
{
    return ImGui::IsAnyItemActive();
}

bool IsAnyItemFocused()
{
    return ImGui::IsAnyItemFocused();
}

Vector2 GetItemRectMin()
{
    return ImGui::GetItemRectMin();
}

Vector2 GetItemRectMax()
{
    return ImGui::GetItemRectMax();
}

Vector2 GetItemRectSize()
{
    return ImGui::GetItemRectSize();
}

void SetItemAllowOverlap()
{
    ImGui::SetItemAllowOverlap();
}

bool IsRectVisible(Vector2 const & size)
{
    return ImGui::IsRectVisible(size);
}

bool IsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max)
{
    return ImGui::IsRectVisible(rect_min, rect_max);
}

double GetTime()
{
    return ImGui::GetTime();
}

int GetFrameCount()
{
    return ImGui::GetFrameCount();
}

char const * GetStyleColorName(Col idx)
{
    return ImGui::GetStyleColorName(idx);
}

Vector2 CalcTextSize(char const * text, char const * text_end, bool hide_text_after_double_hash, float wrap_width)
{
    return ImGui::CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width);
}

void CalcListClipping(int items_count, float items_height, int * out_items_display_start, int * out_items_display_end)
{
    ImGui::CalcListClipping(items_count, items_height, out_items_display_start, out_items_display_end);
}

bool BeginChildFrame(ID id, Vector2 const & size, WindowFlags flags)
{
    return ImGui::BeginChildFrame(id, size, flags);
}

void EndChildFrame()
{
    ImGui::EndChildFrame();
}

Vector4 ColorConvertU32ToFloat4(unsigned int in)
{
    return ImGui::ColorConvertU32ToFloat4(in);
}

unsigned int ColorConvertFloat4ToU32(Vector4 const & in)
{
    return ImGui::ColorConvertFloat4ToU32(in);
}

void ColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v)
{
    ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}

void ColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b)
{
    ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}

int GetKeyIndex(Key imgui_key)
{
    return ImGui::GetKeyIndex(imgui_key);
}

bool IsKeyDown(int user_key_index)
{
    return ImGui::IsKeyDown(user_key_index);
}

bool IsKeyPressed(int user_key_index, bool repeat)
{
    return ImGui::IsKeyPressed(user_key_index, repeat);
}

bool IsKeyReleased(int user_key_index)
{
    return ImGui::IsKeyReleased(user_key_index);
}

int GetKeyPressedAmount(int key_index, float repeat_delay, float rate)
{
    return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate);
}

bool IsMouseDown(int button)
{
    return ImGui::IsMouseDown(button);
}

bool IsAnyMouseDown()
{
    return ImGui::IsAnyMouseDown();
}

bool IsMouseClicked(int button, bool repeat)
{
    return ImGui::IsMouseClicked(button, repeat);
}

bool IsMouseDoubleClicked(int button)
{
    return ImGui::IsMouseDoubleClicked(button);
}

bool IsMouseReleased(int button)
{
    return ImGui::IsMouseReleased(button);
}

bool IsMouseDragging(int button, float lock_threshold)
{
    return ImGui::IsMouseDragging(button, lock_threshold);
}

bool IsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip)
{
    return ImGui::IsMouseHoveringRect(r_min, r_max, clip);
}

bool IsMousePosValid(Vector2 const * mouse_pos)
{
    ImVec2 const POSITION = *mouse_pos;
    return ImGui::IsMousePosValid(&POSITION);
}

Vector2 GetMousePos()
{
    return ImGui::GetMousePos();
}

Vector2 GetMousePosOnOpeningCurrentPopup()
{
    return ImGui::GetMousePosOnOpeningCurrentPopup();
}

Vector2 GetMouseDragDelta(int button, float lock_threshold)
{
    return ImGui::GetMouseDragDelta(button, lock_threshold);
}

void ResetMouseDragDelta(int button)
{
    ImGui::ResetMouseDragDelta(button);
}

MouseCursor GetMouseCursor()
{
    return ImGui::GetMouseCursor();
}

void SetMouseCursor(MouseCursor type)
{
    ImGui::SetMouseCursor(type);
}

void CaptureKeyboardFromApp(bool want_capture_keyboard_value)
{
    ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
}

void CaptureMouseFromApp(bool want_capture_mouse_value)
{
    ImGui::CaptureMouseFromApp(want_capture_mouse_value);
}

char const * GetClipboardText()
{
    return ImGui::GetClipboardText();
}

void SetClipboardText(char const * text)
{
    ImGui::SetClipboardText(text);
}

void LoadIniSettingsFromDisk(char const * ini_filename)
{
    ImGui::LoadIniSettingsFromDisk(ini_filename);
}

void LoadIniSettingsFromMemory(char const * ini_data, size_t ini_size)
{
    ImGui::LoadIniSettingsFromMemory(ini_data, ini_size);
}

void SaveIniSettingsToDisk(char const * ini_filename)
{
    ImGui::SaveIniSettingsToDisk(ini_filename);
}

char const * SaveIniSettingsToMemory(size_t * out_ini_size)
{
    return ImGui::SaveIniSettingsToMemory(out_ini_size);
}

} // namespace gui
} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

