/**
 * @file   RayLua.cpp
 * @brief  RayLua class implementation.
 * @author zer0
 * @date   2019-04-27
 */

#include <libtbag/script/lua/RayLua.hpp>
#include <libtbag/script/lua/RayLuaCore.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

//void DrawPixel(int pos_x, int pos_y, Color color);
//void DrawPixelV(Vector2 position, Color color);
//void DrawLine(int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y, Color color);
//void DrawLineV(Vector2 start_pos, Vector2 end_pos, Color color);
//void DrawLineEx(Vector2 start_pos, Vector2 end_pos, float thick, Color color);
//void DrawLineBezier(Vector2 start_pos, Vector2 end_pos, float thick, Color color);
//void DrawCircle(int center_x, int center_y, float radius, Color color);
//void DrawCircleSector(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color);
//void DrawCircleSectorLines(Vector2 center, float radius, int start_angle, int end_angle, int segments, Color color);
//void DrawCircleGradient(int center_x, int center_y, float radius, Color color1, Color color2);
//void DrawCircleV(Vector2 center, float radius, Color color);
//void DrawCircleLines(int center_x, int center_y, float radius, Color color);
//void DrawRing(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle, int segments, Color color);
//void DrawRingLines(Vector2 center, float inner_radius, float outer_radius, int start_angle, int end_angle, int segments, Color color);
//void DrawRectangle(int pos_x, int pos_y, int width, int height, Color color);
//void DrawRectangleV(Vector2 position, Vector2 size, Color color);
//void DrawRectangleRec(Rectangle rec, Color color);
//void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);
//void DrawRectangleGradientV(int pos_x, int pos_y, int width, int height, Color color1, Color color2);
//void DrawRectangleGradientH(int pos_x, int pos_y, int width, int height, Color color1, Color color2);
//void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4);
//void DrawRectangleLines(int pos_x, int pos_y, int width, int height, Color color);
//void DrawRectangleLinesEx(Rectangle rec, int line_thick, Color color);
//void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
//void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, int line_thick, Color color);
//void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
//void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
//void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
//void DrawPolyEx(Vector2 * points, int num_points, Color color);
//void DrawPolyExLines(Vector2 * points, int num_points, Color color);
//
//void SetShapesTexture(Texture2D texture, Rectangle source);
//
//bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);
//bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
//bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);
//Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2);
//bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
//bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius);
//bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3);
//
//Image LoadImage(char const * file_name);
//Image LoadImageEx(Color * pixels, int width, int height);
//Image LoadImagePro(void * data, int width, int height, int format);
//Image LoadImageRaw(char const * file_name, int width, int height, int format, int header_size);
//void ExportImage(Image image, char const * file_name);
//void ExportImageAsCode(Image image, char const * file_name);
//Texture2D LoadTexture(char const * file_name);
//Texture2D LoadTextureFromImage(Image image);
//TextureCubemap LoadTextureCubemap(Image image, int layout_type);
//RenderTexture2D LoadRenderTexture(int width, int height);
//void UnloadImage(Image image);
//void UnloadTexture(Texture2D texture);
//void UnloadRenderTexture(RenderTexture2D target);
//Color * GetImageData(Image image);
//Vector4 * GetImageDataNormalized(Image image);
//int GetPixelDataSize(int width, int height, int format);
//Image GetTextureData(Texture2D texture);
//Image GetScreenData();
//void UpdateTexture(Texture2D texture, void const * pixels);
//
//Image ImageCopy(Image image);
//void ImageToPOT(Image * image, Color fill_color);
//void ImageFormat(Image * image, int new_format);
//void ImageAlphaMask(Image * image, Image alpha_mask);
//void ImageAlphaClear(Image * image, Color color, float threshold);
//void ImageAlphaCrop(Image * image, float threshold);
//void ImageAlphaPremultiply(Image * image);
//void ImageCrop(Image * image, Rectangle crop);
//void ImageResize(Image * image, int new_width, int new_height);
//void ImageResizeNN(Image * image, int new_width, int new_height);
//void ImageResizeCanvas(Image * image, int new_width, int new_height, int offset_x, int offset_y, Color color);
//void ImageMipmaps(Image * image);
//void ImageDither(Image * image, int r_bpp, int g_bpp, int b_bpp, int a_bpp);
//Color * ImageExtractPalette(Image image, int maxPalette_size, int * extract_count);
//Image ImageText(char const * text, int font_size, Color color);
//Image ImageTextEx(Font font, char const * text, float font_size, float spacing, Color tint);
//void ImageDraw(Image * dst, Image src, Rectangle src_rec, Rectangle dst_rec);
//void ImageDrawRectangle(Image * dst, Rectangle rec, Color color);
//void ImageDrawRectangleLines(Image * dst, Rectangle rec, int thick, Color color);
//void ImageDrawText(Image * dst, Vector2 position, char const * text, int font_size, Color color);
//void ImageDrawTextEx(Image * dst, Vector2 position, Font font, char const * text, float font_size, float spacing, Color color);
//void ImageFlipVertical(Image * image);
//void ImageFlipHorizontal(Image * image);
//void ImageRotateCW(Image * image);
//void ImageRotateCCW(Image * image);
//void ImageColorTint(Image * image, Color color);
//void ImageColorInvert(Image * image);
//void ImageColorGrayscale(Image * image);
//void ImageColorContrast(Image * image, float contrast);
//void ImageColorBrightness(Image * image, int brightness);
//void ImageColorReplace(Image * image, Color color, Color replace);
//
//Image GenImageColor(int width, int height, Color color);
//Image GenImageGradientV(int width, int height, Color top, Color bottom);
//Image GenImageGradientH(int width, int height, Color left, Color right);
//Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);
//Image GenImageChecked(int width, int height, int checks_x, int checks_y, Color col1, Color col2);
//Image GenImageWhiteNoise(int width, int height, float factor);
//Image GenImagePerlinNoise(int width, int height, int offset_x, int offset_y, float scale);
//Image GenImageCellular(int width, int height, int tile_size);
//
//void GenTextureMipmaps(Texture2D * texture);
//void SetTextureFilter(Texture2D texture, int filter_mode);
//void SetTextureWrap(Texture2D texture, int wrap_mode);
//
//void DrawTexture(Texture2D texture, int pos_x, int pos_y, Color tint);
//void DrawTextureV(Texture2D texture, Vector2 position, Color tint);
//void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
//void DrawTextureRec(Texture2D texture, Rectangle source_rec, Vector2 position, Color tint);
//void DrawTextureQuad(Texture2D texture, Vector2 tiling, Vector2 offset, Rectangle quad, Color tint);
//void DrawTexturePro(Texture2D texture, Rectangle source_rec, Rectangle dest_rec, Vector2 origin, float rotation, Color tint);
//void DrawTextureNPatch(Texture2D texture, NPatchInfo n_patch_info, Rectangle dest_rec, Vector2 origin, float rotation, Color tint);
//
//Font GetFontDefault();
//Font LoadFont(char const * file_name);
//Font LoadFontEx(char const * file_name, int font_size, int * font_chars, int chars_count);
//Font LoadFontFromImage(Image image, Color key, int first_char);
//CharInfo * LoadFontData(char const * file_name, int font_size, int * font_chars, int chars_count, int type);
//Image GenImageFontAtlas(CharInfo * chars, int chars_count, int font_size, int padding, int pack_method);
//void UnloadFont(Font font);
//
//void DrawFPS(int pos_x, int pos_y);
//void DrawText(char const * text, int pos_x, int pos_y, int font_size, Color color);
//void DrawTextEx(Font font, char const * text, Vector2 position, float font_size, float spacing, Color tint);
//void DrawTextRec(Font font, char const * text, Rectangle rec, float font_size, float spacing, bool word_wrap, Color tint);
//void DrawTextRecEx(Font font, char const * text, Rectangle rec, float font_size, float spacing, bool word_wrap, Color tint, int select_start, int select_length, Color select_text, Color select_back);
//
//int MeasureText(char const * text, int font_size);
//Vector2 MeasureTextEx(Font font, char const * text, float font_size, float spacing);
//int GetGlyphIndex(Font font, int character);
//
//bool TextIsEqual(char const * text1, char const * text2);
//unsigned int TextLength(char const * text);
//
//char const * TextSubtext(char const * text, int position, int length);
//char const * TextReplace(char * text, char const * replace, char const * by);
//char const * TextInsert(char const * text, char const * insert, int position);
//char const * TextJoin(char const ** textList, int count, char const * delimiter);
//char const ** TextSplit(char const * text, char delimiter, int * count);
//void TextAppend(char * text, char const * append, int * position);
//int TextFindIndex(char const * text, char const * find);
//char const * TextToUpper(char const * text);
//char const * TextToLower(char const * text);
//char const * TextToPascal(char const * text);
//int TextToInteger(char const * text);
//
//void DrawLine3D(Vector3 start_pos, Vector3 end_pos, Color color);
//void DrawCircle3D(Vector3 center, float radius, Vector3 rotation_axis, float rotation_angle, Color color);
//void DrawCube(Vector3 position, float width, float height, float length, Color color);
//void DrawCubeV(Vector3 position, Vector3 size, Color color);
//void DrawCubeWires(Vector3 position, float width, float height, float length, Color color);
//void DrawCubeWiresV(Vector3 position, Vector3 size, Color color);
//void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color);
//void DrawSphere(Vector3 center_pos, float radius, Color color);
//void DrawSphereEx(Vector3 center_pos, float radius, int rings, int slices, Color color);
//void DrawSphereWires(Vector3 center_pos, float radius, int rings, int slices, Color color);
//void DrawCylinder(Vector3 position, float radius_top, float radius_bottom, float height, int slices, Color color);
//void DrawCylinderWires(Vector3 position, float radius_top, float radius_bottom, float height, int slices, Color color);
//void DrawPlane(Vector3 center_pos, Vector2 size, Color color);
//void DrawRay(Ray ray, Color color);
//void DrawGrid(int slices, float spacing);
//void DrawGizmo(Vector3 position);
//
//Model LoadModel(char const * file_name);
//Model LoadModelFromMesh(Mesh mesh);
//void UnloadModel(Model model);
//
//Mesh * LoadMeshes(char const * file_name, int * mesh_count);
//void ExportMesh(Mesh mesh, char const * file_name);
//void UnloadMesh(Mesh * mesh);
//
//Material * LoadMaterials(char const * file_name, int * material_count);
//Material LoadMaterialDefault();
//void UnloadMaterial(Material material);
//void SetMaterialTexture(Material * material, int map_type, Texture2D texture);
//void SetModelMeshMaterial(Model * model, int meshId, int materialId);
//
//ModelAnimation * LoadModelAnimations(char const * file_name, int * anims_count);
//void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);
//void UnloadModelAnimation(ModelAnimation anim);
//bool IsModelAnimationValid(Model model, ModelAnimation anim);
//
//Mesh GenMeshPoly(int sides, float radius);
//Mesh GenMeshPlane(float width, float length, int res_x, int res_z);
//Mesh GenMeshCube(float width, float height, float length);
//Mesh GenMeshSphere(float radius, int rings, int slices);
//Mesh GenMeshHemiSphere(float radius, int rings, int slices);
//Mesh GenMeshCylinder(float radius, float height, int slices);
//Mesh GenMeshTorus(float radius, float size, int rad_seg, int sides);
//Mesh GenMeshKnot(float radius, float size, int rad_seg, int sides);
//Mesh GenMeshHeightmap(Image heightmap, Vector3 size);
//Mesh GenMeshCubicmap(Image cubicmap, Vector3 cube_size);
//
//BoundingBox MeshBoundingBox(Mesh mesh);
//void MeshTangents(Mesh * mesh);
//void MeshBinormals(Mesh * mesh);
//
//void DrawModel(Model model, Vector3 position, float scale, Color tint);
//void DrawModelEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle, Vector3 scale, Color tint);
//void DrawModelWires(Model model, Vector3 position, float scale, Color tint);
//void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle, Vector3 scale, Color tint);
//void DrawBoundingBox(BoundingBox box, Color color);
//void DrawBillboard(Camera camera, Texture2D texture, Vector3 center, float size, Color tint);
//void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle source_rec, Vector3 center, float size, Color tint);
//
//bool CheckCollisionSpheres(Vector3 center_a, float radius_a, Vector3 center_b, float radius_b);
//bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2);
//bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center_sphere, float radius_sphere);
//bool CheckCollisionRaySphere(Ray ray, Vector3 sphere_position, float sphere_radius);
//bool CheckCollisionRaySphereEx(Ray ray, Vector3 sphere_position, float sphere_radius, Vector3 * collision_point);
//bool CheckCollisionRayBox(Ray ray, BoundingBox box);
//RayHitInfo GetCollisionRayModel(Ray ray, Model * model);
//RayHitInfo GetCollisionRayTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);
//RayHitInfo GetCollisionRayGround(Ray ray, float ground_height);
//
//char * LoadText(char const * file_name);
//Shader LoadShader(char const * vs_file_name, char const * fs_file_name);
//Shader LoadShaderCode(char * vs_code, char * fs_code);
//void UnloadShader(Shader shader);
//
//Shader GetShaderDefault();
//Texture2D GetTextureDefault();
//
//int GetShaderLocation(Shader shader, char const * uniform_name);
//void SetShaderValue(Shader shader, int uniform_loc, void const * value, int uniform_type);
//void SetShaderValueV(Shader shader, int uniform_loc, void const * value, int uniform_type, int count);
//void SetShaderValueMatrix(Shader shader, int uniform_loc, Matrix mat);
//void SetShaderValueTexture(Shader shader, int uniform_loc, Texture2D texture);
//void SetMatrixProjection(Matrix proj);
//void SetMatrixModelview(Matrix view);
//Matrix GetMatrixModelview();
//
//Texture2D GenTextureCubemap(Shader shader, Texture2D sky_hdr, int size);
//Texture2D GenTextureIrradiance(Shader shader, Texture2D cubemap, int size);
//Texture2D GenTexturePrefilter(Shader shader, Texture2D cubemap, int size);
//Texture2D GenTextureBRDF(Shader shader, int size);
//
//void BeginShaderMode(Shader shader);
//void EndShaderMode();
//void BeginBlendMode(int mode);
//void EndBlendMode();
//void BeginScissorMode(int x, int y, int width, int height);
//void EndScissorMode();
//
//VrDeviceInfo GetVrDeviceInfo(int vr_device_type);
//void InitVrSimulator(VrDeviceInfo info);
//void UpdateVrTracking(Camera * camera);
//void CloseVrSimulator();
//bool IsVrSimulatorReady();
//void ToggleVrMode();
//void BeginVrDrawing();
//void EndVrDrawing();
//
//void InitAudioDevice();
//void CloseAudioDevice();
//bool IsAudioDeviceReady();
//void SetMasterVolume(float volume);
//
//Wave LoadWave(char const * file_name);
//Wave LoadWaveEx(void * data, int sample_count, int sample_rate, int sample_size, int channels);
//Sound LoadSound(char const * file_name);
//Sound LoadSoundFromWave(Wave wave);
//void UpdateSound(Sound sound, void const * data, int samples_count);
//void UnloadWave(Wave wave);
//void UnloadSound(Sound sound);
//void ExportWave(Wave wave, char const * file_name);
//void ExportWaveAsCode(Wave wave, char const * file_name);
//
//void PlaySound(Sound sound);
//void PauseSound(Sound sound);
//void ResumeSound(Sound sound);
//void StopSound(Sound sound);
//bool IsSoundPlaying(Sound sound);
//void SetSoundVolume(Sound sound, float volume);
//void SetSoundPitch(Sound sound, float pitch);
//void WaveFormat(Wave * wave, int sample_rate, int sample_size, int channels);
//Wave WaveCopy(Wave wave);
//void WaveCrop(Wave * wave, int init_sample, int final_sample);
//float * GetWaveData(Wave wave);
//
//Music LoadMusicStream(char const * file_name);
//void UnloadMusicStream(Music music);
//void PlayMusicStream(Music music);
//void UpdateMusicStream(Music music);
//void StopMusicStream(Music music);
//void PauseMusicStream(Music music);
//void ResumeMusicStream(Music music);
//bool IsMusicPlaying(Music music);
//void SetMusicVolume(Music music, float volume);
//void SetMusicPitch(Music music, float pitch);
//void SetMusicLoopCount(Music music, int count);
//float GetMusicTimeLength(Music music);
//float GetMusicTimePlayed(Music music);
//
//AudioStream InitAudioStream(unsigned int sample_rate, unsigned int sample_size, unsigned int channels);
//void UpdateAudioStream(AudioStream stream, void const * data, int samples_count);
//void CloseAudioStream(AudioStream stream);
//bool IsAudioBufferProcessed(AudioStream stream);
//void PlayAudioStream(AudioStream stream);
//void PauseAudioStream(AudioStream stream);
//void ResumeAudioStream(AudioStream stream);
//bool IsAudioStreamPlaying(AudioStream stream);
//void StopAudioStream(AudioStream stream);
//void SetAudioStreamVolume(AudioStream stream, float volume);
//void SetAudioStreamPitch(AudioStream stream, float pitch);

bool luaopen_ray(lua_State * L)
{
    if (!luaopen_ray_core(L)) {
        tDLogW("luaopen_ray() ray-core load failed.");
    }
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

