/**
 * @file   RayBypass.cpp
 * @brief  RayBypass class implementation.
 * @author zer0
 * @date   2019-04-20
 */

#include <raylib.h>
#include <libtbag/ray/RayBypass.hpp>

#include <cstdarg>
#include <cstdio>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

void InitWindow(int width, int height, char const * title)
{
    ::InitWindow(width, height, title);
}

bool WindowShouldClose()
{
    return ::WindowShouldClose();
}

void CloseWindow()
{
    // Change symbol name: CloseWindow -> CloseWindow_
    // [REASON] LNK2005 CloseWindow already defined in user32.lib(USER32.dll)
    ::CloseWindow_();
}

bool IsWindowReady()
{
    return ::IsWindowReady();
}

bool IsWindowMinimized()
{
    return ::IsWindowMinimized();
}

bool IsWindowResized()
{
    return ::IsWindowResized();
}

bool IsWindowHidden()
{
    return ::IsWindowHidden();
}

void ToggleFullscreen()
{
    ::ToggleFullscreen();
}

void UnhideWindow()
{
    ::UnhideWindow_();
}

void HideWindow()
{
    ::HideWindow();
}

void SetWindowIcon(Image image)
{
    ::SetWindowIcon(image);
}

void SetWindowTitle(char const * title)
{
    ::SetWindowTitle(title);
}

void SetWindowPosition(int x, int y)
{
    ::SetWindowPosition(x, y);
}

void SetWindowMonitor(int monitor)
{
    ::SetWindowMonitor(monitor);
}

void SetWindowMinSize(int width, int height)
{
    ::SetWindowMinSize(width, height);
}

void SetWindowSize(int width, int height)
{
    ::SetWindowSize(width, height);
}

void * GetWindowHandle()
{
    return ::GetWindowHandle();
}

int GetScreenWidth()
{
    return ::GetScreenWidth();
}

int GetScreenHeight()
{
    return ::GetScreenHeight();
}

int GetMonitorCount()
{
    return ::GetMonitorCount();
}

int GetMonitorWidth(int monitor)
{
    return ::GetMonitorWidth(monitor);
}

int GetMonitorHeight(int monitor)
{
    return ::GetMonitorHeight(monitor);
}

int GetMonitorPhysicalWidth(int monitor)
{
    return ::GetMonitorPhysicalWidth(monitor);
}

int GetMonitorPhysicalHeight(int monitor)
{
    return ::GetMonitorPhysicalHeight(monitor);
}

char const * GetMonitorName(int monitor)
{
    return ::GetMonitorName(monitor);
}

char const * GetClipboardText()
{
    return ::GetClipboardText();
}

void SetClipboardText(char const * text)
{
    ::SetClipboardText(text);
}

void ShowCursor()
{
    // Change symbol name: ShowCursor -> ShowCursor_
    // [REASON] LNK2005 ShowCursor already defined in user32.lib(USER32.dll)
    ::ShowCursor_();
}

void HideCursor()
{
    ::HideCursor();
}

bool IsCursorHidden()
{
    return ::IsCursorHidden();
}

void EnableCursor()
{
    ::EnableCursor();
}

void DisableCursor()
{
    ::DisableCursor();
}

void ClearBackground(Color color)
{
    ::ClearBackground(color);
}

void BeginDrawing()
{
    ::BeginDrawing();
}

void SetEndDrawingCallback(void(*cb)(void))
{
    ::SetEndDrawingCallback(cb);
}

void EndDrawing()
{
    ::EndDrawing();
}

void BeginMode2D(Camera2D camera)
{
    ::BeginMode2D(camera);
}

void EndMode2D()
{
    ::EndMode2D();
}

void BeginMode3D(Camera3D camera)
{
    ::BeginMode3D(camera);
}

void EndMode3D()
{
    ::EndMode3D();
}

void BeginTextureMode(RenderTexture2D target)
{
    ::BeginTextureMode(target);
}

void EndTextureMode()
{
    ::EndTextureMode();
}

Ray GetMouseRay(Vector2 mouse_position, Camera camera)
{
    return ::GetMouseRay(mouse_position, camera);
}

Vector2 GetWorldToScreen(Vector3 position, Camera camera)
{
    return ::GetWorldToScreen(position, camera);
}

Matrix GetCameraMatrix(Camera camera)
{
    return ::GetCameraMatrix(camera);
}

void SetTargetFPS(int fps)
{
    ::SetTargetFPS(fps);
}

int GetFPS()
{
    return ::GetFPS();
}

float GetFrameTime()
{
    return ::GetFrameTime();
}

double GetTime()
{
    return ::GetTime();
}

int ColorToInt(Color color)
{
    return ::ColorToInt(color);
}

Vector4 ColorNormalize(Color color)
{
    return ::ColorNormalize(color);
}

Vector3 ColorToHSV(Color color)
{
    return ::ColorToHSV(color);
}

Color ColorFromHSV(Vector3 hsv)
{
    return ::ColorFromHSV(hsv);
}

Color GetColor(int hex_value)
{
    return ::GetColor(hex_value);
}

Color Fade(Color color, float alpha)
{
    return ::Fade(color, alpha);
}

void SetConfigFlags(unsigned char flags)
{
    ::SetConfigFlags(flags);
}

void SetTraceLogLevel(int log_type)
{
    ::SetTraceLogLevel(log_type);
}

void SetTraceLogExit(int log_type)
{
    ::SetTraceLogExit(log_type);
}

void SetTraceLogCallback(TraceLogCallback callback)
{
    ::SetTraceLogCallback(callback);
}

void TraceLog(int log_type, char const * text, ...)
{
    char buffer[MAX_TRACELOG_BUFFER_SIZE] = {0,};
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    ::TraceLog(log_type, buffer);
}

void TakeScreenshot(char const * file_name)
{
    ::TakeScreenshot(file_name);
}

int GetRandomValue(int min, int max)
{
    return ::GetRandomValue(min, max);
}

bool FileExists(char const * file_name)
{
    return ::FileExists(file_name);
}

bool IsFileExtension(char const * file_name, char const * ext)
{
    return ::IsFileExtension(file_name, ext);
}

char const * GetExtension(char const * file_name)
{
    return ::GetExtension(file_name);
}

char const * GetFileName(char const * file_path)
{
    return ::GetFileName(file_path);
}

char const * GetFileNameWithoutExt(char const * file_path)
{
    return ::GetFileNameWithoutExt(file_path);
}

char const * GetDirectoryPath(char const * file_name)
{
    return ::GetDirectoryPath(file_name);
}

char const * GetWorkingDirectory()
{
    return ::GetWorkingDirectory();
}

char ** GetDirectoryFiles(char const * dir_path, int * count)
{
    return ::GetDirectoryFiles(dir_path, count);
}

void ClearDirectoryFiles()
{
    ::ClearDirectoryFiles();
}

bool ChangeDirectory(char const * dir)
{
    return ::ChangeDirectory(dir);
}

bool IsFileDropped()
{
    return ::IsFileDropped();
}

char ** GetDroppedFiles(int * count)
{
    return ::GetDroppedFiles(count);
}

void ClearDroppedFiles()
{
    ::ClearDroppedFiles();
}

long GetFileModTime(char const * file_name)
{
    return ::GetFileModTime(file_name);
}

void StorageSaveValue(int position, int value)
{
    ::StorageSaveValue(position, value);
}

int StorageLoadValue(int position)
{
    return ::StorageLoadValue(position);
}

void OpenURL(char const * url)
{
    ::OpenURL(url);
}

bool IsKeyPressed(int key)
{
    return ::IsKeyPressed(key);
}

bool IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool IsKeyReleased(int key)
{
    return ::IsKeyReleased(key);
}

bool IsKeyUp(int key)
{
    return ::IsKeyUp(key);
}

int GetKeyPressed()
{
    return ::GetKeyPressed();
}

void SetExitKey(int key)
{
    ::SetExitKey(key);
}

bool IsGamepadAvailable(int gamepad)
{
    return ::IsGamepadAvailable(gamepad);
}

bool IsGamepadName(int gamepad, char const * name)
{
    return ::IsGamepadName(gamepad, name);
}

char const * GetGamepadName(int gamepad)
{
    return ::GetGamepadName(gamepad);
}

bool IsGamepadButtonPressed(int gamepad, int button)
{
    return ::IsGamepadButtonPressed(gamepad, button);
}

bool IsGamepadButtonDown(int gamepad, int button)
{
    return ::IsGamepadButtonDown(gamepad, button);
}

bool IsGamepadButtonReleased(int gamepad, int button)
{
    return ::IsGamepadButtonReleased(gamepad, button);
}

bool IsGamepadButtonUp(int gamepad, int button)
{
    return ::IsGamepadButtonUp(gamepad, button);
}

int GetGamepadButtonPressed()
{
    return ::GetGamepadButtonPressed();
}

int GetGamepadAxisCount(int gamepad)
{
    return ::GetGamepadAxisCount(gamepad);
}

float GetGamepadAxisMovement(int gamepad, int axis)
{
    return ::GetGamepadAxisMovement(gamepad, axis);
}

bool IsMouseButtonPressed(int button)
{
    return ::IsMouseButtonPressed(button);
}

bool IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

bool IsMouseButtonReleased(int button)
{
    return ::IsMouseButtonReleased(button);
}

bool IsMouseButtonUp(int button)
{
    return ::IsMouseButtonUp(button);
}

int GetMouseX()
{
    return ::GetMouseX();
}

int GetMouseY()
{
    return ::GetMouseY();
}

Vector2 GetMousePosition()
{
    return ::GetMousePosition();
}

void SetMousePosition(int x, int y)
{
    ::SetMousePosition(x, y);
}

void SetMouseOffset(int offset_x, int offset_y)
{
    ::SetMouseOffset(offset_x, offset_y);
}

void SetMouseScale(float scale_x, float scale_y)
{
    ::SetMouseScale(scale_x, scale_y);
}

int GetMouseWheelMove()
{
    return ::GetMouseWheelMove();
}

int GetTouchX()
{
    return ::GetTouchX();
}

int GetTouchY()
{
    return ::GetTouchY();
}

Vector2 GetTouchPosition(int index)
{
    return ::GetTouchPosition(index);
}

void SetGesturesEnabled(unsigned int gesture_flags)
{
    ::SetGesturesEnabled(gesture_flags);
}

bool IsGestureDetected(int gesture)
{
    return ::IsGestureDetected(gesture);
}

int GetGestureDetected()
{
    return ::GetGestureDetected();
}

int GetTouchPointsCount()
{
    return ::GetTouchPointsCount();
}

float GetGestureHoldDuration()
{
    return ::GetGestureHoldDuration();
}

Vector2 GetGestureDragVector()
{
    return ::GetGestureDragVector();
}

float GetGestureDragAngle()
{
    return ::GetGestureDragAngle();
}

Vector2 GetGesturePinchVector()
{
    return ::GetGesturePinchVector();
}

float GetGesturePinchAngle()
{
    return ::GetGesturePinchAngle();
}

void SetCameraMode(Camera camera, int mode)
{
    ::SetCameraMode(camera, mode);
}

void UpdateCamera(Camera * camera)
{
    ::UpdateCamera(camera);
}

void SetCameraPanControl(int pan_key)
{
    ::SetCameraPanControl(pan_key);
}

void SetCameraAltControl(int alt_key)
{
    ::SetCameraAltControl(alt_key);
}

void SetCameraSmoothZoomControl(int sz_key)
{
    ::SetCameraSmoothZoomControl(sz_key);
}

void SetCameraMoveControls(int front_key, int back_key, int right_key, int left_key, int up_key, int down_key)
{
    ::SetCameraMoveControls(front_key, back_key, right_key, left_key, up_key, down_key);
}

void DrawPixel(int pos_x, int pos_y, Color color)
{
    ::DrawPixel(pos_x, pos_y, color);
}

void DrawPixelV(Vector2 position, Color color)
{
    ::DrawPixelV(position, color);
}

void DrawLine(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y, Color color)
{
    ::DrawLine(start_pos_x, start_pos_y, end_pos_x, end_pos_y, color);
}

void DrawLineV(Vector2 start_pos, Vector2 end_pos, Color color)
{
    ::DrawLineV(start_pos, end_pos, color);
}

void DrawLineEx(Vector2 start_pos, Vector2 end_pos, float thick, Color color)
{
    ::DrawLineEx(start_pos, end_pos, thick, color);
}

void DrawLineBezier(Vector2 start_pos, Vector2 end_pos, float thick, Color color)
{
    ::DrawLineBezier(start_pos, end_pos, thick, color);
}

void DrawCircle(int center_x, int center_y, float radius, Color color)
{
    ::DrawCircle(center_x, center_y, radius, color);
}

void DrawCircleSector(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color)
{
    ::DrawCircleSector(center, radius, start_angle, end_angle, segments, color);
}

void DrawCircleSectorLines(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color)
{
    ::DrawCircleSectorLines(center, radius, start_angle, end_angle, segments, color);
}

void DrawCircleGradient(int center_x, int center_y, float radius, Color color1, Color color2)
{
    ::DrawCircleGradient(center_x, center_y, radius, color1, color2);
}

void DrawCircleV(Vector2 center, float radius, Color color)
{
    ::DrawCircleV(center, radius, color);
}

void DrawCircleLines(int center_x, int center_y, float radius, Color color)
{
    ::DrawCircleLines(center_x, center_y, radius, color);
}

void DrawRing(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle,
              int segments, Color color)
{
    ::DrawRing(center, inner_radius, outer_radius, start_angle, end_angle, segments, color);
}

void DrawRingLines(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle,
                   int segments, Color color)
{
    ::DrawRingLines(center, inner_radius, outer_radius, start_angle, end_angle, segments, color);
}

void DrawRectangle(int pos_x, int pos_y, int width, int height, Color color)
{
    ::DrawRectangle(pos_x, pos_y, width, height, color);
}

void DrawRectangleV(Vector2 position, Vector2 size, Color color)
{
    ::DrawRectangleV(position, size, color);
}

void DrawRectangleRec(Rectangle2 rec, Color color)
{
    ::DrawRectangleRec(rec, color);
}

void DrawRectanglePro(Rectangle2 rec, Vector2 origin, float rotation, Color color)
{
    ::DrawRectanglePro(rec, origin, rotation, color);
}

void DrawRectangleGradientV(int pos_x, int pos_y, int width, int height, Color color1, Color color2)
{
    ::DrawRectangleGradientV(pos_x, pos_y, width, height, color1, color2);
}

void DrawRectangleGradientH(int pos_x, int pos_y, int width, int height, Color color1, Color color2)
{
    ::DrawRectangleGradientH(pos_x, pos_y, width, height, color1, color2);
}

void DrawRectangleGradientEx(Rectangle2 rec, Color col1, Color col2, Color col3, Color col4)
{
    ::DrawRectangleGradientEx(rec, col1, col2, col3, col4);
}

void DrawRectangleLines(int pos_x, int pos_y, int width, int height, Color color)
{
    ::DrawRectangleLines(pos_x, pos_y, width, height, color);
}

void DrawRectangleLinesEx(Rectangle2 rec, int line_thick, Color color)
{
    ::DrawRectangleLinesEx(rec, line_thick, color);
}

void DrawRectangleRounded(Rectangle2 rec, float roundness, int segments, Color color)
{
    ::DrawRectangleRounded(rec, roundness, segments, color);
}

void DrawRectangleRoundedLines(Rectangle2 rec, float roundness, int segments, int line_thick, Color color)
{
    ::DrawRectangleRoundedLines(rec, roundness, segments, line_thick, color);
}

void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
{
    ::DrawTriangle(v1, v2, v3, color);
}

void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color)
{
    ::DrawTriangleLines(v1, v2, v3, color);
}

void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)
{
    ::DrawPoly(center, sides, radius, rotation, color);
}

void SetShapesTexture(Texture2D texture, Rectangle2 source)
{
    ::SetShapesTexture(texture, source);
}

bool CheckCollisionRecs(Rectangle2 rec1, Rectangle2 rec2)
{
    return ::CheckCollisionRecs(rec1, rec2);
}

bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
    return ::CheckCollisionCircles(center1, radius1, center2, radius2);
}

bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle2 rec)
{
    return ::CheckCollisionCircleRec(center, radius, rec);
}

Rectangle2 GetCollisionRec(Rectangle2 rec1, Rectangle2 rec2)
{
    return ::GetCollisionRec(rec1, rec2);
}

bool CheckCollisionPointRec(Vector2 point, Rectangle2 rec)
{
    return ::CheckCollisionPointRec(point, rec);
}

bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius)
{
    return ::CheckCollisionPointCircle(point, center, radius);
}

bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3)
{
    return ::CheckCollisionPointTriangle(point, p1, p2, p3);
}

Image LoadImage(char const * file_name)
{
    return ::LoadImage_(file_name);
}

Image LoadImageEx(Color * pixels, int width, int height)
{
    return ::LoadImageEx(pixels, width, height);
}

Image LoadImagePro(void * data, int width, int height, int format)
{
    return ::LoadImagePro(data, width, height, format);
}

Image LoadImageRaw(char const * file_name, int width, int height, int format, int header_size)
{
    return ::LoadImageRaw(file_name, width, height, format, header_size);
}

void ExportImage(Image image, char const * file_name)
{
    ::ExportImage(image, file_name);
}

void ExportImageAsCode(Image image, char const * file_name)
{
    ::ExportImageAsCode(image, file_name);
}

Texture2D LoadTexture(char const * file_name)
{
    return ::LoadTexture(file_name);
}

Texture2D LoadTextureFromImage(Image image)
{
    return ::LoadTextureFromImage(image);
}

TextureCubemap LoadTextureCubemap(Image image, int layout_type)
{
    return ::LoadTextureCubemap(image, layout_type);
}

RenderTexture2D LoadRenderTexture(int width, int height)
{
    return ::LoadRenderTexture(width, height);
}

void UnloadImage(Image image)
{
    ::UnloadImage(image);
}

void UnloadTexture(Texture2D texture)
{
    ::UnloadTexture(texture);
}

void UnloadRenderTexture(RenderTexture2D target)
{
    ::UnloadRenderTexture(target);
}

Color * GetImageData(Image image)
{
    return ::GetImageData(image);
}

Vector4 * GetImageDataNormalized(Image image)
{
    return ::GetImageDataNormalized(image);
}

int GetPixelDataSize(int width, int height, int format)
{
    return ::GetPixelDataSize(width, height, format);
}

Image GetTextureData(Texture2D texture)
{
    return ::GetTextureData(texture);
}

Image GetScreenData()
{
    return ::GetScreenData();
}

void UpdateTexture(Texture2D texture, void const * pixels)
{
    ::UpdateTexture(texture, pixels);
}

Image ImageCopy(Image image)
{
    return ::ImageCopy(image);
}

void ImageToPOT(Image * image, Color fill_color)
{
    ::ImageToPOT(image, fill_color);
}

void ImageFormat(Image * image, int new_format)
{
    ::ImageFormat(image, new_format);
}

void ImageAlphaMask(Image * image, Image alpha_mask)
{
    ::ImageAlphaMask(image, alpha_mask);
}

void ImageAlphaClear(Image * image, Color color, float threshold)
{
    ::ImageAlphaClear(image, color, threshold);
}

void ImageAlphaCrop(Image * image, float threshold)
{
    ::ImageAlphaCrop(image, threshold);
}

void ImageAlphaPremultiply(Image * image)
{
    ::ImageAlphaPremultiply(image);
}

void ImageCrop(Image * image, Rectangle2 crop)
{
    ::ImageCrop(image, crop);
}

void ImageResize(Image * image, int new_width, int new_height)
{
    ::ImageResize(image, new_width, new_height);
}

void ImageResizeNN(Image * image, int new_width, int new_height)
{
    ::ImageResizeNN(image, new_width, new_height);
}

void ImageResizeCanvas(Image * image, int new_width, int new_height, int offset_x, int offset_y, Color color)
{
    ::ImageResizeCanvas(image, new_width, new_height, offset_x, offset_y, color);
}

void ImageMipmaps(Image * image)
{
    ::ImageMipmaps(image);
}

void ImageDither(Image * image, int r_bpp, int g_bpp, int b_bpp, int a_bpp)
{
    ::ImageDither(image, r_bpp, g_bpp, b_bpp, a_bpp);
}

Color * ImageExtractPalette(Image image, int maxPalette_size, int * extract_count)
{
    return ::ImageExtractPalette(image, maxPalette_size, extract_count);
}

Image ImageText(char const * text, int font_size, Color color)
{
    return ::ImageText(text, font_size, color);
}

Image ImageTextEx(Font font, char const * text, float font_size, float spacing, Color tint)
{
    return ::ImageTextEx(font, text, font_size, spacing, tint);
}

void ImageDraw(Image * dst, Image src, Rectangle2 src_rec, Rectangle2 dst_rec)
{
    ::ImageDraw(dst, src, src_rec, dst_rec);
}

void ImageDrawRectangle(Image * dst, Rectangle2 rec, Color color)
{
    ::ImageDrawRectangle(dst, rec, color);
}

void ImageDrawRectangleLines(Image * dst, Rectangle2 rec, int thick, Color color)
{
    ::ImageDrawRectangleLines(dst, rec, thick, color);
}

void ImageDrawText(Image * dst, Vector2 position, char const * text, int font_size, Color color)
{
    ::ImageDrawText(dst, position, text, font_size, color);
}

void ImageDrawTextEx(Image * dst, Vector2 position, Font font, char const * text, float font_size,
                     float spacing, Color color)
{
    ::ImageDrawTextEx(dst, position, font, text, font_size, spacing, color);
}

void ImageFlipVertical(Image * image)
{
    ::ImageFlipVertical(image);
}

void ImageFlipHorizontal(Image * image)
{
    ::ImageFlipHorizontal(image);
}

void ImageRotateCW(Image * image)
{
    ::ImageRotateCW(image);
}

void ImageRotateCCW(Image * image)
{
    ::ImageRotateCCW(image);
}

void ImageColorTint(Image * image, Color color)
{
    ::ImageColorTint(image, color);
}

void ImageColorInvert(Image * image)
{
    ::ImageColorInvert(image);
}

void ImageColorGrayscale(Image * image)
{
    ::ImageColorGrayscale(image);
}

void ImageColorContrast(Image * image, float contrast)
{
    ::ImageColorContrast(image, contrast);
}

void ImageColorBrightness(Image * image, int brightness)
{
    ::ImageColorBrightness(image, brightness);
}

void ImageColorReplace(Image * image, Color color, Color replace)
{
    ::ImageColorReplace(image, color, replace);
}

Image GenImageColor(int width, int height, Color color)
{
    return ::GenImageColor(width, height, color);
}

Image GenImageGradientV(int width, int height, Color top, Color bottom)
{
    return ::GenImageGradientV(width, height, top, bottom);
}

Image GenImageGradientH(int width, int height, Color left, Color right)
{
    return ::GenImageGradientH(width, height, left, right);
}

Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer)
{
    return ::GenImageGradientRadial(width, height, density, inner, outer);
}

Image GenImageChecked(int width, int height, int checks_x, int checks_y, Color col1, Color col2)
{
    return ::GenImageChecked(width, height, checks_x, checks_y, col1, col2);
}

Image GenImageWhiteNoise(int width, int height, float factor)
{
    return ::GenImageWhiteNoise(width, height, factor);
}

Image GenImagePerlinNoise(int width, int height, int offset_x, int offset_y, float scale)
{
    return ::GenImagePerlinNoise(width, height, offset_x, offset_y, scale);
}

Image GenImageCellular(int width, int height, int tile_size)
{
    return ::GenImageCellular(width, height, tile_size);
}

void GenTextureMipmaps(Texture2D * texture)
{
    ::GenTextureMipmaps(texture);
}

void SetTextureFilter(Texture2D texture, int filter_mode)
{
    ::SetTextureFilter(texture, filter_mode);
}

void SetTextureWrap(Texture2D texture, int wrap_mode)
{
    ::SetTextureWrap(texture, wrap_mode);
}

void DrawTexture(Texture2D texture, int pos_x, int pos_y, Color tint)
{
    ::DrawTexture(texture, pos_x, pos_y, tint);
}

void DrawTextureV(Texture2D texture, Vector2 position, Color tint)
{
    ::DrawTextureV(texture, position, tint);
}

void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint)
{
    ::DrawTextureEx(texture, position, rotation, scale, tint);
}

void DrawTextureRec(Texture2D texture, Rectangle2 source_rec, Vector2 position, Color tint)
{
    ::DrawTextureRec(texture, source_rec, position, tint);
}

void DrawTextureQuad(Texture2D texture, Vector2 tiling, Vector2 offset, Rectangle2 quad, Color tint)
{
    ::DrawTextureQuad(texture, tiling, offset, quad, tint);
}

void DrawTexturePro(Texture2D texture, Rectangle2 source_rec, Rectangle2 dest_rec, Vector2 origin,
                    float rotation, Color tint)
{
    ::DrawTexturePro(texture, source_rec, dest_rec, origin, rotation, tint);
}

void DrawTextureNPatch(Texture2D texture, NPatchInfo n_patch_info, Rectangle2 dest_rec, Vector2 origin,
                       float rotation, Color tint)
{
    ::DrawTextureNPatch(texture, n_patch_info, dest_rec, origin, rotation, tint);
}

Font GetFontDefault()
{
    return ::GetFontDefault();
}

Font LoadFont(char const * file_name)
{
    return ::LoadFont(file_name);
}

Font LoadFontEx(char const * file_name, int font_size, int * font_chars, int chars_count)
{
    return ::LoadFontEx(file_name, font_size, font_chars, chars_count);
}

Font LoadFontFromImage(Image image, Color key, int first_char)
{
    return ::LoadFontFromImage(image, key, first_char);
}

CharInfo * LoadFontData(char const * file_name, int font_size, int * font_chars, int chars_count, int type)
{
    return ::LoadFontData(file_name, font_size, font_chars, chars_count, type);
}

Image GenImageFontAtlas(CharInfo * chars, int chars_count, int font_size, int padding, int pack_method)
{
    return ::GenImageFontAtlas(chars, chars_count, font_size, padding, pack_method);
}

void UnloadFont(Font font)
{
    ::UnloadFont(font);
}

void DrawFPS(int pos_x, int pos_y)
{
    ::DrawFPS(pos_x, pos_y);
}

void DrawText(char const * text, int pos_x, int pos_y, int font_size, Color color)
{
    ::DrawText_(text, pos_x, pos_y, font_size, color);
}

void DrawTextEx(Font font, char const * text, Vector2 position, float font_size, float spacing, Color tint)
{
    ::DrawTextEx_(font, text, position, font_size, spacing, tint);
}

void DrawTextRec(Font font, char const * text, Rectangle2 rec, float font_size, float spacing,
                 bool word_wrap, Color tint)
{
    ::DrawTextRec(font, text, rec, font_size, spacing, word_wrap, tint);
}

void DrawTextRecEx(Font font, char const * text, Rectangle2 rec, float font_size, float spacing, bool word_wrap,
                   Color tint, int select_start, int select_length, Color select_text, Color select_back)
{
    ::DrawTextRecEx(font, text, rec, font_size, spacing, word_wrap, tint,
                    select_start, select_length, select_text, select_back);
}

int MeasureText(char const * text, int font_size)
{
    return ::MeasureText(text, font_size);
}

Vector2 MeasureTextEx(Font font, char const * text, float font_size, float spacing)
{
    return ::MeasureTextEx(font, text, font_size, spacing);
}

int GetGlyphIndex(Font font, int character)
{
    return ::GetGlyphIndex(font, character);
}

bool TextIsEqual(char const * text1, char const * text2)
{
    return ::TextIsEqual(text1, text2);
}

unsigned int TextLength(char const * text)
{
    return ::TextLength(text);
}

char const * TextFormat(char const * text, ...)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = {0,};
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    return buffer;
}

char const * TextSubtext(char const * text, int position, int length)
{
    return ::TextSubtext(text, position, length);
}

char const * TextReplace(char * text, char const * replace, char const * by)
{
    return ::TextReplace(text, replace, by);
}

char const * TextInsert(char const * text, char const * insert, int position)
{
    return ::TextInsert(text, insert, position);
}

char const * TextJoin(char const ** textList, int count, char const * delimiter)
{
    return ::TextJoin(textList, count, delimiter);
}

char const ** TextSplit(char const * text, char delimiter, int * count)
{
    return ::TextSplit(text, delimiter, count);
}

void TextAppend(char * text, char const * append, int * position)
{
    ::TextAppend(text, append, position);
}

int TextFindIndex(char const * text, char const * find)
{
    return ::TextFindIndex(text, find);
}

char const * TextToUpper(char const * text)
{
    return ::TextToUpper(text);
}

char const * TextToLower(char const * text)
{
    return ::TextToLower(text);
}

char const * TextToPascal(char const * text)
{
    return ::TextToPascal(text);
}

int TextToInteger(char const * text)
{
    return ::TextToInteger(text);
}

void DrawLine3D(Vector3 start_pos, Vector3 end_pos, Color color)
{
    ::DrawLine3D(start_pos, end_pos, color);
}

void DrawCircle3D(Vector3 center, float radius, Vector3 rotation_axis, float rotation_angle, Color color)
{
    ::DrawCircle3D(center, radius, rotation_axis, rotation_angle, color);
}

void DrawCube(Vector3 position, float width, float height, float length, Color color)
{
    ::DrawCube(position, width, height, length, color);
}

void DrawCubeV(Vector3 position, Vector3 size, Color color)
{
    ::DrawCubeV(position, size, color);
}

void DrawCubeWires(Vector3 position, float width, float height, float length, Color color)
{
    ::DrawCubeWires(position, width, height, length, color);
}

void DrawCubeWiresV(Vector3 position, Vector3 size, Color color)
{
    ::DrawCubeWiresV(position, size, color);
}

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    ::DrawCubeTexture(texture, position, width, height, length, color);
}

void DrawSphere(Vector3 center_pos, float radius, Color color)
{
    ::DrawSphere(center_pos, radius, color);
}

void DrawSphereEx(Vector3 center_pos, float radius, int rings, int slices, Color color)
{
    ::DrawSphereEx(center_pos, radius, rings, slices, color);
}

void DrawSphereWires(Vector3 center_pos, float radius, int rings, int slices, Color color)
{
    ::DrawSphereWires(center_pos, radius, rings, slices, color);
}

void DrawCylinder(Vector3 position, float radius_top, float radius_bottom, float height, int slices, Color color)
{
    ::DrawCylinder(position, radius_top, radius_bottom, height, slices, color);
}

void DrawCylinderWires(Vector3 position, float radius_top, float radius_bottom, float height, int slices, Color color)
{
    ::DrawCylinderWires(position, radius_top, radius_bottom, height, slices, color);
}

void DrawPlane(Vector3 center_pos, Vector2 size, Color color)
{
    ::DrawPlane(center_pos, size, color);
}

void DrawRay(Ray ray, Color color)
{
    ::DrawRay(ray, color);
}

void DrawGrid(int slices, float spacing)
{
    ::DrawGrid(slices, spacing);
}

void DrawGizmo(Vector3 position)
{
    ::DrawGizmo(position);
}

Model LoadModel(char const * file_name)
{
    return ::LoadModel(file_name);
}

Model LoadModelFromMesh(Mesh mesh)
{
    return ::LoadModelFromMesh(mesh);
}

void UnloadModel(Model model)
{
    ::UnloadModel(model);
}

Mesh * LoadMeshes(char const * file_name, int * mesh_count)
{
    return ::LoadMeshes(file_name, mesh_count);
}

void ExportMesh(Mesh mesh, char const * file_name)
{
    ::ExportMesh(mesh, file_name);
}

void UnloadMesh(Mesh * mesh)
{
    ::UnloadMesh(mesh);
}

Material * LoadMaterials(char const * file_name, int * material_count)
{
    return ::LoadMaterials(file_name, material_count);
}

Material LoadMaterialDefault()
{
    return ::LoadMaterialDefault();
}

void UnloadMaterial(Material material)
{
    ::UnloadMaterial(material);
}

void SetMaterialTexture(Material * material, int map_type, Texture2D texture)
{
    ::SetMaterialTexture(material, map_type, texture);
}

void SetModelMeshMaterial(Model * model, int meshId, int materialId)
{
    ::SetModelMeshMaterial(model, meshId, materialId);
}

ModelAnimation * LoadModelAnimations(char const * file_name, int * anims_count)
{
    return ::LoadModelAnimations(file_name, anims_count);
}

void UpdateModelAnimation(Model model, ModelAnimation anim, int frame)
{
    ::UpdateModelAnimation(model, anim, frame);
}

void UnloadModelAnimation(ModelAnimation anim)
{
    ::UnloadModelAnimation(anim);
}

bool IsModelAnimationValid(Model model, ModelAnimation anim)
{
    return ::IsModelAnimationValid(model, anim);
}

Mesh GenMeshPoly(int sides, float radius)
{
    return ::GenMeshPoly(sides, radius);
}

Mesh GenMeshPlane(float width, float length, int res_x, int res_z)
{
    return ::GenMeshPlane(width, length, res_x, res_z);
}

Mesh GenMeshCube(float width, float height, float length)
{
    return ::GenMeshCube(width, height, length);
}

Mesh GenMeshSphere(float radius, int rings, int slices)
{
    return ::GenMeshSphere(radius, rings, slices);
}

Mesh GenMeshHemiSphere(float radius, int rings, int slices)
{
    return ::GenMeshHemiSphere(radius, rings, slices);
}

Mesh GenMeshCylinder(float radius, float height, int slices)
{
    return ::GenMeshCylinder(radius, height, slices);
}

Mesh GenMeshTorus(float radius, float size, int rad_seg, int sides)
{
    return ::GenMeshTorus(radius, size, rad_seg, sides);
}

Mesh GenMeshKnot(float radius, float size, int rad_seg, int sides)
{
    return ::GenMeshKnot(radius, size, rad_seg, sides);
}

Mesh GenMeshHeightmap(Image heightmap, Vector3 size)
{
    return ::GenMeshHeightmap(heightmap, size);
}

Mesh GenMeshCubicmap(Image cubicmap, Vector3 cube_size)
{
    return ::GenMeshCubicmap(cubicmap, cube_size);
}

BoundingBox MeshBoundingBox(Mesh mesh)
{
    return ::MeshBoundingBox(mesh);
}

void MeshTangents(Mesh * mesh)
{
    ::MeshTangents(mesh);
}

void MeshBinormals(Mesh * mesh)
{
    ::MeshBinormals(mesh);
}

void DrawModel(Model model, Vector3 position, float scale, Color tint)
{
    ::DrawModel(model, position, scale, tint);
}

void DrawModelEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle,
                 Vector3 scale, Color tint)
{
    ::DrawModelEx(model, position, rotation_axis, rotation_angle, scale, tint);
}

void DrawModelWires(Model model, Vector3 position, float scale, Color tint)
{
    ::DrawModelWires(model, position, scale, tint);
}

void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle,
                      Vector3 scale, Color tint)
{
    ::DrawModelWiresEx(model, position, rotation_axis, rotation_angle, scale, tint);
}

void DrawBoundingBox(BoundingBox box, Color color)
{
    ::DrawBoundingBox(box, color);
}

void DrawBillboard(Camera camera, Texture2D texture, Vector3 center, float size, Color tint)
{
    ::DrawBillboard(camera, texture, center, size, tint);
}

void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle2 source_rec, Vector3 center, float size, Color tint)
{
    ::DrawBillboardRec(camera, texture, source_rec, center, size, tint);
}

bool CheckCollisionSpheres(Vector3 center_a, float radius_a, Vector3 center_b, float radius_b)
{
    return ::CheckCollisionSpheres(center_a, radius_a, center_b, radius_b);
}

bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2)
{
    return ::CheckCollisionBoxes(box1, box2);
}

bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center_sphere, float radius_sphere)
{
    return ::CheckCollisionBoxSphere(box, center_sphere, radius_sphere);
}

bool CheckCollisionRaySphere(Ray ray, Vector3 sphere_position, float sphere_radius)
{
    return ::CheckCollisionRaySphere(ray, sphere_position, sphere_radius);
}

bool CheckCollisionRaySphereEx(Ray ray, Vector3 sphere_position, float sphere_radius, Vector3 * collision_point)
{
    return ::CheckCollisionRaySphereEx(ray, sphere_position, sphere_radius, collision_point);
}

bool CheckCollisionRayBox(Ray ray, BoundingBox box)
{
    return ::CheckCollisionRayBox(ray, box);
}

RayHitInfo GetCollisionRayModel(Ray ray, Model * model)
{
    return ::GetCollisionRayModel(ray, model);
}

RayHitInfo GetCollisionRayTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3)
{
    return ::GetCollisionRayTriangle(ray, p1, p2, p3);
}

RayHitInfo GetCollisionRayGround(Ray ray, float ground_height)
{
    return ::GetCollisionRayGround(ray, ground_height);
}

char * LoadText(char const * file_name)
{
    return ::LoadText(file_name);
}

Shader LoadShader(char const * vs_file_name, char const * fs_file_name)
{
    return ::LoadShader(vs_file_name, fs_file_name);
}

Shader LoadShaderCode(char * vs_code, char * fs_code)
{
    return ::LoadShaderCode(vs_code, fs_code);
}

void UnloadShader(Shader shader)
{
    ::UnloadShader(shader);
}

Shader GetShaderDefault()
{
    return ::GetShaderDefault();
}

Texture2D GetTextureDefault()
{
    return ::GetTextureDefault();
}

int GetShaderLocation(Shader shader, char const * uniform_name)
{
    return ::GetShaderLocation(shader, uniform_name);
}

void SetShaderValue(Shader shader, int uniform_loc, void const * value, int uniform_type)
{
    ::SetShaderValue(shader, uniform_loc, value, uniform_type);
}

void SetShaderValueV(Shader shader, int uniform_loc, void const * value, int uniform_type, int count)
{
    ::SetShaderValueV(shader, uniform_loc, value, uniform_type, count);
}

void SetShaderValueMatrix(Shader shader, int uniform_loc, Matrix mat)
{
    ::SetShaderValueMatrix(shader, uniform_loc, mat);
}

void SetShaderValueTexture(Shader shader, int uniform_loc, Texture2D texture)
{
    ::SetShaderValueTexture(shader, uniform_loc, texture);
}

void SetMatrixProjection(Matrix proj)
{
    ::SetMatrixProjection(proj);
}

void SetMatrixModelview(Matrix view)
{
    ::SetMatrixModelview(view);
}

Matrix GetMatrixModelview()
{
    return ::GetMatrixModelview();
}

Texture2D GenTextureCubemap(Shader shader, Texture2D sky_hdr, int size)
{
    return ::GenTextureCubemap(shader, sky_hdr, size);
}

Texture2D GenTextureIrradiance(Shader shader, Texture2D cubemap, int size)
{
    return ::GenTextureIrradiance(shader, cubemap, size);
}

Texture2D GenTexturePrefilter(Shader shader, Texture2D cubemap, int size)
{
    return ::GenTexturePrefilter(shader, cubemap, size);
}

Texture2D GenTextureBRDF(Shader shader, int size)
{
    return ::GenTextureBRDF(shader, size);
}

void BeginShaderMode(Shader shader)
{
    ::BeginShaderMode(shader);
}

void EndShaderMode()
{
    ::EndShaderMode();
}

void BeginBlendMode(int mode)
{
    ::BeginBlendMode(mode);
}

void EndBlendMode()
{
    ::EndBlendMode();
}

void BeginScissorMode(int x, int y, int width, int height)
{
    ::BeginScissorMode(x, y, width, height);
}

void EndScissorMode()
{
    ::EndScissorMode();
}

void InitVrSimulator()
{
    ::InitVrSimulator();
}

void UpdateVrTracking(Camera * camera)
{
    ::UpdateVrTracking(camera);
}

void CloseVrSimulator()
{
    ::CloseVrSimulator();
}

bool IsVrSimulatorReady()
{
    return ::IsVrSimulatorReady();
}

void ToggleVrMode()
{
    ::ToggleVrMode();
}

void BeginVrDrawing()
{
    ::BeginVrDrawing();
}

void EndVrDrawing()
{
    ::EndVrDrawing();
}

void InitAudioDevice()
{
    ::InitAudioDevice();
}

void CloseAudioDevice()
{
    ::CloseAudioDevice();
}

bool IsAudioDeviceReady()
{
    return ::IsAudioDeviceReady();
}

void SetMasterVolume(float volume)
{
    ::SetMasterVolume(volume);
}

Wave LoadWave(char const * file_name)
{
    return ::LoadWave(file_name);
}

Wave LoadWaveEx(void * data, int sample_count, int sample_rate, int sample_size, int channels)
{
    return ::LoadWaveEx(data, sample_count, sample_rate, sample_size, channels);
}

Sound LoadSound(char const * file_name)
{
    return ::LoadSound(file_name);
}

Sound LoadSoundFromWave(Wave wave)
{
    return ::LoadSoundFromWave(wave);
}

void UpdateSound(Sound sound, void const * data, int samples_count)
{
    ::UpdateSound(sound, data, samples_count);
}

void UnloadWave(Wave wave)
{
    ::UnloadWave(wave);
}

void UnloadSound(Sound sound)
{
    ::UnloadSound(sound);
}

void ExportWave(Wave wave, char const * file_name)
{
    ::ExportWave(wave, file_name);
}

void ExportWaveAsCode(Wave wave, char const * file_name)
{
    ::ExportWaveAsCode(wave, file_name);
}

void PlaySound(Sound sound)
{
    ::PlaySound(sound);
}

void PauseSound(Sound sound)
{
    ::PauseSound(sound);
}

void ResumeSound(Sound sound)
{
    ::ResumeSound(sound);
}

void StopSound(Sound sound)
{
    ::StopSound(sound);
}

bool IsSoundPlaying(Sound sound)
{
    return ::IsSoundPlaying(sound);
}

void SetSoundVolume(Sound sound, float volume)
{
    ::SetSoundVolume(sound, volume);
}

void SetSoundPitch(Sound sound, float pitch)
{
    ::SetSoundPitch(sound, pitch);
}

void WaveFormat(Wave * wave, int sample_rate, int sample_size, int channels)
{
    ::WaveFormat(wave, sample_rate, sample_size, channels);
}

Wave WaveCopy(Wave wave)
{
    return ::WaveCopy(wave);
}

void WaveCrop(Wave * wave, int init_sample, int final_sample)
{
    ::WaveCrop(wave, init_sample, final_sample);
}

float * GetWaveData(Wave wave)
{
    return ::GetWaveData(wave);
}

Music LoadMusicStream(char const * file_name)
{
    return ::LoadMusicStream(file_name);
}

void UnloadMusicStream(Music music)
{
    ::UnloadMusicStream(music);
}

void PlayMusicStream(Music music)
{
    ::PlayMusicStream(music);
}

void UpdateMusicStream(Music music)
{
    ::UpdateMusicStream(music);
}

void StopMusicStream(Music music)
{
    ::StopMusicStream(music);
}

void PauseMusicStream(Music music)
{
    ::PauseMusicStream(music);
}

void ResumeMusicStream(Music music)
{
    ::ResumeMusicStream(music);
}

bool IsMusicPlaying(Music music)
{
    return ::IsMusicPlaying(music);
}

void SetMusicVolume(Music music, float volume)
{
    ::SetMusicVolume(music, volume);
}

void SetMusicPitch(Music music, float pitch)
{
    ::SetMusicPitch(music, pitch);
}

void SetMusicLoopCount(Music music, int count)
{
    ::SetMusicLoopCount(music, count);
}

float GetMusicTimeLength(Music music)
{
    return ::GetMusicTimeLength(music);
}

float GetMusicTimePlayed(Music music)
{
    return ::GetMusicTimePlayed(music);
}

AudioStream InitAudioStream(unsigned int sample_rate, unsigned int sample_size, unsigned int channels)
{
    return ::InitAudioStream(sample_rate, sample_size, channels);
}

void UpdateAudioStream(AudioStream stream, void const * data, int samples_count)
{
    ::UpdateAudioStream(stream, data, samples_count);
}

void CloseAudioStream(AudioStream stream)
{
    ::CloseAudioStream(stream);
}

bool IsAudioBufferProcessed(AudioStream stream)
{
    return ::IsAudioBufferProcessed(stream);
}

void PlayAudioStream(AudioStream stream)
{
    ::PlayAudioStream(stream);
}

void PauseAudioStream(AudioStream stream)
{
    ::PauseAudioStream(stream);
}

void ResumeAudioStream(AudioStream stream)
{
    ::ResumeAudioStream(stream);
}

bool IsAudioStreamPlaying(AudioStream stream)
{
    return ::IsAudioStreamPlaying(stream);
}

void StopAudioStream(AudioStream stream)
{
    ::StopAudioStream(stream);
}

void SetAudioStreamVolume(AudioStream stream, float volume)
{
    ::SetAudioStreamVolume(stream, volume);
}

void SetAudioStreamPitch(AudioStream stream, float pitch)
{
    ::SetAudioStreamPitch(stream, pitch);
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

