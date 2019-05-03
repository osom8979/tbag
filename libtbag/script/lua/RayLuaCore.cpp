/**
 * @file   RayLuaCore.cpp
 * @brief  RayLuaCore class implementation.
 * @author zer0
 * @date   2019-04-28
 */

#include <libtbag/script/lua/RayLuaCore.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

static int __lua_ray_InitWindow(lua_State * L)
{
    InitWindow(luaL_checkinteger(L, 1),
               luaL_checkinteger(L, 2),
               luaL_checkstring(L, 3));
    return 0;
}

static int __lua_ray_WindowShouldClose(lua_State * L)
{
    bool result = WindowShouldClose();
    lua_pushboolean(L, result);
    return 1;
}

static int __lua_ray_CloseWindow(lua_State * L)
{
    CloseWindow();
    return 0;
}

static int __lua_ray_IsWindowReady(lua_State * L)
{
    lua_pushboolean(L, IsWindowReady()?1:0);
    return 1;
}

static int __lua_ray_IsWindowMinimized(lua_State * L)
{
    lua_pushboolean(L, IsWindowMinimized()?1:0);
    return 1;
}

static int __lua_ray_IsWindowResized(lua_State * L)
{
    lua_pushboolean(L, IsWindowResized()?1:0);
    return 1;
}

static int __lua_ray_IsWindowHidden(lua_State * L)
{
    lua_pushboolean(L, IsWindowHidden()?1:0);
    return 1;
}

static int __lua_ray_ToggleFullscreen(lua_State * L)
{
    ToggleFullscreen();
    return 0;
}

static int __lua_ray_UnhideWindow(lua_State * L)
{
    UnhideWindow();
    return 0;
}

static int __lua_ray_HideWindow(lua_State * L)
{
    HideWindow();
    return 0;
}

static int __lua_ray_SetWindowIcon(lua_State * L)
{
    Image image;
    // TODO
    SetWindowIcon(image);
    return 0;
}

static int __lua_ray_SetWindowTitle(lua_State * L)
{
    SetWindowTitle(luaL_checkstring(L, 1));
    return 0;
}

static int __lua_ray_SetWindowPosition(lua_State * L)
{
    SetWindowPosition(luaL_checkinteger(L, 1),
                      luaL_checkinteger(L, 2));
    return 0;
}

static int __lua_ray_SetWindowMonitor(lua_State * L)
{
    SetWindowMonitor(luaL_checkinteger(L, 1));
    return 0;
}

static int __lua_ray_SetWindowMinSize(lua_State * L)
{
    SetWindowMinSize(luaL_checkinteger(L, 1),
                     luaL_checkinteger(L, 2));
    return 0;
}

static int __lua_ray_SetWindowSize(lua_State * L)
{
    SetWindowSize(luaL_checkinteger(L, 1),
                  luaL_checkinteger(L, 2));
    return 0;
}

static int __lua_ray_GetScreenWidth(lua_State * L)
{
    lua_pushinteger(L, GetScreenWidth());
    return 1;
}

static int __lua_ray_GetScreenHeight(lua_State * L)
{
    lua_pushinteger(L, GetScreenHeight());
    return 1;
}

static int __lua_ray_GetMonitorCount(lua_State * L)
{
    lua_pushinteger(L, GetMonitorCount());
    return 1;
}

static int __lua_ray_GetMonitorWidth(lua_State * L)
{
    lua_pushinteger(L, GetMonitorWidth(luaL_checkinteger(L, 1)));
    return 1;
}

static int __lua_ray_GetMonitorHeight(lua_State * L)
{
    lua_pushinteger(L, GetMonitorHeight(luaL_checkinteger(L, 1)));
    return 1;
}

static int __lua_ray_GetMonitorPhysicalWidth(lua_State * L)
{
    lua_pushinteger(L, GetMonitorPhysicalWidth(luaL_checkinteger(L, 1)));
    return 1;
}

static int __lua_ray_GetMonitorPhysicalHeight(lua_State * L)
{
    lua_pushinteger(L, GetMonitorPhysicalHeight(luaL_checkinteger(L, 1)));
    return 1;
}

static int __lua_ray_GetMonitorName(lua_State * L)
{
    lua_pushstring(L, GetMonitorName(luaL_checkinteger(L, 1)));
    return 1;
}

static int __lua_ray_GetClipboardText(lua_State * L)
{
    lua_pushstring(L, GetClipboardText());
    return 1;
}

static int __lua_ray_SetClipboardText(lua_State * L)
{
    SetClipboardText(luaL_checkstring(L, 1));
    return 0;
}

static int __lua_ray_ShowCursor(lua_State * L)
{
    ShowCursor();
    return 0;
}

static int __lua_ray_HideCursor(lua_State * L)
{
    HideCursor();
    return 0;
}

static int __lua_ray_IsCursorHidden(lua_State * L)
{
    lua_pushboolean(L, IsCursorHidden()?1:0);
    return 1;
}

static int __lua_ray_EnableCursor(lua_State * L)
{
    EnableCursor();
    return 0;
}

static int __lua_ray_DisableCursor(lua_State * L)
{
    DisableCursor();
    return 0;
}

static int __lua_ray_ClearBackground(lua_State * L)
{
    ClearBackground(lua_ray_getcolor(L, 1));
    return 0;
}

static int __lua_ray_BeginDrawing(lua_State * L)
{
    BeginDrawing();
    return 0;
}

static int __lua_ray_EndDrawing(lua_State * L)
{
    EndDrawing();
    return 0;
}

//void BeginMode2D(Camera2D camera);
//void EndMode2D();
//void BeginMode3D(Camera3D camera);
//void EndMode3D();
//void BeginTextureMode(RenderTexture2D target);
//void EndTextureMode();

//Ray GetMouseRay(Vector2 mouse_position, Camera camera);
//Vector2 GetWorldToScreen(Vector3 position, Camera camera);
//Matrix GetCameraMatrix(Camera camera);

static int __lua_ray_SetTargetFPS(lua_State * L)
{
    SetTargetFPS(luaL_checkinteger(L, 1));
    return 0;
}

static int __lua_ray_GetFPS(lua_State * L)
{
    lua_pushinteger(L, GetFPS());
    return 1;
}

static int __lua_ray_GetFrameTime(lua_State * L)
{
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

static int __lua_ray_GetTime(lua_State * L)
{
    lua_pushnumber(L, GetTime());
    return 1;
}

//int ColorToInt(Color color);
//Vector4 ColorNormalize(Color color);
//Vector3 ColorToHSV(Color color);
//Color ColorFromHSV(Vector3 hsv);
//Color GetColor(int hex_value);
//Color Fade(Color color, float alpha);

//void SetConfigFlags(unsigned char flags);
//void SetTraceLogLevel(int log_type);
//void SetTraceLogExit(int log_type);
//void SetTraceLogCallback(TraceLogCallback callback);
//// void TraceLog(int log_type, char const * text, ...);

//void TakeScreenshot(char const * file_name);
//int GetRandomValue(int min, int max);

//bool FileExists(char const * file_name);
//bool IsFileExtension(char const * file_name, char const * ext);
//char const * GetExtension(char const * file_name);
//char const * GetFileName(char const * file_path);
//char const * GetFileNameWithoutExt(char const * file_path);
//char const * GetDirectoryPath(char const * file_name);
//char const * GetWorkingDirectory();
//char ** GetDirectoryFiles(char const * dir_path, int * count);
//void ClearDirectoryFiles();
//bool ChangeDirectory(char const * dir);
//bool IsFileDropped();
//char ** GetDroppedFiles(int * count);
//void ClearDroppedFiles();
//long GetFileModTime(char const * file_name);

//void StorageSaveValue(int position, int value);
//int StorageLoadValue(int position);

//void OpenURL(char const * url);

//bool IsKeyPressed(int key);
//bool IsKeyDown(int key);
//bool IsKeyReleased(int key);
//bool IsKeyUp(int key);
//int GetKeyPressed();
//void SetExitKey(int key);

//bool IsGamepadAvailable(int gamepad);
//bool IsGamepadName(int gamepad, char const * name);
//char const * GetGamepadName(int gamepad);
//bool IsGamepadButtonPressed(int gamepad, int button);
//bool IsGamepadButtonDown(int gamepad, int button);
//bool IsGamepadButtonReleased(int gamepad, int button);
//bool IsGamepadButtonUp(int gamepad, int button);
//int GetGamepadButtonPressed();
//int GetGamepadAxisCount(int gamepad);
//float GetGamepadAxisMovement(int gamepad, int axis);

//bool IsMouseButtonPressed(int button);
//bool IsMouseButtonDown(int button);
//bool IsMouseButtonReleased(int button);
//bool IsMouseButtonUp(int button);
//int GetMouseX();
//int GetMouseY();
//Vector2 GetMousePosition();
//void SetMousePosition(int x, int y);
//void SetMouseOffset(int offset_x, int offset_y);
//void SetMouseScale(float scale_x, float scale_y);
//int GetMouseWheelMove();

//int GetTouchX();
//int GetTouchY();
//Vector2 GetTouchPosition(int index);

//void SetGesturesEnabled(unsigned int gesture_flags);
//bool IsGestureDetected(int gesture);
//int GetGestureDetected();
//int GetTouchPointsCount();
//float GetGestureHoldDuration();
//Vector2 GetGestureDragVector();
//float GetGestureDragAngle();
//Vector2 GetGesturePinchVector();
//float GetGesturePinchAngle();

//void SetCameraMode(Camera camera, int mode);
//void UpdateCamera(Camera * camera);

//void SetCameraPanControl(int pan_key);
//void SetCameraAltControl(int alt_key);
//void SetCameraSmoothZoomControl(int sz_key);
//void SetCameraMoveControls(int front_key, int back_key, int right_key, int left_key, int up_key, int down_key);

static luaL_Reg const __lua_lay_core[] = {
        // Window-related functions
        TBAG_LUA_REGISTER(InitWindow),
        TBAG_LUA_REGISTER(WindowShouldClose),
        TBAG_LUA_REGISTER(CloseWindow),
        TBAG_LUA_REGISTER(IsWindowReady),
        TBAG_LUA_REGISTER(IsWindowMinimized),
        TBAG_LUA_REGISTER(IsWindowResized),
        TBAG_LUA_REGISTER(IsWindowHidden),
        TBAG_LUA_REGISTER(ToggleFullscreen),
        TBAG_LUA_REGISTER(UnhideWindow),
        TBAG_LUA_REGISTER(HideWindow),
        TBAG_LUA_REGISTER(SetWindowIcon),
        TBAG_LUA_REGISTER(SetWindowTitle),
        TBAG_LUA_REGISTER(SetWindowPosition),
        TBAG_LUA_REGISTER(SetWindowMonitor),
        TBAG_LUA_REGISTER(SetWindowMinSize),
        TBAG_LUA_REGISTER(SetWindowSize),
        TBAG_LUA_REGISTER(GetScreenWidth),
        TBAG_LUA_REGISTER(GetScreenHeight),
        TBAG_LUA_REGISTER(GetMonitorCount),
        TBAG_LUA_REGISTER(GetMonitorWidth),
        TBAG_LUA_REGISTER(GetMonitorHeight),
        TBAG_LUA_REGISTER(GetMonitorPhysicalWidth),
        TBAG_LUA_REGISTER(GetMonitorPhysicalHeight),
        TBAG_LUA_REGISTER(GetMonitorName),
        TBAG_LUA_REGISTER(GetClipboardText),
        TBAG_LUA_REGISTER(SetClipboardText),
        // Cursor-related functions
        TBAG_LUA_REGISTER(ShowCursor),
        TBAG_LUA_REGISTER(HideCursor),
        TBAG_LUA_REGISTER(IsCursorHidden),
        TBAG_LUA_REGISTER(EnableCursor),
        TBAG_LUA_REGISTER(DisableCursor),
        // Drawing-related functions
        TBAG_LUA_REGISTER(ClearBackground),
        TBAG_LUA_REGISTER(BeginDrawing),
        TBAG_LUA_REGISTER(EndDrawing),
        //TBAG_LUA_REGISTER(BeginMode2D),
        //TBAG_LUA_REGISTER(EndMode2D),
        //TBAG_LUA_REGISTER(BeginMode3D),
        //TBAG_LUA_REGISTER(EndMode3D),
        //TBAG_LUA_REGISTER(BeginTextureMode),
        //TBAG_LUA_REGISTER(EndTextureMode),
        // Screen-space-related functions
        //TBAG_LUA_REGISTER(GetMouseRay),
        //TBAG_LUA_REGISTER(GetWorldToScreen),
        //TBAG_LUA_REGISTER(GetCameraMatrix),
        // Timing-related functions
        TBAG_LUA_REGISTER(SetTargetFPS),
        TBAG_LUA_REGISTER(GetFPS),
        TBAG_LUA_REGISTER(GetFrameTime),
        TBAG_LUA_REGISTER(GetTime),
        { nullptr, nullptr }
};

bool luaopen_ray_core(lua_State * L)
{
    luaL_register(L, lua_ray_name(), __lua_lay_core);
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

