/**
 * @file   RayGui.cpp
 * @brief  RayGui class implementation.
 * @author zer0
 * @date   2019-05-06
 */

#include <libtbag/ray/RayGui.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

// Auto-detect GL version
#if !defined(IMGUI_IMPL_OPENGL_ES2) && !defined(IMGUI_IMPL_OPENGL_ES3)
# if (defined(__APPLE__) && TARGET_OS_IOS) || (defined(__ANDROID__))
#  define IMGUI_IMPL_OPENGL_ES3       // iOS, Android  -> GL ES 3, "#version 300 es"
# elif defined(__EMSCRIPTEN__)
#  define IMGUI_IMPL_OPENGL_ES2       // Emscripten    -> GL ES 2, "#version 100"
# endif
#endif

#if defined(IMGUI_IMPL_OPENGL_ES2)
# include <GLES2/gl2.h>
#elif defined(IMGUI_IMPL_OPENGL_ES3)
# include <GLES3/gl3.h>  // Use GL ES 3
#else
# if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#  include <GL/gl3w.h>    // Needs to be initialized with gl3wInit() in user's code
# elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#  include <GL/glew.h>    // Needs to be initialized with glewInit() in user's code
# elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#  include <glad/glad.h>  // Needs to be initialized with gladLoadGL() in user's code
# else
#  include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
# endif
#endif

#include <cassert>
#include <cstdlib>
#include <cstdio>

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

        Image font_image = LoadImagePro(pixels, width, height, UNCOMPRESSED_R8G8B8A8);
        user->font_texture = LoadTextureFromImage(font_image);
        io.Fonts->TexID = reinterpret_cast<ImTextureID>(user->font_texture.id);
        UnloadImage(font_image);
    }

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    char shader_version[32] = {0,};
    snprintf(shader_version, 32, "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    auto const VERSIONS = libtbag::string::splitTokens(shader_version, ".");
    auto const GLSL_VERSION = libtbag::string::fformat("#version {}{} core", VERSIONS[0], VERSIONS[1]);

    //----------------------------------------
    // OpenGL    GLSL      GLSL
    // version   version   string
    //----------------------------------------
    //  2.0       110       "#version 110"
    //  2.1       120       "#version 120"
    //  3.0       130       "#version 130"
    //  3.1       140       "#version 140"
    //  3.2       150       "#version 150"
    //  3.3       330       "#version 330 core"
    //  4.0       400       "#version 400 core"
    //  4.1       410       "#version 410 core"
    //  4.2       420       "#version 410 core"
    //  4.3       430       "#version 430 core"
    //  ES 2.0    100       "#version 100"      = WebGL 1.0
    //  ES 3.0    300       "#version 300 es"   = WebGL 2.0
    //----------------------------------------
    ImGui_ImplOpenGL3_Init(GLSL_VERSION.c_str());
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

void GuiShowDemoWindow(bool * p_open)
{
    return ImGui::ShowDemoWindow(p_open);
}

bool GuiBegin(char const * text)
{
    return ImGui::Begin(text);
}

void GuiEnd()
{
    return ImGui::End();
}

void GuiText(char const * text)
{
    return ImGui::Text("%s", text);
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

