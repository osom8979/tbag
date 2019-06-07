/**
 * @file   RayBypass.hpp
 * @brief  RayBypass class prototype.
 * @author zer0
 * @date   2019-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYBYPASS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYBYPASS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/raylib-2.5/raylib.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

// Max length of one trace-log message
TBAG_CONSTEXPR int const MAX_TRACELOG_BUFFER_SIZE = 128;

// Size of internal static buffers of some Text*() functions
TBAG_CONSTEXPR int const MAX_TEXT_BUFFER_LENGTH = 1024;

TBAG_API void InitWindow(int width, int height, char const * title);
TBAG_API bool WindowShouldClose();
TBAG_API void CloseWindow();
TBAG_API bool IsWindowReady();
TBAG_API bool IsWindowMinimized();
TBAG_API bool IsWindowResized();
TBAG_API bool IsWindowHidden();
TBAG_API void ToggleFullscreen();
TBAG_API void UnhideWindow();
TBAG_API void HideWindow();
TBAG_API void SetWindowIcon(Image image);
TBAG_API void SetWindowTitle(char const * title);
TBAG_API void SetWindowPosition(int x, int y);
TBAG_API void SetWindowMonitor(int monitor);
TBAG_API void SetWindowMinSize(int width, int height);
TBAG_API void SetWindowSize(int width, int height);
TBAG_API void * GetWindowHandle();
TBAG_API int GetScreenWidth();
TBAG_API int GetScreenHeight();
TBAG_API int GetMonitorCount();
TBAG_API int GetMonitorWidth(int monitor);
TBAG_API int GetMonitorHeight(int monitor);
TBAG_API int GetMonitorPhysicalWidth(int monitor);
TBAG_API int GetMonitorPhysicalHeight(int monitor);
TBAG_API char const * GetMonitorName(int monitor);
TBAG_API char const * GetClipboardText();
TBAG_API void SetClipboardText(char const * text);

TBAG_API void ShowCursor();
TBAG_API void HideCursor();
TBAG_API bool IsCursorHidden();
TBAG_API void EnableCursor();
TBAG_API void DisableCursor();

TBAG_API void ClearBackground(Color color);
TBAG_API void BeginDrawing();
TBAG_API void SetEndDrawingCallback(void(*cb)(void));
TBAG_API void EndDrawing();
TBAG_API void BeginMode2D(Camera2D camera);
TBAG_API void EndMode2D();
TBAG_API void BeginMode3D(Camera3D camera);
TBAG_API void EndMode3D();
TBAG_API void BeginTextureMode(RenderTexture2D target);
TBAG_API void EndTextureMode();

TBAG_API Ray GetMouseRay(Vector2 mouse_position, Camera camera);
TBAG_API Vector2 GetWorldToScreen(Vector3 position, Camera camera);
TBAG_API Matrix GetCameraMatrix(Camera camera);

TBAG_API void SetTargetFPS(int fps);
TBAG_API int GetFPS();
TBAG_API float GetFrameTime();
TBAG_API double GetTime();

TBAG_API int ColorToInt(Color color);
TBAG_API Vector4 ColorNormalize(Color color);
TBAG_API Vector3 ColorToHSV(Color color);
TBAG_API Color ColorFromHSV(Vector3 hsv);
TBAG_API Color GetColor(int hex_value);
TBAG_API Color Fade(Color color, float alpha);

TBAG_API void SetConfigFlags(unsigned char flags);
TBAG_API void SetTraceLogLevel(int log_type);
TBAG_API void SetTraceLogExit(int log_type);
TBAG_API void SetTraceLogCallback(TraceLogCallback callback);

/** @warning Not recommended */
TBAG_API void TraceLog(int log_type, char const * text, ...);

TBAG_API void TakeScreenshot(char const * file_name);
TBAG_API int GetRandomValue(int min, int max);

TBAG_API bool FileExists(char const * file_name);
TBAG_API bool IsFileExtension(char const * file_name, char const * ext);
TBAG_API char const * GetExtension(char const * file_name);
TBAG_API char const * GetFileName(char const * file_path);
TBAG_API char const * GetFileNameWithoutExt(char const * file_path);
TBAG_API char const * GetDirectoryPath(char const * file_name);
TBAG_API char const * GetWorkingDirectory();
TBAG_API char ** GetDirectoryFiles(char const * dir_path, int * count);
TBAG_API void ClearDirectoryFiles();
TBAG_API bool ChangeDirectory(char const * dir);
TBAG_API bool IsFileDropped();
TBAG_API char ** GetDroppedFiles(int * count);
TBAG_API void ClearDroppedFiles();
TBAG_API long GetFileModTime(char const * file_name);

TBAG_API void StorageSaveValue(int position, int value);
TBAG_API int StorageLoadValue(int position);

TBAG_API void OpenURL(char const * url);

TBAG_API bool IsKeyPressed(int key);
TBAG_API bool IsKeyDown(int key);
TBAG_API bool IsKeyReleased(int key);
TBAG_API bool IsKeyUp(int key);
TBAG_API int GetKeyPressed();
TBAG_API void SetExitKey(int key);

TBAG_API bool IsGamepadAvailable(int gamepad);
TBAG_API bool IsGamepadName(int gamepad, char const * name);
TBAG_API char const * GetGamepadName(int gamepad);
TBAG_API bool IsGamepadButtonPressed(int gamepad, int button);
TBAG_API bool IsGamepadButtonDown(int gamepad, int button);
TBAG_API bool IsGamepadButtonReleased(int gamepad, int button);
TBAG_API bool IsGamepadButtonUp(int gamepad, int button);
TBAG_API int GetGamepadButtonPressed();
TBAG_API int GetGamepadAxisCount(int gamepad);
TBAG_API float GetGamepadAxisMovement(int gamepad, int axis);

TBAG_API bool IsMouseButtonPressed(int button);
TBAG_API bool IsMouseButtonDown(int button);
TBAG_API bool IsMouseButtonReleased(int button);
TBAG_API bool IsMouseButtonUp(int button);
TBAG_API int GetMouseX();
TBAG_API int GetMouseY();
TBAG_API Vector2 GetMousePosition();
TBAG_API void SetMousePosition(int x, int y);
TBAG_API void SetMouseOffset(int offset_x, int offset_y);
TBAG_API void SetMouseScale(float scale_x, float scale_y);
TBAG_API int GetMouseWheelMove();

TBAG_API int GetTouchX();
TBAG_API int GetTouchY();
TBAG_API Vector2 GetTouchPosition(int index);

TBAG_API void SetGesturesEnabled(unsigned int gesture_flags);
TBAG_API bool IsGestureDetected(int gesture);
TBAG_API int GetGestureDetected();
TBAG_API int GetTouchPointsCount();
TBAG_API float GetGestureHoldDuration();
TBAG_API Vector2 GetGestureDragVector();
TBAG_API float GetGestureDragAngle();
TBAG_API Vector2 GetGesturePinchVector();
TBAG_API float GetGesturePinchAngle();

TBAG_API void SetCameraMode(Camera camera, int mode);
TBAG_API void UpdateCamera(Camera * camera);

TBAG_API void SetCameraPanControl(int pan_key);
TBAG_API void SetCameraAltControl(int alt_key);
TBAG_API void SetCameraSmoothZoomControl(int sz_key);
TBAG_API void SetCameraMoveControls(int front_key, int back_key, int right_key, int left_key, int up_key, int down_key);

TBAG_API void DrawPixel(int pos_x, int pos_y, Color color);
TBAG_API void DrawPixelV(Vector2 position, Color color);
TBAG_API void DrawLine(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y, Color color);
TBAG_API void DrawLineV(Vector2 start_pos, Vector2 end_pos, Color color);
TBAG_API void DrawLineEx(Vector2 start_pos, Vector2 end_pos, float thick, Color color);
TBAG_API void DrawLineBezier(Vector2 start_pos, Vector2 end_pos, float thick, Color color);
TBAG_API void DrawCircle(int center_x, int center_y, float radius, Color color);
TBAG_API void DrawCircleSector(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color);
TBAG_API void DrawCircleSectorLines(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color);
TBAG_API void DrawCircleGradient(int center_x, int center_y, float radius, Color color1, Color color2);
TBAG_API void DrawCircleV(Vector2 center, float radius, Color color);
TBAG_API void DrawCircleLines(int center_x, int center_y, float radius, Color color);
TBAG_API void DrawRing(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle,
                       int segments, Color color);
TBAG_API void DrawRingLines(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle,
                            int segments, Color color);
TBAG_API void DrawRectangle(int pos_x, int pos_y, int width, int height, Color color);
TBAG_API void DrawRectangleV(Vector2 position, Vector2 size, Color color);
TBAG_API void DrawRectangleRec(Rectangle2 rec, Color color);
TBAG_API void DrawRectanglePro(Rectangle2 rec, Vector2 origin, float rotation, Color color);
TBAG_API void DrawRectangleGradientV(int pos_x, int pos_y, int width, int height, Color color1, Color color2);
TBAG_API void DrawRectangleGradientH(int pos_x, int pos_y, int width, int height, Color color1, Color color2);
TBAG_API void DrawRectangleGradientEx(Rectangle2 rec, Color col1, Color col2, Color col3, Color col4);
TBAG_API void DrawRectangleLines(int pos_x, int pos_y, int width, int height, Color color);
TBAG_API void DrawRectangleLinesEx(Rectangle2 rec, int line_thick, Color color);
TBAG_API void DrawRectangleRounded(Rectangle2 rec, float roundness, int segments, Color color);
TBAG_API void DrawRectangleRoundedLines(Rectangle2 rec, float roundness, int segments, int line_thick, Color color);
TBAG_API void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
TBAG_API void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
TBAG_API void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);

TBAG_API void SetShapesTexture(Texture2D texture, Rectangle2 source);

TBAG_API bool CheckCollisionRecs(Rectangle2 rec1, Rectangle2 rec2);
TBAG_API bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
TBAG_API bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle2 rec);
TBAG_API Rectangle2 GetCollisionRec(Rectangle2 rec1, Rectangle2 rec2);
TBAG_API bool CheckCollisionPointRec(Vector2 point, Rectangle2 rec);
TBAG_API bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius);
TBAG_API bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3);

TBAG_API Image LoadImage(char const * file_name);
TBAG_API Image LoadImageEx(Color * pixels, int width, int height);
TBAG_API Image LoadImagePro(void * data, int width, int height, int format);
TBAG_API Image LoadImageRaw(char const * file_name, int width, int height, int format, int header_size);
TBAG_API void ExportImage(Image image, char const * file_name);
TBAG_API void ExportImageAsCode(Image image, char const * file_name);
TBAG_API Texture2D LoadTexture(char const * file_name);
TBAG_API Texture2D LoadTextureFromImage(Image image);
TBAG_API TextureCubemap LoadTextureCubemap(Image image, int layout_type);
TBAG_API RenderTexture2D LoadRenderTexture(int width, int height);
TBAG_API void UnloadImage(Image image);
TBAG_API void UnloadTexture(Texture2D texture);
TBAG_API void UnloadRenderTexture(RenderTexture2D target);
TBAG_API Color * GetImageData(Image image);
TBAG_API Vector4 * GetImageDataNormalized(Image image);
TBAG_API int GetPixelDataSize(int width, int height, int format);
TBAG_API Image GetTextureData(Texture2D texture);
TBAG_API Image GetScreenData();
TBAG_API void UpdateTexture(Texture2D texture, void const * pixels);

TBAG_API Image ImageCopy(Image image);
TBAG_API void ImageToPOT(Image * image, Color fill_color);
TBAG_API void ImageFormat(Image * image, int new_format);
TBAG_API void ImageAlphaMask(Image * image, Image alpha_mask);
TBAG_API void ImageAlphaClear(Image * image, Color color, float threshold);
TBAG_API void ImageAlphaCrop(Image * image, float threshold);
TBAG_API void ImageAlphaPremultiply(Image * image);
TBAG_API void ImageCrop(Image * image, Rectangle2 crop);
TBAG_API void ImageResize(Image * image, int new_width, int new_height);
TBAG_API void ImageResizeNN(Image * image, int new_width, int new_height);
TBAG_API void ImageResizeCanvas(Image * image, int new_width, int new_height, int offset_x, int offset_y, Color color);
TBAG_API void ImageMipmaps(Image * image);
TBAG_API void ImageDither(Image * image, int r_bpp, int g_bpp, int b_bpp, int a_bpp);
TBAG_API Color * ImageExtractPalette(Image image, int maxPalette_size, int * extract_count);
TBAG_API Image ImageText(char const * text, int font_size, Color color);
TBAG_API Image ImageTextEx(Font font, char const * text, float font_size, float spacing, Color tint);
TBAG_API void ImageDraw(Image * dst, Image src, Rectangle2 src_rec, Rectangle2 dst_rec);
TBAG_API void ImageDrawRectangle(Image * dst, Rectangle2 rec, Color color);
TBAG_API void ImageDrawRectangleLines(Image * dst, Rectangle2 rec, int thick, Color color);
TBAG_API void ImageDrawText(Image * dst, Vector2 position, char const * text, int font_size, Color color);
TBAG_API void ImageDrawTextEx(Image * dst, Vector2 position, Font font, char const * text, float font_size,
                              float spacing, Color color);
TBAG_API void ImageFlipVertical(Image * image);
TBAG_API void ImageFlipHorizontal(Image * image);
TBAG_API void ImageRotateCW(Image * image);
TBAG_API void ImageRotateCCW(Image * image);
TBAG_API void ImageColorTint(Image * image, Color color);
TBAG_API void ImageColorInvert(Image * image);
TBAG_API void ImageColorGrayscale(Image * image);
TBAG_API void ImageColorContrast(Image * image, float contrast);
TBAG_API void ImageColorBrightness(Image * image, int brightness);
TBAG_API void ImageColorReplace(Image * image, Color color, Color replace);

TBAG_API Image GenImageColor(int width, int height, Color color);
TBAG_API Image GenImageGradientV(int width, int height, Color top, Color bottom);
TBAG_API Image GenImageGradientH(int width, int height, Color left, Color right);
TBAG_API Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);
TBAG_API Image GenImageChecked(int width, int height, int checks_x, int checks_y, Color col1, Color col2);
TBAG_API Image GenImageWhiteNoise(int width, int height, float factor);
TBAG_API Image GenImagePerlinNoise(int width, int height, int offset_x, int offset_y, float scale);
TBAG_API Image GenImageCellular(int width, int height, int tile_size);

TBAG_API void GenTextureMipmaps(Texture2D * texture);
TBAG_API void SetTextureFilter(Texture2D texture, int filter_mode);
TBAG_API void SetTextureWrap(Texture2D texture, int wrap_mode);

TBAG_API void DrawTexture(Texture2D texture, int pos_x, int pos_y, Color tint);
TBAG_API void DrawTextureV(Texture2D texture, Vector2 position, Color tint);
TBAG_API void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
TBAG_API void DrawTextureRec(Texture2D texture, Rectangle2 source_rec, Vector2 position, Color tint);
TBAG_API void DrawTextureQuad(Texture2D texture, Vector2 tiling, Vector2 offset, Rectangle2 quad, Color tint);
TBAG_API void DrawTexturePro(Texture2D texture, Rectangle2 source_rec, Rectangle2 dest_rec, Vector2 origin,
                             float rotation, Color tint);
TBAG_API void DrawTextureNPatch(Texture2D texture, NPatchInfo n_patch_info, Rectangle2 dest_rec, Vector2 origin,
                                float rotation, Color tint);

TBAG_API Font GetFontDefault();
TBAG_API Font LoadFont(char const * file_name);
TBAG_API Font LoadFontEx(char const * file_name, int font_size, int * font_chars, int chars_count);
TBAG_API Font LoadFontFromImage(Image image, Color key, int first_char);
TBAG_API CharInfo * LoadFontData(char const * file_name, int font_size, int * font_chars, int chars_count, int type);
TBAG_API Image GenImageFontAtlas(CharInfo * chars, int chars_count, int font_size, int padding, int pack_method);
TBAG_API void UnloadFont(Font font);

TBAG_API void DrawFPS(int pos_x, int pos_y);
TBAG_API void DrawText(char const * text, int pos_x, int pos_y, int font_size, Color color);
TBAG_API void DrawTextEx(Font font, char const * text, Vector2 position, float font_size, float spacing, Color tint);
TBAG_API void DrawTextRec(Font font, char const * text, Rectangle2 rec, float font_size, float spacing,
                          bool word_wrap, Color tint);
TBAG_API void DrawTextRecEx(Font font, char const * text, Rectangle2 rec, float font_size, float spacing, bool word_wrap,
                            Color tint, int select_start, int select_length, Color select_text, Color select_back);

TBAG_API int MeasureText(char const * text, int font_size);
TBAG_API Vector2 MeasureTextEx(Font font, char const * text, float font_size, float spacing);
TBAG_API int GetGlyphIndex(Font font, int character);

TBAG_API bool TextIsEqual(char const * text1, char const * text2);
TBAG_API unsigned int TextLength(char const * text);

/** @warning Not recommended */
TBAG_API char const * TextFormat(char const * text, ...);

TBAG_API char const * TextSubtext(char const * text, int position, int length);
TBAG_API char const * TextReplace(char * text, char const * replace, char const * by);
TBAG_API char const * TextInsert(char const * text, char const * insert, int position);
TBAG_API char const * TextJoin(char const ** textList, int count, char const * delimiter);
TBAG_API char const ** TextSplit(char const * text, char delimiter, int * count);
TBAG_API void TextAppend(char * text, char const * append, int * position);
TBAG_API int TextFindIndex(char const * text, char const * find);
TBAG_API char const * TextToUpper(char const * text);
TBAG_API char const * TextToLower(char const * text);
TBAG_API char const * TextToPascal(char const * text);
TBAG_API int TextToInteger(char const * text);

TBAG_API void DrawLine3D(Vector3 start_pos, Vector3 end_pos, Color color);
TBAG_API void DrawCircle3D(Vector3 center, float radius, Vector3 rotation_axis, float rotation_angle, Color color);
TBAG_API void DrawCube(Vector3 position, float width, float height, float length, Color color);
TBAG_API void DrawCubeV(Vector3 position, Vector3 size, Color color);
TBAG_API void DrawCubeWires(Vector3 position, float width, float height, float length, Color color);
TBAG_API void DrawCubeWiresV(Vector3 position, Vector3 size, Color color);
TBAG_API void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);
TBAG_API void DrawSphere(Vector3 center_pos, float radius, Color color);
TBAG_API void DrawSphereEx(Vector3 center_pos, float radius, int rings, int slices, Color color);
TBAG_API void DrawSphereWires(Vector3 center_pos, float radius, int rings, int slices, Color color);
TBAG_API void DrawCylinder(Vector3 position, float radius_top, float radius_bottom, float height,
                           int slices, Color color);
TBAG_API void DrawCylinderWires(Vector3 position, float radius_top, float radius_bottom, float height,
                                int slices, Color color);
TBAG_API void DrawPlane(Vector3 center_pos, Vector2 size, Color color);
TBAG_API void DrawRay(Ray ray, Color color);
TBAG_API void DrawGrid(int slices, float spacing);
TBAG_API void DrawGizmo(Vector3 position);

TBAG_API Model LoadModel(char const * file_name);
TBAG_API Model LoadModelFromMesh(Mesh mesh);
TBAG_API void UnloadModel(Model model);

TBAG_API Mesh * LoadMeshes(char const * file_name, int * mesh_count);
TBAG_API void ExportMesh(Mesh mesh, char const * file_name);
TBAG_API void UnloadMesh(Mesh * mesh);

TBAG_API Material * LoadMaterials(char const * file_name, int * material_count);
TBAG_API Material LoadMaterialDefault();
TBAG_API void UnloadMaterial(Material material);
TBAG_API void SetMaterialTexture(Material * material, int map_type, Texture2D texture);
TBAG_API void SetModelMeshMaterial(Model * model, int meshId, int materialId);

TBAG_API ModelAnimation * LoadModelAnimations(char const * file_name, int * anims_count);
TBAG_API void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);
TBAG_API void UnloadModelAnimation(ModelAnimation anim);
TBAG_API bool IsModelAnimationValid(Model model, ModelAnimation anim);

TBAG_API Mesh GenMeshPoly(int sides, float radius);
TBAG_API Mesh GenMeshPlane(float width, float length, int res_x, int res_z);
TBAG_API Mesh GenMeshCube(float width, float height, float length);
TBAG_API Mesh GenMeshSphere(float radius, int rings, int slices);
TBAG_API Mesh GenMeshHemiSphere(float radius, int rings, int slices);
TBAG_API Mesh GenMeshCylinder(float radius, float height, int slices);
TBAG_API Mesh GenMeshTorus(float radius, float size, int rad_seg, int sides);
TBAG_API Mesh GenMeshKnot(float radius, float size, int rad_seg, int sides);
TBAG_API Mesh GenMeshHeightmap(Image heightmap, Vector3 size);
TBAG_API Mesh GenMeshCubicmap(Image cubicmap, Vector3 cube_size);

TBAG_API BoundingBox MeshBoundingBox(Mesh mesh);
TBAG_API void MeshTangents(Mesh * mesh);
TBAG_API void MeshBinormals(Mesh * mesh);

TBAG_API void DrawModel(Model model, Vector3 position, float scale, Color tint);
TBAG_API void DrawModelEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle,
                          Vector3 scale, Color tint);
TBAG_API void DrawModelWires(Model model, Vector3 position, float scale, Color tint);
TBAG_API void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle,
                               Vector3 scale, Color tint);
TBAG_API void DrawBoundingBox(BoundingBox box, Color color);
TBAG_API void DrawBillboard(Camera camera, Texture2D texture, Vector3 center, float size, Color tint);
TBAG_API void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle2 source_rec, Vector3 center,
                               float size, Color tint);

TBAG_API bool CheckCollisionSpheres(Vector3 center_a, float radius_a, Vector3 center_b, float radius_b);
TBAG_API bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2);
TBAG_API bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center_sphere, float radius_sphere);
TBAG_API bool CheckCollisionRaySphere(Ray ray, Vector3 sphere_position, float sphere_radius);
TBAG_API bool CheckCollisionRaySphereEx(Ray ray, Vector3 sphere_position, float sphere_radius,
                                        Vector3 * collision_point);
TBAG_API bool CheckCollisionRayBox(Ray ray, BoundingBox box);
TBAG_API RayHitInfo GetCollisionRayModel(Ray ray, Model * model);
TBAG_API RayHitInfo GetCollisionRayTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);
TBAG_API RayHitInfo GetCollisionRayGround(Ray ray, float ground_height);

TBAG_API char * LoadText(char const * file_name);
TBAG_API Shader LoadShader(char const * vs_file_name, char const * fs_file_name);
TBAG_API Shader LoadShaderCode(char * vs_code, char * fs_code);
TBAG_API void UnloadShader(Shader shader);

TBAG_API Shader GetShaderDefault();
TBAG_API Texture2D GetTextureDefault();

TBAG_API int GetShaderLocation(Shader shader, char const * uniform_name);
TBAG_API void SetShaderValue(Shader shader, int uniform_loc, void const * value, int uniform_type);
TBAG_API void SetShaderValueV(Shader shader, int uniform_loc, void const * value, int uniform_type, int count);
TBAG_API void SetShaderValueMatrix(Shader shader, int uniform_loc, Matrix mat);
TBAG_API void SetShaderValueTexture(Shader shader, int uniform_loc, Texture2D texture);
TBAG_API void SetMatrixProjection(Matrix proj);
TBAG_API void SetMatrixModelview(Matrix view);
TBAG_API Matrix GetMatrixModelview();

TBAG_API Texture2D GenTextureCubemap(Shader shader, Texture2D sky_hdr, int size);
TBAG_API Texture2D GenTextureIrradiance(Shader shader, Texture2D cubemap, int size);
TBAG_API Texture2D GenTexturePrefilter(Shader shader, Texture2D cubemap, int size);
TBAG_API Texture2D GenTextureBRDF(Shader shader, int size);

TBAG_API void BeginShaderMode(Shader shader);
TBAG_API void EndShaderMode();
TBAG_API void BeginBlendMode(int mode);
TBAG_API void EndBlendMode();
TBAG_API void BeginScissorMode(int x, int y, int width, int height);
TBAG_API void EndScissorMode();

TBAG_API void InitVrSimulator();
TBAG_API void UpdateVrTracking(Camera * camera);
TBAG_API void CloseVrSimulator();
TBAG_API bool IsVrSimulatorReady();
TBAG_API void ToggleVrMode();
TBAG_API void BeginVrDrawing();
TBAG_API void EndVrDrawing();

TBAG_API void InitAudioDevice();
TBAG_API void CloseAudioDevice();
TBAG_API bool IsAudioDeviceReady();
TBAG_API void SetMasterVolume(float volume);

TBAG_API Wave LoadWave(char const * file_name);
TBAG_API Wave LoadWaveEx(void * data, int sample_count, int sample_rate, int sample_size, int channels);
TBAG_API Sound LoadSound(char const * file_name);
TBAG_API Sound LoadSoundFromWave(Wave wave);
TBAG_API void UpdateSound(Sound sound, void const * data, int samples_count);
TBAG_API void UnloadWave(Wave wave);
TBAG_API void UnloadSound(Sound sound);
TBAG_API void ExportWave(Wave wave, char const * file_name);
TBAG_API void ExportWaveAsCode(Wave wave, char const * file_name);

TBAG_API void PlaySound(Sound sound);
TBAG_API void PauseSound(Sound sound);
TBAG_API void ResumeSound(Sound sound);
TBAG_API void StopSound(Sound sound);
TBAG_API bool IsSoundPlaying(Sound sound);
TBAG_API void SetSoundVolume(Sound sound, float volume);
TBAG_API void SetSoundPitch(Sound sound, float pitch);
TBAG_API void WaveFormat(Wave * wave, int sample_rate, int sample_size, int channels);
TBAG_API Wave WaveCopy(Wave wave);
TBAG_API void WaveCrop(Wave * wave, int init_sample, int final_sample);
TBAG_API float * GetWaveData(Wave wave);

TBAG_API Music LoadMusicStream(char const * file_name);
TBAG_API void UnloadMusicStream(Music music);
TBAG_API void PlayMusicStream(Music music);
TBAG_API void UpdateMusicStream(Music music);
TBAG_API void StopMusicStream(Music music);
TBAG_API void PauseMusicStream(Music music);
TBAG_API void ResumeMusicStream(Music music);
TBAG_API bool IsMusicPlaying(Music music);
TBAG_API void SetMusicVolume(Music music, float volume);
TBAG_API void SetMusicPitch(Music music, float pitch);
TBAG_API void SetMusicLoopCount(Music music, int count);
TBAG_API float GetMusicTimeLength(Music music);
TBAG_API float GetMusicTimePlayed(Music music);

TBAG_API AudioStream InitAudioStream(unsigned int sample_rate, unsigned int sample_size, unsigned int channels);
TBAG_API void UpdateAudioStream(AudioStream stream, void const * data, int samples_count);
TBAG_API void CloseAudioStream(AudioStream stream);
TBAG_API bool IsAudioBufferProcessed(AudioStream stream);
TBAG_API void PlayAudioStream(AudioStream stream);
TBAG_API void PauseAudioStream(AudioStream stream);
TBAG_API void ResumeAudioStream(AudioStream stream);
TBAG_API bool IsAudioStreamPlaying(AudioStream stream);
TBAG_API void StopAudioStream(AudioStream stream);
TBAG_API void SetAudioStreamVolume(AudioStream stream, float volume);
TBAG_API void SetAudioStreamPitch(AudioStream stream, float pitch);

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYBYPASS_HPP__

