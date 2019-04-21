/**
 * @file   RayLuaBinding.cpp
 * @brief  RayLuaBinding class implementation.
 * @author zer0
 * @date   2019-04-20
 */

#include <libtbag/ray/RayLuaBinding.hpp>
#include <libtbag/ray/RayBypass.hpp>
#include <libtbag/ray/RayGuiBypass.hpp>
#include <libtbag/script/Sol2Bypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ray {

using namespace libtbag::script;

template <typename T>
static void _new_vector4_type(sol::table & t, std::string const & name)
{
    t.new_usertype<T>(name,
                      "x", &T::x,
                      "y", &T::y,
                      "z", &T::z,
                      "w", &T::w);
}

template <typename T>
static void _new_texture2d_type(sol::table & t, std::string const & name)
{
    t.new_usertype<T>(name,
                      "id", &T::id,
                      "width", &T::width,
                      "height", &T::height,
                      "mipmaps", &T::mipmaps,
                      "format", &T::format);
}

template <typename T>
static void _new_rendertexture2d_type(sol::table & t, std::string const & name)
{
    t.new_usertype<T>(name,
                      "id", &T::id,
                      "texture", &T::texture,
                      "depth", &T::depth,
                      "depthTexture", &T::depthTexture);
}

template <typename T>
static void _new_font_type(sol::table & t, std::string const & name)
{
    t.new_usertype<T>(name,
                      "texture", &T::texture,
                      "baseSize", &T::baseSize,
                      "charsCount", &T::charsCount,
                      "offsetY", &T::chars);
}

template <typename T>
static void _new_camera3d_type(sol::table & t, std::string const & name)
{
    t.new_usertype<T>(name,
                      "position", &T::position,
                      "target", &T::target,
                      "up", &T::up,
                      "fovy", &T::fovy,
                      "type", &T::type);
}

bool bindRayForLua(lua_State * L)
{
    sol::state_view lua(L);
    auto tbag = lua["tbag"].get_or_create<sol::table>();
    auto ray = tbag["ray"].get_or_create<sol::table>();

    ray.new_usertype<Vector2>("Vector2",
                              "x", &Vector2::x,
                              "y", &Vector2::y);
    ray.new_usertype<Vector3>("Vector3",
                              "x", &Vector3::x,
                              "y", &Vector3::y,
                              "z", &Vector3::z);

    _new_vector4_type<Vector4>(ray, "Vector4");
    _new_vector4_type<Quaternion>(ray, "Quaternion");

    ray.new_usertype<Matrix>("Matrix",
                             "m0", &Matrix::m0, "m4", &Matrix::m4, "m8", &Matrix::m8, "m12", &Matrix::m12,
                             "m1", &Matrix::m1, "m5", &Matrix::m5, "m9", &Matrix::m9, "m13", &Matrix::m13,
                             "m2", &Matrix::m2, "m6", &Matrix::m6, "m10", &Matrix::m10, "m14", &Matrix::m14,
                             "m3", &Matrix::m3, "m7", &Matrix::m7, "m11", &Matrix::m11, "m15", &Matrix::m15);
    ray.new_usertype<Color>("Color",
                            "r", &Color::r,
                            "g", &Color::g,
                            "b", &Color::b,
                            "a", &Color::a);
    ray.new_usertype<Rectangle>("Rectangle",
                                "x", &Rectangle::x,
                                "y", &Rectangle::y,
                                "width", &Rectangle::width,
                                "height", &Rectangle::height);
    ray.new_usertype<Image>("Image",
                            "data", &Image::data,
                            "width", &Image::width,
                            "height", &Image::height,
                            "mipmaps", &Image::mipmaps,
                            "format", &Image::format);

    _new_texture2d_type<Texture2D>(ray, "Texture2D");
    _new_texture2d_type<Texture>(ray, "Texture2D");
    _new_texture2d_type<TextureCubemap>(ray, "Texture2D");

    _new_rendertexture2d_type<RenderTexture2D>(ray, "RenderTexture2D");
    _new_rendertexture2d_type<RenderTexture>(ray, "RenderTexture");

    ray.new_usertype<NPatchInfo>("NPatchInfo",
                                 "sourceRec", &NPatchInfo::sourceRec,
                                 "left", &NPatchInfo::left,
                                 "top", &NPatchInfo::top,
                                 "right", &NPatchInfo::right,
                                 "bottom", &NPatchInfo::bottom,
                                 "type", &NPatchInfo::type);
    ray.new_usertype<CharInfo>("CharInfo",
                               "value", &CharInfo::value,
                               "rec", &CharInfo::rec,
                               "offsetX", &CharInfo::offsetX,
                               "offsetY", &CharInfo::offsetY,
                               "advanceX", &CharInfo::advanceX,
                               "data", &CharInfo::data);

    _new_font_type<Font>(ray, "Font");
    _new_font_type<SpriteFont>(ray, "SpriteFont");

    _new_camera3d_type<Camera3D>(ray, "Camera3D");
    _new_camera3d_type<Camera>(ray, "Camera");

    ray.new_usertype<Camera2D>("Camera2D",
                               "offset", &Camera2D::offset,
                               "target", &Camera2D::target,
                               "rotation", &Camera2D::rotation,
                               "zoom", &Camera2D::zoom);
    ray.new_usertype<BoundingBox>("BoundingBox",
                                  "min", &BoundingBox::min,
                                  "max", &BoundingBox::max);
    ray.new_usertype<Mesh>("Mesh",
                           "vertexCount", &Mesh::vertexCount,
                           "triangleCount", &Mesh::triangleCount,
                           "vertices", &Mesh::vertices,
                           "texcoords", &Mesh::texcoords,
                           "texcoords2", &Mesh::texcoords2,
                           "normals", &Mesh::normals,
                           "tangents", &Mesh::tangents,
                           "colors", &Mesh::colors,
                           "indices", &Mesh::indices,
                           "animVertices", &Mesh::animVertices,
                           "animNormals", &Mesh::animNormals,
                           "boneIds", &Mesh::boneIds,
                           "boneWeights", &Mesh::boneWeights,
                           "vaoId", &Mesh::vaoId,
                           "vboId", sol::property([](Mesh & self) { return self.vboId; }));

//    ray.new_usertype<Shader>("Shader",
//                             "id", &Shader::id,
//                             "locs", &Shader::locs);

//    typedef struct MaterialMap {
//        Texture2D texture;
//        Color color;
//        float value;
//    } MaterialMap;

//    typedef struct Material {
//        Shader shader;
//        MaterialMap maps[MAX_MATERIAL_MAPS];
//        float * params;
//    } Material;

//    typedef struct Transform {
//        Vector3 translation;
//        Quaternion rotation;
//        Vector3 scale;
//    } Transform;

//    typedef struct BoneInfo {
//        char name[32];
//        int parent;
//    } BoneInfo;

//    typedef struct Model {
//        Matrix transform;
//        int meshCount;
//        Mesh * meshes;
//        int materialCount;
//        Material * materials;
//        int * meshMaterial;
//        int boneCount;
//        BoneInfo * bones;
//        Transform * bindPose;
//    } Model;

//    typedef struct ModelAnimation {
//        int boneCount;
//        BoneInfo * bones;
//        int frameCount;
//        Transform ** framePoses;
//    } ModelAnimation;

//    typedef struct Ray {
//        Vector3 position;
//        Vector3 direction;
//    } Ray;

//    typedef struct RayHitInfo {
//        bool hit;
//        float distance;
//        Vector3 position;
//        Vector3 normal;
//    } RayHitInfo;

//    typedef struct Wave {
//        unsigned int sampleCount;
//        unsigned int sampleRate;
//        unsigned int sampleSize;
//        unsigned int channels;
//        void * data;
//    } Wave;

//    typedef struct Sound {
//        void * audioBuffer;
//        unsigned int source;
//        unsigned int buffer;
//        int format;
//    } Sound;

//    typedef struct MusicData * Music;

//    typedef struct AudioStream {
//        unsigned int sampleRate;
//        unsigned int sampleSize;
//        unsigned int channels;
//        void * audioBuffer;
//        int format;
//        unsigned int source;
//        unsigned int buffers[2];
//    } AudioStream;

//    typedef struct VrDeviceInfo {
//        int hResolution;
//        int vResolution;
//        float hScreenSize;
//        float vScreenSize;
//        float vScreenCenter;
//        float eyeToScreenDistance;
//        float lensSeparationDistance;
//        float interpupillaryDistance;
//        float lensDistortionValues[4];
//        float chromaAbCorrection[4];
//    } VrDeviceInfo;

//    typedef struct VrStereoConfig {
//        RenderTexture2D stereoFbo;
//        Shader distortionShader;
//        Matrix eyesProjection[2];
//        Matrix eyesViewOffset[2];
//        int eyeViewportRight[4];
//        int eyeViewportLeft[4];
//    } VrStereoConfig;

//    typedef enum {
//        FLAG_SHOW_LOGO          = 1,
//        FLAG_FULLSCREEN_MODE    = 2,
//        FLAG_WINDOW_RESIZABLE   = 4,
//        FLAG_WINDOW_UNDECORATED = 8,
//        FLAG_WINDOW_TRANSPARENT = 16,
//        FLAG_WINDOW_HIDDEN      = 128,
//        FLAG_MSAA_4X_HINT       = 32,
//        FLAG_VSYNC_HINT         = 64
//    } ConfigFlag;

//    typedef enum {
//        LOG_ALL = 0,
//        LOG_TRACE,
//        LOG_DEBUG,
//        LOG_INFO,
//        LOG_WARNING,
//        LOG_ERROR,
//        LOG_FATAL,
//        LOG_NONE
//    } TraceLogType;

//    typedef enum {
//        KEY_APOSTROPHE      = 39,
//        KEY_COMMA           = 44,
//        KEY_MINUS           = 45,
//        KEY_PERIOD          = 46,
//        KEY_SLASH           = 47,
//        KEY_ZERO            = 48,
//        KEY_ONE             = 49,
//        KEY_TWO             = 50,
//        KEY_THREE           = 51,
//        KEY_FOUR            = 52,
//        KEY_FIVE            = 53,
//        KEY_SIX             = 54,
//        KEY_SEVEN           = 55,
//        KEY_EIGHT           = 56,
//        KEY_NINE            = 57,
//        KEY_SEMICOLON       = 59,
//        KEY_EQUAL           = 61,
//        KEY_A               = 65,
//        KEY_B               = 66,
//        KEY_C               = 67,
//        KEY_D               = 68,
//        KEY_E               = 69,
//        KEY_F               = 70,
//        KEY_G               = 71,
//        KEY_H               = 72,
//        KEY_I               = 73,
//        KEY_J               = 74,
//        KEY_K               = 75,
//        KEY_L               = 76,
//        KEY_M               = 77,
//        KEY_N               = 78,
//        KEY_O               = 79,
//        KEY_P               = 80,
//        KEY_Q               = 81,
//        KEY_R               = 82,
//        KEY_S               = 83,
//        KEY_T               = 84,
//        KEY_U               = 85,
//        KEY_V               = 86,
//        KEY_W               = 87,
//        KEY_X               = 88,
//        KEY_Y               = 89,
//        KEY_Z               = 90,

//        KEY_SPACE           = 32,
//        KEY_ESCAPE          = 256,
//        KEY_ENTER           = 257,
//        KEY_TAB             = 258,
//        KEY_BACKSPACE       = 259,
//        KEY_INSERT          = 260,
//        KEY_DELETE          = 261,
//        KEY_RIGHT           = 262,
//        KEY_LEFT            = 263,
//        KEY_DOWN            = 264,
//        KEY_UP              = 265,
//        KEY_PAGE_UP         = 266,
//        KEY_PAGE_DOWN       = 267,
//        KEY_HOME            = 268,
//        KEY_END             = 269,
//        KEY_CAPS_LOCK       = 280,
//        KEY_SCROLL_LOCK     = 281,
//        KEY_NUM_LOCK        = 282,
//        KEY_PRINT_SCREEN    = 283,
//        KEY_PAUSE           = 284,
//        KEY_F1              = 290,
//        KEY_F2              = 291,
//        KEY_F3              = 292,
//        KEY_F4              = 293,
//        KEY_F5              = 294,
//        KEY_F6              = 295,
//        KEY_F7              = 296,
//        KEY_F8              = 297,
//        KEY_F9              = 298,
//        KEY_F10             = 299,
//        KEY_F11             = 300,
//        KEY_F12             = 301,
//        KEY_LEFT_SHIFT      = 340,
//        KEY_LEFT_CONTROL    = 341,
//        KEY_LEFT_ALT        = 342,
//        KEY_LEFT_SUPER      = 343,
//        KEY_RIGHT_SHIFT     = 344,
//        KEY_RIGHT_CONTROL   = 345,
//        KEY_RIGHT_ALT       = 346,
//        KEY_RIGHT_SUPER     = 347,
//        KEY_KB_MENU         = 348,
//        KEY_LEFT_BRACKET    = 91,
//        KEY_BACKSLASH       = 92,
//        KEY_RIGHT_BRACKET   = 93,
//        KEY_GRAVE           = 96,

//        KEY_KP_0            = 320,
//        KEY_KP_1            = 321,
//        KEY_KP_2            = 322,
//        KEY_KP_3            = 323,
//        KEY_KP_4            = 324,
//        KEY_KP_5            = 325,
//        KEY_KP_6            = 326,
//        KEY_KP_7            = 327,
//        KEY_KP_8            = 328,
//        KEY_KP_9            = 329,
//        KEY_KP_DECIMAL      = 330,
//        KEY_KP_DIVIDE       = 331,
//        KEY_KP_MULTIPLY     = 332,
//        KEY_KP_SUBTRACT     = 333,
//        KEY_KP_ADD          = 334,
//        KEY_KP_ENTER        = 335,
//        KEY_KP_EQUAL        = 336
//    } KeyboardKey;

//    typedef enum {
//        KEY_BACK            = 4,
//        KEY_MENU            = 82,
//        KEY_VOLUME_UP       = 24,
//        KEY_VOLUME_DOWN     = 25
//    } AndroidButton;

//    typedef enum {
//        MOUSE_LEFT_BUTTON   = 0,
//        MOUSE_RIGHT_BUTTON  = 1,
//        MOUSE_MIDDLE_BUTTON = 2
//    } MouseButton;

//    typedef enum {
//        GAMEPAD_PLAYER1     = 0,
//        GAMEPAD_PLAYER2     = 1,
//        GAMEPAD_PLAYER3     = 2,
//        GAMEPAD_PLAYER4     = 3
//    } GamepadNumber;

//    typedef enum {
//        GAMEPAD_PS3_BUTTON_TRIANGLE = 0,
//        GAMEPAD_PS3_BUTTON_CIRCLE   = 1,
//        GAMEPAD_PS3_BUTTON_CROSS    = 2,
//        GAMEPAD_PS3_BUTTON_SQUARE   = 3,
//        GAMEPAD_PS3_BUTTON_L1       = 6,
//        GAMEPAD_PS3_BUTTON_R1       = 7,
//        GAMEPAD_PS3_BUTTON_L2       = 4,
//        GAMEPAD_PS3_BUTTON_R2       = 5,
//        GAMEPAD_PS3_BUTTON_START    = 8,
//        GAMEPAD_PS3_BUTTON_SELECT   = 9,
//        GAMEPAD_PS3_BUTTON_PS       = 12,
//        GAMEPAD_PS3_BUTTON_UP       = 24,
//        GAMEPAD_PS3_BUTTON_RIGHT    = 25,
//        GAMEPAD_PS3_BUTTON_DOWN     = 26,
//        GAMEPAD_PS3_BUTTON_LEFT     = 27
//    } GamepadPS3Button;

//    typedef enum {
//        GAMEPAD_PS3_AXIS_LEFT_X     = 0,
//        GAMEPAD_PS3_AXIS_LEFT_Y     = 1,
//        GAMEPAD_PS3_AXIS_RIGHT_X    = 2,
//        GAMEPAD_PS3_AXIS_RIGHT_Y    = 5,
//        GAMEPAD_PS3_AXIS_L2         = 3,
//        GAMEPAD_PS3_AXIS_R2         = 4
//    } GamepadPS3Axis;

//    typedef enum {
//        GAMEPAD_XBOX_BUTTON_A       = 0,
//        GAMEPAD_XBOX_BUTTON_B       = 1,
//        GAMEPAD_XBOX_BUTTON_X       = 2,
//        GAMEPAD_XBOX_BUTTON_Y       = 3,
//        GAMEPAD_XBOX_BUTTON_LB      = 4,
//        GAMEPAD_XBOX_BUTTON_RB      = 5,
//        GAMEPAD_XBOX_BUTTON_SELECT  = 6,
//        GAMEPAD_XBOX_BUTTON_START   = 7,
//        GAMEPAD_XBOX_BUTTON_HOME    = 8,
//        GAMEPAD_XBOX_BUTTON_UP      = 10,
//        GAMEPAD_XBOX_BUTTON_RIGHT   = 11,
//        GAMEPAD_XBOX_BUTTON_DOWN    = 12,
//        GAMEPAD_XBOX_BUTTON_LEFT    = 13
//    } GamepadXbox360Button;

//    typedef enum {
//        GAMEPAD_XBOX_AXIS_LEFT_X    = 0,
//        GAMEPAD_XBOX_AXIS_LEFT_Y    = 1,
//        GAMEPAD_XBOX_AXIS_RIGHT_X   = 2,
//        GAMEPAD_XBOX_AXIS_RIGHT_Y   = 3,
//        GAMEPAD_XBOX_AXIS_LT        = 4,
//        GAMEPAD_XBOX_AXIS_RT        = 5
//    } GamepadXbox360Axis;

//    typedef enum {
//        GAMEPAD_ANDROID_DPAD_UP     = 19,
//        GAMEPAD_ANDROID_DPAD_DOWN   = 20,
//        GAMEPAD_ANDROID_DPAD_LEFT   = 21,
//        GAMEPAD_ANDROID_DPAD_RIGHT  = 22,
//        GAMEPAD_ANDROID_DPAD_CENTER = 23,
//        GAMEPAD_ANDROID_BUTTON_A    = 96,
//        GAMEPAD_ANDROID_BUTTON_B    = 97,
//        GAMEPAD_ANDROID_BUTTON_C    = 98,
//        GAMEPAD_ANDROID_BUTTON_X    = 99,
//        GAMEPAD_ANDROID_BUTTON_Y    = 100,
//        GAMEPAD_ANDROID_BUTTON_Z    = 101,
//        GAMEPAD_ANDROID_BUTTON_L1   = 102,
//        GAMEPAD_ANDROID_BUTTON_R1   = 103,
//        GAMEPAD_ANDROID_BUTTON_L2   = 104,
//        GAMEPAD_ANDROID_BUTTON_R2   = 105
//    } GamepadAndroid;

//    typedef enum {
//        LOC_VERTEX_POSITION = 0,
//        LOC_VERTEX_TEXCOORD01,
//        LOC_VERTEX_TEXCOORD02,
//        LOC_VERTEX_NORMAL,
//        LOC_VERTEX_TANGENT,
//        LOC_VERTEX_COLOR,
//        LOC_MATRIX_MVP,
//        LOC_MATRIX_MODEL,
//        LOC_MATRIX_VIEW,
//        LOC_MATRIX_PROJECTION,
//        LOC_VECTOR_VIEW,
//        LOC_COLOR_DIFFUSE,
//        LOC_COLOR_SPECULAR,
//        LOC_COLOR_AMBIENT,
//        LOC_MAP_ALBEDO,
//        LOC_MAP_METALNESS,
//        LOC_MAP_NORMAL,
//        LOC_MAP_ROUGHNESS,
//        LOC_MAP_OCCLUSION,
//        LOC_MAP_EMISSION,
//        LOC_MAP_HEIGHT,
//        LOC_MAP_CUBEMAP,
//        LOC_MAP_IRRADIANCE,
//        LOC_MAP_PREFILTER,
//        LOC_MAP_BRDF
//    } ShaderLocationIndex;

//#define LOC_MAP_DIFFUSE   LOC_MAP_ALBEDO
//#define LOC_MAP_SPECULAR  LOC_MAP_METALNESS

//    typedef enum {
//        UNIFORM_FLOAT = 0,
//        UNIFORM_VEC2,
//        UNIFORM_VEC3,
//        UNIFORM_VEC4,
//        UNIFORM_INT,
//        UNIFORM_IVEC2,
//        UNIFORM_IVEC3,
//        UNIFORM_IVEC4,
//        UNIFORM_SAMPLER2D
//    } ShaderUniformDataType;

//    typedef enum {
//        MAP_ALBEDO    = 0,
//        MAP_METALNESS = 1,
//        MAP_NORMAL    = 2,
//        MAP_ROUGHNESS = 3,
//        MAP_OCCLUSION,
//        MAP_EMISSION,
//        MAP_HEIGHT,
//        MAP_CUBEMAP,
//        MAP_IRRADIANCE,
//        MAP_PREFILTER,
//        MAP_BRDF
//    } MaterialMapType;

//#define MAP_DIFFUSE   MAP_ALBEDO
//#define MAP_SPECULAR  MAP_METALNESS

//    typedef enum {
//        UNCOMPRESSED_GRAYSCALE = 1,
//        UNCOMPRESSED_GRAY_ALPHA,
//        UNCOMPRESSED_R5G6B5,
//        UNCOMPRESSED_R8G8B8,
//        UNCOMPRESSED_R5G5B5A1,
//        UNCOMPRESSED_R4G4B4A4,
//        UNCOMPRESSED_R8G8B8A8,
//        UNCOMPRESSED_R32,
//        UNCOMPRESSED_R32G32B32,
//        UNCOMPRESSED_R32G32B32A32,
//        COMPRESSED_DXT1_RGB,
//        COMPRESSED_DXT1_RGBA,
//        COMPRESSED_DXT3_RGBA,
//        COMPRESSED_DXT5_RGBA,
//        COMPRESSED_ETC1_RGB,
//        COMPRESSED_ETC2_RGB,
//        COMPRESSED_ETC2_EAC_RGBA,
//        COMPRESSED_PVRT_RGB,
//        COMPRESSED_PVRT_RGBA,
//        COMPRESSED_ASTC_4x4_RGBA,
//        COMPRESSED_ASTC_8x8_RGBA
//    } PixelFormat;

//    typedef enum {
//        FILTER_POINT = 0,
//        FILTER_BILINEAR,
//        FILTER_TRILINEAR,
//        FILTER_ANISOTROPIC_4X,
//        FILTER_ANISOTROPIC_8X,
//        FILTER_ANISOTROPIC_16X,
//    } TextureFilterMode;

//    typedef enum {
//        CUBEMAP_AUTO_DETECT = 0,
//        CUBEMAP_LINE_VERTICAL,
//        CUBEMAP_LINE_HORIZONTAL,
//        CUBEMAP_CROSS_THREE_BY_FOUR,
//        CUBEMAP_CROSS_FOUR_BY_THREE,
//        CUBEMAP_PANORAMA
//    } CubemapLayoutType;

//    typedef enum {
//        WRAP_REPEAT = 0,
//        WRAP_CLAMP,
//        WRAP_MIRROR_REPEAT,
//        WRAP_MIRROR_CLAMP
//    } TextureWrapMode;

//    typedef enum {
//        FONT_DEFAULT = 0,
//        FONT_BITMAP,
//        FONT_SDF
//    } FontType;

//    typedef enum {
//        BLEND_ALPHA = 0,
//        BLEND_ADDITIVE,
//        BLEND_MULTIPLIED
//    } BlendMode;

//    typedef enum {
//        GESTURE_NONE        = 0,
//        GESTURE_TAP         = 1,
//        GESTURE_DOUBLETAP   = 2,
//        GESTURE_HOLD        = 4,
//        GESTURE_DRAG        = 8,
//        GESTURE_SWIPE_RIGHT = 16,
//        GESTURE_SWIPE_LEFT  = 32,
//        GESTURE_SWIPE_UP    = 64,
//        GESTURE_SWIPE_DOWN  = 128,
//        GESTURE_PINCH_IN    = 256,
//        GESTURE_PINCH_OUT   = 512
//    } GestureType;

//    typedef enum {
//        CAMERA_CUSTOM = 0,
//        CAMERA_FREE,
//        CAMERA_ORBITAL,
//        CAMERA_FIRST_PERSON,
//        CAMERA_THIRD_PERSON
//    } CameraMode;

//    typedef enum {
//        CAMERA_PERSPECTIVE = 0,
//        CAMERA_ORTHOGRAPHIC
//    } CameraType;

//    typedef enum {
//        HMD_DEFAULT_DEVICE = 0,
//        HMD_OCULUS_RIFT_DK2,
//        HMD_OCULUS_RIFT_CV1,
//        HMD_OCULUS_GO,
//        HMD_VALVE_HTC_VIVE,
//        HMD_SONY_PSVR
//    } VrDeviceType;

//    typedef enum {
//        NPT_9PATCH = 0,
//        NPT_3PATCH_VERTICAL,
//        NPT_3PATCH_HORIZONTAL
//    } NPatchType;

    return true;
}

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

