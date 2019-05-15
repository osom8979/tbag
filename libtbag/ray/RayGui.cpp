/**
 * @file   RayGui.cpp
 * @brief  RayGui class implementation.
 * @author zer0
 * @date   2019-05-06
 * @date   2019-05-13 (Move: libtbag/ray -> libtbag/ray/gui)
 * @date   2019-05-14 (Move: libtbag/ray/gui -> libtbag/ray)
 */

#include <libtbag/ray/RayGui.hpp>
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

bool GuiInitRay()
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

void GuiUpdateRay()
{
    ImGuiIO & io = ImGui::GetIO();

    io.DeltaTime = 1.0f/60.0f;              // set the time elapsed since the previous frame (in seconds)
    io.DisplaySize.x = 1920.0f;             // set the current display width
    io.DisplaySize.y = 1280.0f;             // set the current display height here

    io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    io.MouseWheel += (float)GetMouseWheelMove();

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

void GuiShutdownRay()
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
    STATIC_ASSERT_INTEGER_EQUAL(x, Im##x)
#endif

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Text);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TextDisabled);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_WindowBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ChildBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_PopupBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Border);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_BorderShadow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_FrameBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_FrameBgHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_FrameBgActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TitleBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TitleBgActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TitleBgCollapsed);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_MenuBarBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ScrollbarBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ScrollbarGrab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ScrollbarGrabHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ScrollbarGrabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_CheckMark);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_SliderGrab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_SliderGrabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Button);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ButtonHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ButtonActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Header);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_HeaderHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_HeaderActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Separator);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_SeparatorHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_SeparatorActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ResizeGrip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ResizeGripHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ResizeGripActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_Tab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TabHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TabActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TabUnfocused);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TabUnfocusedActive);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_PlotLines);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_PlotLinesHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_PlotHistogram);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_PlotHistogramHovered);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_TextSelectedBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_DragDropTarget);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_NavHighlight);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_NavWindowingHighlight);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_NavWindowingDimBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_ModalWindowDimBg);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCol_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoTitleBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoResize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoMove);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoScrollWithMouse);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoCollapse);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_AlwaysAutoResize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoBackground);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoSavedSettings);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoMouseInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_MenuBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_HorizontalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoFocusOnAppearing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoBringToFrontOnFocus);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_AlwaysVerticalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_AlwaysHorizontalScrollbar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_AlwaysUseWindowPadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoNavInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoNavFocus);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_UnsavedDocument);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoNav);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoDecoration);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiWindowFlags_NoInputs);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiFocusedFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiFocusedFlags_ChildWindows);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiFocusedFlags_RootWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiFocusedFlags_AnyWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiFocusedFlags_RootAndChildWindows);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_ChildWindows);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_RootWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AnyWindow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AllowWhenBlockedByPopup);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AllowWhenBlockedByModal);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AllowWhenBlockedByActiveItem);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AllowWhenOverlapped);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_AllowWhenDisabled);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_RectOnly);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiHoveredFlags_RootAndChildWindows);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCond_Always);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCond_Once);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCond_FirstUseEver);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiCond_Appearing);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_Alpha);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_WindowPadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_WindowRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_WindowBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_WindowMinSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_WindowTitleAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ChildRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ChildBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_PopupRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_PopupBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_FramePadding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_FrameRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_FrameBorderSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ItemSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ItemInnerSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_IndentSpacing);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ScrollbarSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ScrollbarRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_GrabMinSize);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_GrabRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_TabRounding);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_ButtonTextAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_SelectableTextAlign);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiStyleVar_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_Left);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_Right);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_Up);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_Down);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDir_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_PopupAlignLeft);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_HeightSmall);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_HeightRegular);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_HeightLarge);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_HeightLargest);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_NoArrowButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_NoPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiComboFlags_HeightMask_);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_S8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_U8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_S16);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_U16);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_S32);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_U32);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_S64);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_U64);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_Float);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_Double);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDataType_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoAlpha);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoPicker);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoOptions);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoSmallPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoInputs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoLabel);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoSidePreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_NoDragDrop);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_AlphaBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_AlphaPreview);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_AlphaPreviewHalf);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_HDR);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_DisplayRGB);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_DisplayHSV);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_DisplayHex);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_Uint8);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_Float);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_PickerHueBar);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_PickerHueWheel);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_InputRGB);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags_InputHSV);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiColorEditFlags__OptionsDefault);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_Selected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_Framed);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_AllowItemOverlap);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_NoTreePushOnOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_NoAutoOpenOnLog);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_DefaultOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_OpenOnDoubleClick);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_OpenOnArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_Leaf);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_Bullet);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_FramePadding);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_SpanAllAvailWidth);
//STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_NoScrollOnOpen);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_NavLeftJumpsBackHere);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTreeNodeFlags_CollapsingHeader);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiSelectableFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiSelectableFlags_DontClosePopups);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiSelectableFlags_SpanAllColumns);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiSelectableFlags_AllowDoubleClick);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiSelectableFlags_Disabled);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_Reorderable);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_AutoSelectNewTabs);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_TabListPopupButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_NoCloseWithMiddleMouseButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_NoTabListScrollingButtons);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_NoTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_FittingPolicyResizeDown);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_FittingPolicyScroll);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_FittingPolicyMask_);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabBarFlags_FittingPolicyDefault_);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabItemFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabItemFlags_UnsavedDocument);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabItemFlags_SetSelected);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabItemFlags_NoCloseWithMiddleMouseButton);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiTabItemFlags_NoPushId);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceNoPreviewTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceNoDisableHover);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceNoHoldToOpenOthers);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceAllowNullID);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceExtern);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_SourceAutoExpirePayload);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_AcceptBeforeDelivery);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_AcceptNoDrawDefaultRect);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_AcceptNoPreviewTooltip);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiDragDropFlags_AcceptPeekOnly);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Tab);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_LeftArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_RightArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_UpArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_DownArrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_PageUp);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_PageDown);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Home);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_End);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Insert);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Delete);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Backspace);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Space);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Enter);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Escape);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_A);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_C);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_V);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_X);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Y);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_Z);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiKey_COUNT);

STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_None);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_Arrow);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_TextInput);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_ResizeAll);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_ResizeNS);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_ResizeEW);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_ResizeNESW);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_ResizeNWSE);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_Hand);
STATIC_ASSERT_CHECK_IS_EQUALS_FOR_IMGUI(GuiMouseCursor_COUNT);

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
static void __style_copy(GuiStyle & dest, ImGuiStyle const & src) { __imgui_style_copy(dest, src); }
static void __style_copy(ImGuiStyle & dest, GuiStyle const & src) { __imgui_style_copy(dest, src); }
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
static void __payload_copy(GuiPayload & dest, ImGuiPayload const & src) { __imgui_payload_copy(dest, src); }
static void __payload_copy(ImGuiPayload & dest, GuiPayload const & src) { __imgui_payload_copy(dest, src); }
// clang-format on

void GuiShowDemoWindow(bool * p_open)
{
    ImGui::ShowDemoWindow(p_open);
}

void GuiShowAboutWindow(bool * p_open)
{
    ImGui::ShowAboutWindow(p_open);
}

void GuiShowMetricsWindow(bool * p_open)
{
    ImGui::ShowMetricsWindow(p_open);
}

void GuiShowStyleEditor()
{
    ImGui::ShowStyleEditor();
}

bool GuiShowStyleSelector(char const * label)
{
    return ImGui::ShowStyleSelector(label);
}

void GuiShowFontSelector(char const * label)
{
    ImGui::ShowFontSelector(label);
}

void GuiShowUserGuide()
{
    ImGui::ShowUserGuide();
}

char const * GuiGetVersion()
{
    return ImGui::GetVersion();
}

void GuiStyleColorsDark()
{
    ImGui::StyleColorsDark();
}

void GuiStyleColorsClassic()
{
    ImGui::StyleColorsClassic();
}

void GuiStyleColorsLight()
{
    ImGui::StyleColorsLight();
}

GuiStyle GuiGetStyle()
{
    GuiStyle result = {0,};
    __style_copy(result, ImGui::GetStyle());
    return result;
}

void GuiSetStyle(GuiStyle const & style)
{
    __style_copy(ImGui::GetStyle(), style);
}

bool GuiBegin(char const * text, bool * p_open, int flags)
{
    return ImGui::Begin(text, p_open, flags);
}

void GuiEnd()
{
    ImGui::End();
}

bool GuiBeginChild(char const * str_id, Vector2 const & size, bool border, GuiWindowFlags flags)
{
    return ImGui::BeginChild(str_id, size, border, flags);
}

bool GuiBeginChild(GuiID id, Vector2 const & size, bool border, GuiWindowFlags flags)
{
    return ImGui::BeginChild(id, size, border, flags);
}

void GuiEndChild()
{
    ImGui::EndChild();
}

bool GuiIsWindowAppearing()
{
    return ImGui::IsWindowAppearing();
}

bool GuiIsWindowCollapsed()
{
    return ImGui::IsWindowCollapsed();
}

bool GuiIsWindowFocused(GuiFocusedFlags flags)
{
    return ImGui::IsWindowFocused(flags);
}

bool GuiIsWindowHovered(GuiHoveredFlags flags)
{
    return ImGui::IsWindowHovered(flags);
}

Vector2 GuiGetWindowPos()
{
    return ImGui::GetWindowPos();
}

Vector2 GuiGetWindowSize()
{
    return ImGui::GetWindowSize();
}

float GuiGetWindowWidth()
{
    return ImGui::GetWindowWidth();
}

float GuiGetWindowHeight()
{
    return ImGui::GetWindowHeight();
}

void GuiSetNextWindowPos(Vector2 const & pos, GuiCond cond, Vector2 const & pivot)
{
    ImGui::SetNextWindowPos(pos, cond, pivot);
}

void GuiSetNextWindowSize(Vector2 const & size, GuiCond cond)
{
    ImGui::SetNextWindowSize(size, cond);
}

void GuiSetNextWindowContentSize(Vector2 const & size)
{
    ImGui::SetNextWindowContentSize(size);
}

void GuiSetNextWindowCollapsed(bool collapsed, GuiCond cond)
{
    ImGui::SetNextWindowCollapsed(collapsed, cond);
}

void GuiSetNextWindowFocus()
{
    ImGui::SetNextWindowFocus();
}

void GuiSetNextWindowBgAlpha(float alpha)
{
    ImGui::SetNextWindowBgAlpha(alpha);
}

void GuiSetWindowPos(Vector2 const & pos, GuiCond cond)
{
    ImGui::SetWindowPos(pos, cond);
}

void GuiSetWindowSize(Vector2 const & size, GuiCond cond)
{
    ImGui::SetWindowSize(size, cond);
}

void GuiSetWindowCollapsed(bool collapsed, GuiCond cond)
{
    ImGui::SetWindowCollapsed(collapsed, cond);
}

void GuiSetWindowFocus()
{
    ImGui::SetWindowFocus();
}

void GuiSetWindowFontScale(float scale)
{
    ImGui::SetWindowFontScale(scale);
}

void GuiSetWindowPos(char const * name, Vector2 const & pos, GuiCond cond)
{
    ImGui::SetWindowPos(name, pos, cond);
}

void GuiSetWindowSize(char const * name, Vector2 const & size, GuiCond cond)
{
    ImGui::SetWindowSize(name, size, cond);
}

void GuiSetWindowCollapsed(char const * name, bool collapsed, GuiCond cond)
{
    ImGui::SetWindowCollapsed(name, collapsed, cond);
}

void GuiSetWindowFocus(char const * name)
{
    ImGui::SetWindowFocus();
}

Vector2 GuiGetContentRegionMax()
{
    return ImGui::GetContentRegionMax();
}

Vector2 GuiGetContentRegionAvail()
{
    return ImGui::GetContentRegionAvail();
}

Vector2 GuiGetWindowContentRegionMin()
{
    return ImGui::GetWindowContentRegionMin();
}

Vector2 GuiGetWindowContentRegionMax()
{
    return ImGui::GetWindowContentRegionMax();
}

float GuiGetWindowContentRegionWidth()
{
    return ImGui::GetWindowContentRegionWidth();
}

float GuiGetScrollX()
{
    return ImGui::GetScrollX();
}

float GuiGetScrollY()
{
    return ImGui::GetScrollY();
}

float GuiGetScrollMaxX()
{
    return ImGui::GetScrollMaxX();
}

float GuiGetScrollMaxY()
{
    return ImGui::GetScrollMaxY();
}

void GuiSetScrollX(float scroll_x)
{
    ImGui::SetScrollX(scroll_x);
}

void GuiSetScrollY(float scroll_y)
{
    ImGui::SetScrollY(scroll_y);
}

void GuiSetScrollHereY(float center_y_ratio)
{
    ImGui::SetScrollHereY(center_y_ratio);
}

void GuiSetScrollFromPosY(float local_y, float center_y_ratio)
{
    ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

void GuiPushStyleColor(GuiCol idx, unsigned int col)
{
    ImGui::PushStyleColor(idx, col);
}

void GuiPushStyleColor(GuiCol idx, Vector4 const & col)
{
    ImGui::PushStyleColor(idx, col);
}

void GuiPopStyleColor(int count)
{
    ImGui::PopStyleColor(count);
}

void GuiPushStyleVar(GuiStyleVar idx, float val)
{
    ImGui::PushStyleVar(idx, val);
}

void GuiPushStyleVar(GuiStyleVar idx, Vector2 const & val)
{
    ImGui::PushStyleVar(idx, val);
}

void GuiPopStyleVar(int count)
{
    ImGui::PopStyleVar(count);
}

Vector4 GuiGetStyleColorVec4(GuiCol idx)
{
    return ImGui::GetStyleColorVec4(idx);
}

float GuiGetFontSize()
{
    return ImGui::GetFontSize();
}

Vector2 GuiGetFontTexUvWhitePixel()
{
    return ImGui::GetFontTexUvWhitePixel();
}

unsigned int GuiGetColorU32(GuiCol idx, float alpha_mul)
{
    return ImGui::GetColorU32(idx, alpha_mul);
}

unsigned int GuiGetColorU32(Vector4 const & col)
{
    return ImGui::GetColorU32(col);
}

unsigned int GuiGetColorU32(unsigned int col)
{
    return ImGui::GetColorU32(col);
}

void GuiPushItemWidth(float item_width)
{
    ImGui::PushItemWidth(item_width);
}

void GuiPopItemWidth()
{
    ImGui::PopItemWidth();
}

void GuiSetNextItemWidth(float item_width)
{
    ImGui::SetNextItemWidth(item_width);
}

float GuiCalcItemWidth()
{
    return ImGui::CalcItemWidth();
}

void GuiPushTextWrapPos(float wrap_local_pos_x)
{
    ImGui::PushTextWrapPos(wrap_local_pos_x);
}

void GuiPopTextWrapPos()
{
    ImGui::PopTextWrapPos();
}

void GuiPushAllowKeyboardFocus(bool allow_keyboard_focus)
{
    ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
}

void GuiPopAllowKeyboardFocus()
{
    ImGui::PopAllowKeyboardFocus();
}

void GuiPushButtonRepeat(bool repeat)
{
    ImGui::PushButtonRepeat(repeat);
}

void GuiPopButtonRepeat()
{
    ImGui::PopButtonRepeat();
}

void GuiSeparator()
{
    ImGui::Separator();
}

void GuiSameLine(float offset_from_start_x, float spacing)
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

void GuiNewLine()
{
    ImGui::NewLine();
}

void GuiSpacing()
{
    ImGui::Spacing();
}

void GuiDummy(Vector2 const & size)
{
    ImGui::Dummy(size);
}

void GuiIndent(float indent_w)
{
    ImGui::Indent(indent_w);
}

void GuiUnindent(float indent_w)
{
    ImGui::Unindent(indent_w);
}

void GuiBeginGroup()
{
    ImGui::BeginGroup();
}

void GuiEndGroup()
{
    ImGui::EndGroup();
}

Vector2 GuiGetCursorPos()
{
    return ImGui::GetCursorPos();
}

float GuiGetCursorPosX()
{
    return ImGui::GetCursorPosX();
}

float GuiGetCursorPosY()
{
    return ImGui::GetCursorPosY();
}

void GuiSetCursorPos(Vector2 const & local_pos)
{
    ImGui::SetCursorPos(local_pos);
}

void GuiSetCursorPosX(float local_x)
{
    ImGui::SetCursorPosX(local_x);
}

void GuiSetCursorPosY(float local_y)
{
    ImGui::SetCursorPosY(local_y);
}

Vector2 GuiGetCursorStartPos()
{
    return ImGui::GetCursorStartPos();
}

Vector2 GuiGetCursorScreenPos()
{
    return ImGui::GetCursorScreenPos();
}

void GuiSetCursorScreenPos(Vector2 const & pos)
{
    ImGui::SetCursorScreenPos(pos);
}

void GuiAlignTextToFramePadding()
{
    ImGui::AlignTextToFramePadding();
}

float GuiGetTextLineHeight()
{
    return ImGui::GetTextLineHeight();
}

float GuiGetTextLineHeightWithSpacing()
{
    return ImGui::GetTextLineHeightWithSpacing();
}

float GuiGetFrameHeight()
{
    return ImGui::GetFrameHeight();
}

float GuiGetFrameHeightWithSpacing()
{
    return ImGui::GetFrameHeightWithSpacing();
}

void GuiPushID(char const * str_id)
{
    ImGui::PushID(str_id);
}

void GuiPushID(char const * str_id_begin, char const * str_id_end)
{
    ImGui::PushID(str_id_begin, str_id_end);
}

void GuiPushID(void const * ptr_id)
{
    ImGui::PushID(ptr_id);
}

void GuiPushID(int int_id)
{
    ImGui::PushID(int_id);
}

void GuiPopID()
{
    ImGui::PopID();
}

GuiID GuiGetID(char const * str_id)
{
    return ImGui::GetID(str_id);
}

GuiID GuiGetID(char const * str_id_begin, char const * str_id_end)
{
    return ImGui::GetID(str_id_begin, str_id_end);
}

GuiID GuiGetID(void const * ptr_id)
{
    return ImGui::GetID(ptr_id);
}

void GuiTextUnformatted(char const * text, char const * text_end)
{
    ImGui::TextUnformatted(text, text_end);
}

void GuiText(char const * text)
{
    ImGui::Text("%s", text);
}

void GuiTextColored(Vector4 const & col, char const * text)
{
    ImGui::TextColored(col, "%s", text);
}

void GuiTextDisabled(char const * text)
{
    return ImGui::TextDisabled("%s", text);
}

void GuiTextWrapped(char const * text)
{
    ImGui::TextWrapped("%s", text);
}

void GuiLabelText(char const * label, char const * text)
{
    ImGui::LabelText(label, "%s", text);
}

void GuiBulletText(char const * text)
{
    ImGui::BulletText("%s", text);
}

bool GuiButton(char const * label, Vector2 const & size)
{
    return ImGui::Button(label, size);
}

bool GuiSmallButton(char const * label)
{
    return ImGui::SmallButton(label);
}

bool GuiInvisibleButton(char const * str_id, Vector2 const & size)
{
    return ImGui::InvisibleButton(str_id, size);
}

bool GuiArrowButton(char const * str_id, GuiDir dir)
{
    return ImGui::ArrowButton(str_id, dir);
}

void GuiImage(GuiTextureID user_texture_id, Vector2 const & size,
              Vector2 const & uv0, Vector2 const & uv1,
              Vector4 const & tint_col, Vector4 const & border_col)
{
    ImGui::Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
}

bool GuiImageButton(GuiTextureID user_texture_id, Vector2 const & size,
                    Vector2 const & uv0, Vector2 const & uv1, int frame_padding,
                    Vector4 const & bg_col, Vector4 const & tint_col)
{
    return ImGui::ImageButton(user_texture_id, size, uv0, uv1, frame_padding, bg_col, tint_col);
}

bool GuiCheckbox(char const * label, bool * v)
{
    return ImGui::Checkbox(label, v);
}

bool GuiCheckboxFlags(char const * label, unsigned int * flags, unsigned int flags_value)
{
    return ImGui::CheckboxFlags(label, flags, flags_value);
}

bool GuiRadioButton(char const * label, bool active)
{
    return ImGui::RadioButton(label, active);
}

bool GuiRadioButton(char const * label, int * v, int v_button)
{
    return ImGui::RadioButton(label, v, v_button);
}

void GuiProgressBar(float fraction, Vector2 const & size_arg, char const * overlay)
{
    ImGui::ProgressBar(fraction, size_arg, overlay);
}

void GuiBullet()
{
    ImGui::Bullet();
}

bool GuiBeginCombo(char const * label, char const * preview_value, GuiComboFlags flags)
{
    return ImGui::BeginCombo(label, preview_value, flags);
}

void GuiEndCombo()
{
    ImGui::EndCombo();
}

bool GuiCombo(char const * label, int * current_item, char const * const items[],
              int items_count, int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items);
}

bool GuiCombo(char const * label, int * current_item, char const * items_separated_by_zeros,
              int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}

bool GuiCombo(char const * label, int * current_item,
              bool(*items_getter)(void * data, int idx, char const ** out_text),
              void * data, int items_count, int popup_max_height_in_items)
{
    return ImGui::Combo(label, current_item, items_getter, data, items_count, popup_max_height_in_items);
}

bool GuiDragFloat(char const * label, float * v, float v_speed, float v_min, float v_max,
                  char const * format, float power)
{
    return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, power);
}

bool GuiDragFloat2(char const * label, float v[2], float v_speed, float v_min, float v_max,
                   char const * format, float power)
{
    return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, power);
}

bool GuiDragFloat3(char const * label, float v[3], float v_speed, float v_min, float v_max,
                   char const * format, float power)
{
    return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, power);
}

bool GuiDragFloat4(char const * label, float v[4], float v_speed, float v_min, float v_max,
                   char const * format, float power)
{
    return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, power);
}

bool GuiDragFloatRange2(char const * label, float * v_current_min, float * v_current_max, float v_speed,
                        float v_min, float v_max, char const * format, char const * format_max, float power)
{
    return ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed,
                                  v_min, v_max, format, format_max, power);
}

bool GuiDragInt(char const * label, int * v, float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt(label, v, v_speed, v_min, v_max, format);
}

bool GuiDragInt2(char const * label, int v[2], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format);
}

bool GuiDragInt3(char const * label, int v[3], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format);
}

bool GuiDragInt4(char const * label, int v[4], float v_speed, int v_min, int v_max, char const * format)
{
    return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format);
}

bool GuiDragIntRange2(char const * label, int * v_current_min, int * v_current_max,
                      float v_speed, int v_min, int v_max, char const * format, char const * format_max)
{
    return ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max);
}

bool GuiDragScalar(char const * label, GuiDataType data_type, void * v, float v_speed, const void * v_min,
                   const void * v_max, char const * format, float power)
{
    return ImGui::DragScalar(label, data_type, v, v_speed, v_min, v_max, format, power);
}

bool GuiDragScalarN(char const * label, GuiDataType data_type, void * v, int components, float v_speed,
                    const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::DragScalarN(label, data_type, v, components, v_speed, v_min, v_max, format, power);
}

bool GuiSliderFloat(char const * label, float * v, float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat(label, v, v_min, v_max, format, power);
}

bool GuiSliderFloat2(char const * label, float v[2], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat2(label, v, v_min, v_max, format, power);
}

bool GuiSliderFloat3(char const * label, float v[3], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, power);
}

bool GuiSliderFloat4(char const * label, float v[4], float v_min, float v_max, char const * format, float power)
{
    return ImGui::SliderFloat4(label, v, v_min, v_max, format, power);
}

bool GuiSliderAngle(char const * label, float * v_rad, float v_degrees_min, float v_degrees_max, char const * format)
{
    return ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format);
}

bool GuiSliderInt(char const * label, int * v, int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt(label, v, v_min, v_max, format);
}

bool GuiSliderInt2(char const * label, int v[2], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt2(label, v, v_min, v_max, format);
}

bool GuiSliderInt3(char const * label, int v[3], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt3(label, v, v_min, v_max, format);
}

bool GuiSliderInt4(char const * label, int v[4], int v_min, int v_max, char const * format)
{
    return ImGui::SliderInt4(label, v, v_min, v_max, format);
}

bool GuiSliderScalar(char const * label, GuiDataType data_type, void * v, void const * v_min,
                     void const * v_max, char const * format, float power)
{
    return ImGui::SliderScalar(label, data_type, v, v_min, v_max, format, power);
}

bool GuiSliderScalarN(char const * label, GuiDataType data_type, void * v, int components,
                      const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::SliderScalarN(label, data_type, v, components, v_min, v_max, format, power);
}

bool GuiVSliderFloat(char const * label, Vector2 const & size, float * v, float v_min, float v_max,
                     char const * format, float power)
{
    return ImGui::VSliderFloat(label, size, v, v_min, v_max, format, power);
}

bool GuiVSliderInt(char const * label, Vector2 const & size, int * v, int v_min, int v_max, char const * format)
{
    return ImGui::VSliderInt(label, size, v, v_min, v_max, format);
}

bool GuiVSliderScalar(char const * label, Vector2 const & size, GuiDataType data_type, void * v,
                      const void * v_min, const void * v_max, char const * format, float power)
{
    return ImGui::VSliderScalar(label, size, data_type, v, v_min, v_max, format, power);
}

bool GuiInputText(char const * label, char * buf, size_t buf_size, GuiInputTextFlags flags)
{
    return ImGui::InputText(label, buf, buf_size, flags);
}

bool GuiInputTextMultiline(char const * label, char * buf, size_t buf_size, Vector2 const & size, GuiInputTextFlags flags)
{
    return ImGui::InputTextMultiline(label, buf, buf_size, size, flags);
}

bool GuiInputTextWithHint(char const * label, char const * hint, char * buf, size_t buf_size, GuiInputTextFlags flags)
{
    return ImGui::InputTextWithHint(label, hint, buf, buf_size, flags);
}

bool GuiInputFloat(char const * label, float * v, float step, float step_fast, char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}

bool GuiInputFloat2(char const * label, float v[2], char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputFloat2(label, v, format, flags);
}

bool GuiInputFloat3(char const * label, float v[3], char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputFloat3(label, v, format, flags);
}

bool GuiInputFloat4(char const * label, float v[4], char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputFloat4(label, v, format, flags);
}

bool GuiInputInt(char const * label, int * v, int step, int step_fast, GuiInputTextFlags flags)
{
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

bool GuiInputInt2(char const * label, int v[2], GuiInputTextFlags flags)
{
    return ImGui::InputInt2(label, v, flags);
}

bool GuiInputInt3(char const * label, int v[3], GuiInputTextFlags flags)
{
    return ImGui::InputInt3(label, v, flags);
}

bool GuiInputInt4(char const * label, int v[4], GuiInputTextFlags flags)
{
    return ImGui::InputInt4(label, v, flags);
}

bool GuiInputDouble(char const * label, double * v, double step, double step_fast,
                    char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputDouble(label, v, step, step_fast, format, flags);
}

bool GuiInputScalar(char const * label, GuiDataType data_type, void * v, void const * step,
                    void const * step_fast, char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputScalar(label, data_type, v, step, step_fast, format, flags);
}

bool GuiInputScalarN(char const * label, GuiDataType data_type, void * v, int components, void const * step,
                     void const * step_fast, char const * format, GuiInputTextFlags flags)
{
    return ImGui::InputScalarN(label, data_type, v, components, step, step_fast, format, flags);
}

bool GuiColorEdit3(char const * label, float col[3], GuiColorEditFlags flags)
{
    return ImGui::ColorEdit3(label, col, flags);
}

bool GuiColorEdit4(char const * label, float col[4], GuiColorEditFlags flags)
{
    return ImGui::ColorEdit4(label, col, flags);
}

bool GuiColorPicker3(char const * label, float col[3], GuiColorEditFlags flags)
{
    return ImGui::ColorPicker3(label, col, flags);
}

bool GuiColorPicker4(char const * label, float col[4], GuiColorEditFlags flags, float const * ref_col)
{
    return ImGui::ColorPicker4(label, col, flags, ref_col);
}

bool GuiColorButton(char const * desc_id, Vector4 const & col, GuiColorEditFlags flags, Vector2 size)
{
    return ImGui::ColorButton(desc_id, col, flags, size);
}

void GuiSetColorEditOptions(GuiColorEditFlags flags)
{
    ImGui::SetColorEditOptions(flags);
}

bool GuiTreeNode(char const * label)
{
    return ImGui::TreeNode(label);
}

bool GuiTreeNode(char const * str_id, char const * text)
{
    return ImGui::TreeNode(str_id, "%s", text);
}

bool GuiTreeNodeEx(char const * label, GuiTreeNodeFlags flags)
{
    return ImGui::TreeNodeEx(label, flags);
}

bool GuiTreeNodeEx(char const * str_id, GuiTreeNodeFlags flags, char const * text)
{
    return ImGui::TreeNodeEx(str_id, flags, "%s", text);
}

void GuiTreePush(char const * str_id)
{
    ImGui::TreePush(str_id);
}

void GuiTreePush(void const * ptr_id)
{
    ImGui::TreePush(ptr_id);
}

void GuiTreePop()
{
    ImGui::TreePop();
}

void GuiTreeAdvanceToLabelPos()
{
    ImGui::TreeAdvanceToLabelPos();
}

float GuiGetTreeNodeToLabelSpacing()
{
    return ImGui::GetTreeNodeToLabelSpacing();
}

void GuiSetNextTreeNodeOpen(bool is_open, GuiCond cond)
{
    ImGui::SetNextTreeNodeOpen(is_open, cond);
}

bool GuiCollapsingHeader(char const * label, GuiTreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, flags);
}

bool GuiCollapsingHeader(char const * label, bool * p_open, GuiTreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, p_open, flags);
}

bool GuiSelectable(char const * label, bool selected, GuiSelectableFlags flags, Vector2 const & size)
{
    return ImGui::Selectable(label, selected, flags, size);
}

bool GuiSelectable(char const * label, bool * p_selected, GuiSelectableFlags flags, Vector2 const & size)
{
    return ImGui::Selectable(label, p_selected, flags, size);
}

bool GuiListBox(char const * label, int * current_item, char const * const items[], int items_count, int height_in_items)
{
    return ImGui::ListBox(label, current_item, items, items_count, height_in_items);
}

bool GuiListBox(char const * label, int * current_item,
                bool (*items_getter)(void * data, int idx, char const ** out_text),
                void * data, int items_count, int height_in_items)
{
    return ImGui::ListBox(label, current_item, items_getter, data, items_count, height_in_items);
}

bool GuiListBoxHeader(char const * label, Vector2 const & size)
{
    return ImGui::ListBoxHeader(label, size);
}

bool GuiListBoxHeader(char const * label, int items_count, int height_in_items)
{
    return ImGui::ListBoxHeader(label, items_count, height_in_items);
}

void GuiListBoxFooter()
{
    ImGui::ListBoxFooter();
}

void GuiPlotLines(char const * label, float const * values, int values_count, int values_offset,
                  char const * overlay_text, float scale_min, float scale_max, Vector2 graph_size, int stride)
{
    ImGui::PlotLines(label, values, values_count, values_offset, overlay_text,
                     scale_min, scale_max, graph_size, stride);
}

void GuiPlotLines(char const * label, float(*values_getter)(void * data, int idx), void * data,
                  int values_count, int values_offset, char const * overlay_text,
                  float scale_min, float scale_max, Vector2 graph_size)
{
    ImGui::PlotLines(label, values_getter, data, values_count, values_offset,
                     overlay_text, scale_min, scale_max, graph_size);
}

void GuiPlotHistogram(char const * label, float const * values, int values_count, int values_offset,
                      char const * overlay_text, float scale_min, float scale_max,
                      Vector2 graph_size, int stride)
{
    ImGui::PlotHistogram(label, values, values_count, values_offset, overlay_text,
                         scale_min, scale_max, graph_size, stride);
}

void GuiPlotHistogram(char const * label, float(*values_getter)(void * data, int idx), void * data,
                      int values_count, int values_offset, char const * overlay_text,
                      float scale_min, float scale_max, Vector2 graph_size)
{
    ImGui::PlotHistogram(label, values_getter, data, values_count, values_offset,
                         overlay_text, scale_min, scale_max, graph_size);
}

void GuiValue(char const * prefix, bool b)
{
    ImGui::Value(prefix, b);
}

void GuiValue(char const * prefix, int v)
{
    ImGui::Value(prefix, v);
}

void GuiValue(char const * prefix, unsigned int v)
{
    ImGui::Value(prefix, v);
}

void GuiValue(char const * prefix, float v, char const * float_format)
{
    ImGui::Value(prefix, v, float_format);
}

bool GuiBeginMainMenuBar()
{
    return ImGui::BeginMainMenuBar();
}

void GuiEndMainMenuBar()
{
    ImGui::EndMainMenuBar();
}

bool GuiBeginMenuBar()
{
    return ImGui::BeginMenuBar();
}

void GuiEndMenuBar()
{
    ImGui::EndMenuBar();
}

bool GuiBeginMenu(char const * label, bool enabled)
{
    return ImGui::BeginMenu(label, enabled);
}

void GuiEndMenu()
{
    ImGui::EndMenu();
}

bool GuiMenuItem(char const * label, char const * shortcut, bool selected, bool enabled)
{
    return ImGui::MenuItem(label, shortcut, selected, enabled);
}

bool GuiMenuItem(char const * label, char const * shortcut, bool * p_selected, bool enabled)
{
    return ImGui::MenuItem(label, shortcut, p_selected, enabled);
}

void GuiBeginTooltip()
{
    ImGui::BeginTooltip();
}

void GuiEndTooltip()
{
    ImGui::EndTooltip();
}

void GuiSetTooltip(char const * text)
{
    ImGui::SetTooltip("%s", text);
}

void GuiOpenPopup(char const * str_id)
{
    ImGui::OpenPopup(str_id);
}

bool GuiBeginPopup(char const * str_id, GuiWindowFlags flags)
{
    return ImGui::BeginPopup(str_id, flags);
}

bool GuiBeginPopupContextItem(char const * str_idg, int mouse_button)
{
    return ImGui::BeginPopupContextItem(str_idg, mouse_button);
}

bool GuiBeginPopupContextWindow(char const * str_idg, int mouse_button, bool also_over_items)
{
    return ImGui::BeginPopupContextWindow(str_idg, mouse_button, also_over_items);
}

bool GuiBeginPopupContextVoid(char const * str_idg, int mouse_button)
{
    return ImGui::BeginPopupContextVoid(str_idg, mouse_button);
}

bool GuiBeginPopupModal(char const * name, bool * p_openg, GuiWindowFlags flags)
{
    return ImGui::BeginPopupModal(name, p_openg, flags);
}

void GuiEndPopup()
{
    ImGui::EndPopup();
}

bool GuiOpenPopupOnItemClick(char const * str_idg, int mouse_button)
{
    return ImGui::OpenPopupOnItemClick(str_idg, mouse_button);
}

bool GuiIsPopupOpen(char const * str_id)
{
    return ImGui::IsPopupOpen(str_id);
}

void GuiCloseCurrentPopup()
{
    ImGui::CloseCurrentPopup();
}

void GuiColumns(int count, char const * idg, bool border)
{
    ImGui::Columns(count, idg, border);
}

void GuiNextColumn()
{
    ImGui::NextColumn();
}

int GuiGetColumnIndex()
{
    return ImGui::GetColumnIndex();
}

float GuiGetColumnWidth(int column_index)
{
    return ImGui::GetColumnWidth(column_index);
}

void GuiSetColumnWidth(int column_index, float width)
{
    ImGui::SetColumnWidth(column_index, width);
}

float GuiGetColumnOffset(int column_index)
{
    return ImGui::GetColumnOffset(column_index);
}

void GuiSetColumnOffset(int column_index, float offset_x)
{
    ImGui::SetColumnOffset(column_index, offset_x);
}

int GuiGetColumnsCount()
{
    return ImGui::GetColumnsCount();
}

bool GuiBeginTabBar(char const * str_id, GuiTabBarFlags flags)
{
    return ImGui::BeginTabBar(str_id, flags);
}

void GuiEndTabBar()
{
    ImGui::EndTabBar();
}

bool GuiBeginTabItem(char const * label, bool * p_open, GuiTabItemFlags flags)
{
    return ImGui::BeginTabItem(label, p_open, flags);
}

void GuiEndTabItem()
{
    ImGui::EndTabItem();
}

void GuiSetTabItemClosed(char const * tab_or_docked_window_label)
{
    ImGui::SetTabItemClosed(tab_or_docked_window_label);
}

void GuiLogToTTY(int auto_open_depth)
{
    ImGui::LogToTTY(auto_open_depth);
}

void GuiLogToFile(int auto_open_depth, char const * filename)
{
    ImGui::LogToFile(auto_open_depth, filename);
}

void GuiLogToClipboard(int auto_open_depth)
{
    ImGui::LogToClipboard(auto_open_depth);
}

void GuiLogFinish()
{
    ImGui::LogFinish();
}

void GuiLogButtons()
{
    ImGui::LogButtons();
}

void GuiLogText(char const * text)
{
    ImGui::LogText("%s", text);
}

bool GuiBeginDragDropSource(GuiDragDropFlags flags)
{
    return ImGui::BeginDragDropSource(flags);
}

bool GuiSetDragDropPayload(char const * type, void const * data, size_t sz, GuiCond cond)
{
    return ImGui::SetDragDropPayload(type, data, sz, cond);
}

void GuiEndDragDropSource()
{
    ImGui::EndDragDropSource();
}

bool GuiBeginDragDropTarget()
{
    return ImGui::BeginDragDropTarget();
}

GuiPayload GuiAcceptDragDropPayload(char const * type, GuiDragDropFlags flags)
{
    GuiPayload result = {0,};
    __payload_copy(result, *ImGui::AcceptDragDropPayload(type, flags));
    return result;
}

void GuiEndDragDropTarget()
{
    ImGui::EndDragDropTarget();
}

GuiPayload GuiGetDragDropPayload()
{
    GuiPayload result = {0,};
    __payload_copy(result, *ImGui::GetDragDropPayload());
    return result;
}

void GuiPushClipRect(Vector2 const & clip_rect_min, Vector2 const & clip_rect_max, bool intersect_with_current_clip_rect)
{
    ImGui::PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
}

void GuiPopClipRect()
{
    ImGui::PopClipRect();
}

void GuiSetItemDefaultFocus()
{
    ImGui::SetItemDefaultFocus();
}

void GuiSetKeyboardFocusHere(int offset)
{
    ImGui::SetKeyboardFocusHere(offset);
}

bool GuiIsItemHovered(GuiHoveredFlags flags)
{
    return ImGui::IsItemHovered(flags);
}

bool GuiIsItemActive()
{
    return ImGui::IsItemActive();
}

bool GuiIsItemFocused()
{
    return ImGui::IsItemFocused();
}

bool GuiIsItemClicked(int mouse_button)
{
    return ImGui::IsItemClicked(mouse_button);
}

bool GuiIsItemVisible()
{
    return ImGui::IsItemVisible();
}

bool GuiIsItemEdited()
{
    return ImGui::IsItemEdited();
}

bool GuiIsItemActivated()
{
    return ImGui::IsItemActivated();
}

bool GuiIsItemDeactivated()
{
    return ImGui::IsItemDeactivated();
}

bool GuiIsItemDeactivatedAfterEdit()
{
    return ImGui::IsItemDeactivatedAfterEdit();
}

bool GuiIsAnyItemHovered()
{
    return ImGui::IsAnyItemHovered();
}

bool GuiIsAnyItemActive()
{
    return ImGui::IsAnyItemActive();
}

bool GuiIsAnyItemFocused()
{
    return ImGui::IsAnyItemFocused();
}

Vector2 GuiGetItemRectMin()
{
    return ImGui::GetItemRectMin();
}

Vector2 GuiGetItemRectMax()
{
    return ImGui::GetItemRectMax();
}

Vector2 GuiGetItemRectSize()
{
    return ImGui::GetItemRectSize();
}

void GuiSetItemAllowOverlap()
{
    ImGui::SetItemAllowOverlap();
}

bool GuiIsRectVisible(Vector2 const & size)
{
    return ImGui::IsRectVisible(size);
}

bool GuiIsRectVisible(Vector2 const & rect_min, Vector2 const & rect_max)
{
    return ImGui::IsRectVisible(rect_min, rect_max);
}

double GuiGetTime()
{
    return ImGui::GetTime();
}

int GuiGetFrameCount()
{
    return ImGui::GetFrameCount();
}

char const * GuiGetStyleColorName(GuiCol idx)
{
    return ImGui::GetStyleColorName(idx);
}

Vector2 GuiCalcTextSize(char const * text, char const * text_end, bool hide_text_after_double_hash, float wrap_width)
{
    return ImGui::CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width);
}

void GuiCalcListClipping(int items_count, float items_height, int * out_items_display_start, int * out_items_display_end)
{
    ImGui::CalcListClipping(items_count, items_height, out_items_display_start, out_items_display_end);
}

bool GuiBeginChildFrame(GuiID id, Vector2 const & size, GuiWindowFlags flags)
{
    return ImGui::BeginChildFrame(id, size, flags);
}

void GuiEndChildFrame()
{
    ImGui::EndChildFrame();
}

Vector4 GuiColorConvertU32ToFloat4(unsigned int in)
{
    return ImGui::ColorConvertU32ToFloat4(in);
}

unsigned int GuiColorConvertFloat4ToU32(Vector4 const & in)
{
    return ImGui::ColorConvertFloat4ToU32(in);
}

void GuiColorConvertRGBtoHSV(float r, float g, float b, float & out_h, float & out_s, float & out_v)
{
    ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}

void GuiColorConvertHSVtoRGB(float h, float s, float v, float & out_r, float & out_g, float & out_b)
{
    ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}

int GuiGetKeyIndex(GuiKey imgui_key)
{
    return ImGui::GetKeyIndex(imgui_key);
}

bool GuiIsKeyDown(int user_key_index)
{
    return ImGui::IsKeyDown(user_key_index);
}

bool GuiIsKeyPressed(int user_key_index, bool repeat)
{
    return ImGui::IsKeyPressed(user_key_index, repeat);
}

bool GuiIsKeyReleased(int user_key_index)
{
    return ImGui::IsKeyReleased(user_key_index);
}

int GuiGetKeyPressedAmount(int key_index, float repeat_delay, float rate)
{
    return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate);
}

bool GuiIsMouseDown(int button)
{
    return ImGui::IsMouseDown(button);
}

bool GuiIsAnyMouseDown()
{
    return ImGui::IsAnyMouseDown();
}

bool GuiIsMouseClicked(int button, bool repeat)
{
    return ImGui::IsMouseClicked(button, repeat);
}

bool GuiIsMouseDoubleClicked(int button)
{
    return ImGui::IsMouseDoubleClicked(button);
}

bool GuiIsMouseReleased(int button)
{
    return ImGui::IsMouseReleased(button);
}

bool GuiIsMouseDragging(int button, float lock_threshold)
{
    return ImGui::IsMouseDragging(button, lock_threshold);
}

bool GuiIsMouseHoveringRect(Vector2 const & r_min, Vector2 const & r_max, bool clip)
{
    return ImGui::IsMouseHoveringRect(r_min, r_max, clip);
}

bool GuiIsMousePosValid(Vector2 const * mouse_pos)
{
    ImVec2 const POSITION = *mouse_pos;
    return ImGui::IsMousePosValid(&POSITION);
}

Vector2 GuiGetMousePos()
{
    return ImGui::GetMousePos();
}

Vector2 GuiGetMousePosOnOpeningCurrentPopup()
{
    return ImGui::GetMousePosOnOpeningCurrentPopup();
}

Vector2 GuiGetMouseDragDelta(int button, float lock_threshold)
{
    return ImGui::GetMouseDragDelta(button, lock_threshold);
}

void GuiResetMouseDragDelta(int button)
{
    ImGui::ResetMouseDragDelta(button);
}

GuiMouseCursor GuiGetMouseCursor()
{
    return ImGui::GetMouseCursor();
}

void GuiSetMouseCursor(GuiMouseCursor type)
{
    ImGui::SetMouseCursor(type);
}

void GuiCaptureKeyboardFromApp(bool want_capture_keyboard_value)
{
    ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
}

void GuiCaptureMouseFromApp(bool want_capture_mouse_value)
{
    ImGui::CaptureMouseFromApp(want_capture_mouse_value);
}

char const * GuiGetClipboardText()
{
    return ImGui::GetClipboardText();
}

void GuiSetClipboardText(char const * text)
{
    ImGui::SetClipboardText(text);
}

void GuiLoadIniSettingsFromDisk(char const * ini_filename)
{
    ImGui::LoadIniSettingsFromDisk(ini_filename);
}

void GuiLoadIniSettingsFromMemory(char const * ini_data, size_t ini_size)
{
    ImGui::LoadIniSettingsFromMemory(ini_data, ini_size);
}

void GuiSaveIniSettingsToDisk(char const * ini_filename)
{
    ImGui::SaveIniSettingsToDisk(ini_filename);
}

char const * GuiSaveIniSettingsToMemory(size_t * out_ini_size)
{
    return ImGui::SaveIniSettingsToMemory(out_ini_size);
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

