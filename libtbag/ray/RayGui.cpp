/**
 * @file   RayGui.cpp
 * @brief  RayGui class implementation.
 * @author zer0
 * @date   2019-05-06
 */

#include <libtbag/ray/RayGui.hpp>
#include <libtbag/Noncopyable.hpp>

#include <imgui.h>
#include <cassert>
#include <cstdlib>

#include <external/raylib/src/rlgl.h>

#if defined(_MSC_VER) && (_MSC_VER <= 1500) // MSVC 2008 or earlier
# include <stddef.h> // intptr_t
#else
# include <stdint.h> // intptr_t
#endif

#if defined(_WIN32) && !defined(APIENTRY)
// It is customary to use APIENTRY for OpenGL function pointer declarations on all platforms.
// Additionally, the Windows OpenGL header needs APIENTRY.
# define APIENTRY __stdcall
#endif

#if defined(_WIN32) && !defined(WINGDIAPI)
// Some Windows OpenGL headers need this
# define WINGDIAPI __declspec(dllimport)
#endif

#if defined(__APPLE__)
# ifndef GL_SILENCE_DEPRECATION
# define GL_SILENCE_DEPRECATION
# endif
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

struct RayGui final : private Noncopyable
{
    bool is_pad1 = false;
    bool is_pad2 = false;
    bool is_pad3 = false;
    bool is_pad4 = false;

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

bool InitRayGui()
{
    ImGuiContext * context = ImGui::CreateContext();
    assert(context != nullptr);

    ImGuiIO & io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // User data configuration.
    auto * user = new RayGui();
    assert(user != nullptr);
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
        unsigned char * pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        Image font_image = LoadImagePro(pixels, width, height, UNCOMPRESSED_R8G8B8A8);
        user->font_texture = LoadTextureFromImage(font_image);
        io.Fonts->TexID = reinterpret_cast<ImTextureID>(user->font_texture.id);
        UnloadImage(font_image);
    }

    return true;
}

void UpdateRayGui()
{
    ImGuiIO & io = ImGui::GetIO();

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

    io.DisplaySize.x = (float)GetScreenWidth();
    io.DisplaySize.y = (float)GetScreenHeight();
    io.DeltaTime = GetFrameTime();

    if (io.WantSetMousePos) {
        SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
    } else {
        io.MousePos = GetMousePosition();
    }

    ImGui::NewFrame();
}

static void ImGui_ImplOpenGL2_SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height)
{
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
    // you may need to backup/reset/restore current shader using the lines below. DO NOT MODIFY THIS FILE! Add the code in your calling function:
    //  GLint last_program;
    //  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    //  glUseProgram(0);
    //  ImGui_ImplOpenGL2_RenderDrawData(...);
    //  glUseProgram(last_program)

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x, draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

// OpenGL2 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
void ImGui_ImplOpenGL2_RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    // Backup GL state
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

    // Setup desired GL state
    ImGui_ImplOpenGL2_SetupRenderState(draw_data, fb_width, fb_height);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == nullptr)
                    ImGui_ImplOpenGL2_SetupRenderState(draw_data, fb_width, fb_height);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    // Apply scissor/clipping rectangle
                    glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glPolygonMode(GL_FRONT, (GLenum)last_polygon_mode[0]); glPolygonMode(GL_BACK, (GLenum)last_polygon_mode[1]);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void RenderRayGui()
{
    ImGui::Render();
    ImDrawData * draw_data = ImGui::GetDrawData();
    assert(draw_data != nullptr);

    if (draw_data->CmdListsCount == 0) {
        return;
    }

    ImGuiIO & io = ImGui::GetIO();

    // scale stuff (needed for proper handling of window resize)
    int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) {
        return;
    }
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    for (int n = 0; n < draw_data->CmdListsCount; ++n) {
        ImDrawList const * cmd_list = draw_data->CmdLists[n];
        unsigned char const * vtx_buffer = (unsigned char const *)&cmd_list->VtxBuffer.front();
        ImDrawIdx const * idx_buffer = &cmd_list->IdxBuffer.front();

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i) {
            ImDrawCmd const * pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                GLuint tex_id = (GLuint)*((unsigned int*)&pcmd->TextureId);
                glBindTexture(GL_TEXTURE_2D, tex_id);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
}

void ShutdownRayGui()
{
    ImGuiIO & io = ImGui::GetIO();

    auto * user = (RayGui*)io.UserData;
    assert(user != nullptr);
    UnloadTexture(user->font_texture);
    delete user;
    io.UserData = nullptr;

    // ImGui::GetIO().Fonts->TexID = nullptr;
    ImGui::DestroyContext();
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

