#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdarg.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

#define MAX_TOUCH_POINTS        10
#define MAX_SHADER_LOCATIONS    32
#define MAX_MATERIAL_MAPS       12

#if defined(__cplusplus)
#define CLITERAL
#else
#define CLITERAL  (Color)
#endif

/*
 * #define LIGHTGRAY  CLITERAL{ 200, 200, 200, 255 }
 * #define GRAY       CLITERAL{ 130, 130, 130, 255 }
 * #define DARKGRAY   CLITERAL{ 80, 80, 80, 255 }
 * #define YELLOW     CLITERAL{ 253, 249, 0, 255 }
 * #define GOLD       CLITERAL{ 255, 203, 0, 255 }
 * #define ORANGE     CLITERAL{ 255, 161, 0, 255 }
 * #define PINK       CLITERAL{ 255, 109, 194, 255 }
 * #define RED        CLITERAL{ 230, 41, 55, 255 }
 * #define MAROON     CLITERAL{ 190, 33, 55, 255 }
 * #define GREEN      CLITERAL{ 0, 228, 48, 255 }
 * #define LIME       CLITERAL{ 0, 158, 47, 255 }
 * #define DARKGREEN  CLITERAL{ 0, 117, 44, 255 }
 * #define SKYBLUE    CLITERAL{ 102, 191, 255, 255 }
 * #define BLUE       CLITERAL{ 0, 121, 241, 255 }
 * #define DARKBLUE   CLITERAL{ 0, 82, 172, 255 }
 * #define PURPLE     CLITERAL{ 200, 122, 255, 255 }
 * #define VIOLET     CLITERAL{ 135, 60, 190, 255 }
 * #define DARKPURPLE CLITERAL{ 112, 31, 126, 255 }
 * #define BEIGE      CLITERAL{ 211, 176, 131, 255 }
 * #define BROWN      CLITERAL{ 127, 106, 79, 255 }
 * #define DARKBROWN  CLITERAL{ 76, 63, 47, 255 }
 * #define WHITE      CLITERAL{ 255, 255, 255, 255 }
 * #define BLACK      CLITERAL{ 0, 0, 0, 255 }
 * #define BLANK      CLITERAL{ 0, 0, 0, 0 }
 * #define MAGENTA    CLITERAL{ 255, 0, 255, 255 }
 * #define RAYWHITE   CLITERAL{ 245, 245, 245, 255 }
 */

#define FormatText  TextFormat
#define SubText     TextSubtext
#define ShowWindow  UnhideWindow

#if defined(__STDC__) && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
typedef enum { false, true } bool;
#endif

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} Vector4;

typedef Vector4 Quaternion;

typedef struct Matrix {
    float m0, m4, m8, m12;
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
} Matrix;

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

typedef struct Rectangle2 {
    float x;
    float y;
    float width;
    float height;
} Rectangle2;

typedef struct Image {
    void * data;
    int width;
    int height;
    int mipmaps;
    int format;
} Image;

typedef struct Texture2D {
    unsigned int id;
    int width;
    int height;
    int mipmaps;
    int format;
} Texture2D;

typedef Texture2D Texture;
typedef Texture2D TextureCubemap;

typedef struct RenderTexture2D {
    unsigned int id;
    Texture2D texture;
    Texture2D depth;
    bool depthTexture;
} RenderTexture2D;

typedef RenderTexture2D RenderTexture;

typedef struct NPatchInfo {
    Rectangle2 sourceRec;
    int left;
    int top;
    int right;
    int bottom;
    int type;
} NPatchInfo;

typedef struct CharInfo {
    int value;
    Rectangle2 rec;
    int offsetX;
    int offsetY;
    int advanceX;
    unsigned char * data;
} CharInfo;

typedef struct Font {
    Texture2D texture;
    int baseSize;
    int charsCount;
    CharInfo * chars;
} Font;

#define SpriteFont Font

typedef struct Camera3D {
    Vector3 position;
    Vector3 target;
    Vector3 up;
    float fovy;
    int type;
} Camera3D;

#define Camera Camera3D

typedef struct Camera2D {
    Vector2 offset;
    Vector2 target;
    float rotation;
    float zoom;
} Camera2D;

typedef struct BoundingBox {
    Vector3 min;
    Vector3 max;
} BoundingBox;

typedef struct Mesh {
    int vertexCount;
    int triangleCount;
    float * vertices;
    float * texcoords;
    float * texcoords2;
    float * normals;
    float * tangents;
    unsigned char * colors;
    unsigned short * indices;
    float * animVertices;
    float * animNormals;
    int * boneIds;
    float * boneWeights;
    unsigned int vaoId;
    unsigned int vboId[7];
} Mesh;

typedef struct Shader {
    unsigned int id;
    int locs[MAX_SHADER_LOCATIONS];
} Shader;

typedef struct MaterialMap {
    Texture2D texture;
    Color color;
    float value;
} MaterialMap;

typedef struct Material {
    Shader shader;
    MaterialMap maps[MAX_MATERIAL_MAPS];
    float * params;
} Material;

typedef struct Transform {
    Vector3 translation;
    Quaternion rotation;
    Vector3 scale;
} Transform;

typedef struct BoneInfo {
    char name[32];
    int parent;
} BoneInfo;

typedef struct Model {
    Matrix transform;
    int meshCount;
    Mesh * meshes;
    int materialCount;
    Material * materials;
    int * meshMaterial;
    int boneCount;
    BoneInfo * bones;
    Transform * bindPose;
} Model;

typedef struct ModelAnimation {
    int boneCount;
    BoneInfo * bones;
    int frameCount;
    Transform ** framePoses;
} ModelAnimation;

typedef struct Ray {
    Vector3 position;
    Vector3 direction;
} Ray;

typedef struct RayHitInfo {
    bool hit;
    float distance;
    Vector3 position;
    Vector3 normal;
} RayHitInfo;

typedef struct Wave {
    unsigned int sampleCount;
    unsigned int sampleRate;
    unsigned int sampleSize;
    unsigned int channels;
    void * data;
} Wave;

typedef struct Sound {
    void * audioBuffer;
    unsigned int source;
    unsigned int buffer;
    int format;
} Sound;

typedef struct MusicData * Music;

typedef struct AudioStream {
    unsigned int sampleRate;
    unsigned int sampleSize;
    unsigned int channels;
    void * audioBuffer;
    int format;
    unsigned int source;
    unsigned int buffers[2];
} AudioStream;

typedef struct VrDeviceInfo {
    int hResolution;
    int vResolution;
    float hScreenSize;
    float vScreenSize;
    float vScreenCenter;
    float eyeToScreenDistance;
    float lensSeparationDistance;
    float interpupillaryDistance;
    float lensDistortionValues[4];
    float chromaAbCorrection[4];
} VrDeviceInfo;

typedef struct VrStereoConfig {
    RenderTexture2D stereoFbo;
    Shader distortionShader;
    Matrix eyesProjection[2];
    Matrix eyesViewOffset[2];
    int eyeViewportRight[4];
    int eyeViewportLeft[4];
} VrStereoConfig;

typedef enum {
    FLAG_SHOW_LOGO          = 1,
    FLAG_FULLSCREEN_MODE    = 2,
    FLAG_WINDOW_RESIZABLE   = 4,
    FLAG_WINDOW_UNDECORATED = 8,
    FLAG_WINDOW_TRANSPARENT = 16,
    FLAG_WINDOW_HIDDEN      = 128,
    FLAG_MSAA_4X_HINT       = 32,
    FLAG_VSYNC_HINT         = 64
} ConfigFlag;

typedef enum {
    LOG_ALL = 0,
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE
} TraceLogType;

typedef enum {
    KEY_APOSTROPHE      = 39,
    KEY_COMMA           = 44,
    KEY_MINUS           = 45,
    KEY_PERIOD          = 46,
    KEY_SLASH           = 47,
    KEY_ZERO            = 48,
    KEY_ONE             = 49,
    KEY_TWO             = 50,
    KEY_THREE           = 51,
    KEY_FOUR            = 52,
    KEY_FIVE            = 53,
    KEY_SIX             = 54,
    KEY_SEVEN           = 55,
    KEY_EIGHT           = 56,
    KEY_NINE            = 57,
    KEY_SEMICOLON       = 59,
    KEY_EQUAL           = 61,
    KEY_A               = 65,
    KEY_B               = 66,
    KEY_C               = 67,
    KEY_D               = 68,
    KEY_E               = 69,
    KEY_F               = 70,
    KEY_G               = 71,
    KEY_H               = 72,
    KEY_I               = 73,
    KEY_J               = 74,
    KEY_K               = 75,
    KEY_L               = 76,
    KEY_M               = 77,
    KEY_N               = 78,
    KEY_O               = 79,
    KEY_P               = 80,
    KEY_Q               = 81,
    KEY_R               = 82,
    KEY_S               = 83,
    KEY_T               = 84,
    KEY_U               = 85,
    KEY_V               = 86,
    KEY_W               = 87,
    KEY_X               = 88,
    KEY_Y               = 89,
    KEY_Z               = 90,

    KEY_SPACE           = 32,
    KEY_ESCAPE          = 256,
    KEY_ENTER           = 257,
    KEY_TAB             = 258,
    KEY_BACKSPACE       = 259,
    KEY_INSERT          = 260,
    KEY_DELETE          = 261,
    KEY_RIGHT           = 262,
    KEY_LEFT            = 263,
    KEY_DOWN            = 264,
    KEY_UP              = 265,
    KEY_PAGE_UP         = 266,
    KEY_PAGE_DOWN       = 267,
    KEY_HOME            = 268,
    KEY_END             = 269,
    KEY_CAPS_LOCK       = 280,
    KEY_SCROLL_LOCK     = 281,
    KEY_NUM_LOCK        = 282,
    KEY_PRINT_SCREEN    = 283,
    KEY_PAUSE           = 284,
    KEY_F1              = 290,
    KEY_F2              = 291,
    KEY_F3              = 292,
    KEY_F4              = 293,
    KEY_F5              = 294,
    KEY_F6              = 295,
    KEY_F7              = 296,
    KEY_F8              = 297,
    KEY_F9              = 298,
    KEY_F10             = 299,
    KEY_F11             = 300,
    KEY_F12             = 301,
    KEY_LEFT_SHIFT      = 340,
    KEY_LEFT_CONTROL    = 341,
    KEY_LEFT_ALT        = 342,
    KEY_LEFT_SUPER      = 343,
    KEY_RIGHT_SHIFT     = 344,
    KEY_RIGHT_CONTROL   = 345,
    KEY_RIGHT_ALT       = 346,
    KEY_RIGHT_SUPER     = 347,
    KEY_KB_MENU         = 348,
    KEY_LEFT_BRACKET    = 91,
    KEY_BACKSLASH       = 92,
    KEY_RIGHT_BRACKET   = 93,
    KEY_GRAVE           = 96,

    KEY_KP_0            = 320,
    KEY_KP_1            = 321,
    KEY_KP_2            = 322,
    KEY_KP_3            = 323,
    KEY_KP_4            = 324,
    KEY_KP_5            = 325,
    KEY_KP_6            = 326,
    KEY_KP_7            = 327,
    KEY_KP_8            = 328,
    KEY_KP_9            = 329,
    KEY_KP_DECIMAL      = 330,
    KEY_KP_DIVIDE       = 331,
    KEY_KP_MULTIPLY     = 332,
    KEY_KP_SUBTRACT     = 333,
    KEY_KP_ADD          = 334,
    KEY_KP_ENTER        = 335,
    KEY_KP_EQUAL        = 336
} KeyboardKey;

typedef enum {
    KEY_BACK            = 4,
    KEY_MENU            = 82,
    KEY_VOLUME_UP       = 24,
    KEY_VOLUME_DOWN     = 25
} AndroidButton;

typedef enum {
    MOUSE_LEFT_BUTTON   = 0,
    MOUSE_RIGHT_BUTTON  = 1,
    MOUSE_MIDDLE_BUTTON = 2
} MouseButton;

typedef enum {
    GAMEPAD_PLAYER1     = 0,
    GAMEPAD_PLAYER2     = 1,
    GAMEPAD_PLAYER3     = 2,
    GAMEPAD_PLAYER4     = 3
} GamepadNumber;

typedef enum {
    GAMEPAD_PS3_BUTTON_TRIANGLE = 0,
    GAMEPAD_PS3_BUTTON_CIRCLE   = 1,
    GAMEPAD_PS3_BUTTON_CROSS    = 2,
    GAMEPAD_PS3_BUTTON_SQUARE   = 3,
    GAMEPAD_PS3_BUTTON_L1       = 6,
    GAMEPAD_PS3_BUTTON_R1       = 7,
    GAMEPAD_PS3_BUTTON_L2       = 4,
    GAMEPAD_PS3_BUTTON_R2       = 5,
    GAMEPAD_PS3_BUTTON_START    = 8,
    GAMEPAD_PS3_BUTTON_SELECT   = 9,
    GAMEPAD_PS3_BUTTON_PS       = 12,
    GAMEPAD_PS3_BUTTON_UP       = 24,
    GAMEPAD_PS3_BUTTON_RIGHT    = 25,
    GAMEPAD_PS3_BUTTON_DOWN     = 26,
    GAMEPAD_PS3_BUTTON_LEFT     = 27
} GamepadPS3Button;

typedef enum {
    GAMEPAD_PS3_AXIS_LEFT_X     = 0,
    GAMEPAD_PS3_AXIS_LEFT_Y     = 1,
    GAMEPAD_PS3_AXIS_RIGHT_X    = 2,
    GAMEPAD_PS3_AXIS_RIGHT_Y    = 5,
    GAMEPAD_PS3_AXIS_L2         = 3,
    GAMEPAD_PS3_AXIS_R2         = 4
} GamepadPS3Axis;

typedef enum {
    GAMEPAD_XBOX_BUTTON_A       = 0,
    GAMEPAD_XBOX_BUTTON_B       = 1,
    GAMEPAD_XBOX_BUTTON_X       = 2,
    GAMEPAD_XBOX_BUTTON_Y       = 3,
    GAMEPAD_XBOX_BUTTON_LB      = 4,
    GAMEPAD_XBOX_BUTTON_RB      = 5,
    GAMEPAD_XBOX_BUTTON_SELECT  = 6,
    GAMEPAD_XBOX_BUTTON_START   = 7,
    GAMEPAD_XBOX_BUTTON_HOME    = 8,
    GAMEPAD_XBOX_BUTTON_UP      = 10,
    GAMEPAD_XBOX_BUTTON_RIGHT   = 11,
    GAMEPAD_XBOX_BUTTON_DOWN    = 12,
    GAMEPAD_XBOX_BUTTON_LEFT    = 13
} GamepadXbox360Button;

typedef enum {
    GAMEPAD_XBOX_AXIS_LEFT_X    = 0,
    GAMEPAD_XBOX_AXIS_LEFT_Y    = 1,
    GAMEPAD_XBOX_AXIS_RIGHT_X   = 2,
    GAMEPAD_XBOX_AXIS_RIGHT_Y   = 3,
    GAMEPAD_XBOX_AXIS_LT        = 4,
    GAMEPAD_XBOX_AXIS_RT        = 5
} GamepadXbox360Axis;

typedef enum {
    GAMEPAD_ANDROID_DPAD_UP     = 19,
    GAMEPAD_ANDROID_DPAD_DOWN   = 20,
    GAMEPAD_ANDROID_DPAD_LEFT   = 21,
    GAMEPAD_ANDROID_DPAD_RIGHT  = 22,
    GAMEPAD_ANDROID_DPAD_CENTER = 23,
    GAMEPAD_ANDROID_BUTTON_A    = 96,
    GAMEPAD_ANDROID_BUTTON_B    = 97,
    GAMEPAD_ANDROID_BUTTON_C    = 98,
    GAMEPAD_ANDROID_BUTTON_X    = 99,
    GAMEPAD_ANDROID_BUTTON_Y    = 100,
    GAMEPAD_ANDROID_BUTTON_Z    = 101,
    GAMEPAD_ANDROID_BUTTON_L1   = 102,
    GAMEPAD_ANDROID_BUTTON_R1   = 103,
    GAMEPAD_ANDROID_BUTTON_L2   = 104,
    GAMEPAD_ANDROID_BUTTON_R2   = 105
} GamepadAndroid;

typedef enum {
    LOC_VERTEX_POSITION = 0,
    LOC_VERTEX_TEXCOORD01,
    LOC_VERTEX_TEXCOORD02,
    LOC_VERTEX_NORMAL,
    LOC_VERTEX_TANGENT,
    LOC_VERTEX_COLOR,
    LOC_MATRIX_MVP,
    LOC_MATRIX_MODEL,
    LOC_MATRIX_VIEW,
    LOC_MATRIX_PROJECTION,
    LOC_VECTOR_VIEW,
    LOC_COLOR_DIFFUSE,
    LOC_COLOR_SPECULAR,
    LOC_COLOR_AMBIENT,
    LOC_MAP_ALBEDO,
    LOC_MAP_METALNESS,
    LOC_MAP_NORMAL,
    LOC_MAP_ROUGHNESS,
    LOC_MAP_OCCLUSION,
    LOC_MAP_EMISSION,
    LOC_MAP_HEIGHT,
    LOC_MAP_CUBEMAP,
    LOC_MAP_IRRADIANCE,
    LOC_MAP_PREFILTER,
    LOC_MAP_BRDF
} ShaderLocationIndex;

#define LOC_MAP_DIFFUSE   LOC_MAP_ALBEDO
#define LOC_MAP_SPECULAR  LOC_MAP_METALNESS

typedef enum {
    UNIFORM_FLOAT = 0,
    UNIFORM_VEC2,
    UNIFORM_VEC3,
    UNIFORM_VEC4,
    UNIFORM_INT,
    UNIFORM_IVEC2,
    UNIFORM_IVEC3,
    UNIFORM_IVEC4,
    UNIFORM_SAMPLER2D
} ShaderUniformDataType;

typedef enum {
    MAP_ALBEDO    = 0,
    MAP_METALNESS = 1,
    MAP_NORMAL    = 2,
    MAP_ROUGHNESS = 3,
    MAP_OCCLUSION,
    MAP_EMISSION,
    MAP_HEIGHT,
    MAP_CUBEMAP,
    MAP_IRRADIANCE,
    MAP_PREFILTER,
    MAP_BRDF
} MaterialMapType;

#define MAP_DIFFUSE   MAP_ALBEDO
#define MAP_SPECULAR  MAP_METALNESS

typedef enum {
    UNCOMPRESSED_GRAYSCALE = 1,
    UNCOMPRESSED_GRAY_ALPHA,
    UNCOMPRESSED_R5G6B5,
    UNCOMPRESSED_R8G8B8,
    UNCOMPRESSED_R5G5B5A1,
    UNCOMPRESSED_R4G4B4A4,
    UNCOMPRESSED_R8G8B8A8,
    UNCOMPRESSED_R32,
    UNCOMPRESSED_R32G32B32,
    UNCOMPRESSED_R32G32B32A32,
    COMPRESSED_DXT1_RGB,
    COMPRESSED_DXT1_RGBA,
    COMPRESSED_DXT3_RGBA,
    COMPRESSED_DXT5_RGBA,
    COMPRESSED_ETC1_RGB,
    COMPRESSED_ETC2_RGB,
    COMPRESSED_ETC2_EAC_RGBA,
    COMPRESSED_PVRT_RGB,
    COMPRESSED_PVRT_RGBA,
    COMPRESSED_ASTC_4x4_RGBA,
    COMPRESSED_ASTC_8x8_RGBA
} PixelFormat;

typedef enum {
    FILTER_POINT = 0,
    FILTER_BILINEAR,
    FILTER_TRILINEAR,
    FILTER_ANISOTROPIC_4X,
    FILTER_ANISOTROPIC_8X,
    FILTER_ANISOTROPIC_16X,
} TextureFilterMode;

typedef enum {
    CUBEMAP_AUTO_DETECT = 0,
    CUBEMAP_LINE_VERTICAL,
    CUBEMAP_LINE_HORIZONTAL,
    CUBEMAP_CROSS_THREE_BY_FOUR,
    CUBEMAP_CROSS_FOUR_BY_THREE,
    CUBEMAP_PANORAMA
} CubemapLayoutType;

typedef enum {
    WRAP_REPEAT = 0,
    WRAP_CLAMP,
    WRAP_MIRROR_REPEAT,
    WRAP_MIRROR_CLAMP
} TextureWrapMode;

typedef enum {
    FONT_DEFAULT = 0,
    FONT_BITMAP,
    FONT_SDF
} FontType;

typedef enum {
    BLEND_ALPHA = 0,
    BLEND_ADDITIVE,
    BLEND_MULTIPLIED
} BlendMode;

typedef enum {
    GESTURE_NONE        = 0,
    GESTURE_TAP         = 1,
    GESTURE_DOUBLETAP   = 2,
    GESTURE_HOLD        = 4,
    GESTURE_DRAG        = 8,
    GESTURE_SWIPE_RIGHT = 16,
    GESTURE_SWIPE_LEFT  = 32,
    GESTURE_SWIPE_UP    = 64,
    GESTURE_SWIPE_DOWN  = 128,
    GESTURE_PINCH_IN    = 256,
    GESTURE_PINCH_OUT   = 512
} GestureType;

typedef enum {
    CAMERA_CUSTOM = 0,
    CAMERA_FREE,
    CAMERA_ORBITAL,
    CAMERA_FIRST_PERSON,
    CAMERA_THIRD_PERSON
} CameraMode;

typedef enum {
    CAMERA_PERSPECTIVE = 0,
    CAMERA_ORTHOGRAPHIC
} CameraType;

typedef enum {
    HMD_DEFAULT_DEVICE = 0,
    HMD_OCULUS_RIFT_DK2,
    HMD_OCULUS_RIFT_CV1,
    HMD_OCULUS_GO,
    HMD_VALVE_HTC_VIVE,
    HMD_SONY_PSVR
} VrDeviceType;

typedef enum {
    NPT_9PATCH = 0,
    NPT_3PATCH_VERTICAL,
    NPT_3PATCH_HORIZONTAL
} NPatchType;

typedef void (*TraceLogCallback)(int log_type, char const * text, va_list args);

#endif

